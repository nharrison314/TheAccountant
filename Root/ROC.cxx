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
namespace VD = VariableDefinitions;


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

 

  deltaEta_W_quark1_jet1 = book(m_name,"deltaEta_W_quark1_jet1","#Delta #eta",50,-2.5,2.5);
  deltaPhi_W_quark1_jet1 = book(m_name,"deltaPhi_W_quark1_jet1","#Delta #phi",50,-6.3,6.3);
  deltaEta_W_quark1_jet2 = book(m_name,"deltaEta_W_quark1_jet2","#Delta #eta",50,-2.5,2.5);
  deltaPhi_W_quark1_jet2 = book(m_name,"deltaPhi_W_quark1_jet2","#Delta #phi",50,-6.3,6.3);
  deltaEta_W_quark1_jet3 = book(m_name,"deltaEta_W_quark1_jet3","#Delta #eta",50,-2.5,2.5);
  deltaPhi_W_quark1_jet3 = book(m_name,"deltaPhi_W_quark1_jet3","#Delta #phi",50,-6.3,6.3);
  deltaEta_W_quark1_jet4 = book(m_name,"deltaEta_W_quark1_jet4","#Delta #eta",50,-2.5,2.5);
  deltaPhi_W_quark1_jet4 = book(m_name,"deltaPhi_W_quark1_jet4","#Delta #phi",50,-6.3,6.3);


  return EL::StatusCode::SUCCESS;
}

