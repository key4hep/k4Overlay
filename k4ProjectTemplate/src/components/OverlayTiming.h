#pragma once

// GAUDI
#include "Gaudi/Property.h"
#include "GaudiAlg/GaudiAlgorithm.h"

class OverlayTiming : public GaudiAlgorithm {
public:
  explicit OverlayTiming(const std::string&, ISvcLocator*);
  virtual ~OverlayTiming();
  /**  Initialize.
   *   @return status code
   */
  virtual StatusCode initialize() final;
  /**  Execute.
   *   @return status code
   */
  virtual StatusCode execute() final;
  /**  Finalize.
   *   @return status code
   */
  virtual StatusCode finalize() final;

private:
  // member variable
    Gaudi::Property<std::vector<std::string> >inputFiles{this, "inputFiles", NULL,
                                          "Name of the lcio input file(s) with background - assume one file per bunch crossing."};
    Gaudi::Property<int> nEvents{this, "nEvents", float(1),
                                          "Number of Background events to overlay - either fixed or Poisson mean"};
    Gaudi::Property<std::string> mcPhysicsParticleCollection{this, "mcPhysicsParticleCollection", "MCPhysicsParticle",
                                           "The output MC Particle Collection Name for the physics event"};
    Gaudi::Property<int> startFileIndex{this, "startFileIndex", -1,
                                          "Which background file to startWith"};
    Gaudi::Property<int> startEventIndex{this, "startEventIndex", -1,
                                          "Which background event to startWith"};
    Gaudi::Property<bool> reuseFiles{this, "reuseFiles", false,
                                          "If true the same background file can be used for the same event"};
    Gaudi::Property<bool> applyTimeFilter{this, "applyTimeFilter", true,
                                          "Flag defining whether collections should be filtered by the object's time"};
    Gaudi::Property<float> filterTimeMin{this, "filterTimeMin", float(-0.25),
                                          "Lower border of the integration time window for all collections"};
    Gaudi::Property<float> filterTimeMax{this, "filterTimeMax", float(0.25),
                                          "Upper border of the integration time window for all collections"};
    Gaudi::Property<std::vector<std::string> > inputCollections{this, "inputCollections", NULL,
                                          "Integration times for the Collections"};
};
  