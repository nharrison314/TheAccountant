#ifndef TheAccountant_ROC_H
#define TheAccountant_ROC_H

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

//P4Helper includes
#include "FourMomUtils/xAODP4Helpers.h"

namespace TheAccountant
{
  class ROC : public HistogramManager
  {
    
  public:
    ROC(std::string name);
    virtual ~ROC() ;
    
    StatusCode initialize();
StatusCode execute (const xAOD::EventInfo* eventInfo, const xAOD::MissingET* in_met, const xAOD::JetContainer* in_jets, const xAOD::JetContainer* in_jetsLargeR, const float eventWeight);
    
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