StatusCode TheAccountant::ROC::execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, float eventWeight)
{


  float jetmass_1 =0;
  float jetmass_2=0; 
  float jetmass_3=0; 
  float jetmass_4=0;
  int i=0;


  std::cout << "size of in_jetsLargeR: " << in_jetsLargeR->size() << std::endl;


  //  static SG::AuxElement::Accessor<bool> containsTruthW_acc("containsTruthW");
  //static SG::AuxElement::Accessor<bool> containsTruthTop_acc("containsTruthTop");
  //static SG::AuxElement::Accessor<bool> notContainedB_acc("notContainedB");
  //static SG::AuxElement::Accessor<float> deltaR_W_jet("deltaR_W_jet");
  //static SG::AuxElement::Accessor<float> deltaR_W_top("deltaR_W_top");
  //static SG::AuxElement::Accessor<float> deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");  
  //static SG::AuxElement::Accessor<float> deltaR_W_b("deltaR_W_b");
  //static SG::AuxElement::Accessor<const xAOD::TruthParticle*> top ("top_particle");
  //static SG::AuxElement::Accessor<const xAOD::TruthParticle*> W_quark1 ("W_quark1");
  //static SG::AuxElement::Accessor<const xAOD::TruthParticle*> W_quark2 ("W_quark2");
  
  std::cout <<"ROC 1" <<std::endl;
  for(const auto jet: *in_jetsLargeR)
    {
      std::cout << "inside loop ROC 1" <<std::endl;
      // if (jet->pt()/1000.<200 || jet->pt()/1000.>300)
      //	continue;
      //fill histograms for all largeR jets
      i++;
      if (i==1)
	{
	  jetmass_1= jet->m()/1.e3;
	  std::cout<<" Inside loop ROC 2" <<std::endl;
	  std::cout<<"Jet mass: " << jetmass_1 << "\tJet pt: " << jet->pt()/1000. << std::endl; 
	  if (jetmass_1 > 0  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	    {
	      std::cout << "passed pT requirements" << std::endl;
	      std::cout << "eventWeight: " << eventWeight << std::endl;
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

      std::cout<< "inside loop ROC 2" <<std::endl;

      if (i==1 && VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass1_Wlabel->Fill(jetmass_1, eventWeight);
      else if (i==2 && VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass2_Wlabel->Fill(jetmass_2, eventWeight);
      else if (i==3 && VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass3_Wlabel->Fill(jetmass_3, eventWeight);
      else if (i==4 && VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
	jetmass4_Wlabel->Fill(jetmass_4, eventWeight);

      if (VD::acc_deltaR_W_jet(*jet) != -1.0 && i==1)
	deltaR_W_jet_jet1->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
      if (VD::acc_deltaR_W_b(*jet) != -1.0 && i==1)
	deltaR_W_b_jet1->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);
     

      if (VD::acc_deltaR_W_jet(*jet) != -1.0 && i==2)
        deltaR_W_jet_jet2->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
      if (VD::acc_deltaR_W_b(*jet) != -1.0 && i==2)
        deltaR_W_b_jet2->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);

      if (VD::acc_deltaR_W_jet(*jet) != -1.0 && i==3)
        deltaR_W_jet_jet3->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
      if (VD::acc_deltaR_W_b(*jet) != -1.0 && i==3)
        deltaR_W_b_jet3->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);

      if (VD::acc_deltaR_W_jet(*jet) != -1.0 && i==4)
        deltaR_W_jet_jet4->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
      if (VD::acc_deltaR_W_b(*jet) != -1.0 && i==4)
        deltaR_W_b_jet4->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);

      if (VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet) && i==1)
	{
	  deltaR_W_jet_semiboosted_jet1->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet1->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);
	}

      if (VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet) && i==2)
	{
	  deltaR_W_jet_semiboosted_jet2->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet2->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);
	}


      if (VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet) && i==3)
	{
	  deltaR_W_jet_semiboosted_jet3->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet3->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);
	}


      if (VD::acc_containsTruthW(*jet) && VD::acc_notContainedB(*jet) && i==4)
	{
	  deltaR_W_jet_semiboosted_jet4->Fill(VD::acc_deltaR_W_jet(*jet),eventWeight);
	  deltaR_W_b_semiboosted_jet4->Fill(VD::acc_deltaR_W_b(*jet),eventWeight);
	}

     
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && VD::acc_notContainedB(*jet) && i==1)
	{
	  deltaR_W_top_semiboosted_jet1->Fill(VD::acc_deltaR_W_top(*jet),eventWeight);
	}
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && VD::acc_notContainedB(*jet) && i==2)
	{
	  deltaR_W_top_semiboosted_jet2->Fill(VD::acc_deltaR_W_top(*jet),eventWeight);
	}

      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && VD::acc_notContainedB(*jet) && i==3)
	{
	  deltaR_W_top_semiboosted_jet3->Fill(VD::acc_deltaR_W_top(*jet),eventWeight);
	}

      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && VD::acc_notContainedB(*jet) && i==4)
	{
	  deltaR_W_top_semiboosted_jet4->Fill(VD::acc_deltaR_W_top_semiboosted(*jet),eventWeight);
	}

      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==1)
	{
	  deltaR_W_top_jet1->Fill(VD::acc_deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet1_fn_top_pt->Fill(VD::acc_top_quark(*jet)->pt()/1000.,VD::acc_deltaR_W_jet(*jet), eventWeight);

	}
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==2)
	{
	  deltaR_W_top_jet2->Fill(VD::acc_deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet2_fn_top_pt->Fill(VD::acc_top_quark(*jet)->pt()/1000.,VD::acc_deltaR_W_jet(*jet), eventWeight);
	}
      
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==3)
	{
	  deltaR_W_top_jet3->Fill(VD::acc_deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet3_fn_top_pt->Fill(VD::acc_top_quark(*jet)->pt()/1000.,VD::acc_deltaR_W_jet(*jet), eventWeight);
	}
      
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==4)
	{
	  deltaR_W_top_jet4->Fill(VD::acc_deltaR_W_top(*jet), eventWeight);
	  deltaR_W_jet4_fn_top_pt->Fill(VD::acc_top_quark(*jet)->pt()/1000.,VD::acc_deltaR_W_jet(*jet), eventWeight);
	}
      

      std::cout <<"ROC 2" <<std::endl;
      if (VD::acc_W_quark1(*jet)==NULL)
	std::cout <<"W quark NULL" << std::endl;

      std::cout <<"ROC 3" <<std::endl;
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==1 && VD::acc_W_quark1(*jet)!=NULL)
	{
	  float deltaEta = xAOD::P4Helpers::deltaPhi(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));
	  float deltaPhi = xAOD::P4Helpers::deltaEta(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));

	  deltaEta_W_quark1_jet1->Fill(deltaEta, eventWeight);
	  deltaPhi_W_quark1_jet1->Fill(deltaPhi, eventWeight);
	}

      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==2 && VD::acc_W_quark1(*jet)!=NULL)
	{
	  float deltaEta = xAOD::P4Helpers::deltaPhi(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));
	  float deltaPhi = xAOD::P4Helpers::deltaEta(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));

	  deltaEta_W_quark1_jet2->Fill(deltaEta, eventWeight);
	  deltaPhi_W_quark1_jet2->Fill(deltaPhi, eventWeight);
	}
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==3 && VD::acc_W_quark1(*jet)!=NULL)
	{
	  float deltaEta = xAOD::P4Helpers::deltaPhi(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));
	  float deltaPhi = xAOD::P4Helpers::deltaEta(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));

	  deltaEta_W_quark1_jet3->Fill(deltaEta, eventWeight);
	  deltaPhi_W_quark1_jet3->Fill(deltaPhi, eventWeight);
	}
      if (VD::acc_containsTruthW(*jet) && VD::acc_containsTruthTop(*jet) && i==4 && VD::acc_W_quark1(*jet)!=NULL)
	{
	  float deltaEta = xAOD::P4Helpers::deltaPhi(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));
	  float deltaPhi = xAOD::P4Helpers::deltaEta(VD::acc_top_quark(*jet),VD::acc_W_quark1(*jet));

	  deltaEta_W_quark1_jet4->Fill(deltaEta, eventWeight);
	  deltaPhi_W_quark1_jet4->Fill(deltaPhi, eventWeight);
	}

   }

   return StatusCode::SUCCESS;
}
