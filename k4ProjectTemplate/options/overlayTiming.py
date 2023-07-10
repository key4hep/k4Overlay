from Gaudi.Configuration import *

from Configurables import OverlayTiming
producer = OverlayTiming()

from Configurables import ApplicationMgr
ApplicationMgr( TopAlg=[producer],
                EvtSel="NONE",
                EvtMax=1,
                OutputLevel=INFO,
                )

