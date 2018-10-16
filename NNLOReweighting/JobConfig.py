from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "DYReweighting_TEST" # -- used for workspace name
generator.classCodePath = "NNLOReweighting/MakeHist_aMCNLO.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M50to100": 3,
  # "DYMuMu_M100to200": 1,
  # "DYMuMu_M200to400": 1  
  }

generator.outputPath = "/Users/KyeongPil_Lee/temp"

generator.Generate()


