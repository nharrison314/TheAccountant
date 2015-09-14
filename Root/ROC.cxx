#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <WTag/ROC.h>

// EDM includes                                                                                 
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMissingET/MissingETContainer.h"
//#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"                                    
#include "xAODBTagging/BTagging.h"
#include "JetSubStructureUtils/BosonTag.h"

// Infrastructure includes                                                                      
#include "xAODRootAccess/Init.h"

// xAH includes                                                                                 
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

// root includes                                                                                
#include <TCanvas.h>
#include <TVector3.h>
#include <TH1F.h>
// c++ includes                                                                                 
#include <set>
using namespace std;

namespace HF = HelperFunctions;

WTag::WTag::ROC (std::string name) :
  HistogramManager(name,"")
{
}

//ClassImp(ROC)
//ROC :: ROC () :{}

//EL::StatusCode ROC :: setupJob (EL::Job& job)
//{
// job.useXAOD();
// xAOD::Init("ROC").ignore();
// return EL::StatusCode::SUCCESS;
//}

//EL::StatusCode ROC :: histInitialize () { return EL::StatusCode::SUCCESS; }
//EL::StatusCode ROC :: fileExecute () { return EL::StatusCode::SUCCESS; }
//EL::StatusCode ROC :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }
//WTag::WTag::ROC (std::string name) :
// HistogramManager(name,"")//
//{/
//}

WTag::WTag::~ROC () {}

StatusCode ROC :: initialize () {
  // assign m_event and m_store                                          
  jetmass1 = book(m_name,"jetmass1","Leading Jet Mass (GeV)",650,0, 6500);
  jetmass2 = book(m_name,"jetmass2","Subleading Jet Mass (GeV)",650, 0 , 6500);
  jetmass3 = book(m_name,"jetmass3","Third Jet Mass (GeV)",650,0,6500);
  jetmass4 = book(m_name,"jetmass4","Fourth Jet Mass (GeV)",650,0,6500);
  jetmass1_Wlabel = book(m_name,"jetmass1_Wlabel","Leading Jet Mass (GeV",650,0,6500);
  jetmass2_Wlabel = book(m_name,"jetmass2_Wlabel","Subleading JetMass (GeV",650,0,6500);
  jetmass3_Wlabel = book(m_name,"jetmass3_Wlabel","Third Jet Mass (GeV)",650,0,6500);
  jetmass4_Wlabel = book(m_name,"jetmass4_Wlabel","Fourth Jet Mass (GeV)",650,0,6500);

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  //positive = 0.0;
  //f/ake = 0.0;
  //totalEvents = 0.0;
  return EL::StatusCode::SUCCESS;
}

