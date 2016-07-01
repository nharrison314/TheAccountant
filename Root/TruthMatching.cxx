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
  if (m_rc_enable) m_inputLargeRJets = m_RCJetsContainerName;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TruthMatching :: execute ()
{
  std::cout <<"TruthMatching" << std::endl;
  if(m_debug) Info("execute()", "Calling execute...");
  const xAOD::EventInfo*                eventInfo     (nullptr);
  const xAOD::JetContainer*             in_jetsLargeR (nullptr);
  const xAOD::JetContainer*             in_jets       (nullptr);
  const xAOD::MissingETContainer*       in_missinget  (nullptr);
  const xAOD::ElectronContainer*        in_electrons  (nullptr);
  const xAOD::MuonContainer*            in_muons      (nullptr);
  const xAOD::TauJetContainer*          in_taus       (nullptr);
  const xAOD::PhotonContainer*          in_photons    (nullptr);
  const xAOD::TruthParticleContainer*   in_truth    (nullptr);

  event_num++;

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

  if(!m_truthParticles.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_truth, m_truthParticles, m_event, m_store, m_debug), "Could not get the truthParticles container.");


  const xAOD::MissingET* in_met(nullptr);
  if(!m_inputMET.empty()){
    in_met = (*in_missinget)[m_inputMETName.c_str()];
    if (!in_met) {
      Error("execute()", "No %s inside MET container", m_inputMETName.c_str());
      return EL::StatusCode::FAILURE;
    }
  }
  

  // dump information about the jets and met at least
  if(m_debug){
    if(!m_inputJets.empty()){
      Info("execute()", "Details about input jets...");
      for(const auto &jet: *in_jets)
          Info("execute()", "\tpT: %0.2f GeV\tm: %0.2f GeV\teta: %0.2f\tphi: %0.2f", jet->pt()/1000., jet->m()/1000., jet->eta(), jet->phi());
    }

    if(!m_inputMET.empty()){
      Info("execute()", "Details about MET...");
      Info("execute()", "\tpx: %0.2f GeV\tpy: %0.2f GeV\tpz: %0.2f GeV", in_met->mpx()/1000., in_met->mpy()/1000., 0.0/1000.);
    }
  }

  static SG::AuxElement::Decorator<bool> containsTruthW         ("containsTruthW");
  static SG::AuxElement::Decorator<bool> notContainedB         ("notContainedB");
  static SG::AuxElement::Decorator<bool> WTAG ("WTAG");
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
      WTAG(*jet) = false;
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
      std::cout <<"TM 2"<<std::endl;
      if (!in_truth->size()==0)
	{
      
	  for (const auto truth_particle: *in_truth)
	    {
	      

	      //std::cout <<"nChildren() " << truth_particle->nChildren() << std::endl;
	      //  std::cout <<"truth_particle->child(0)" << truth_particle->child(0) << std::endl;
	      //std::cout <<"#1" << std::endl;
	      int pdgId = abs(truth_particle->pdgId());
	      if(pdgId ==5)
		{
		  b_particles_in_event.push_back(truth_particle);
		}
	      //std::cout <<"#2" << std::endl;
	      if(truth_particle->isW())
		{
		  W_particles_in_event.push_back(truth_particle);
		  bool hasSelfChild = false;
		  std::cout <<"is W" << std::endl;
		  std::cout <<"W->nChildren() " << truth_particle->nChildren() << std::endl;
		  for(unsigned int it = 0; it < truth_particle->nChildren(); ++it){
		    std::cout<<"inside loop"<<std::endl;
		    std::cout << "child(it): " << truth_particle->child(it) << std::endl;
		    int daughter_pdgId = truth_particle->child(it)->pdgId();
		    std::cout <<"daughter id: " << daughter_pdgId <<std::endl;
		    if( daughter_pdgId == truth_particle->pdgId() ){
		      //Info("execute()", "\t\tIt contains itself. Skipping it.");
		      hasSelfChild = true;
		      break;
		    }
		    // std::cout <<"inside loop 2" << std::endl;
		    
		    if(daughter_pdgId==1 ||daughter_pdgId==2 || daughter_pdgId==3 || daughter_pdgId==4)
		      {  
			daughter_W_particles_cand.push_back(truth_particle);
			std::cout << "we have a daughter" << std::endl;
		      }
		    //std::cout <<"inside loop 3" << std::endl;
		  }
		  
		}
	      //std::cout << "#3" <<std::endl;
	      if(truth_particle->isTop())
		{
		  tops_in_event.push_back(truth_particle);
		}
	    }
	  //std::cout <<"#4" <<std::endl;
	  for (const auto truth_particle: *in_truth)
	    {	   
	      int num_jet =0;
	      


	      for(const auto& jet: *in_jetsLargeR)
		{
		  //std::cout <<"#5" <<std::endl;
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
			      //      if (event_num<10)
			      //{
				  //std::cout <<"Event #" << event_num << "\tJet in event #" << num_jet << "\tphi: "<<jet->phi() <<"\trapidity: " << jet->rapidity() << std::endl;
				  //std::cout <<"\tTop # " << num_top << "\trapidity: " << (*k)->rapidity() << "\tphi: " << (*k)->phi() <<std::endl;
			      //}
			    }
			}
		    }
		  //std::cout <<"#6" <<std::endl;
		  tops_in_event.remove(top_particle(*jet));

		  float Min_deltaR_for_this_jet = 1000.;
		  std::list<const xAOD::TruthParticle*>::iterator i;
		  for(i=W_particles_in_event.begin(); i != W_particles_in_event.end(); ++i)
		    {  
		      num_W_in_event++;
		      float deltaR = xAOD::P4Helpers::deltaR(jet, *i);
		      //if (event_num < 10)
			//{
			  //std::cout <<"\tW#" << num_W_in_event<< "\tphi: " << (*i)->phi() << "\trapidity: " << (*i)->rapidity() << std::endl;  
			  //std::cout <<"\tdeltaR between jet #" << num_jet << " and W #" << num_W_in_event << " = " << deltaR << std::endl;
			  //} 
		      if (deltaR < Min_deltaR_for_this_jet)
			{
			  Min_deltaR_for_this_jet = deltaR;
			  W_particle(*jet) = *i;
			  hasW(*jet) = true;
			}
		    }
		  //std::cout <<"#7" <<std::endl;
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

		      //      if (event_num < 10)
		      //{
			  //std::cout << "\tB in event: " << "\tphi: " << (*j)->phi() << "\trapidity: " << (*j)->rapidity() << std::endl;
			  //std::cout << "\tdeltaR_W_b = " << xAOD::P4Helpers::deltaR(W_particle(*jet),*j) << std::endl;
		      //}
		      if (deltaR < deltaR_W_b_min)
			{
			  deltaR_W_b_min = deltaR;
			  b_particle(*jet) = *j;
			  hasB(*jet) = true;
			}
		      //	   }
		      // }	
		    }
		  //std::cout <<"#8" <<std::endl;
		  if (b_particle(*jet)!=NULL)
		    deltaR_W_b(*jet) = xAOD::P4Helpers::deltaR(jet,b_particle(*jet));
		  else
		    deltaR_W_b(*jet) = 1000;
		  b_particles_in_event.remove(b_particle(*jet));
		  
		  //std::cout <<"#10"<<std::endl;
		  if (W_particle(*jet)!=NULL)
		    {
		      std::list<const xAOD::TruthParticle*>::iterator l;
		      std::list<const xAOD::TruthParticle*>::iterator m;
		      float min_quark_pt = 13000;
		      //std::cout <<"BEFORE " << std::endl;
		      for(l=daughter_W_particles_cand.begin(); l != daughter_W_particles_cand.end(); ++l)
			{
			  //std::cout <<"(*l)->pt()/1000. = " << (*l)->pt()/1000. << std::endl;
			  if ((*l)->pt()/1000. < min_quark_pt)
			    {
			      min_quark_pt = (*l)->pt()/1000.;
			      W_quark1(*jet) = *l;
			    }
			}

		      daughter_W_particles_cand.remove(W_quark1(*jet));
		      for(m=daughter_W_particles_cand.begin(); m != daughter_W_particles_cand.end(); ++m)
			{
			  if ((*m)->pt()/1000. < min_quark_pt)
			    {
			      min_quark_pt = (*m)->pt()/1000.;
			      W_quark2(*jet) = *m;
			    }
			}
		     
		      
		    }		 
		  //std::cout << "#11"<<std::endl;
		  
		}
	    }
	  
	 
	  for(const auto& jet: *in_jetsLargeR)
	    {
	      //if (event_num<10)
		//std::cout <<"deltaR_W_jet = " << deltaR_W_jet(*jet) << std::endl;
		//std::cout <<"a" << std::endl;
	      if (deltaR_W_jet(*jet) <= 0.3 && hasW(*jet))
		{
		 
		  //std::cout <<"ContainsWjet" << std::endl;
		  containsTruthW(*jet) = true;
		}
	      //std::cout <<"b" << std::endl;
	      if(hasB(*jet))
		{
		  float deltaR = xAOD::P4Helpers::deltaR(jet, b_particle(*jet));
		  if (deltaR >= 1.0)
		    notContainedB(*jet) = true;
		}
	      //std::cout << "c" <<std::endl;
	      if (containsTruthW(*jet) && containsTruthTop(*jet))
		{
		  deltaR_W_top(*jet) = xAOD::P4Helpers::deltaR(W_particle(*jet),top_particle(*jet));
		}
	      //std::cout<< "d" <<std::endl;
	     
	      if (containsTruthW(*jet) && notContainedB(*jet) && containsTruthTop(*jet))
		{
		  deltaR_W_top_semiboosted(*jet) = xAOD::P4Helpers::deltaR(W_particle(*jet),top_particle(*jet));
		}
	      //std::cout <<"e" <<std::endl;
	    }

          for(const auto& jet: *in_jetsLargeR)
            {
	      if (containsTruthW(*jet) && notContainedB(*jet))
		WTAG(*jet) = true;
	    }
	  
	}
    }
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TruthMatching :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TruthMatching :: histFinalize () { return EL::StatusCode::SUCCESS; }
