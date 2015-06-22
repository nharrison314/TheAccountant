#include <TheAccountant/VariableDefinitions.h>

// P4Helpers
#include <FourMomUtils/xAODP4Helpers.h>

// for static casting
#include <xAODBase/IParticleContainer.h>

// c++ includes
#include <math.h>
#include <algorithm>
#include <string>

// substructure
#include <JetSubStructureUtils/Nsubjettiness.h>
#include <JetSubStructureUtils/KtSplittingScale.h>

// btagging
#include <xAODBTagging/BTagging.h>

// for event re-weighting
#include <SampleHandler/MetaFields.h>

namespace VD = VariableDefinitions;

std::string VD::wp2str(VD::WP wp){
  switch(wp){
    case VD::WP::VeryLoose:
      return "VeryLoose";
    break;
    case VD::WP::Loose:
      return "Loose";
    break;
    case VD::WP::Medium:
      return "Medium";
    break;
    case VD::WP::Tight:
      return "Tight";
    break;
  }

  // should never reach here, we should be synced with the enum class
  return "BadWorkingPoint";
}

VD::WP VD::str2wp(std::string str){
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  if(str == "veryloose") return VD::WP::VeryLoose;
  if(str == "loose")     return VD::WP::Loose;
  if(str == "medium")    return VD::WP::Medium;
  if(str == "tight")     return VD::WP::Tight;
  return VD::WP::None;
}


float VD::Meff(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int numJets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els){
  float meff(0.0);

  for(int i=0; i<numJets; i++)
    meff += jets->at(i)->pt();

  if(muons)
    for(auto muon: *muons)
      meff += muon->pt();

  if(els)
    for(auto el: *els)
      meff += el->pt();

  meff += met->met();

  return meff;
}

float VD::HT(const xAOD::JetContainer* jets, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els){
  float ht(0.0);

  for(auto jet: *jets)
    ht += jet->pt();

  if(muons)
    for(auto muon: *muons)
      ht += muon->pt();

  if(els)
    for(auto el: *els)
      ht += el->pt();

  return ht;
}

float VD::mT(const xAOD::MissingET* met, const xAOD::MuonContainer* muons, const xAOD::ElectronContainer* els){
  float mt(0.0);

  bool els_exist = els && els->size() > 0;
  bool muons_exist = muons && muons->size() > 0;

  // get leading lepton
  const xAOD::IParticle* leadingLepton(nullptr);

  // if no leptons passed, return 0.0
  if     (!muons_exist && !els_exist)
    return mt;
  else if(muons_exist && !els_exist)
    leadingLepton = static_cast<const xAOD::IParticle*>(muons->at(0));
  else if(!muons_exist && els_exist)
    leadingLepton = static_cast<const xAOD::IParticle*>(els->at(0));
  else {
    // if muon pt > electron pt, then leading lepton is muon
    if(muons->at(0)->pt() > els->at(0)->pt())
      leadingLepton = static_cast<const xAOD::IParticle*>(muons->at(0));
    // otherwise it is electron
    else
      leadingLepton = static_cast<const xAOD::IParticle*>(els->at(0));
  }

  mt = 2*leadingLepton->pt()*met->met()*(1-cos(xAOD::P4Helpers::deltaPhi(leadingLepton, met)));
  return sqrt(fabs(mt));
}

float VD::dPhiMETMin(const xAOD::MissingET* met, const xAOD::IParticleContainer* particles){
  float dPhiMin(2*TMath::Pi());
  // compute dPhiMin
  for(const auto particle: *particles) dPhiMin = std::min<float>( fabs(dPhiMin), xAOD::P4Helpers::deltaPhi(particle, met) );
  return dPhiMin;
}

float VD::METSignificance(const xAOD::MissingET* met, const xAOD::JetContainer* jets, int njets){
  float met_significance(0.0);
  float ht(0.0);

  for(int i=0; i < std::min<int>(njets, jets->size()); i++){
    const auto jet = jets->at(i);
    if(jet->pt()/1.e3 < 30.) continue;
    ht += jet->pt();
  }
  if(ht > 0) met_significance = (met->met()/1.e3)/sqrt(ht/1.e3);
  return met_significance;
}

