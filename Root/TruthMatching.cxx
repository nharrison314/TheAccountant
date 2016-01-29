#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <TheAccountant/TruthMatching.h>

// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

//P4Helper includes
#include "FourMomUtils/xAODP4Helpers.h"

// Infrastructure includes
#include "xAODRootAccess/Init.h"

// xAH includes
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

// root includes
#include <TCanvas.h>
#include <TVector3.h>

// c++ includes
#include <set>

namespace HF = HelperFunctions;

// this is needed to distribute the algorithm to the workers
ClassImp(TruthMatching)
TruthMatching :: TruthMatching () {} 

EL::StatusCode TruthMatching :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("TruthMatching").ignore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TruthMatching :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TruthMatching :: initialize () {
  // assign m_event and m_store
  event_num =0;
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TruthMatching :: execute ()
{
  if(m_debug) Info("execute()", "Calling execute...");
  const xAOD::EventInfo*                eventInfo     (nullptr);
  const xAOD::JetContainer*             in_jetsLargeR (nullptr);
  const xAOD::JetContainer*             in_jets       (nullptr);
  const xAOD::TruthParticleContainer*   in_truth    (nullptr);

  event_num++;

  if(!m_truthParticles.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_truth, m_truthParticles, m_event, m_store, m_debug), "Could not get the truthParticles container.");

  static SG::AuxElement::Decorator<bool> containsTruthW         ("containsTruthW");
  static SG::AuxElement::Decorator<bool> notContainedB         ("notContainedB");
  static SG::AuxElement::Decorator<bool> containsTruthTop ("containsTruthTop");
  static SG::AuxElement::Decorator<bool> hasB ("hasB");
  static SG::AuxElement::Decorator<bool> hasW("hasW");
  static SG::AuxElement::Decorator<float> deltaR_W_jet ("deltaR_W_jet");
  static SG::AuxElement::Decorator<float> deltaR_W_b ("deltaR_W_b");
  static SG::AuxElement::Decorator<float> deltaR_W_top ("deltaR_W_top");
  static SG::AuxElement::Decorator<float> deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");

  static SG::AuxElement::Decorator<const xAOD::TruthParticle*> b_particle ("b_particle");
  static SG::AuxElement::Decorator<const xAOD::TruthParticle*> W_particle ("W_particle");
  static SG::AuxElement::Decorator<const xAOD::TruthParticle*> top_particle("top_particle");
  static SG::AuxElement::Decorator<const xAOD::TruthParticle*> W_quark1("W_quark1");
  static SG::AuxElement::Decorator<const xAOD::TruthParticle*> W_quark2("W_quark2");

  std::list<const xAOD::TruthParticle*> b_particles_in_event;
  std::list<const xAOD::TruthParticle*> W_particles_in_event;
  std::list<const xAOD::TruthParticle*> tops_in_event;

  std::list<const xAOD::TruthParticle*> daughter_W_particles_cand;
  std::list<const xAOD::TruthParticle*> daughter_W_particles;
 

  for (const auto& jet: *in_jetsLargeR)
    {
      containsTruthW(*jet) = false;
      containsTruthTop(*jet) = false;
      notContainedB(*jet) = false;
      b_particle(*jet) = NULL;
      W_particle(*jet) = NULL;
      top_particle(*jet) = NULL;
      deltaR_W_jet(*jet) = -1;
      deltaR_W_b(*jet) = -1;
      deltaR_W_top(*jet) = -1;
      deltaR_W_top_semiboosted(*jet)=-1;
    }

 
  if (!m_truthParticles.empty())
    { 
      if (!in_truth->size()==0)
	{
      
	  for (const auto truth_particle: *in_truth)
	    {
	      std::cout <<"#1" << std::endl;
	      int pdgId = abs(truth_particle->pdgId());
	      if(pdgId ==5)
		{
		  b_particles_in_event.push_back(truth_particle);
		}
	      std::cout <<"#2" << std::endl;
	      if(truth_particle->isW())
		{
		  W_particles_in_event.push_back(truth_particle);
		  bool hasSelfChild = false;
		  std::cout <<"is W" << std::endl;
		  for(unsigned int it = 0; it < truth_particle->nChildren(); ++it){
		    std::cout<<"inside loop"<<std::endl;
		    int daughter_pdgId = truth_particle->child(it)->pdgId();
		    if( daughter_pdgId == truth_particle->pdgId() ){
		      //Info("execute()", "\t\tIt contains itself. Skipping it.");
		      hasSelfChild = true;
		      break;
		    }
		    std::cout <<"inside loop 2" << std::endl;
		    
		    if(daughter_pdgId==1 ||daughter_pdgId==2 || daughter_pdgId==3 || daughter_pdgId==4)
		      {  
			daughter_W_particles_cand.push_back(truth_particle);
		      }
		    std::cout <<"inside loop 3" << std::endl;
		  }
		  
		}
	      std::cout << "#3" <<std::endl;
	      if(truth_particle->isTop())
		{
		  tops_in_event.push_back(truth_particle);
		}
	    }
	  std::cout <<"#4" <<std::endl;
	  for (const auto truth_particle: *in_truth)
	    {	   
	      int num_jet =0;
	      


	      for(const auto& jet: *in_jetsLargeR)
		{
		  std::cout <<"#5" <<std::endl;
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
			      containsTruthTop(*jet) = true;
			      top_particle(*jet) = *k;
			      if (event_num<10)
				{
				  std::cout <<"Event #" << event_num << "\tJet in event #" << num_jet << "\tphi: "<<jet->phi() <<"\trapidity: " << jet->rapidity() << std::endl;
				  std::cout <<"\tTop # " << num_top << "\trapidity: " << (*k)->rapidity() << "\tphi: " << (*k)->phi() <<std::endl;
				}
			    }
			}
		    }
		  std::cout <<"#6" <<std::endl;
		  tops_in_event.remove(top_particle(*jet));

		  float Min_deltaR_for_this_jet = 1000.;
		  std::list<const xAOD::TruthParticle*>::iterator i;
		  for(i=W_particles_in_event.begin(); i != W_particles_in_event.end(); ++i)
		    {  
		      num_W_in_event++;
		      float deltaR = xAOD::P4Helpers::deltaR(jet, *i);
		      if (event_num < 10)
			{
			  std::cout <<"\tW#" << num_W_in_event<< "\tphi: " << (*i)->phi() << "\trapidity: " << (*i)->rapidity() << std::endl;  
			  std::cout <<"\tdeltaR between jet #" << num_jet << " and W #" << num_W_in_event << " = " << deltaR << std::endl;
			} 
		      if (deltaR < Min_deltaR_for_this_jet)
			{
			  Min_deltaR_for_this_jet = deltaR;
			  W_particle(*jet) = *i;
			  hasW(*jet) = true;
			}
		    }
		  std::cout <<"#7" <<std::endl;
		  if (W_particle(*jet)!=NULL)
		    deltaR_W_jet(*jet) = xAOD::P4Helpers::deltaR(jet,W_particle(*jet));
		  else
		    deltaR_W_jet(*jet) = 1000;
		  W_particles_in_event.remove(W_particle(*jet));
		 

		  std::list<const xAOD::TruthParticle*>::iterator j;
		  float deltaR_W_b_min = 1000.;
		  for(j=b_particles_in_event.begin(); j != b_particles_in_event.end(); ++j)
		    {
		      float deltaR = xAOD::P4Helpers::deltaR(jet, *j);

		      if (event_num < 10)
			{
			  std::cout << "\tB in event: " << "\tphi: " << (*j)->phi() << "\trapidity: " << (*j)->rapidity() << std::endl;
			  std::cout << "\tdeltaR_W_b = " << xAOD::P4Helpers::deltaR(W_particle(*jet),*j) << std::endl;
			}
		      if (deltaR < deltaR_W_b_min)
			{
			  deltaR_W_b_min = deltaR;
			  b_particle(*jet) = *j;
			  hasB(*jet) = true;
			}
		      //	   }
		      // }	
		    }
		  std::cout <<"#8" <<std::endl;
		  if (b_particle(*jet)!=NULL)
		    deltaR_W_b(*jet) = xAOD::P4Helpers::deltaR(jet,b_particle(*jet));
		  else
		    deltaR_W_b(*jet) = 1000;
		  b_particles_in_event.remove(b_particle(*jet));
		  
		  std::cout <<"#10"<<std::endl;
		  if (W_particle(*jet)!=NULL)
		    {
		      std::list<const xAOD::TruthParticle*>::iterator l;
		      std::list<const xAOD::TruthParticle*>::iterator m;
		      float min_quark_pt = 13000;
		      for(l=daughter_W_particles_cand.begin(); l != daughter_W_particles_cand.end(); ++l)
			{
			  if ((*l)->pt() < min_quark_pt)
			    {
			      min_quark_pt = (*l)->pt();
			      W_quark1(*jet) = *l;
			    }
			}

		      daughter_W_particles_cand.remove(W_quark1(*jet));
		      for(m=daughter_W_particles_cand.begin(); m != daughter_W_particles_cand.end(); ++m)
			{
			  if ((*m)->pt() < min_quark_pt)
			    {
			      min_quark_pt = (*m)->pt();
			      W_quark2(*jet) = *m;
			    }
			}
		     
		      
		    }		 
		  std::cout << "#11"<<std::endl;
		  
		}
	    }
	  
	 
	  for(const auto& jet: *in_jetsLargeR)
	    {
	      if (event_num<10)
		std::cout <<"deltaR_W_jet = " << deltaR_W_jet(*jet) << std::endl;
	      std::cout <<"a" << std::endl;
	      if (deltaR_W_jet(*jet) <= 0.3 && hasW(*jet))
		{
		 
		  std::cout <<"ContainsWjet" << std::endl;
		  containsTruthW(*jet) = true;
		}
	      std::cout <<"b" << std::endl;
	      if(hasB(*jet))
		{
		  float deltaR = xAOD::P4Helpers::deltaR(jet, b_particle(*jet));
		  if (deltaR >= 1.0)
		    notContainedB(*jet) = true;
		}
	      std::cout << "c" <<std::endl;
	      if (containsTruthW(*jet) && containsTruthTop(*jet))
		{
		  deltaR_W_top(*jet) = xAOD::P4Helpers::deltaR(W_particle(*jet),top_particle(*jet));
		}
	      std::cout<< "d" <<std::endl;
	     
	      if (containsTruthW(*jet) && notContainedB(*jet) && containsTruthTop(*jet))
		{
		  deltaR_W_top_semiboosted(*jet) = xAOD::P4Helpers::deltaR(W_particle(*jet),top_particle(*jet));
		}
	      std::cout <<"e" <<std::endl;
	    }
	}
    }
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TruthMatching :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: histFinalize () { return EL::StatusCode::SUCCESS; }
