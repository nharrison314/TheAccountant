#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <TheAccountant/Report.h>

// Infrastructure includes
#include "xAODRootAccess/Init.h"

// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMissingET/MissingETContainer.h"

// xAH includes
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

#include <TheAccountant/VariableDefinitions.h>

namespace HF = HelperFunctions;
namespace VD = VariableDefinitions;

// this is needed to distribute the algorithm to the workers
ClassImp(Report)

Report :: Report () : {}

EL::StatusCode Report :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("Report").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Report :: histInitialize () {
  // initialize all histograms here

  m_ROCPlots["all/roc"] = new TheAccountant::ROC("all/roc/");
  


  for (auto rocPlot: m_ROCPlots){
    RETURN_CHECK("Report::initialize()",rocPlot.second->initialize(),"");
    rocPlot.second->record(wk());
  }

  return EL::StatusCode::SUCCESS;
}
EL::StatusCode Report :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Report :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode Report :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Report :: execute ()
{
  if(m_debug) Info("execute()", "Calling execute...");
  const xAOD::EventInfo*                eventInfo     (nullptr);
  const xAOD::JetContainer*             in_jetsLargeR (nullptr);
  const xAOD::JetContainer*             in_jets       (nullptr);
  const xAOD::MissingETContainer*       in_missinget  (nullptr);
  const xAOD::ElectronContainer*        in_electrons  (nullptr);
  const xAOD::MuonContainer*            in_muons      (nullptr);
  const xAOD::TauJetContainer*          in_taus       (nullptr);
  const xAOD::PhotonContainer*          in_photons    (nullptr);
  const xAOD::TruthParticleContainer*   truth_particles    (nullptr);

  // start grabbing all the containers that we can
  RETURN_CHECK("Report::execute()", HF::retrieve(eventInfo,    m_eventInfo,        m_event, m_store, m_debug), "Could not get the EventInfo container.");
  if(!m_inputJets.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_jets,     m_inputJets,       m_event, m_store, m_debug), "Could not get the inputJets container.");
  if(!m_inputLargeRJets.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_jetsLargeR,      m_inputLargeRJets,        m_event, m_store, m_debug), "Could not get the inputLargeRJets container.");
  if(!m_inputMET.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_missinget, m_inputMET,         m_event, m_store, m_debug), "Could not get the inputMET container.");
  if(!m_inputElectrons.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_electrons, m_inputElectrons,   m_event, m_store, m_debug), "Could not get the inputElectrons container.");
  if(!m_inputMuons.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_muons,     m_inputMuons,       m_event, m_store, m_debug), "Could not get the inputMuons container.");
  if(!m_inputTauJets.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_taus,      m_inputTauJets,     m_event, m_store, m_debug), "Could not get the inputTauJets container.");
  if(!m_inputPhotons.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(in_photons,   m_inputPhotons,     m_event, m_store, m_debug), "Could not get the inputPhotons container.");
  if(!m_truthParticles.empty())
    RETURN_CHECK("Report::execute()", HF::retrieve(truth_particles, m_truthParticles, m_event, m_store, m_debug),"Could not get truth particle container.");

  // prepare the jets by creating a view container to look at them
  ConstDataVector<xAOD::JetContainer> in_jetsCDV(SG::VIEW_ELEMENTS);
  if(!m_inputJets.empty()){
    for(auto jet: *in_jets){
      if(jet->pt()/1.e3 < m_jet_minPtView) continue;
      if(fabs(jet->eta()) > m_jet_maxAbsEtaView) continue;
      in_jetsCDV.push_back(jet);
    }
    // make in_jets point to a view instead
    in_jets = in_jetsCDV.asDataVector();
  }

  ConstDataVector<xAOD::JetContainer> in_jetsLargeRCDV(SG::VIEW_ELEMENTS);
  if(!m_inputLargeRJets.empty()){
    for(auto jet: *in_jetsLargeR){
      if(jet->pt()/1.e3 < m_jetLargeR_minPtView) continue;
      if(fabs(jet->eta()) > m_jetLargeR_maxAbsEtaView) continue;
      in_jetsLargeRCDV.push_back(jet);
    }
    // make in_jetsLargeR point to a view instead
    in_jetsLargeR = in_jetsLargeRCDV.asDataVector();
  }

  const xAOD::MissingET* in_met(nullptr);
  if(!m_inputMET.empty()){
    // retrieve CalibMET_RefFinal for METContainer
    xAOD::MissingETContainer::const_iterator met_id = in_missinget->find(m_inputMETName);
    if (met_id == in_missinget->end()) {
      Error("execute()", "No %s inside MET container", m_inputMETName.c_str());
      return EL::StatusCode::FAILURE;
    }
    // dereference the iterator since it's just a single object
    in_met = *met_id;
  }

  float eventWeight = VD::eventWeight(eventInfo, wk()->metaData());
<<<<<<< HEAD


  //RETURN_CHECK("Report::execute()", m_RazorPlots["all/razor"]->execute(eventInfo, in_met,in_jets, in_jetsLargeR, in_muons, in_electrons,eventWeight),"");
  RETURN_CHECK("Report::execute()", m_ROCPlots["all/roc"]->execute(eventInfo, in_jetsLargeR, in_jets, truth_particles,eventWeight),"");
=======
  RETURN_CHECK("Report::execute()", m_ROCPlots["all/roc"]->execute(eventInfo, in_jetsLargeR, in_jets, truth_particles, eventWeight),"");
>>>>>>> eb82b9e52c8c9d6334aa90288c00939ef8e2e491


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Report :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode Report :: finalize () {
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Report :: histFinalize () { return EL::StatusCode::SUCCESS; }