float VD::eventWeight(const xAOD::EventInfo* ei, const SH::MetaObject* metaData){
  // is it data?
  if(!ei->eventType( xAOD::EventInfo::IS_SIMULATION ))
    return 1;

  static SG::AuxElement::ConstAccessor<float> weight_mc("weight_mc");
  float weight(weight_mc.isAvailable(*ei)?weight_mc(*ei):ei->mcEventWeight());

  float crossSection(1),
        kFactor(1),
        filterEfficiency(1),
  //      xsUncertainty(1),
        numEvents(1);

  // if metadata is set, use it, otherwise pass the event weight, uncorrected
  if(metaData){
    metaData->castDouble(SH::MetaFields::crossSection, crossSection);
    metaData->castDouble(SH::MetaFields::kfactor, kFactor);
    metaData->castDouble(SH::MetaFields::filterEfficiency, filterEfficiency);
    //metaData->castDouble(SH::MetaFields::crossSectionRelUncertainty, xsUncertainty);
    metaData->castDouble(SH::MetaFields::numEvents, numEvents);
  }

  return (weight*crossSection*kFactor*filterEfficiency/numEvents);
}

int VD::topTag(const xAOD::EventInfo* eventInfo, const xAOD::JetContainer* jets, VD::WP wp){

  static SG::AuxElement::Decorator< int > nTops_wp("nTops_"+VD::wp2str(wp));

  // set or reset to 0 top tags
  nTops_wp(*eventInfo) = 0;

  // loop over jets, tag, and count top tags
  int nTops(0);
  for(auto jet: *jets) nTops += static_cast<int>(VD::topTag(jet, wp));

  // tag the event itself with # of jets tagged
  nTops_wp(*eventInfo) = nTops;
  return nTops;
}

bool VD::topTag(const xAOD::Jet* jet, VD::WP wp){
  bool isTop_tagged = false;
  switch(wp){
    case VD::WP::Loose:
    {
      isTop_tagged = (jet->m()/1.e3 > 100.) &&
                     (VD::Split12(jet)/1.e3 > 40.);
    }
    break;
    case VD::WP::Medium:
    {
      isTop_tagged = (jet->m()/1.e3 > 100.) &&
                     (VD::Split12(jet)/1.e3 > 40.) &&
                     (VD::Split23(jet)/1.e3 > 20.);
    }
    break;
    case VD::WP::Tight:
    {
      float tau21 = VD::Tau21(jet);
      float tau32 = VD::Tau32(jet);

      isTop_tagged = (VD::Split12(jet)/1.e3 > 40.) &&
                     (tau21 > 0.4 && tau21 < 0.9) &&
                     (tau32 < 0.65);
    }
    break;
    default:
    {
      isTop_tagged = false;
    }
    break;
  }

  static SG::AuxElement::Decorator< int > isTop_wp("isTop_"+VD::wp2str(wp));
  // tag the jet
  isTop_wp(*jet) = static_cast<int>(isTop_tagged);

  return isTop_tagged;
}

int VD::bTag(const xAOD::EventInfo* eventInfo, const xAOD::JetContainer* jets, VD::WP wp){

  static SG::AuxElement::Decorator< int > nBJets_wp("nBJets_"+VD::wp2str(wp));

  // set or reset to 0 b tags
  nBJets_wp(*eventInfo) = 0;

  // loop over jets, tag, and count top tags
  int nBJets(0);
  for(auto jet: *jets) nBJets += static_cast<int>(VD::bTag(jet, wp));

  // tag the event itself with # of jets tagged
  nBJets_wp(*eventInfo) = nBJets;
  return nBJets;
}

bool VD::bTag(const xAOD::Jet* jet, VD::WP wp){

  // stupid btagging doesn't overload the fucking MVx_discriminant()
  // float btag_weight(-99.);
  double btag_weight(-99.);
  if(!jet->getAttribute("btag_MV2c20", btag_weight))
    if(!jet->btagging()->MVx_discriminant("MV2c20", btag_weight))
      return false;

  bool isB_tagged = false;
  switch(wp){
    case VD::WP::Loose: // 85%
    {
      isB_tagged = btag_weight > -0.7682;
    }
    break;
    case VD::WP::Medium: // 70%
    {
      isB_tagged = btag_weight > 0.0314;
    }
    break;
    case VD::WP::Tight: // 60%
    {
      isB_tagged = btag_weight > 0.5102;
    }
    break;
    default:
    {
      isB_tagged = false;
    }
    break;
  }

  static SG::AuxElement::Decorator< int > isB_wp("isB_"+VD::wp2str(wp));
  // tag the jet
  isB_wp(*jet) = static_cast<int>(isB_tagged);

  return isB_tagged;
}


