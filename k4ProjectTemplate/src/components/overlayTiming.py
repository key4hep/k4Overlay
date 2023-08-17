from Gaudi.Configuration import *

from Configurables import k4DataSvc
podioevent = k4DataSvc("EventDataSvc")
podioevent.input = "../../simhit/pgun_mu_simhim.root"

from Configurables import PodioOutput
out = PodioOutput("out")
out.filename = "../../output_k4test/exampledata.root"
out.outputCommands = ["keep *"]

from Configurables import PodioInput

inp = PodioInput()
inp.collections = [
    "MCParticles",
    "VertexBarrelCollection",
    "VertexEndcapCollection",
]

from Configurables import OverlayTiming
overlay = OverlayTiming()
overlay.nEvents = 9
overlay.startEventIndex = 0
overlay.inputFiles = [
    "../../simhit/bib_1_simhim.root",
    "../../simhit/bib_2_simhim.root",
    "../../simhit/bib_3_simhim.root",
]
overlay.filterTimeMin = -0.25
overlay.filterTimeMax = 29.25
# Supported formats:
#   <collection name>: []  << all objects
#   <collection name>: [t_max]  << all objects with time < t_max
#   <collection name>: [t_min, t_max]  << all objects with time between t_min and t_max
overlay.inputCollections = {
    "MCParticles": [],
    "VertexBarrelCollection": [],
}

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg=[inp, overlay, out],
                EvtSel="NONE",
                EvtMax=1,
                ExtSvc=[podioevent],
                OutputLevel=INFO,
                )

