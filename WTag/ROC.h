#ifndef WTag_ROC_H
#define WTag_ROC_H

#include <xAODAnaHelpers/HistogramManager.h>
// Infrastructure include(s):                                                   
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODJet/JetContainer.h"
#include <string>

namespace WTag
{
  class ROC : public HistogramManager
  {
    
  public:
    ROC(std::string name);
    virtual ~ROC() ;
    
    StatusCode initialize();
    StatusCode execute (const xAOD::EventInfo* eventInfo, float eventWeight);
    
    bool m_passPreSel        = true;
    bool m_debug = false;
    std::string m_eventInfo       = "EventInfo",
      m_inputLargeRJets       = "",
      m_inputJets      = "",
      m_inputMET        = "MET_RefFinal",
      m_inputMETName    = "Final",
      m_inputElectrons  = "",
      m_inputMuons      = "",
      m_inputTauJets    = "",
      m_inputPhotons    = "",
      m_truth           = "";
    std::string m_decor_jetTags_b   = "", // isB,                                                 
      m_decor_jetTags_top = "", // isTop,                                               
      m_decor_jetTags_w   = ""; // isW;                                         

    using HistogramManager::book;
    using HistogramManager::execute;
  
  private:
    xAOD::TEvent *m_event; //!                                                                    
    xAOD::TStore *m_store; //!   
    
    //float positive, fake, totalEvents, trueRate, fakeRate;
    TH1F* jetmass1;
    TH1F* jetmass2;
    TH1F* jetmass3;
    TH1F* jetmass4;
    TH1F* jetmass1_Wlabel;
    TH1F* jetmass2_Wlabel;
    TH1F* jetmass3_Wlabel;
    TH1F* jetmass4_Wlabel;
    
    
  };
}
#endif
