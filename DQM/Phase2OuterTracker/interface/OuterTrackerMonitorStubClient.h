#ifndef Phase2OuterTracker_OuterTrackerMonitorStubClient_h
#define Phase2OuterTracker_OuterTrackerMonitorStubClient_h


#include <vector>
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DQM/SiStripCommon/interface/TkHistoMap.h"

class DQMStore;

class OuterTrackerMonitorStubClient : public edm::EDAnalyzer {

public:
  explicit OuterTrackerMonitorStubClient(const edm::ParameterSet&);
  ~OuterTrackerMonitorStubClient();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  //virtual void beginJob() ;
  virtual void endJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  
};
#endif
