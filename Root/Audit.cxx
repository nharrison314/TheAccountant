#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <TheAccountant/Audit.h>

// RestFrames includes
#include "RestFrames/FramePlot.hh"

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
namespace RF = RestFrames;

// this is needed to distribute the algorithm to the workers
ClassImp(Audit)
Audit :: Audit () :
  LAB("LAB", "lab"),
  SS("SS", "SS"),
  S1("S1", "#tilde{S}_{a}"),
  S2("S2", "#tilde{S}_{b}"),
  V1("V1", "V_{a}"),
  V2("V2", "V_{b}"),
  I1("I1", "I_{a}"),
  I2("I2", "I_{b}"),
  VIS("VIS", "Visible Object Jigsaws"),
  INV("INV", "Invisible Frame Jigsaws"),
  MinMassJigsaw("MINMASS_JIGSAW", "Invisible system mass Jigsaw"),
  RapidityJigsaw("RAPIDITY_JIGSAW", "Invisible system rapidity Jigsaw"),
  ContraBoostJigsaw("CB_JIGSAW", "Contraboost invariant Jigsaw"),
  HemiJigsaw("HEM_JIGSAW", "Minimize m _{V_{a,b}} Jigsaw")
{}

EL::StatusCode Audit :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("Audit").ignore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Audit :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Audit :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Audit :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode Audit :: initialize () {
  // assign m_event and m_store
  event_num =0;
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // create the decay structure
  LAB.SetChildFrame(SS);
  SS.AddChildFrame(S1);
  SS.AddChildFrame(S2);
  S1.AddChildFrame(V1);
  S1.AddChildFrame(I1);
  S2.AddChildFrame(V2);
  S2.AddChildFrame(I2);

  // The invisible group is all of the weakly interacting particles
  INV.AddFrame(I1);
  INV.AddFrame(I2);

  // the combinatoric group is the list of visible objects
  // that go into our hemispheres
  VIS.AddFrame(V1);
  VIS.AddFrame(V2);
  // require at least one element in each hemisphere
  VIS.SetNElementsForFrame(V1,1,false);
  VIS.SetNElementsForFrame(V2,1,false);

  if(LAB.InitializeTree()){ Info("initialize()", "We do have consistent tree topology."); }
  else                    { Error("initialize()", "We do not have consistent tree topology."); return EL::StatusCode::FAILURE; }

  // now we define 'jigsaw rules' that tell the tree how to define the objects
  // in our groups, indented to cleanly see the Jigsaws being defined
  if(m_minMassJigsaw){
    INV.AddJigsaw(MinMassJigsaw);
  }

  if(m_rapidityJigsaw){
    INV.AddJigsaw(RapidityJigsaw);
    RapidityJigsaw.AddVisibleFrame((LAB.GetListVisibleFrames()));
  }

  if(m_contraBoostJigsaw){
    INV.AddJigsaw(ContraBoostJigsaw);
    ContraBoostJigsaw.AddVisibleFrame((S1.GetListVisibleFrames()), 0);
    ContraBoostJigsaw.AddVisibleFrame((S2.GetListVisibleFrames()), 1);
    ContraBoostJigsaw.AddInvisibleFrame((S1.GetListInvisibleFrames()), 0);
    ContraBoostJigsaw.AddInvisibleFrame((S2.GetListInvisibleFrames()), 1);
  }

  if(m_hemiJigsaw){
    VIS.AddJigsaw(HemiJigsaw);
    HemiJigsaw.AddFrame(V1,0);
    HemiJigsaw.AddFrame(V2,1);
  }

  if(LAB.InitializeAnalysis()){ Info("initialize()", "Our tree is ok for analysis."); }
  else                        { Error("initialize()", "Our tree is not ok for analysis."); return EL::StatusCode::FAILURE; }

  // only output this thing once, perhaps only during direct
  if(m_drawDecayTreePlots){
    RF::FramePlot* decayTree_plot = new RF::FramePlot("tree", "Decay Tree");
    // start with the lab frame
    decayTree_plot->AddFrameTree(LAB);
    // add the jigsaws
    if(m_minMassJigsaw)     decayTree_plot->AddJigsaw(MinMassJigsaw);
    if(m_rapidityJigsaw)    decayTree_plot->AddJigsaw(RapidityJigsaw);
    if(m_contraBoostJigsaw) decayTree_plot->AddJigsaw(ContraBoostJigsaw);
    if(m_hemiJigsaw)        decayTree_plot->AddJigsaw(HemiJigsaw);
    decayTree_plot->DrawFramePlot();
    TCanvas* plotCanvas = decayTree_plot->GetCanvas();
    plotCanvas->SetName("decayTree");
    wk()->addOutput(plotCanvas);

    RF::FramePlot* visGroup_plot = new RF::FramePlot("VIStree", "Visible Group");
    visGroup_plot->AddGroupTree(VIS);
    visGroup_plot->DrawFramePlot();
    TCanvas* visPlotCanvas = visGroup_plot->GetCanvas();
    visPlotCanvas->SetName("visTree");
    wk()->addOutput(visPlotCanvas);

    RF::FramePlot* invGroup_plot = new RF::FramePlot("INVtree", "Invisible Group");
    invGroup_plot->AddGroupTree(INV);
    invGroup_plot->DrawFramePlot();
    TCanvas* invPlotCanvas = invGroup_plot->GetCanvas();
    invPlotCanvas->SetName("invTree");
    wk()->addOutput(invPlotCanvas);
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Audit :: execute ()
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
  const xAOD::TruthParticleContainer*   in_truth    (nullptr);

  event_num++;
  //std::cout << "event_num: " << event_num << std::endl;
  //std::cout << "Audit: Here #1" << std::endl;
  // start grabbing all the containers that we can
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

  /////////////////////////////////////////////////////////////////////////
  if(!m_truthParticles.empty())
    RETURN_CHECK("Audit::execute()", HF::retrieve(in_truth, m_truthParticles, m_event, m_store, m_debug), "Could not get the truthParticles container.");
  //std::cout << "Audit: Here #2" << std::endl;
  const xAOD::MissingET* in_met(nullptr);
  if(!m_inputMET.empty()){
    in_met = (*in_missinget)[m_inputMETName.c_str()];
    if (!in_met) {
      Error("execute()", "No %s inside MET container", m_inputMETName.c_str());
      return EL::StatusCode::FAILURE;
    }
  }


  //truth matching decorators. Decorators are jet level. 
  int i=0;
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
	     int pdgId = abs(truth_particle->pdgId());
	     if(pdgId ==5 || pdgId==-5)
	       {
		 b_particles_in_event.push_back(truth_particle);
	       }
	     if(truth_particle->isW())
	       {
		 W_particles_in_event.push_back(truth_particle);
	       }
	     if(truth_particle->isTop())
	       {
		 tops_in_event.push_back(truth_particle);
	       }
	     if(pdgId==1 ||pdgId==2 || pdgId==3 || pdgId==4 ||  pdgId==-1 ||pdgId==-2 || pdgId==-3 || pdgId==-4)
	       {  
		 daughter_W_particles_cand.push_back(truth_particle);
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
		 list<const xAOD::TruthParticle*>::iterator k;
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
	       
		 tops_in_event.remove(top_particle(*jet));

		 float Min_deltaR_for_this_jet = 1000.;
		 list<const xAOD::TruthParticle*>::iterator i;
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
			 //	 if(Min_deltaR_for_this_jet < 0.3)
			 //{		
			 W_particle(*jet) = *i;
			 hasW(*jet) = true;
			 //  containsTruthW(*jet) = true;
			 //}
		       }
		   }
		 if (W_particle(*jet)!=NULL)
		   deltaR_W_jet(*jet) = xAOD::P4Helpers::deltaR(jet,W_particle(*jet));
		 else
		   deltaR_W_jet(*jet) = 1000;
		 W_particles_in_event.remove(W_particle(*jet));
		 

		 list<const xAOD::TruthParticle*>::iterator j;
		 float deltaR_W_b_min = 1000.;
		 for(j=b_particles_in_event.begin(); j != b_particles_in_event.end(); ++j)
		   {
		     float deltaR = xAOD::P4Helpers::deltaR(jet, *j);
		     //if (deltaR >=1)
		     //{		     
		     //	 notContainedB(*jet) = true;
		     //
		     //	 if (containsTruthW(*jet) == true)
		     //	   {
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
		 if (b_particle(*jet)!=NULL)
		   deltaR_W_b(*jet) = xAOD::P4Helpers::deltaR(jet,b_particle(*jet));
		 else
		   deltaR_W_b(*jet) = 1000;
		 b_particles_in_event.remove(b_particle(*jet));
		 


		 list<const xAOD::TruthParticle*>::iterator l;
		 list<const xAOD::TruthParticle*>::iterator m;
		 float deltaR_W_quark1_min = 1000.;
		 if (W_particle(*jet)!=NULL)
		   {
		     for(l=daughter_W_particles_cand.begin(); l != daughter_W_particles_cand.end(); ++l)
		       {
			 float deltaR = xAOD::P4Helpers::deltaR(W_particle(*jet), *l);
			 if (deltaR<deltaR_W_quark1_min)
			   {
			     deltaR_W_quark1_min = deltaR;
			     if(deltaR<1)
			       W_quark1(*jet) = *l;
			   }
		       }
		     daughter_W_particles_cand.remove(W_quark1(*jet));
		   
		     for(m=daughter_W_particles_cand.begin(); m != daughter_W_particles_cand.end(); ++m)
		       {
			 float deltaR = xAOD::P4Helpers::deltaR(W_particle(*jet), *m);
			 if (deltaR<deltaR_W_quark1_min)
			   {
			     deltaR_W_quark1_min = deltaR;
			     if(deltaR<1)
			       W_quark2(*jet) = *l;
			   }
		       }  

		     
		     
		   } 
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
      

 // for(const auto& jet: *in_jetsLargeR)
 //   {
 //     containsTruthW(*jet) = false;
 //     containsTruthTop(*jet) = false;
 //     if (!in_truth->size()==0)
 //       {
 // 	 for (const auto truth_particle: *in_truth){
 //  	    int pdgId = abs(truth_particle->pdgId());
 // 	    // first ask if it is top, perform deltaR calculation, either call top or not
 //  	    if(truth_particle->isTop())
 //  	      {
 // 		float deltaR = xAOD::P4Helpers::deltaR(jet, truth_particle);
 // 		if (deltaR < 1)
 // 		  containsTruthTop(*jet) = true;
 // 	      }
 // 	    else if (truth_particle->isW())
 // 	      {
 // 		float deltaR = xAOD::P4Helpers::deltaR(jet, truth_particle);
 // 		if (deltaR < 0.8)
 // 		  containsTruthW(*jet) = true;
 // 	      }

 // 	    std::cout <<"containsTruthTop(*jet) = " << containsTruthTop(*jet) << std::endl;
 // 	 }
 //       }

 //   }


  // for(const auto& jet: *in_jetsLargeR)
  //   {
  //     std::vector<const xAOD::TruthParticle*> associated_truthParticles;
  //     containsTruthW(*eventInfo) = false;
  //     if (jet->getAssociatedObjects<xAOD::TruthParticle>("Truth",associated_truthParticles))
  // 	{
  // 	  std::cout <<"Audit: inside in_jetsLargeR loop. Jet does have associated truth particles" << std::endl;
  // 	  std::cout <<"Audit: There are " << associated_truthParticles.size() << " truth particles" << std::endl;
  // 	//	if (!associated_truthParticles.isValid())
  // 	for (const auto& truth_particle: associated_truthParticles)
  // 	  {
  // 	    std::cout <<"Audit: Looping through truth particles" << std::endl;
  // 	    containsTruthW(*eventInfo) |= truth_particle->isW();
  // 	    std::cout <<"containsTruthW is " << containsTruthW(*eventInfo) << std::endl;
  // 	  }
  // 	}
  //   }


  // define razor decorations
  static SG::AuxElement::Decorator<float> SS_mass_decor             ("SS_mass");
  static SG::AuxElement::Decorator<float> SS_invgamma_decor         ("SS_invgamma");
  static SG::AuxElement::Decorator<float> SS_dphivis_decor          ("SS_dphivis");
  static SG::AuxElement::Decorator<float> SS_costheta_decor         ("SS_costheta");
  static SG::AuxElement::Decorator<float> SS_dphidecayangle_decor   ("SS_dphidecayangle");
  static SG::AuxElement::Decorator<float> SS_mdeltaR_decor          ("SS_mdeltaR");
  static SG::AuxElement::Decorator<float> S1_mass_decor             ("S1_mass");
  static SG::AuxElement::Decorator<float> S2_mass_decor             ("S2_mass");
  static SG::AuxElement::Decorator<float> S1_costheta_decor         ("S1_costheta");
  static SG::AuxElement::Decorator<float> S2_costheta_decor         ("S2_costheta");
  static SG::AuxElement::Decorator<float> I1_depth_decor            ("I1_depth");
  static SG::AuxElement::Decorator<float> I2_depth_decor            ("I2_depth");
  static SG::AuxElement::Decorator<float> V1_nelements_decor        ("V1_nelements");
  static SG::AuxElement::Decorator<float> V2_nelements_decor        ("V2_nelements");
  // initialize to zero on event
  SS_mass_decor(*eventInfo)             = -99;
  SS_invgamma_decor(*eventInfo)         = -99;
  SS_dphivis_decor(*eventInfo)          = -99;
  SS_costheta_decor(*eventInfo)         = -99;
  SS_dphidecayangle_decor(*eventInfo)   = -99;
  SS_mdeltaR_decor(*eventInfo)          = -99;
  S1_mass_decor(*eventInfo)             = -99;
  S2_mass_decor(*eventInfo)             = -99;
  S1_costheta_decor(*eventInfo)         = -99;
  S2_costheta_decor(*eventInfo)         = -99;
  I1_depth_decor(*eventInfo)            = -99;
  I2_depth_decor(*eventInfo)            = -99;
  V1_nelements_decor(*eventInfo)        = -99;
  V2_nelements_decor(*eventInfo)        = -99;

  //isWlabel                              = false;


  // clear the event
  LAB.ClearEvent();

  // create a vector to hold the group element ids for when adding jets
  std::map<const RF::GroupElementID, const xAOD::Jet*> in_jets_IDs;
  if(!m_inputJets.empty()){
    for(const auto &jet: *in_jets)
      in_jets_IDs[VIS.AddLabFrameFourVector( jet->p4() )] = jet;
  }

  if(!m_inputMET.empty()){
    // no mpz, but why set it this way???
    INV.SetLabFrameThreeVector(  TVector3( in_met->mpx(), in_met->mpy(), 0 ) );
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

  // analyze the event
  if(LAB.AnalyzeEvent()){ if(m_debug) Info("execute()", "Analyzed the event successfully."); }
  else                  { Error("execute()", "Run #%u, Event #%llu: Analyzed the event unsuccessfully.", eventInfo->runNumber(), eventInfo->eventNumber()); return EL::StatusCode::SUCCESS; }

  if(m_debug){
    Info("execute()", "Details about RestFrames analysis...");
    Info("execute()", "\tSS...");
    Info("execute()", "\t\tMass:          %0.2f GeV", SS.GetMass()/1000.);
    Info("execute()", "\t\tInvGamma:      %0.2f", 1./SS.GetGammaInParentFrame());
    Info("execute()", "\t\tdPhiVis:       %0.2f", SS.GetDeltaPhiBoostVisible());
    Info("execute()", "\t\tCosTheta:      %0.2f", SS.GetCosDecayAngle());
    Info("execute()", "\t\tdPhiDecayAngle:%0.2f", SS.GetDeltaPhiDecayAngle());
    Info("execute()", "\t\tVisShape:      %0.2f", SS.GetVisibleShape());
    Info("execute()", "\t\tMDeltaR:       %0.2f", SS.GetVisibleShape()*SS.GetMass());
    Info("execute()", "\tS1...");
    Info("execute()", "\t\tMass:          %0.2f GeV", S1.GetMass()/1000.);
    Info("execute()", "\t\tCosTheta:      %0.2f", S1.GetCosDecayAngle());
    Info("execute()", "\tS2...");
    Info("execute()", "\t\tMass:          %0.2f GeV", S2.GetMass()/1000.);
    Info("execute()", "\t\tCosTheta:      %0.2f", S2.GetCosDecayAngle());
    Info("execute()", "\tI1...");
    Info("execute()", "\t\tDepth:         %d",    S1.GetFrameDepth(I1));
    Info("execute()", "\tI2...");
    Info("execute()", "\t\tDepth:         %d",    S2.GetFrameDepth(I2));
    Info("execute()", "\tV1...");
    Info("execute()", "\t\tNElements:     %d",    VIS.GetNElementsInFrame(V1));
    Info("execute()", "\tV2...");
    Info("execute()", "\t\tNElements:     %d",    VIS.GetNElementsInFrame(V2));
  }

  SS_mass_decor(*eventInfo)             = SS.GetMass();
  SS_invgamma_decor(*eventInfo)         = 1/SS.GetGammaInParentFrame();
  SS_dphivis_decor(*eventInfo)          = SS.GetDeltaPhiBoostVisible();
  SS_costheta_decor(*eventInfo)         = SS.GetCosDecayAngle();
  SS_dphidecayangle_decor(*eventInfo)   = SS.GetDeltaPhiDecayAngle();
  SS_mdeltaR_decor(*eventInfo)          = SS.GetVisibleShape()*SS.GetMass();
  S1_mass_decor(*eventInfo)             = S1.GetMass();
  S2_mass_decor(*eventInfo)             = S2.GetMass();
  S1_costheta_decor(*eventInfo)         = S1.GetCosDecayAngle();
  S2_costheta_decor(*eventInfo)         = S2.GetCosDecayAngle();
  I1_depth_decor(*eventInfo)            = S1.GetFrameDepth(I1);
  I2_depth_decor(*eventInfo)            = S2.GetFrameDepth(I2);
  V1_nelements_decor(*eventInfo)        = VIS.GetNElementsInFrame(V1);
  V2_nelements_decor(*eventInfo)        = VIS.GetNElementsInFrame(V2);

  /* TODO
     QCD rejection stuff
  */
 return EL::StatusCode::SUCCESS;
}

EL::StatusCode Audit :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Audit :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Audit :: histFinalize () { return EL::StatusCode::SUCCESS; }
