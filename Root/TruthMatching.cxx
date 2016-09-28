#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <TheAccountant/TruthMatching.h>

#include "EventLoop/OutputStream.h"

// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

#include "xAODBTagging/BTagging.h"

//P4Helper includes
#include "FourMomUtils/xAODP4Helpers.h"

// Infrastructure includes
#include "xAODRootAccess/Init.h"

// xAH includes
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"
#include <TheAccountant/VariableDefinitions.h>
// root includes
#include <TCanvas.h>
#include <TH1F.h>
#include <TVector3.h>

#include "xAODAnaHelpers/HistogramManager.h"


// c++ includes
#include <set>

namespace HF = HelperFunctions;
namespace VD = VariableDefinitions;
namespace TM = TruthMatchingVars;

TheAccountant::TruthMatching::TruthMatching (std::string name) :
  HistogramManager(name,"")
{
}

TheAccountant::TruthMatching::~TruthMatching () {}

StatusCode TheAccountant::TruthMatching::initialize () {

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


StatusCode TheAccountant::TruthMatching::execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, const xAOD::TruthParticleContainer* in_truth, float eventWeight){

  event_num++;

  for (const auto& jet: *in_jetsLargeR)
    {
      TM::decor_containsTruthW(*jet) = false;
      TM::decor_semiBoostedW(*jet) = false;
      TM::decor_containsTruthTop(*jet) = false;
      TM::decor_notContainedB(*jet) = false;
      TM::decor_b_particle(*jet) = NULL;
      TM::decor_W_particle(*jet) = NULL;
      TM::decor_top_particle(*jet) = NULL;
      TM::decor_deltaR_W_jet(*jet) = -1;
      TM::decor_deltaR_W_b(*jet) = -1;
      TM::decor_deltaR_W_top(*jet) = -1;
      TM::decor_deltaR_W_top_semiboosted(*jet)=-1;
      TM::decor_hasB(*jet) = false;
      TM::decor_hasW(*jet) = false;
      TM::decor_b_particle(*jet) = NULL;
      TM::decor_W_particle(*jet) = NULL;
      TM::decor_top_particle(*jet) = NULL;
      TM::decor_W_quark1(*jet) = NULL;
      TM::decor_W_quark2(*jet) = NULL;
      TM::decor_top_quark(*jet) = NULL;

    }



  std::list<const xAOD::TruthParticle*> b_particles_in_event;
  std::list<const xAOD::TruthParticle*> W_particles_in_event;
  std::list<const xAOD::TruthParticle*> tops_in_event;

  std::list<const xAOD::TruthParticle*> daughter_W_particles_cand;
  std::list<const xAOD::TruthParticle*> daughter_W_particles;
 

 
  if (!in_truth->empty())
    { 
      std::cout <<"TM 2"<<std::endl;
      if (!in_truth->size()==0)
	{
      
	  for (const auto truth_particle: *in_truth)
	    {
	      

	      int pdgId = abs(truth_particle->pdgId());
	      if(pdgId ==5)
		{
		  b_particles_in_event.push_back(truth_particle);
		}
	      if(truth_particle->isW())
		{
		  W_particles_in_event.push_back(truth_particle);
		  bool hasSelfChild = false;
		  for(unsigned int it = 0; it < truth_particle->nChildren(); ++it){
		    int daughter_pdgId = truth_particle->child(it)->pdgId();
		    if( daughter_pdgId == truth_particle->pdgId() ){
		      hasSelfChild = true;
		      break;
		    }
		    if(daughter_pdgId==1 ||daughter_pdgId==2 || daughter_pdgId==3 || daughter_pdgId==4)
		      {  
			daughter_W_particles_cand.push_back(truth_particle);
		      }
		  }
		  
		}
	      if(truth_particle->isTop())
		{
		  tops_in_event.push_back(truth_particle);
		}
	    }
	  for (const auto truth_particle: *in_truth)
	    {	   
	      int num_jet =0;
	      for(const auto& jet: *in_jetsLargeR)
		{
		  num_jet++;
		  int num_W_in_event=0;
		  
		  float Min_deltaR_jet_top = 1000.;
		  std::list<const xAOD::TruthParticle*>::iterator k;
		  int num_top=0;
		  for(k=tops_in_event.begin(); k != tops_in_event.end(); ++k)
		    {
		      num_top++;
		      float deltaR = xAOD::P4Helpers::deltaR(jet,*k);
		      if(deltaR < Min_deltaR_jet_top)
			{
			  Min_deltaR_jet_top = deltaR;
			  if (deltaR<1)
			    {
			      TM::decor_containsTruthTop(*jet) = true;
			      TM::decor_top_particle(*jet) = *k;
			    }
			}
		    }
		  tops_in_event.remove(TM::decor_top_particle(*jet));

		  float Min_deltaR_for_this_jet = 1000.;
		  std::list<const xAOD::TruthParticle*>::iterator i;
		  for(i=W_particles_in_event.begin(); i != W_particles_in_event.end(); ++i)
		    {  
		      num_W_in_event++;
		      float deltaR = xAOD::P4Helpers::deltaR(jet, *i);
		      if (deltaR < Min_deltaR_for_this_jet)
			{
			  Min_deltaR_for_this_jet = deltaR;
			  TM::decor_W_particle(*jet) = *i;
			  TM::decor_hasW(*jet) = true;
			}
		    }
		  if (TM::acc_W_particle(*jet)!=NULL)
		    TM::decor_deltaR_W_jet(*jet) = xAOD::P4Helpers::deltaR(jet,TM::decor_W_particle(*jet));
		  else
		    TM::decor_deltaR_W_jet(*jet) = 1000;
		  W_particles_in_event.remove(TM::acc_W_particle(*jet));
		 

		  std::list<const xAOD::TruthParticle*>::iterator j;
		  float deltaR_W_b_min = 1000.;
		  for(j=b_particles_in_event.begin(); j != b_particles_in_event.end(); ++j)
		    {
		      float deltaR = xAOD::P4Helpers::deltaR(jet, *j);

		      if (deltaR < deltaR_W_b_min)
			{
			  deltaR_W_b_min = deltaR;
			  TM::decor_b_particle(*jet) = *j;
			  TM::decor_hasB(*jet) = true;
			}
		    }
		  if (TM::acc_b_particle(*jet)!=NULL)
		    TM::decor_deltaR_W_b(*jet) = xAOD::P4Helpers::deltaR(jet,TM::acc_b_particle(*jet));
		  else
		    TM::decor_deltaR_W_b(*jet) = 1000;
		  b_particles_in_event.remove(TM::acc_b_particle(*jet));
		  
		  if (TM::acc_W_particle(*jet)!=NULL)
		    {
		      std::list<const xAOD::TruthParticle*>::iterator l;
		      std::list<const xAOD::TruthParticle*>::iterator m;
		      float max_quark_pt = 13000;
		      for(l=daughter_W_particles_cand.begin(); l != daughter_W_particles_cand.end(); ++l)
			{
			  if ((*l)->pt()/1000. < max_quark_pt)
			    {
			      max_quark_pt = (*l)->pt()/1000.;
			      TM::decor_W_quark1(*jet) = *l;
			    }
			}

		      daughter_W_particles_cand.remove(TM::acc_W_quark1(*jet));
		      for(m=daughter_W_particles_cand.begin(); m != daughter_W_particles_cand.end(); ++m)
			{
			  if ((*m)->pt()/1000. < max_quark_pt)
			    {
			      max_quark_pt = (*m)->pt()/1000.;
			      TM::decor_W_quark2(*jet) = *m;
			    }
			}
		     
		      
		    }		 
		  
		}
	    }
	  
	 
	  for(const auto& jet: *in_jetsLargeR)
	    {
	      if (TM::acc_deltaR_W_jet(*jet) <= 0.3 && TM::acc_hasW(*jet))
		{
		  TM::decor_containsTruthW(*jet) = true;
		}
	      if(TM::acc_hasB(*jet))
		{
		  float deltaR = xAOD::P4Helpers::deltaR(jet, TM::acc_b_particle(*jet));
		  if (deltaR >= 1.0)
		    TM::decor_notContainedB(*jet) = true;
		}
	      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet))
		{
		  TM::decor_deltaR_W_top(*jet) = xAOD::P4Helpers::deltaR(TM::acc_W_particle(*jet),TM::acc_top_particle(*jet));
		}
	      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet) && TM::acc_containsTruthTop(*jet))
		{
		  TM::decor_deltaR_W_top_semiboosted(*jet) = xAOD::P4Helpers::deltaR(TM::acc_W_particle(*jet),TM::acc_top_particle(*jet));
		}
	    }

          for(const auto& jet: *in_jetsLargeR)
            {
	      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet))
		TM::decor_semiBoostedW(*jet) = true;
	    }
	  
	}
    }
  /// done with truth matching, now histograms:


  float jetmass_1 =0;
  float jetmass_2=0;
  float jetmass_3=0;
  float jetmass_4=0;
  int i=0;

  std::cout <<" making histograms in TM " << std::endl;
  for(const auto jet: *in_jetsLargeR)
    {
      i++;
      if (i==1)
        {
          jetmass_1= jet->m()/1.e3;
	  std::cout<<"Jet mass: " << jetmass_1 << "\tJet pt: " << jet->pt()/1000. << std::endl;
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


      if (i==1 && TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
        jetmass1_Wlabel->Fill(jetmass_1, eventWeight);
      else if (i==2 && TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
        jetmass2_Wlabel->Fill(jetmass_2, eventWeight);
      else if (i==3 && TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
        jetmass3_Wlabel->Fill(jetmass_3, eventWeight);
      else if (i==4 && TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet)  && jet->pt()/1000.>=200 && jet->pt()/1000.<=300)
        jetmass4_Wlabel->Fill(jetmass_4, eventWeight);

      if (TM::acc_deltaR_W_jet(*jet) != -1.0 && i==1)
        deltaR_W_jet_jet1->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
      if (TM::acc_deltaR_W_b(*jet) != -1.0 && i==1)
        deltaR_W_b_jet1->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);


      if (TM::acc_deltaR_W_jet(*jet) != -1.0 && i==2)
        deltaR_W_jet_jet2->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
      if (TM::acc_deltaR_W_b(*jet) != -1.0 && i==2)
        deltaR_W_b_jet2->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);

      if (TM::acc_deltaR_W_jet(*jet) != -1.0 && i==3)
        deltaR_W_jet_jet3->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
      if (TM::acc_deltaR_W_b(*jet) != -1.0 && i==3)
        deltaR_W_b_jet3->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);

      if (TM::acc_deltaR_W_jet(*jet) != -1.0 && i==4)
        deltaR_W_jet_jet4->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
      if (TM::acc_deltaR_W_b(*jet) != -1.0 && i==4)
        deltaR_W_b_jet4->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);

      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet) && i==1)
        {
          deltaR_W_jet_semiboosted_jet1->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
          deltaR_W_b_semiboosted_jet1->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet) && i==2)
        {
          deltaR_W_jet_semiboosted_jet2->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
          deltaR_W_b_semiboosted_jet2->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);
        }


      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet) && i==3)
        {
          deltaR_W_jet_semiboosted_jet3->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
          deltaR_W_b_semiboosted_jet3->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);
        }


      if (TM::acc_containsTruthW(*jet) && TM::acc_notContainedB(*jet) && i==4)
        {
          deltaR_W_jet_semiboosted_jet4->Fill(TM::acc_deltaR_W_jet(*jet),eventWeight);
          deltaR_W_b_semiboosted_jet4->Fill(TM::acc_deltaR_W_b(*jet),eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && TM::acc_notContainedB(*jet) && i==1)
        {
          deltaR_W_top_semiboosted_jet1->Fill(TM::acc_deltaR_W_top(*jet),eventWeight);
        }
      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && TM::acc_notContainedB(*jet) && i==2)
        {
          deltaR_W_top_semiboosted_jet2->Fill(TM::acc_deltaR_W_top(*jet),eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && TM::acc_notContainedB(*jet) && i==3)
        {
          deltaR_W_top_semiboosted_jet3->Fill(TM::acc_deltaR_W_top(*jet),eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && TM::acc_notContainedB(*jet) && i==4)
        {
          deltaR_W_top_semiboosted_jet4->Fill(TM::acc_deltaR_W_top_semiboosted(*jet),eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==1)
        {
          deltaR_W_top_jet1->Fill(TM::acc_deltaR_W_top(*jet), eventWeight);
          deltaR_W_jet1_fn_top_pt->Fill(TM::acc_top_quark(*jet)->pt()/1000.,TM::acc_deltaR_W_jet(*jet), eventWeight);

        }
      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==2)
        {
          deltaR_W_top_jet2->Fill(TM::acc_deltaR_W_top(*jet), eventWeight);
          deltaR_W_jet2_fn_top_pt->Fill(TM::acc_top_quark(*jet)->pt()/1000.,TM::acc_deltaR_W_jet(*jet), eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==3)
        {
          deltaR_W_top_jet3->Fill(TM::acc_deltaR_W_top(*jet), eventWeight);
          deltaR_W_jet3_fn_top_pt->Fill(TM::acc_top_quark(*jet)->pt()/1000.,TM::acc_deltaR_W_jet(*jet), eventWeight);
        }

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==4)
        {
          deltaR_W_top_jet4->Fill(TM::acc_deltaR_W_top(*jet), eventWeight);
          deltaR_W_jet4_fn_top_pt->Fill(TM::acc_top_quark(*jet)->pt()/1000.,TM::acc_deltaR_W_jet(*jet), eventWeight);
        }


      //std::cout <<"ROC 2" <<std::endl;
      if (TM::acc_W_quark1(*jet)==NULL)
	std::cout <<"W quark NULL" << std::endl;

      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==1 && TM::acc_W_quark1(*jet)!=NULL)
        {
          float deltaEta = xAOD::P4Helpers::deltaPhi(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));
          float deltaPhi = xAOD::P4Helpers::deltaEta(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));
          deltaEta_W_quark1_jet1->Fill(deltaEta, eventWeight);
          deltaPhi_W_quark1_jet1->Fill(deltaPhi, eventWeight);
        }
      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==2 && TM::acc_W_quark1(*jet)!=NULL)
        {
          float deltaEta = xAOD::P4Helpers::deltaPhi(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));
          float deltaPhi = xAOD::P4Helpers::deltaEta(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));

          deltaEta_W_quark1_jet2->Fill(deltaEta, eventWeight);
          deltaPhi_W_quark1_jet2->Fill(deltaPhi, eventWeight);
        }
      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==3 && TM::acc_W_quark1(*jet)!=NULL)
        {
          float deltaEta = xAOD::P4Helpers::deltaPhi(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));
          float deltaPhi = xAOD::P4Helpers::deltaEta(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));

          deltaEta_W_quark1_jet3->Fill(deltaEta, eventWeight);
          deltaPhi_W_quark1_jet3->Fill(deltaPhi, eventWeight);
        }
      if (TM::acc_containsTruthW(*jet) && TM::acc_containsTruthTop(*jet) && i==4 && TM::acc_W_quark1(*jet)!=NULL)
        {
          float deltaEta = xAOD::P4Helpers::deltaPhi(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));
          float deltaPhi = xAOD::P4Helpers::deltaEta(TM::acc_top_quark(*jet),TM::acc_W_quark1(*jet));

          deltaEta_W_quark1_jet4->Fill(deltaEta, eventWeight);
          deltaPhi_W_quark1_jet4->Fill(deltaPhi, eventWeight);
        }

    }

  return StatusCode::SUCCESS;
}


