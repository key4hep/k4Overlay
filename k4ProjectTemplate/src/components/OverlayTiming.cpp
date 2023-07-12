#include "OverlayTiming.h"

DECLARE_COMPONENT(OverlayTiming)

OverlayTiming::OverlayTiming(const std::string& aName, ISvcLocator* aSvcLoc) : GaudiAlgorithm(aName, aSvcLoc) {}

OverlayTiming::~OverlayTiming() {}

StatusCode OverlayTiming::initialize() { 
  if (GaudiAlgorithm::initialize().isFailure()) {
    return StatusCode::FAILURE;
  }
  std::string key;
  float value, low=filterTimeMin, high=filterTimeMax;
  for (auto const& entry : inputCollections) {
      // Extracting the 1 or 2 time values
      low=filterTimeMin;
      high=filterTimeMax;
      key =entry.first;
      if(entry.second.size()>2){
      info() << "_____________________________________________" << std::endl;
      info() << std::endl<< "Error too much elements"<< std::endl;
      info() << "_____________________________________________" << std::endl;
      return StatusCode::FAILURE;
      break;
      }
      if (entry.second.size() == 1) {
        low = filterTimeMin;
        high = entry.second[0];
      } else 
      if (entry.second.size() == 2) {
        low = entry.second[0];
        high = entry.second[1];
      }
     
     collectionFilterTimes[key] = std::pair<float, float>(low, high);
  }
      info() << "_____________________________________________" << std::endl;

       info() << "Collection integration times:"<< std::endl;
      for (auto const& entry : collectionFilterTimes) {
           info() << "  " << entry.first << ": " << entry.second.first << " -> " << entry.second.second << std::endl;
       }
    info() << "_____________________________________________";



  return StatusCode::SUCCESS; }

StatusCode OverlayTiming::execute() { 
    info() << endmsg;
    info() << endmsg;
    // info() << inputFiles << endmsg;
    // info() << nEvents << endmsg;
    // info() << mcPhysicsParticleCollection << endmsg;
    // info() << startFileIndex << endmsg;
    // info() << startEventIndex << endmsg;
    // info() << reuseFiles << endmsg;
    // info() << applyTimeFilter << endmsg;
    // info() << filterTimeMin << endmsg;
    // info() << filterTimeMax << endmsg;
    // info() << inputCollections << endmsg;
    info() << endmsg;
    info() << endmsg;
    return StatusCode::SUCCESS;
     }

StatusCode OverlayTiming::finalize() { return GaudiAlgorithm::finalize();}
