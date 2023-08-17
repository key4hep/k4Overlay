#include "OverlayTiming.h"
#include "podio/ROOTFrameReader.h"
#include "podio/Frame.h"
#include "podio/CollectionBase.h"

DECLARE_COMPONENT(OverlayTiming)

OverlayTiming::OverlayTiming(const std::string& aName, ISvcLocator* aSvcLoc) : GaudiAlgorithm(aName, aSvcLoc) {
    // declareProperty("vectorfloat", n_mcParticleHandle, "Dummy collection (output)");
}

template <typename T>
void OverlayTiming::overlayCollection(std::string collName, const podio::CollectionBase& inputColl) {
  // Converting generic type of collections to the specific one
  auto outColl = (T*)mo_collections.at(collName);
  const T& inColl = (const T&)inputColl;
  // Adding objects to the output collection
  for(int objIdx=0; objIdx < inColl.size(); objIdx++){
   // if(inColl[objIdx].getTime()<collectionFilterTimes[collName].second && inColl[objIdx].getTime()>collectionFilterTimes[collName].first){
      info() << "Adding object: " << inColl[objIdx].id() << "  at index: " << objIdx << " to collection: " << collName << endmsg;
      outColl->push_back(inColl[objIdx].clone());
      info() << "Added object at index: " << objIdx << endmsg;
    //}
  }
};

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
        info() << std::endl<< "Error. Number of elements should be <= 2 in collection "<< key<< std::endl;
        return StatusCode::FAILURE;
      }
      if (entry.second.size() == 1) {
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

  return StatusCode::SUCCESS;
  
   }

StatusCode OverlayTiming::execute() { 
    info() << endmsg;
    info() << endmsg;

    podio::ROOTFrameReader rootFileReader;
    // Opening all files
    for (int i = 0; i < inputFiles.size(); i++)
    {
      rootFileReader.openFile(inputFiles[i]);
    }
    unsigned number_of_events = rootFileReader.getEntries("events");

    std::vector <int> event_list(nEvents);

    if(startEventIndex == -1){
      srand(0);
      for(unsigned i = 0; i <nEvents; i++){
        event_list[i]=rand()%nEvents;
      }
    } else {
      int event_index = startEventIndex;
      for(unsigned i = 0; i < nEvents; i++){
        if(event_index>=number_of_events) event_index=0;
        event_list[i]=event_index;
        event_index++;
      }
    }

    for(auto &elem : event_list  ){
      std::cout<<" event "<<elem<<std::endl;
    }
 
    auto newColl = std::make_unique<edm4hep::MCParticleCollection>();

    for (auto const& inColl : inputCollections) {
      DataHandle<podio::CollectionBase>    handle{inColl.first, Gaudi::DataHandle::Reader, this};
      auto eventColl = handle.get();
      if(eventColl->getValueTypeName()=="edm4hep::MCParticle"){
        mo_collections[inColl.first] = new edm4hep::MCParticleCollection();
      }
      if(eventColl->getValueTypeName()=="edm4hep::SimTrackerHit"){
        mo_collections[inColl.first] = new edm4hep::SimTrackerHitCollection();
      }
    }

    for (auto const& inColl : inputCollections) {
      DataHandle<podio::CollectionBase>    handle{inColl.first, Gaudi::DataHandle::Reader, this};
      auto eventColl = handle.get();
      if(eventColl->getValueTypeName()=="edm4hep::MCParticle"){
        overlayCollection<edm4hep::MCParticleCollection>(inColl.first, *eventColl);
      }
      if(eventColl->getValueTypeName()=="edm4hep::SimTrackerHit"){
        overlayCollection<edm4hep::SimTrackerHitCollection>(inColl.first, *eventColl);
      }
    }

    for (auto const& inColl : inputCollections) {
      DataHandle<podio::CollectionBase>    handle{inColl.first, Gaudi::DataHandle::Reader, this};
      auto eventColl = handle.get();
      for(int eventIdx=0; eventIdx < nEvents; eventIdx++) {
        int eventId = event_list.at(eventIdx);
        // Reading the event
        const auto event = podio::Frame(rootFileReader.readEntry("events", eventId));
        const auto inputColl = event.get(inColl.first);
        if(eventColl->getValueTypeName()=="edm4hep::MCParticle"){
          overlayCollection<edm4hep::MCParticleCollection>(inColl.first, *inputColl);
        }
        if(eventColl->getValueTypeName()=="edm4hep::SimTrackerHit"){
          overlayCollection<edm4hep::SimTrackerHitCollection>(inColl.first, *inputColl);
        }
      } 
    }
    // Adding new collection to the main event
    for (auto const& inColl : inputCollections) {
      DataHandle<podio::CollectionBase>    handle{inColl.first, Gaudi::DataHandle::Reader, this};
      auto eventColl = handle.get();
      if(eventColl->getValueTypeName()=="edm4hep::MCParticle"){
        n_mcParticleHandle.put(std::move(mo_collections["MCParticles"]));
      }
    }
    info() << endmsg;
    info() << endmsg;
    return StatusCode::SUCCESS;
  }

StatusCode OverlayTiming::finalize() { return GaudiAlgorithm::finalize();}
