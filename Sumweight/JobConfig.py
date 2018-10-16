from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "sumWeight_DYLL_M15to50" # -- used for workspace name
generator.classCodePath = "SumWeight/MakeHist_SumWeight.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818 # -- not used anyway
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M15to50": 10,
  "DYMuMu_M15to50": 10
  }

generator.outputPath = "/scratch/kplee/DYJob"

generator.Generate()


