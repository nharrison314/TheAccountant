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
#include <string>

namespace TheAccountant
{
  class ROC : public HistogramManager
  {
    
  public:
    ROC(std::string name);
    virtual ~ROC() ;
    
    StatusCode initialize();
    StatusCode execute (const xAOD::EventInfo* eventInfo, const xAOD::JetContainer* in_ffjets, const xAOD::JetContainer* in_jets, const xAOD::TruthParticleContainer* truth_particles, const float eventWeight);
    
    std::string m_decor_jetTags_b   = "", // isB,                          
      m_decor_jetTags_top = "", // isTop,                                               
      m_decor_jetTags_w   = ""; // isW;                                         

    using HistogramManager::book;
    using HistogramManager::execute;
  
  private:    
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
