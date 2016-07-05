#ifndef TheAccountant_VariableDefinitions_H
#define TheAccountant_VariableDefinitions_H

// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

// we want to use the metadata if we can
#include "SampleHandler/MetaObject.h"

/* Caveats: input containers are assumed unsorted */
namespace VariableDefinitions {

  // typedef it to make it easier to type: VD::accessor_t<char>
  // http://stackoverflow.com/a/19192151/1532974
  template <typename T>
  using decor_t = typename SG::AuxElement::Decorator<T>;
  template <typename T>
  using accessor_t = typename SG::AuxElement::ConstAccessor<T>;

  // global definitions for decorations
  static accessor_t<char> decor_baseline("baseline");
  static accessor_t<char> decor_passOverlap("passOR");
  static accessor_t<char> decor_signal("signal"); // from SUSYTools, wrong isolation
  static accessor_t<char> decor_signalIso("signal_pt_dependent_iso");
  static accessor_t<char> decor_cosmic("cosmic");
  static accessor_t<char> decor_bad("bad");

  // for tagging, preselect decorates and everyone else accesses
  static decor_t<char> decor_tag_b("isB");
  static decor_t<char> decor_tag_w("isW");
  static decor_t<char> decor_tag_top("isTop");
  static accessor_t<char> acc_tag_b("isB");
  static accessor_t<char> acc_tag_w("isW");
  static accessor_t<char> acc_tag_top("isTop");

  // loose tagging
  static decor_t< int > decor_tag_W_inc("isWinc");
  static decor_t< int > decor_tag_W_exc("isWexc");
  static decor_t< int > decor_tag_top_inc("isTopInc");
  static accessor_t< int > acc_tag_W_inc("isWinc");
  static accessor_t< int > acc_tag_W_exc("isWexc");
  static accessor_t< int > acc_tag_top_inc("isTopInc");


  // for preselections to make it consistent and uniform
  static decor_t< int > dec_pass_preSel("pass_preSel");
  static decor_t< int > dec_pass_preSel_b("pass_preSel_b");
  static decor_t< int > dec_pass_preSel_w("pass_preSel_w");
  static decor_t< int > dec_pass_preSel_top("pass_preSel_top");
  static accessor_t< int > acc_pass_preSel("pass_preSel");
  static accessor_t< int > acc_pass_preSel_b("pass_preSel_b");
  static accessor_t< int > acc_pass_preSel_w("pass_preSel_w");
  static accessor_t< int > acc_pass_preSel_top("pass_preSel_top");


  // Truth matching

  static accessor_t<bool> acc_containsTruthW("containsTruthW"); // true if the W associated with the jet is within a certain radius
  static accessor_t<bool> acc_notContainedB("notContainedB"); // true if the b associated with the jet is within a certain radius
  static accessor_t<bool> acc_semiBoostedW("semiBoostedW"); // containsTruthW = true and notContainedB=true
  static accessor_t<bool> acc_containsTruthTop("containsTruthTop"); // there is a top with deltaR < 1 from the jet
  static accessor_t<bool> acc_hasB("hasB"); //there is a truth B within a certain radius of this jet. Finds that b with min distance from jet
  static accessor_t<bool> acc_hasW("hasW"); //there is a truth W within a certain radius of this jet. Finds that W with min distance from jet
  static accessor_t<bool> acc_deltaR_W_jet("deltaR_W_jet");
  static accessor_t<bool> acc_deltaR_W_b("deltaR_W_b");
  static accessor_t<bool> acc_deltaR_W_top("deltaR_W_top");
  static accessor_t<bool> acc_deltaR_W_top_semiboosted("deltaR_W_top_semiboosted");

