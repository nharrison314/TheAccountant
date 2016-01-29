#ifndef TheAccountant_TruthMatching_H
#define TheAccountant_TruthMatching_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


// c++ includes
#include <string>

class TruthMatching : public EL::Algorithm
{

public:
  bool m_debug = false;
  std::string m_eventInfo       = "EventInfo",
    m_inputLargeRJets       = "",
    m_inputJets      = "",
    m_truthParticles  = "",
    m_inputMET = "",
    m_inputMETName = "",
    m_inputElectrons ="",
    m_inputMuons = "",
    m_inputTauJets = "",
    m_inputPhotons = "";
  std::string m_decor_jetTags_b  = "", // isB,
    m_decor_jetTags_top = "", // isTop,
    m_decor_jetTags_w   = ""; // isW;
  int event_num;

private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

public:
  // this is a standard constructor
  TruthMatching ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(TruthMatching, 1);
};

#endif
