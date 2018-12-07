# -- Split jobs & generate script to run split jobs in paralel
# -- python JobConfig.py

from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "MakeHist_ZMuMu" # -- used for workspace name
generator.classCodePath = "ControlPlot/v02_ZPeak/MakeHist_ZMuMu.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "SingleMuon_Run2016B": 3,
  "SingleMuon_Run2016C": 3,
  "SingleMuon_Run2016D": 3,
  "SingleMuon_Run2016E": 3,
  "SingleMuon_Run2016F": 3,
  "SingleMuon_Run2016G": 6,
  "SingleMuon_Run2016Hver2": 6,
  "SingleMuon_Run2016Hver3": 2,
   }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()