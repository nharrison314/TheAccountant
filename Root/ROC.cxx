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
                 
#include <set>
using namespace std;

namespace HF = HelperFunctions;
//namespace VD = VariableDefinitions;

WTag::ROC::ROC (std::string name) :
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

WTag::ROC::~ROC () {}

StatusCode WTag::ROC::initialize () {

  std::cout << "Initializing ROC" << std::endl;
  // assign m_event and m_store                                          
  jetmass1 = book(m_name,"jetmass1","Leading Jet Mass (GeV)",650,0, 6500);
  jetmass2 = book(m_name,"jetmass2","Subleading Jet Mass (GeV)",650, 0 , 6500);
  jetmass3 = book(m_name,"jetmass3","Third Jet Mass (GeV)",650,0,6500);
  jetmass4 = book(m_name,"jetmass4","Fourth Jet Mass (GeV)",650,0,6500);
  jetmass1_Wlabel = book(m_name,"jetmass1_Wlabel","Leading Jet Mass (GeV",650,0,6500);
  jetmass2_Wlabel = book(m_name,"jetmass2_Wlabel","Subleading JetMass (GeV",650,0,6500);
  jetmass3_Wlabel = book(m_name,"jetmass3_Wlabel","Third Jet Mass (GeV)",650,0,6500);
  jetmass4_Wlabel = book(m_name,"jetmass4_Wlabel","Fourth Jet Mass (GeV)",650,0,6500);

  //positive = 0.0;
  //f/ake = 0.0;
  //totalEvents = 0.0;
  return EL::StatusCode::SUCCESS;
}