StatusCode ROC :: execute (const xAOD::EventInfo* eventInfo,float eventWeight)
{
  //static SG::AuxElement::ConstAccessor<float> Wlabel("Wlabel");

  if(m_debug) Info("execute()", "Calling execute...");
  const xAOD::EventInfo*                eventInfo     (nullptr);
  const xAOD::JetContainer*             in_jetsLargeR (nullptr);
  const xAOD::JetContainer*             in_jets       (nullptr);
  const xAOD::MissingETContainer*       in_missinget  (nullptr);
  const xAOD::ElectronContainer*        in_electrons  (nullptr);
  const xAOD::MuonContainer*            in_muons      (nullptr);
  const xAOD::TauJetContainer*          in_taus       (nullptr);
  const xAOD::PhotonContainer*          in_photons    (nullptr);
  const xAOD::TruthParticleContainer*   truth_particles  (nullptr);      

  static JetSubStructureUtils::BosonTag WTagger("medium", "smooth", "$ROOTCOREBIN/data/JetSub\
StructureUtils/config_13TeV_20150528_Wtagging.dat", true, true);     

  RETURN_CHECK("Audit::execute()", HF::retrieve(eventInfo,    m_eventInfo,        m_event, m_store, m_debug), "Could not get the EventInfo container.");
  if(!m_inputLargeRJets.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_jetsLargeR,      m_inputLargeRJets,        m_event, m_store, m_debug), "Could not get the inputLargeRJets container.");
  if(!m_inputJets.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_jets,     m_inputJets,       m_event, m_store, m_debug), "Could not get the inputJets container.");
  if(!m_inputMET.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_missinget, m_inputMET,         m_event, m_store, m_debug), "Could not get the inputMET container.");
  if(!m_inputElectrons.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_electrons, m_inputElectrons,   m_event, m_store, m_debug), "Could not get the inputElectrons container.");
  if(!m_inputMuons.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_muons,     m_inputMuons,       m_event, m_store, m_debug), "Could not get the inputMuons container.");
  if(!m_inputTauJets.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_taus,      m_inputTauJets,     m_event, m_store, m_debug), "Could not get the inputTauJets container.");
  if(!m_inputPhotons.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_photons,   m_inputPhotons,     m_event, m_store, m_debug), "Could not get the inputPhotons container.");
  if(!m_truth.empty())
    RETURN_CHECK("ClassifyEvent::execute()", HF::retrieve(truth_particles, "TruthParticle", m_event, m_store, true), "");


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

  // create a vector to hold the group element ids for when adding jets                            
  std::map<const RF::GroupElementID, const xAOD::Jet*> in_jets_IDs;
  for(const auto jet: *in_jets)
    {
      in_jets_IDs[VIS.AddLabFrameFourVector( jet->p4() )] = jet;
    }
  //totalEvents = totalEvents + 1;
  //bool isWTagged = false;
  //tags jets that are likely W boson.                                                             
  //for(const auto jet: *in_jetsLargeR)                                                            
  //  {                                                                                            
  //    isWTagged = WTagger.result(*jet);                                                     
  //  } 
  float jetmass_1 =0;
  float jetmass_2=0; 
  float jetmass_3=0; 
  float jetmass_4=0;
  int i=0;
  //int j=0;
  //int numlargeRjets = 0;
  //std::vector<int> WjetIndex;
  //for (const auto jet: *in_jetsLargeR)
  //{
  //  numlargeRjets++;
  //}
  for(const auto jet: *in_jetsLargeR)
    {
      //bool signalW = false;
      i++;
      if (i==1)
	{
	  jetmass_1= jet->m()/1.e3;
	  if (jetmass_1 > 0)
	    jetmass1->Fill(jetmass_1);
	}
      else if (i==2)
	{
	  jetmass_2 = jet->m()/1.e3;
	  if (jetmass_2 > 0)
	    jetmass2->Fill(jetmass_2);
	}
      else if (i==3)
	{
	  jetmass_3 = jet->m()/1.e3;
	  if (jetmass_3 > 0)
	    jetmass3->Fill(jetmass_3);
	}
      else
	{
	  jetmass_4 = jet->m()/1.e3;
	  if (jetmass_4 > 0)
	    jetmass4->Fill(jetmass_4);
	}
    

      //bool truePositive = false;
      //bool falsePositive = false;
      for (const auto truth_particle: *truth_particles){
	int pdgId = abs(truth_particle->pdgId());
	if (pdgId==24 || pdgId==-24)
	  {
	    signalW = true;
	  }
	
      }

      if (i ==1 && signalW)
	jetmass1_Wlabel->Fill(jetmass_1);
      else if (i == 2 && signalW)
	jetmass2_Wlabel->Fill(jetmass_2);
      else if (i == 3 && signalW)
	jetmass3_Wlabel->Fill(jetmass_3);
      else if (i == 4 && signalW)
	jetmass4_Wlabel->Fill(jetmass_4);

      // if (isTruthW && isWTagged)
      //	positive = positive + 1;
      //if (!isTruthW && isWTagged)
      //	fake = fake + 1;
    }

  // dump information about the jets and met at least                                              
  if(m_debug){
    Info("execute()", "Details about input jets...");
    for(const auto jet: *in_jets)
      {
	Info("execute()", "\tpT: %0.2f GeV\tm: %0.2f GeV\teta: %0.2f\tphi: %0.2f", jet->pt()/1000., jet->m()/1000., jet->eta(), jet->phi());
      }
    Info("execute()", "Details about MET...");
    Info("execute()", "\tpx: %0.2f GeV\tpy: %0.2f GeV\tpz: %0.2f GeV", in_met->mpx()/1000., in_met\
	 ->mpy()/1000., 0.0/1000.);



  }


  return StatusCode::SUCCESS;
}
