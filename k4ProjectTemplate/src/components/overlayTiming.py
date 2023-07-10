from Gaudi.Configuration import *

from Configurables import OverlayTiming
producer = OverlayTiming()
# producer.inputFiles = ["Names","file"]
# producer.nEvents = 5
# producer.mcPhysicsParticleCollection = "_mcPhysicsParticleCollectionName"
# producer.startFileIndex = 69
# producer.startEventIndex = 96
# # producer.reuseFiles = False
# producer.applyTimeFilter = False
# producer.filterTimeMin = -10.5
# producer.filterTimeMax = 78
# producer.inputCollections =["input","number"]
producer.filterTimeMin = -0.25
producer.filterTimeMax = 0.35
producer.inputCollections = [
    "VertexBarrelCollection"
    "VertexEndcapCollection 0.24"
    "InnerTrackerBarrelCollection -0.36 0.48"
]

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg=[producer],
                EvtSel="NONE",
                EvtMax=1,
                OutputLevel=INFO,
                )

