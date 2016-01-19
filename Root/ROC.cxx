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
  tops = book(m_name,"tops","tops",10,0,10);
  deltaR_W_b_jet1 = book(m_name,"deltaR_W_b_jet1","#Delta R_{W, b} leading jet",50,0,3);
  deltaR_W_jet_jet1 = book(m_name,"deltaR_W_jet_jet1","#Delta R_{W, jet} leading jet",50,0,3);

  deltaR_W_b_jet2 = book(m_name,"deltaR_W_b_jet2","#Delta R_{W, b} subleading jet",50,0,3);
  deltaR_W_jet_jet2 = book(m_name,"deltaR_W_jet_jet2","#Delta R_{W, jet} subleading jet",50,0,3);

  deltaR_W_b_jet3 = book(m_name,"deltaR_W_b_jet3","#Delta R_{W, b} 3rd leading jet",50,0,3);
  deltaR_W_jet_jet3 = book(m_name,"deltaR_W_jet_jet3","#Delta R_{W, jet} 3rd leading jet",50,0,3);

  deltaR_W_b_jet4 = book(m_name,"deltaR_W_b_jet4","#Delta R_{W, b} 4th leading jet",50,0,3);
  deltaR_W_jet_jet4 = book(m_name,"deltaR_W_jet_jet4","#Delta R_{W, jet} 4th leading jet",50,0,3);

  deltaR_W_jet_semiboosted_jet1 = book(m_name,"deltaR_W_jet_semiboosted_jet1","#Delta R_{W,jet} leading jet",50,0,3);
  deltaR_W_b_semiboosted_jet1 = book(m_name,"deltaR_W_b_semiboosted_jet1","#Delta R_{W,b} leading jet",50,0,3);

  deltaR_W_jet_semiboosted_jet2 = book(m_name,"deltaR_W_jet_semiboosted_jet2","#Delta R_{W,jet} subleading jet",50,0,3);
  deltaR_W_b_semiboosted_jet2 = book(m_name,"deltaR_W_b_semiboosted_jet2","#Delta R_{W,b} subleading jet",50,0,3);

  deltaR_W_jet_semiboosted_jet3 = book(m_name,"deltaR_W_jet_semiboosted_jet3","#Delta R_{W,jet} 3rd-leading jet",50,0,3);
  deltaR_W_b_semiboosted_jet3 = book(m_name,"deltaR_W_b_semiboosted_jet3","#Delta R_{W,b} 3rd-leading jet",50,0,3);

  deltaR_W_jet_semiboosted_jet4 = book(m_name,"deltaR_W_jet_semiboosted_jet4","#Delta R_{W,jet} 4th-leading jet",50,0,3);
  deltaR_W_b_semiboosted_jet4 = book(m_name,"deltaR_W_b_semiboosted_jet4","#Delta R_{W,b} 4th-leading jet",50,0,3);


  deltaR_W_top_semiboosted_jet1 = book(m_name,"deltaR_W_top_semiboosted_jet1","#Delta R_{W,top} leading jet",50,0,3);

  deltaR_W_top_semiboosted_jet2 = book(m_name,"deltaR_W_top_semiboosted_jet2","#Delta R_{W,top} subleading jet",50,0,3);

  deltaR_W_top_semiboosted_jet3 = book(m_name,"deltaR_W_top_semiboosted_jet3","#Delta R_{W,top} 3rd-leading jet",50,0,3);

  deltaR_W_top_semiboosted_jet4 = book(m_name,"deltaR_W_top_semiboosted_jet4","#Delta R_{W,top} 4th-leading jet",50,0,3);

  deltaR_W_top_jet1 = book(m_name,"deltaR_W_top_jet1","#Delta R_{W,top} leading jet", 50, 0, 3);
  deltaR_W_top_jet2 = book(m_name,"deltaR_W_top_jet1","#Delta R_{W,top} subleading jet", 50, 0, 3);
  deltaR_W_top_jet3 = book(m_name,"deltaR_W_top_jet1","#Delta R_{W,top} 3rd-leading jet", 50, 0, 3);
  deltaR_W_top_jet4 = book(m_name,"deltaR_W_top_jet1","#Delta R_{W,top} 4th-leading jet", 50, 0, 3);
  

  deltaR_W_jet1_fn_top_pt = book(m_name,"deltaR_W_jet1_fn_top_pt","p_T of top quark",100,0,6500,"#Delta R_{W,jet} leading jet",50,0,3);

  deltaR_W_jet2_fn_top_pt = book(m_name,"deltaR_W_jet2_fn_top_pt","p_T of top quark",100,0,6500,"#Delta R_{W,jet} subleading jet",50,0,3);

  deltaR_W_jet3_fn_top_pt = book(m_name,"deltaR_W_jet3_fn_top_pt","p_T of top quark",100,0,6500,"#Delta R_{W,jet} 3rd leading jet",50,0,3);

  deltaR_W_jet4_fn_top_pt = book(m_name,"deltaR_W_jet4_fn_top_pt","p_T of top quark",100,0,6500,"#Delta R_{W,jet} 4th leading jet",50,0,3);

  return EL::StatusCode::SUCCESS;
}

