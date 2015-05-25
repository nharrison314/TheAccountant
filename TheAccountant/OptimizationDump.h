#ifndef TheAccountant_OptimizationDump_H
#define TheAccountant_OptimizationDump_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// root includes
#include <TTree.h>

// c++ includes
#include <string>

class OptimizationDump : public EL::Algorithm
{

public:
  // standard across all algorithms for configuration
  bool m_debug = false;
  std::string m_eventInfo       = "EventInfo",
              m_inputLargeRJets       = "",
              m_inputJets      = "",
              m_inputMET        = "MET_RefFinal",
              m_inputElectrons  = "",
              m_inputMuons      = "",
              m_inputTauJets    = "",
              m_inputPhotons    = "";
  std::string m_decor_jetTags_b   = "", // isB,
              m_decor_jetTags_top = "", // isTop,
              m_decor_jetTags_w   = ""; // isW;

private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!
  TTree* m_tree; //!

  // everything below here is filled in the ttree
  float m_eventWeight; //!
  float m_effectiveMass; //!
  float m_totalTransverseMomentum; //!
  float m_totalTransverseMass; //!
  int m_numJets; //!
  int m_numJetsLargeR; //!

  /* tagging */
  int m_n_topTag_Loose; //!
  int m_n_topTag_Medium; //!
  int m_n_topTag_Tight; //!

public:
  // this is a standard constructor
  OptimizationDump ();

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
  ClassDef(OptimizationDump, 1);
};

#endif