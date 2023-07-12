from Gaudi.Configuration import *

from Configurables import OverlayTiming
producer = OverlayTiming()

producer.filterTimeMin = -0.25
producer.filterTimeMax = 0.25
producer.inputCollections = {
    "AertexBarrelCollection": [],
    "BertexEndcapCollection": [0.23],
    "CnnerTrackerBarrelColl": [-0.66, 0.12],
    "DertexBarrelCollection": [],
    "EertexEndcapCollection": [-0.76, 0.45],
    "FnnerTrackerBarrelColl": [-0.26, 0.19, 0.55],
    "GertexBarrelCollection": [0.27],
    "HertexEndcapCollection": [0.33],
    "InnerTrackerBarrelColl": []
}

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg=[producer],
                EvtSel="NONE",
                EvtMax=1,
                OutputLevel=INFO,
                )