StatusCode TheAccountant::ROC::execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, float eventWeight)
{
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
  static SG::AuxElement::Accessor<bool> containsTruthTop_acc("containsTruthTop");
  static SG::AuxElement::Accessor<bool> notContainedB_acc("notContainedB");
  static SG::AuxElement::Accessor<float> deltaR_W_jet("deltaR_W_jet");
  static SG::AuxElement::Accessor<float> deltaR_W_top("deltaR_W_top");
  static SG::AuxElement::Accessor<float> deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");  
  static SG::AuxElement::Accessor<float> deltaR_W_b("deltaR_W_b");
  static SG::AuxElement::Accessor<const xAOD::TruthParticle*> top ("top_particle");
  static SG::AuxElement::Accessor<const xAOD::TruthParticle*> W_quark1 ("W_quark1");
  static SG::AuxElement::Accessor<const xAOD::TruthParticle*> W_quark2 ("W_quark2");
  
  for(const auto jet: *in_jetsLargeR)
    {
      // if (jet->pt()/1000.<200 || jet->pt()/1000.>300)
      //	continue;
      //fill histograms for all largeR jets
      i++;
      if (i==1)
	{
	  jetmass_1= jet->m()/1.e3;
	  if (jetmass_1 > 0  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	    {
	      jetmass1->Fill(jetmass_1, eventWeight);
	    }
	}
      else if (i==2)
	{
	  jetmass_2 = jet->m()/1.e3;
	  if (jetmass_2 > 0  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	    jetmass2->Fill(jetmass_2, eventWeight);
	}
      else if (i==3)
	{
	  jetmass_3 = jet->m()/1.e3;
	  if (jetmass_3 > 0  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	    jetmass3->Fill(jetmass_3, eventWeight);
	}
      else
	{
	  jetmass_4 = jet->m()/1.e3;
	  if (jetmass_4 > 0  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	    jetmass4->Fill(jetmass_4, eventWeight);
	}

      // if(containsTruthWacc(*jet) && hasB(*jet))
      //float deltaR = xAOD::P4Helpers::deltaR(jet, truth_particle	\
	// );


      if (i==1 && containsTruthW_acc(*jet) && notContainedB_acc(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass1_Wlabel->Fill(jetmass_1, eventWeight);
      else if (i==2 && containsTruthW_acc(*jet) && notContainedB_acc(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass2_Wlabel->Fill(jetmass_2, eventWeight);
      else if (i==3 && containsTruthW_acc(*jet) && notContainedB_acc(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass3_Wlabel->Fill(jetmass_3, eventWeight);
      else if (i==4 && containsTruthW_acc(*jet) && notContainedB_acc(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass4_Wlabel->Fill(jetmass_4, eventWeight);

      if (deltaR_W_jet(*jet) != -1.0 && i==1)
	deltaR_W_jet_jet1->Fill(deltaR_W_jet(*jet),eventWeight);
      if (deltaR_W_b(*jet) != -1.0 && i==1)
	deltaR_W_b_jet1->Fill(deltaR_W_b(*jet),eventWeight);
     

      if (deltaR_W_jet(*jet) != -1.0 && i==2)
        deltaR_W_jet_jet2->Fill(deltaR_W_jet(*jet),eventWeight);
      if (deltaR_W_b(*jet) != -1.0 && i==2)
        deltaR_W_b_jet2->Fill(deltaR_W_b(*jet),eventWeight);

      if (deltaR_W_jet(*jet) != -1.0 && i==3)
        deltaR_W_jet_jet3->Fill(deltaR_W_jet(*jet),eventWeight);
      if (deltaR_W_b(*jet) != -1.0 && i==3)
        deltaR_W_b_jet3->Fill(deltaR_W_b(*jet),eventWeight);

      if (deltaR_W_jet(*jet) != -1.0 && i==4)
        deltaR_W_jet_jet4->Fill(deltaR_W_jet(*jet),eventWeight);
      if (deltaR_W_b(*jet) != -1.0 && i==4)
        deltaR_W_b_jet4->Fill(deltaR_W_b(*jet),eventWeight);

      if (containsTruthW_acc(*jet) && notContainedB_acc(*jet) && i==1)
	{
	  deltaR_W_jet_semiboosted_jet1->Fill(deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet1->Fill(deltaR_W_b(*jet),eventWeight);
	}

      if (containsTruthW_acc(*jet) && notContainedB_acc(*jet) && i==2)
	{
	  deltaR_W_jet_semiboosted_jet2->Fill(deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet2->Fill(deltaR_W_b(*jet),eventWeight);
	}


      if (containsTruthW_acc(*jet) && notContainedB_acc(*jet) && i==3)
	{
	  deltaR_W_jet_semiboosted_jet3->Fill(deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet3->Fill(deltaR_W_b(*jet),eventWeight);
	}


      if (containsTruthW_acc(*jet) && notContainedB_acc(*jet) && i==4)
	{
	  deltaR_W_jet_semiboosted_jet4->Fill(deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet4->Fill(deltaR_W_b(*jet),eventWeight);
	}

     
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && notContainedB_acc(*jet) && i==1)
	{
	  deltaR_W_top_semiboosted_jet1->Fill(deltaR_W_top(*jet),eventWeight);
	}
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && notContainedB_acc(*jet) && i==2)
	{
	  deltaR_W_top_semiboosted_jet2->Fill(deltaR_W_top(*jet),eventWeight);
	}

      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && notContainedB_acc(*jet) && i==3)
	{
	  deltaR_W_top_semiboosted_jet3->Fill(deltaR_W_top(*jet),eventWeight);
	}

      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && notContainedB_acc(*jet) && i==4)
	{
	  deltaR_W_top_semiboosted_jet4->Fill(deltaR_W_top_semiboosted(*jet),eventWeight);
	}

      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && i==1)
	{
	  deltaR_W_top_jet1->Fill(deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet1_fn_top_pt->Fill(top->pt(),deltaR_W_jet(*jet), eventWeight);

	}
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && i==2)
	{
	  deltaR_W_top_jet2->Fill(deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet2_fn_top_pt->Fill(top->pt(),deltaR_W_jet(*jet), eventWeight);
	}
      
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && i==3)
	{
	  deltaR_W_top_jet3->Fill(deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet3_fn_top_pt->Fill(top->pt(),deltaR_W_jet(*jet), eventWeight);
	}
      
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && i==4)
	{
	  deltaR_W_top_jet4->Fill(deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet4_fn_top_pt->Fill(top->pt(),deltaR_W_jet(*jet), eventWeight);
	}
      
      if (containsTruthW_acc(*jet) && containsTruthTop_acc(*jet) && i==1)
	{
	  float deltaEta, deltaPhi;
	  //NEED TO FINISH, need to put in right delta phi calculation
	  //if (W_quark1->pt() > W_quark2->pt())
	    //  deltaEta
	}

   }

   return StatusCode::SUCCESS;
}
