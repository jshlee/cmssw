#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

#include <DataFormats/Math/interface/deltaR.h>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <string>
#include <vector>

template <class T>
class MiniAODTriggerCandProducer : public edm::EDProducer {

  typedef std::vector<T> TCollection;
  typedef edm::Ref<TCollection> TRef;
  typedef edm::RefVector<TCollection> TRefVector;

public:
  explicit MiniAODTriggerCandProducer(const edm::ParameterSet&);
  ~MiniAODTriggerCandProducer();

  void init(const edm::TriggerResults &result, const edm::TriggerNames & triggerNames);
  bool onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
			     edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
			     TRef ref, std::string filterLabel, float dRmin);

  //bool L1Matching(edm::Handle<edm::View<l1extra::L1EmParticle>> l1H, math::XYZTLorentzVector cand, float ptThreshold); 
  std::vector<math::XYZTLorentzVector> getHLTP4(const edm::TriggerNames & triggerNames, 
						edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
						std::vector<std::string> filterLabels);

private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  
  std::vector<std::string> filterNames_;
  edm::EDGetTokenT<TRefVector> inputs_;
  //edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1iso_;
  //edm::EDGetTokenT<edm::View<l1extra::L1EmParticle>> l1noniso_;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
  //edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
  float dRMatch_;
  bool isAND_;
  edm::ParameterSetID triggerNamesID_;
  std::map<std::string, unsigned int> trigger_indices;
  //std::map<std::string, unsigned int> prescales;
  //std::map<std::string, unsigned int> prescale_counter;
};
  
template <class T>
MiniAODTriggerCandProducer<T>::MiniAODTriggerCandProducer(const edm::ParameterSet& iConfig ):
  filterNames_(iConfig.getParameter<std::vector<std::string> >("filterNames")),
  inputs_(consumes<TRefVector>(iConfig.getParameter<edm::InputTag>("inputs"))),
  //l1iso_(consumes<edm::View<l1extra::L1EmParticle>>(iConfig.getParameter<edm::InputTag>("l1Iso"))),
  //l1noniso_(consumes<edm::View<l1extra::L1EmParticle>>(iConfig.getParameter<edm::InputTag>("l1NonIso"))),
  triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
  triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
  //triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))) {
  dRMatch_(iConfig.getParameter<double>("dR")),
  isAND_(iConfig.getParameter<bool>("isAND")) {
  
  produces<TRefVector>();
}

template <class T>
MiniAODTriggerCandProducer<T>::~MiniAODTriggerCandProducer()
{}

template <class T>
void MiniAODTriggerCandProducer<T>::init(const edm::TriggerResults &result, const edm::TriggerNames & triggerNames) {
  
  trigger_indices.clear();
  for (unsigned int i=0; i<triggerNames.triggerNames().size(); i++) {    
    std::string trimmedName = HLTConfigProvider::removeVersion(triggerNames.triggerName(i));
    trigger_indices[trimmedName] = triggerNames.triggerIndex(triggerNames.triggerName(i));    
  }
}

template <class T>
void MiniAODTriggerCandProducer<T>::produce(edm::Event &iEvent, const edm::EventSetup &eventSetup) {
  edm::Handle<edm::TriggerResults> triggerBits;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  
  //edm::Handle<TCollection> inputs;
  edm::Handle<TRefVector> inputs;

  iEvent.getByToken(triggerBits_, triggerBits);
  iEvent.getByToken(triggerObjects_, triggerObjects);
  iEvent.getByToken(inputs_, inputs);

  // Create the output collection
  std::auto_ptr<TRefVector> outColRef(new TRefVector);
  
  if (!triggerBits.isValid()) {
    LogDebug("") << "TriggerResults product not found - returning result=false!";
    return;
  }

  const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerBits);
  if (triggerNamesID_ != triggerNames.parameterSetID()) {
    triggerNamesID_ = triggerNames.parameterSetID();
    init(*triggerBits, triggerNames);
  } 
  
  for (size_t i=0; i<inputs->size(); i++) {
    bool saveObj = true;
    TRef ref = (*inputs)[i];
    for (size_t f=0; f<filterNames_.size(); f++) {
      if (onlineOfflineMatching(triggerNames, triggerObjects, ref, filterNames_[f], dRMatch_)) {
	if (!isAND_) {
	  outColRef->push_back(ref);
	  break;
	} 
      } else {
	if (isAND_) {
	  saveObj = false;
	  break;
	}	
      }
    }

    if (saveObj)
      outColRef->push_back(ref);
  }	  

  iEvent.put(outColRef);
}

template <class T>
std::vector<math::XYZTLorentzVector> MiniAODTriggerCandProducer<T>::getHLTP4(const edm::TriggerNames & triggerNames, 
									     edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
									     std::vector<std::string> filterLabels) {
  
  std::vector< math::XYZTLorentzVector> triggerCandidates;
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    for (std::string filter : filterLabels) {
      if (obj.hasFilterLabel(filter)) 
	triggerCandidates.push_back(obj.p4());
    }
  }
  
  return triggerCandidates;
}

template <class T>
bool MiniAODTriggerCandProducer<T>::onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
							  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
							  TRef ref, std::string filterLabel, float dRmin) {
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->p4(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

template <>
bool MiniAODTriggerCandProducer<pat::Electron>::onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
								      edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
								      pat::ElectronRef ref, std::string filterLabel, float dRmin) {
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->superCluster()->position(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

template <>
bool MiniAODTriggerCandProducer<pat::Photon>::onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
								    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
								    pat::PhotonRef ref, std::string filterLabel, float dRmin) {
  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->superCluster()->position(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

template <>
bool MiniAODTriggerCandProducer<reco::RecoEcalCandidate>::onlineOfflineMatching(const edm::TriggerNames & triggerNames, 
										edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, 
										edm::Ref<std::vector<reco::RecoEcalCandidate>> ref, std::string filterLabel, float dRmin) {

  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { 
    obj.unpackPathNames(triggerNames); 
    if (obj.hasFilterLabel(filterLabel)) {
      float dR = deltaR(ref->superCluster()->position(), obj.p4());
      if (dR < dRmin)
	return true;
    }
  }

  return false;
}

typedef MiniAODTriggerCandProducer<pat::Electron> PatElectronTriggerCandProducer;
DEFINE_FWK_MODULE(PatElectronTriggerCandProducer);

typedef MiniAODTriggerCandProducer<pat::Photon> PatPhotonTriggerCandProducer;
DEFINE_FWK_MODULE(PatPhotonTriggerCandProducer);

typedef MiniAODTriggerCandProducer<reco::RecoEcalCandidate> RecoEcalCandidateTriggerCandProducer;
DEFINE_FWK_MODULE(RecoEcalCandidateTriggerCandProducer);
