#include "OverlayTiming.h"
#include "podio/ROOTFrameReader.h"
#include "podio/Frame.h"




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
  declareProperty("mcparticles", m_mcParticleHandle, "Dummy Particle collection (output)");
  
  return StatusCode::SUCCESS;
  
   }

StatusCode OverlayTiming::execute() { 
    info() << endmsg;
    info() << endmsg;
    std::vector <std::pair<int, int> > file_event_list(nEvents);
    // file_event_list.reserve(nEvents);
    // file_event_list[0] = std::pair<int, int>(-1, -1);
    // file_event_list[1] = std::pair<int, int>(-1, -1);
    // file_event_list[2] = std::pair<int, int>(-1, -1);
    // file_event_list[3] = std::pair<int, int>(-1, -1);
    // file_event_list[4] = std::pair<int, int>(-1, -1);
    // file_event_list[5] = std::pair<int, int>(-1, -1);
    // file_event_list[6] = std::pair<int, int>(-1, -1);
    // file_event_list[7] = std::pair<int, int>(-1, -1);
    // file_event_list[8] = std::pair<int, int>(-1, -1);
    podio::ROOTFrameReader rootFileReader;
    int last_file_index=-1;
    int file_size=inputFiles.size();
    if(startFileIndex == -1 && startEventIndex == -1){
      for(unsigned i = 0; i <nEvents; i++){
        file_event_list[i]= std::pair<int, int>(rand()%file_size, rand()%eventsPerFile);
      }
    } else {
      int file_index = startFileIndex;
      int event_index = startEventIndex;

      if(startFileIndex == -1) file_index = rand()%file_size;
      if(startEventIndex == -1) event_index = rand()%eventsPerFile;

      for(unsigned i = 0; i < nEvents; i++){
        
        if (event_index >= eventsPerFile) {
          file_index++;
          event_index=startEventIndex;
        }
        if(file_index>=file_size)file_index=startFileIndex;
        file_event_list[i]= std::pair<int, int>(file_index, event_index);
        event_index++;
      }
      file_event_list[0]= std::pair<int, int>(startFileIndex, startEventIndex);
    }

    sort(file_event_list.begin(), file_event_list.end());

    for(auto &elem : file_event_list  ){
      std::cout<<" file "<<elem.first<<" event "<<elem.second<<std::endl;
    }
 
    for(int eventIdx=0; eventIdx < nEvents; eventIdx++) {
      int fileId = file_event_list.at(eventIdx).first;
      int eventId = file_event_list.at(eventIdx).second;
      // Opening file if not opened yet
      if (fileId != last_file_index) {
        std::string inputFile = inputFiles[fileId];
        std::cout << "Opening file: " << inputFile << std::endl;
        rootFileReader.openFile(inputFile);
        last_file_index = fileId;
      }
      // Reading the event
      const auto event = podio::Frame(rootFileReader.readEntry("events", eventId)); // N is the number of the entry you want, 0 for first entry, 1 for the second one and so on
      std::cout<<" file "<<fileId<<" event "<<eventId<<std::endl;
        unsigned number_of_events = rootFileReader.getEntries("events");
              std::cout << "number_of_events: " << number_of_events << std::endl;
      const auto event_n = podio::Frame(rootFileReader.readEntry("events", 0));
      const auto& particles = event_n.get<edm4hep::MCParticleCollection>("MCParticles");
      // DataHandle<edm4hep::MCParticleCollection>    m_mcParticleHandle{"MCParticles", Gaudi::DataHandle::Reader, this};
      // auto particles = m_mcParticleHandle.get();
      std::cout << "# of MCParticles: " << particles.size();
      std::cout << "# of MCParticles: " << particles[0].getPDG();
      // // auto particle  = (*particles)[0];
         
    }
  
    auto particles_handle = m_mcParticleHandle.get();
    // auto particle_handle  = (*particles_handle)[0];

    // for(unsigned i = 0; i <nEvents; i++){
    //   std::cout<<" file "<<file_event_list.at(i).first<<" event "<<file_event_list.at(i).second<<std::endl;
    // }


    // To get a specific event
    //const auto event = podio::Frame(rootFileReader.readEntry("events", file_event_list.at(0).second)); // N is the number of the entry you want, 0 for first entry, 1 for the second one and so on












    info() << endmsg;
    info() << endmsg;
    return StatusCode::SUCCESS;
  }

StatusCode OverlayTiming::finalize() { return GaudiAlgorithm::finalize();}
