#ifndef WTag_METHists_H
#define WTag_METHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODMissingET/MissingETContainer.h>

namespace WTag
{
  class METHists : public HistogramManager
  {
    public:
      METHists(std::string name);
      virtual ~METHists() ;
      StatusCode initialize();
      StatusCode execute( const xAOD::MissingET*, float eventWeight);
      using HistogramManager::book; // make other overloaded version of book() to show up in subclass
      using HistogramManager::execute; // overload

    private:
      TH1F* m_mpx;                //!
      TH1F* m_mpy;                //!
      TH1F* m_MET;                //!
      TH1F* m_phi;                //!
      TH1F* m_sumet;              //!
  };
}
#endif
