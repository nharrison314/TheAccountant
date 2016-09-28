#ifndef TheAccountant_TruthMatching_H
#define TheAccountant_TruthMatching_H

#include <TheAccountant/Algorithm.h>
#include "xAODTruth/TruthParticleContainer.h"

// Infrastructure include(s):
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <TheAccountant/VariableDefinitions.h>


namespace VD = VariableDefinitions;
//namespace TA = TheAccountant

#include <xAODAnaHelpers/HistogramManager.h>
// Infrastructure include(s):                                                                                                  
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include <string>
#include "TheAccountant/VariableDefinitions.h"                                                                                                  
#include <string>          
#include "FourMomUtils/xAODP4Helpers.h"

namespace TruthMatchingVars
{  
  template <typename T>
using decor_t = typename SG::AuxElement::Decorator<T>;
  template <typename T>
  using accessor_t = typename SG::AuxElement::ConstAccessor<T>;

  static accessor_t<bool> acc_containsTruthW("containsTruthW");
  static accessor_t<bool> acc_notContainedB("notContainedB");
  static accessor_t<bool> acc_semiBoostedW("semiBoostedW");
  static accessor_t<bool> acc_containsTruthTop("containsTruthTop");
  static accessor_t<bool> acc_hasB("hasB");
  static accessor_t<bool> acc_hasW("hasW");
  static accessor_t<bool> acc_deltaR_W_jet("deltaR_W_jet");
  static accessor_t<bool> acc_deltaR_W_b("deltaR_W_b");
  static accessor_t<bool> acc_deltaR_W_top("deltaR_W_top");
  static accessor_t<bool> acc_deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");

  static accessor_t<const xAOD::TruthParticle*> acc_b_particle("b_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_W_particle("W_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_top_particle("top_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_W_quark1("W_quark1");
  static accessor_t<const xAOD::TruthParticle*> acc_W_quark2("W_quark2");
  static accessor_t<const xAOD::TruthParticle*> acc_top_quark("top_quark");

  static decor_t<bool> decor_containsTruthW("containsTruthW");
  static decor_t<bool> decor_notContainedB("notContainedB");
  static decor_t<bool> decor_semiBoostedW("semiBoostedW");
  static decor_t<bool> decor_containsTruthTop("containsTruthTop");
  static decor_t<bool> decor_hasB("hasB");
  static decor_t<bool> decor_hasW("hasW");
  static decor_t<bool> decor_deltaR_W_jet("deltaR_W_jet");
  static decor_t<bool> decor_deltaR_W_b("deltaR_W_b");
  static decor_t<bool> decor_deltaR_W_top("deltaR_W_top");
  static decor_t<bool> decor_deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");

  static decor_t<const xAOD::TruthParticle*> decor_b_particle("b_particle");
  static decor_t<const xAOD::TruthParticle*> decor_W_particle("W_particle");
  static decor_t<const xAOD::TruthParticle*> decor_top_particle("top_particle");
  static decor_t<const xAOD::TruthParticle*> decor_W_quark1("W_quark1");
  static decor_t<const xAOD::TruthParticle*> decor_W_quark2("W_quark2");
  static decor_t<const xAOD::TruthParticle*> decor_top_quark("top_quark");
}

namespace TheAccountant
{

  class TruthMatching : public HistogramManager
    {
      
    public:
      int event_num;
      
    public:
      // this is a standard constructor
      TruthMatching (std::string name);
      virtual ~TruthMatching() ;
      
      StatusCode initialize();
      StatusCode execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* in_met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, const xAOD::TruthParticleContainer* in_truth, const float eventWeight);

      std::string m_decor_jetTags_b   = "", // isB,                                                            
	m_decor_jetTags_top = "", // isTop,                                                              
	m_decor_jetTags_w   = ""; // isW;                                                                                        
      
      using HistogramManager::book;
      using HistogramManager::execute;
      
    private:
      TH1F* jetmass1;
      TH1F* jetmass2;
      TH1F* jetmass3;
      TH1F* jetmass4;
      TH1F* jetmass1_Wlabel;
      TH1F* jetmass2_Wlabel;
      TH1F* jetmass3_Wlabel;
      TH1F* jetmass4_Wlabel;
      TH1F* tops;
      
      
      TH1F* deltaR_W_b_jet1;
      TH1F* deltaR_W_jet_jet1;
      TH1F* deltaR_W_b_jet2;
      TH1F* deltaR_W_jet_jet2;
      TH1F* deltaR_W_b_jet3;
      TH1F* deltaR_W_jet_jet3;
      TH1F* deltaR_W_b_jet4;
      TH1F* deltaR_W_jet_jet4;
      
      
      TH1F* deltaR_W_jet_semiboosted_jet1;
      TH1F* deltaR_W_b_semiboosted_jet1;
      TH1F* deltaR_W_jet_semiboosted_jet2;
      TH1F* deltaR_W_b_semiboosted_jet2;
      TH1F* deltaR_W_jet_semiboosted_jet3;
      TH1F* deltaR_W_b_semiboosted_jet3;
      TH1F* deltaR_W_jet_semiboosted_jet4;
      TH1F* deltaR_W_b_semiboosted_jet4;
      
      TH1F* deltaR_W_top_semiboosted_jet1;
      TH1F* deltaR_W_top_semiboosted_jet2;
      TH1F* deltaR_W_top_semiboosted_jet3;
      TH1F* deltaR_W_top_semiboosted_jet4;
      
      TH1F* deltaR_W_top_jet1;
      TH1F* deltaR_W_top_jet2;
      TH1F* deltaR_W_top_jet3;
      TH1F* deltaR_W_top_jet4;
      
      TH2F* deltaR_W_jet1_fn_top_pt;
      TH2F* deltaR_W_jet2_fn_top_pt;
      TH2F* deltaR_W_jet3_fn_top_pt;
      TH2F* deltaR_W_jet4_fn_top_pt;
      
      TH1F* deltaEta_W_quark1_jet1;
      TH1F* deltaEta_W_quark1_jet2;
      TH1F* deltaEta_W_quark1_jet3;
      TH1F* deltaEta_W_quark1_jet4;
      
      TH1F* deltaPhi_W_quark1_jet1;
      TH1F* deltaPhi_W_quark1_jet2;
      TH1F* deltaPhi_W_quark1_jet3;
      TH1F* deltaPhi_W_quark1_jet4;
      
      
      

    };
}

#endif
