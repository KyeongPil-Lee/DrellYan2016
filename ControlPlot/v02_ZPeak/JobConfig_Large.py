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
  "SingleMuon_Run2016All": 20,
  "WJets": 10,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()