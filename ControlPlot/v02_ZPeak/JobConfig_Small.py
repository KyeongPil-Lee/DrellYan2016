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
  "tW":    1,
  "tbarW": 1,
  "WW":    1,
  "WZ":    1,
  "ZZ":    1,
  "ttbar":  12,
  "DYMuMu_M50toInf": 12,
   }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()