#include "OverlayTiming.h"
#include "podio/ROOTFrameReader.h"
#include "podio/Frame.h"
#include "podio/CollectionBase.h"

DECLARE_COMPONENT(OverlayTiming)

OverlayTiming::OverlayTiming(const std::string& aName, ISvcLocator* aSvcLoc) : GaudiAlgorithm(aName, aSvcLoc) {
    // declareProperty("vectorfloat", n_mcParticleHandle, "Dummy collection (output)");
}

template <typename T>
void OverlayTiming::overlayCollection(std::string collName, const podio::Frame& event, T* newColl) {
  const auto& eventColl = event.get<T>(collName);

  for(int objIdx=0; objIdx < eventColl.size(); objIdx++){
    if(eventColl[objIdx].getTime()<collectionFilterTimes[collName].second && eventColl[objIdx].getTime()>collectionFilterTimes[collName].first){
      info() << "Adding object: " << eventColl[objIdx].id() << "  at index: " << objIdx << endmsg;
      newColl->push_back(eventColl[objIdx].clone());
      info() << "Added object at index: " << objIdx << endmsg;
    }
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

    auto co_mcparticles = m_mcParticleHandle.get();
    auto co_vertexbarrel = m_vertexBarrelCollection.get();

    auto cn_mcparticles = new edm4hep::MCParticleCollection();
    auto cn_vertexbarrel = edm4hep::SimTrackerHitCollection();

    auto handle = DataHandle<edm4hep::MCParticleCollection>();
    auto coll = handle.createAndPut();


    mapCollections["MCParticles"] = {};
    mapCollections["MCParticles"].first = std::move(handle);
    mapCollections["MCParticles"].second = coll;

    // Testing colleciton-type identification
    const auto event = podio::Frame(rootFileReader.readEntry("events", 0));
    DataHandle<podio::CollectionBase>    handle_mc{"MCParticles", Gaudi::DataHandle::Reader, this};
    auto eventColl = handle_mc.get();
    std::cout<<"Collection type is: " << eventColl->getValueTypeName()<<std::endl;

    for(int eventIdx=0; eventIdx < nEvents; eventIdx++) {
      int eventId = event_list.at(eventIdx);
      // Reading the event
      const auto event = podio::Frame(rootFileReader.readEntry("events", eventId));


      // overlayCollection<edm4hep::MCParticleCollection>("MCParticles", event, newColl);

    } 

    // Adding new collection to the main event

    info() << endmsg;
    info() << endmsg;
    return StatusCode::SUCCESS;
  }

StatusCode OverlayTiming::finalize() { return GaudiAlgorithm::finalize();}