StatusCode WTag::ROC::execute (const xAOD::EventInfo* eventInfo,const xAOD::JetContainer* in_ffjets,const xAOD::JetContainer* in_jets, const xAOD::TruthParticleContainer* truth_particles, float eventWeight)
{
  std::cout <<"Executing ROC" << std::endl;
  //static SG::AuxElement::ConstAccessor<float> Wlabel("Wlabel");

  //if(m_debug) Info("execute()", "Calling execute...");
  //const xAOD::EventInfo*                eventInfo     (nullptr);
  //const xAOD::JetContainer*             in_jetsLargeR (nullptr);
  //const xAOD::JetContainer*             in_jets       (nullptr);
  //c/onst xAOD::MissingETContainer*       in_missinget  (nullptr);
  //const xAOD::ElectronContainer*        in_electrons  (nullptr);
  //const xAOD::MuonContainer*            in_muons      (nullptr);
  //const xAOD::TauJetContainer*          in_taus       (nullptr);
  //const xAOD::PhotonContainer*          in_photons    (nullptr);
  //const xAOD::TruthParticleContainer*   truth_particles  (nullptr);      

  std::cout <<"Before Boson Tagger" << std::endl;
  static JetSubStructureUtils::BosonTag WTagger("medium", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150528_Wtagging.dat", true, true);   std::cout <<" After Boson Tager" << std::endl;  

  //RETURN_CHECK("Audit::execute()", HF::retrieve(eventInfo,    m_eventInfo,        m_event, m_store, m_debug), "Could not get the EventInfo container.");
  // if(!m_inputLargeRJets.empty())
  //  RETURN_CHECK("Audit::execute()", HF::retrieve(in_jetsLargeR,      m_inputLargeRJets,        m_event, m_store, m_debug), "Could not get the inputLargeRJets container.");
  //if(!m_inputJets.empty())
  // RETURN_CHECK("Audit::execute()", HF::retrieve(in_jets,     m_inputJets,       m_event, m_store, m_debug), "Could not get the inputJets container.");
  //if(!m_inputMET.empty())
  // RETURN_CHECK("Audit::execute()", HF::retrieve(in_missinget, m_inputMET,         m_event, m_store, m_debug), "Could not get the inputMET container.");
  //if(!m_inputElectrons.empty())
  // RETURN_CHECK("Audit::execute()", HF::retrieve(in_electrons, m_inputElectrons,   m_event, m_store, m_debug), "Could not get the inputElectrons container.");
  // if(!m_inputMuons.empty())
  // RETURN_CHECK("Audit::execute()", HF::retrieve(in_muons,     m_inputMuons,       m_event, m_store, m_debug), "Could not get the inputMuons container.");
  //if(!m_inputTauJets.empty())
  //  RETURN_CHECK("Audit::execute()", HF::retrieve(in_taus,      m_inputTauJets,     m_event, m_store, m_debug), "Could not get the inputTauJets container.");
  //if(!m_inputPhotons.empty())
  // RETURN_CHECK("Audit::execute()", HF::retrieve(in_photons,   m_inputPhotons,     m_event, m_store, m_debug), "Could not get the inputPhotons container.");
  //if(!m_truth.empty())
  // RETURN_CHECK("ClassifyEvent::execute()", HF::retrieve(truth_particles, "TruthParticle", m_event, m_store, true), "");


  //const xAOD::MissingET* in_met(nullptr);
  //if(!m_inputMET.empty()){
    // retrieve CalibMET_RefFinal for METContainer                                                 
  //xAOD::MissingETContainer::const_iterator met_id = in_missinget->find(m_inputMETName);

  //totalEvents = totalEvents + 1;
  //bool isWTagged = false;
  //tags jets that are likely W boson.                                                             
  //for(const auto jet: *in_jetsLargeR)                                                            
  //  {                                                                                            
  //    isWTagged = WTagger.result(*jet);                                                     
  //  } 
  //float jetmass_1 =0;
  //float jetmass_2=0; 
  //float jetmass_3=0; 
  //float jetmass_4=0;
  int i=0;
  //int j=0;
  //int numlargeRjets = 0;
  //std::vector<int> WjetIndex;
  //for (const auto jet: *in_jetsLargeR)
  //{
  //  numlargeRjets++;
  //}
  

  //std::cout << "# fat jets: " << in_ffjets->size() << std::endl;
  std::cout << "HERE 1" << std::endl;
  for(const auto jet: *in_ffjets)
    {
      std::cout << "within in_ffjets" <<std::endl;
      float jetmass_1 =0;
      float jetmass_2=0;
      float jetmass_3=0;
      float jetmass_4=0;
      std::cout << "Inside FFjets container" << std::endl;
      bool signalW = false;
      i++;
      std::cout << "i: " << i << std::endl;
      if (i==1)
	{
	  jetmass_1= jet->m()/1.e3;
	  std::cout << "Jetmass_1: " << jetmass_1 << " GeV" <<std::endl;
	  if (jetmass_1 > 0)
	    jetmass1->Fill(jetmass_1, eventWeight);
	}
      else if (i==2)
	{
	  jetmass_2 = jet->m()/1.e3;
	  if (jetmass_2 > 0)
	    jetmass2->Fill(jetmass_2, eventWeight);
	}
      else if (i==3)
	{
	  jetmass_3 = jet->m()/1.e3;
	  if (jetmass_3 > 0)
	    jetmass3->Fill(jetmass_3, eventWeight);
	}
      else
	{
	  jetmass_4 = jet->m()/1.e3;
	  if (jetmass_4 > 0)
	    jetmass4->Fill(jetmass_4, eventWeight);
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
	jetmass1_Wlabel->Fill(jetmass_1, eventWeight);
      else if (i == 2 && signalW)
	jetmass2_Wlabel->Fill(jetmass_2, eventWeight);
      else if (i == 3 && signalW)
	jetmass3_Wlabel->Fill(jetmass_3, eventWeight);
      else if (i == 4 && signalW)
	jetmass4_Wlabel->Fill(jetmass_4, eventWeight);

      // if (isTruthW && isWTagged)
      //	positive = positive + 1;
      //if (!isTruthW && isWTagged)
      //	fake = fake + 1;
    }
  
  // dump information about the jets and met at least                          
  std::cout << "Looped through all the jets, yay!" << std::endl;

  return StatusCode::SUCCESS;
}
