#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <TheAccountant/ROC.h>

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

TheAccountant::ROC::ROC (std::string name) :
  HistogramManager(name,"")
{
}

TheAccountant::ROC::~ROC () {}

StatusCode TheAccountant::ROC::initialize () {
  // assign m_event and m_store                                          
  jetmass1 = book(m_name,"jetmass1","Leading Jet Mass (GeV)",650,0, 6500);
  jetmass2 = book(m_name,"jetmass2","Subleading Jet Mass (GeV)",650, 0 , 6500);
  jetmass3 = book(m_name,"jetmass3","Third Jet Mass (GeV)",650,0,6500);
  jetmass4 = book(m_name,"jetmass4","Fourth Jet Mass (GeV)",650,0,6500);
  jetmass1_Wlabel = book(m_name,"jetmass1_Wlabel","Leading Jet Mass (GeV",650,0,6500);
  jetmass2_Wlabel = book(m_name,"jetmass2_Wlabel","Subleading JetMass (GeV",650,0,6500);
  jetmass3_Wlabel = book(m_name,"jetmass3_Wlabel","Third Jet Mass (GeV)",650,0,6500);
  jetmass4_Wlabel = book(m_name,"jetmass4_Wlabel","Fourth Jet Mass (GeV)",650,0,6500);

  return EL::StatusCode::SUCCESS;
}

StatusCode TheAccountant::ROC::execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, float eventWeight)
{
  std::cout << "ROC #1" << std::endl;
  // static JetSubStructureUtils::BosonTag WTagger("medium", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_20150528_Wtagging.dat", true, true);     

  // static SG::AuxElement::ConstAccessor<bool> Jet1_containW_acc("Jet1_containW");
  // static SG::AuxElement::ConstAccessor<bool> Jet2_containW_acc("Jet2_containW");
  // static SG::AuxElement::ConstAccessor<bool> Jet3_containW_acc("Jet3_containW");
  // static SG::AuxElement::ConstAccessor<bool> Jet4_containW_acc("Jet4_containW");


  float jetmass_1 =0;
  float jetmass_2=0; 
  float jetmass_3=0; 
  float jetmass_4=0;
  int i=0;

  static SG::AuxElement::Accessor<bool> containsTruthW_acc("containsTruthW");
  std::cout <<"ROC #2" << std::endl;
  for(const auto jet: *in_jetsLargeR)
    {
      std::cout <<"ROC #3 inside large R Jets loop" << std::endl;
      std::cout << "jetmass1: " << jetmass1 << std::endl;
      i++;
      if (i==1)
	{
	  jetmass_1= jet->m()/1.e3;
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
      std::cout <<"ROC #4" <<std::endl;
      std::cout <<"containsTruthW_acc = " << containsTruthW_acc(jet) << std::endl;
      std::cout <<"ROC #5" << std::endl;
     if (i==1 && containsTruthW_acc(*jet))
	jetmass1_Wlabel->Fill(jetmass_1, eventWeight);
      else if (i==2 && containsTruthW_acc(*jet))
	jetmass2_Wlabel->Fill(jetmass_2, eventWeight);
      else if (i==3 && containsTruthW_acc(*jet))
	jetmass3_Wlabel->Fill(jetmass_3, eventWeight);
      else if (i==4 && containsTruthW_acc(*jet))
	jetmass4_Wlabel->Fill(jetmass_4, eventWeight);
   }

   return StatusCode::SUCCESS;
}