  static accessor_t<const xAOD::TruthParticle*> acc_b_particle("b_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_W_particle("W_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_top_particle("top_particle");
  static accessor_t<const xAOD::TruthParticle*> acc_W_quark1("W_quark1"); //
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



  // define isXXXX functions
  //    - note that when using a <char> decoration, you often compare to an int
  template <typename T1, typename T2>
  bool isDecor(const T1& obj, const accessor_t<T2>& decor, const T2& val, const bool& requireDecor=true){
    if(requireDecor && !decor.isAvailable(obj)) return false;
    return (decor(obj) == val);
  }
  template <typename T1>
  bool isDecor(const T1& obj, const accessor_t<char>& decor, const int& val, const bool& requireDecor=true){
    if(requireDecor && !decor.isAvailable(obj)) return false;
    return (decor(obj) == val);
  }

  template <typename T>
  bool isBaseline(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_baseline, 1, requireDecor); }
  template <typename T>
  bool isPassOverlap(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_passOverlap, 1, requireDecor); }
  template <typename T>
  bool isSignal(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_signal, 1, requireDecor); }
  template <typename T>
  bool isSignalIso(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_signalIso, 1, requireDecor); }
  template <typename T>
  bool isCosmic(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_cosmic, 1, requireDecor); }
  template <typename T>
  bool isBad(const T& obj, bool requireDecor=true){ return isDecor(obj, decor_bad, 1, requireDecor); }

  // for tagging primarily, but an enum for working points
  //  - an enum class enforces strong typing
  enum class WP {
    VeryLoose,
      Loose,
      Medium,
      Tight,
      None,
      SmoothLoose, // For top tagging
      SmoothTight  // For top tagging
  };
  // helper to convert to actual string for the tagging
  std::string wp2str(WP wp);
  WP str2wp(std::string str);

  // Effective Mass calculated using exclusive or inclusive definition depending on args
  //    Exclusive: Meff(met, jets, 4, 0, 0)
  //    Inclusive: Meff(met, jets, jets->size(), muons, els)
  float Meff(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int numJets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);
  float Meff_exclusive(const xAOD::MissingET* met, const xAOD::JetContainer* jets);
  float Meff_inclusive(const xAOD::MissingET* met, const xAOD::JetContainer* jets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // HT calculated using all jets, will include muons and/or electrons if specified
  float HT(const xAOD::JetContainer* jets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // Transverse Mass calculated using MET and leading lepton
  float mT(const xAOD::MissingET* met, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els);

  // Minimum transverse mass of leading 3 b-jets and the met with a mass set at 0 (default)
  float mTb(const xAOD::MissingET* met, const xAOD::JetContainer* bjets);

  // Calculates the minimum dPhi between the met and the container of particles passed in
  float dPhiMETMin(const xAOD::MissingET* met, const xAOD::IParticleContainer* particles, unsigned int numLeadingParticles=4);

  // Calculates the MET significance as MET/sqrt(HT)
  float METSignificance(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int njets);

  // Get the event weight consistently
  float eventWeight(const xAOD::EventInfo*, const SH::MetaObject* metaData = nullptr);

  // Get the event heavy flavour classification
  int ttbarHF(const xAOD::EventInfo*);
  int ttbarHF_ext(const xAOD::EventInfo*);

  // top tagging on jet, for extra sanity -- make sure we have pT > 100 GeV and |eta| < 2.0
  bool topTag(const xAOD::Jet* jet, std::string wp, float maxAbsEta=2.0, float minPt=300.0);

  // b-tagging on jet, for extra sanity -- make sure we cut at 2.5
  bool bTag(const xAOD::Jet* jet, std::string wp, float maxAbsEta=2.5);

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

  // build lepton veto using overlaps on baseline/signal leptons
  template <typename T>
  ConstDataVector<T> filterLeptons(const T* leptons, const bool& requireSignal = false, const bool& additionalMuonCuts = false){
    ConstDataVector<T> VetoLeptons(SG::VIEW_ELEMENTS);
    for(const auto &l: *leptons){
      if(!isBaseline(*l)) continue;
      if(requireSignal && !isSignalIso(*l)) continue;
      if(!isPassOverlap(*l)) continue;
      if(additionalMuonCuts){
        if(isCosmic(*l)) continue;
        if(isBad(*l)) continue;
      }
      VetoLeptons.push_back(l);
    }
    return VetoLeptons;
  }

  // the most arbitrarily generic subset creator ever
  //    - for example, subset_using_decoration(jets, VD::signal, 1) returns a CDV of jets that pass the signal
  template <typename T1, typename T2, typename T3>
  ConstDataVector<T1> subset_using_decor(const T1* cont, const accessor_t<T2>& decor, const T3& val, const bool& requireDecor=true){
    ConstDataVector<T1> subset(SG::VIEW_ELEMENTS);
    for(const auto &i: *cont){
      if(!isDecor(*i, decor, val, requireDecor)) continue;
      subset.push_back(i);
    }
    return subset;
  }

  // this contains all the RJigsaw variables we care about in our analysis and we pass this around because we're lazy
  struct RJigsaw {

  };
}

#endif
