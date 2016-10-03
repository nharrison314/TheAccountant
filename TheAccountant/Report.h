#ifndef TheAccountant_Report_H
#define TheAccountant_Report_H

#include <TheAccountant/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "xAODEventInfo/EventInfo.h"

// plots to include
#include <TheAccountant/IParticleKinematicHists.h>
#include <TheAccountant/JetHists.h>
#include <TheAccountant/JetMETHists.h>
#include <TheAccountant/METHists.h>
#include <TheAccountant/JetTagHists.h>

#include <TheAccountant/RazorVariableHists.h>
class Report : public TA::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  bool m_passPreSel = false;
  int m_numLeadingJets = 0;

  float m_jet_minPtView,
        m_jetLargeR_minPtView,
        m_jet_maxAbsEtaView,
        m_jetLargeR_maxAbsEtaView;

private:
  std::map< std::string, TheAccountant::IParticleKinematicHists* > m_jetKinematicPlots; //!
  std::map< std::string, TheAccountant::JetHists* > m_jetPlots; //!
  std::map< std::string, TheAccountant::JetMETHists* > m_jetMETPlots; //!
  std::map< std::string, TheAccountant::JetTagHists* > m_jetTagPlots; //!
  std::map< std::string, TheAccountant::METHists* > m_METPlots; //!
  std::map< std::string, TheAccountant::RazorVariableHists* > m_razorPlots; //!
  std::map< std::string, TheAccountant::ROC* > m_rocPlots; //!

  std::vector<std::string> m_topTagDecorationNames; //!

public:
  // this is a standard constructor
  Report ();
  Report (std::string name, std::string configName);

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
  ClassDef(Report, 1);
};

#endif
