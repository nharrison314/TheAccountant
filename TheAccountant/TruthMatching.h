#ifndef TheAccountant_TruthMatching_H
#define TheAccountant_TruthMatching_H

#include <TheAccountant/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


// c++ includes
#include <string>

class TruthMatching : public TA::Algorithm
{

public:
  int event_num;

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
