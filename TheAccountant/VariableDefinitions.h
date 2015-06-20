#ifndef TheAccountant_VariableDefinitions_H
#define TheAccountant_VariableDefinitions_H

// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMissingET/MissingETContainer.h"

/* Caveats: input containers are assumed sorted */
namespace VariableDefinitions {
  // for tagging primarily, but an enum for working points
  //  - an enum class enforces strong typing
  enum class WP {
    VeryLoose,
    Loose,
    Medium,
    Tight,
    None
  };
  // helper to convert to actual string for the tagging
  std::string wp2str(WP wp);
  WP str2wp(std::string str);

  // Effective Mass calculated using exclusive or inclusive definition depending on args
  //    Exclusive: Meff(met, jets, 4, 0, 0)
  //    Inclusive: Meff(met, jets, jets->size(), muons, els)
  float Meff(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int numJets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // HT calculated using all jets, will include muons and/or electrons if specified
  float HT(const xAOD::JetContainer* jets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // Transverse Mass calculated using MET and leading lepton
  float mT(const xAOD::MissingET* met, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // Calculates the minimum dPhi between the met and the container of particles passed in
  float dPhiMETMin(const xAOD::MissingET* met, const xAOD::IParticleContainer* particles);

  // Calculates the MET significance as MET/sqrt(HT)
  float METSignificance(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int njets);

  // top tagging on jets, set the eventInfo with "nTops_<WP>" int decoration
  //    - static SG::AuxElement::Accessor< int > nTops_wp("nTops_<WP>");
  //        * string of WP is equivalent to how you type it out in enum class
  int topTag(const xAOD::EventInfo* eventInfo, const xAOD::JetContainer* jets, WP wp);
  // top tagging on jet, set "isTop_<WP>" int decoration
  bool topTag(const xAOD::Jet* jet, WP wp);

  // b-tagging on jets, set the eventInfo with "nBJets_<WP>" int decoration
  //    - static SG::AuxElement::Accessor< int > nBJets_wp("nBJets_<WP>");
  //        * string of WP is equivalent to how you type it out in enum class
  int bTag(const xAOD::EventInfo* eventInfo, const xAOD::JetContainer* jets, WP wp);
  // b-tagging on jet, set "isB_<WP>" int decoration
  bool bTag(const xAOD::Jet* jet, WP wp);

  // returning subjettiness ratios for a given jet
  //    - use Tau21 if set
  //    - else use Tau2/Tau1 if both set
  //    - else recompute using Nsubjettiness
  float Tau21(const xAOD::Jet* jet);
  float Tau32(const xAOD::Jet* jet);
  void Nsubjettiness(const xAOD::Jet* jet, float alpha = 1.0);

  // returning splitting scale calculations for a given jet
  //    - use Split12 if set
  //    - else recompute using KtSplittingScale
  float Split12(const xAOD::Jet* jet);
  float Split23(const xAOD::Jet* jet);
  float Split34(const xAOD::Jet* jet);
  void KtSplittingScale(const xAOD::Jet* jet);
}

#endif