float VD::Tau21(const xAOD::Jet* jet){
  float tau21(0.0), tau1(0.0), tau2(0.0);

  if(jet->getAttribute("Tau21", tau21)){
    return tau21;
  } else if(jet->getAttribute("Tau1", tau1) && jet->getAttribute("Tau2", tau2)){
    static SG::AuxElement::Decorator< float > Tau21("Tau21");
    Tau21(*jet) = tau2/tau1;
    return tau2/tau1;
  } else {
    // this should never be called, but we include it
    std::cout << "Warning <VariableDefinition>: Calculating subjettiness." << std::endl;
    VD::Nsubjettiness(jet);
    return VD::Tau21(jet);
  }
}

float VD::Tau32(const xAOD::Jet* jet){
  float tau32(0.0), tau2(0.0), tau3(0.0);

  if(jet->getAttribute("Tau32", tau32)){
    return tau32;
  } else if(jet->getAttribute("Tau2", tau2) && jet->getAttribute("Tau3", tau3)){
    static SG::AuxElement::Decorator< float > Tau32("Tau32");
    Tau32(*jet) = tau3/tau2;
    return tau3/tau2;
  } else {
    // this should never be called, but we include it
    std::cout << "Warning <VariableDefinition>: Calculating subjettiness." << std::endl;
    VD::Nsubjettiness(jet);
    return VD::Tau32(jet);
  }
}

void VD::Nsubjettiness(const xAOD::Jet* jet, float alpha){
  fastjet::contrib::NormalizedCutoffMeasure normalized_measure(alpha, jet->getSizeParameter(), 1000000);
  fastjet::contrib::KT_Axes kt_axes;

  JetSubStructureUtils::Nsubjettiness tau1_cal(1, kt_axes, normalized_measure);
  JetSubStructureUtils::Nsubjettiness tau2_cal(2, kt_axes, normalized_measure);
  JetSubStructureUtils::Nsubjettiness tau3_cal(3, kt_axes, normalized_measure);

  // compute the subjettiness
  float tau1 = tau1_cal.result(*jet);
  float tau2 = tau2_cal.result(*jet);
  float tau3 = tau3_cal.result(*jet);

  // set up decorators for setting the properties
  static SG::AuxElement::Decorator< float > Tau1("Tau1");
  static SG::AuxElement::Decorator< float > Tau2("Tau2");
  static SG::AuxElement::Decorator< float > Tau3("Tau3");
  static SG::AuxElement::Decorator< float > Tau21("Tau21");
  static SG::AuxElement::Decorator< float > Tau32("Tau32");

  // start decorating our jets
  Tau1(*jet) = tau1;
  Tau2(*jet) = tau2;
  Tau3(*jet) = tau3;

  if(fabs(tau1) > 1e-8) // Prevent div-0
    Tau21(*jet) = tau2/tau1;
  else
    Tau21(*jet) = -999.0;
  if(fabs(tau2) > 1e-8) // Prevent div-0
    Tau32(*jet) = tau3/tau2;
  else
    Tau32(*jet) = -999.0;

  return;
}

float VD::Split12(const xAOD::Jet* jet){
  float split12(0.0);
  if(jet->getAttribute("Split12", split12)){
      return split12;
  } else {
    VD::KtSplittingScale(jet);
    return VD::Split12(jet);
  }
}

float VD::Split23(const xAOD::Jet* jet){
  float split23(0.0);
  if(jet->getAttribute("Split23", split23)){
      return split23;
  } else {
    VD::KtSplittingScale(jet);
    return VD::Split23(jet);
  }
}

float VD::Split34(const xAOD::Jet* jet){
  float split34(0.0);
  if(jet->getAttribute("Split34", split34)){
      return split34;
  } else {
    VD::KtSplittingScale(jet);
    return VD::Split34(jet);
  }
}

void VD::KtSplittingScale(const xAOD::Jet* jet){
  static SG::AuxElement::Decorator< float > Split12("Split12");
  static SG::AuxElement::Decorator< float > Split23("Split23");
  static SG::AuxElement::Decorator< float > Split34("Split34");

  JetSubStructureUtils::KtSplittingScale split12_cal(1);
  JetSubStructureUtils::KtSplittingScale split23_cal(2);
  JetSubStructureUtils::KtSplittingScale split34_cal(3);

  Split12(*jet) = split12_cal.result(*jet);
  Split23(*jet) = split23_cal.result(*jet);
  Split34(*jet) = split34_cal.result(*jet);

  return;
}
