#include "OverlayTiming.h"

DECLARE_COMPONENT(OverlayTiming)

OverlayTiming::OverlayTiming(const std::string& aName, ISvcLocator* aSvcLoc) : GaudiAlgorithm(aName, aSvcLoc) {}

OverlayTiming::~OverlayTiming() {}

StatusCode OverlayTiming::initialize() { 
  if (GaudiAlgorithm::initialize().isFailure()) {
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS; }

StatusCode OverlayTiming::execute() { 
    info() << endmsg;
    info() << endmsg;
    info() << inputFiles << endmsg;
    info() << nEvents << endmsg;
    info() << mcPhysicsParticleCollection << endmsg;
    info() << startFileIndex << endmsg;
    info() << startEventIndex << endmsg;
    info() << reuseFiles << endmsg;
    info() << applyTimeFilter << endmsg;
    info() << filterTimeMin << endmsg;
    info() << filterTimeMax << endmsg;
    info() << inputCollections << endmsg;
    info() << endmsg;
    info() << endmsg;
    return StatusCode::SUCCESS;
     }

StatusCode OverlayTiming::finalize() { return GaudiAlgorithm::finalize();}
