from Gaudi.Configuration import *

from Configurables import k4DataSvc
podioevent = k4DataSvc("EventDataSvc")
podioevent.input = "../../simhit/pgun_mu_simhim.root"

from Configurables import OverlayTiming
overlay = OverlayTiming()
overlay.nEvents = 9
overlay.startFileIndex = -1
overlay.startEventIndex = -1
overlay.eventsPerFile = 3
overlay.inputFiles = [
    "../../simhit/bib_1_simhim.root",
    "../../simhit/bib_2_simhim.root",
    "../../simhit/bib_3_simhim.root",
]
overlay.filterTimeMin = -0.25
overlay.filterTimeMax = 0.25
overlay.inputCollections = {
    "AertexBarrelCollection": [],
    "BertexEndcapCollection": [0.23]
}

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg=[overlay],
                EvtSel="NONE",
                EvtMax=1,
                ExtSvc=[podioevent],
                OutputLevel=INFO,
                )

