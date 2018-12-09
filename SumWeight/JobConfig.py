from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "sumWeight" # -- used for workspace name
generator.classCodePath = "SumWeight/MakeHist_SumWeight.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818 # -- not used anyway
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

# generator.dic_nJob = { 
#   "DYMuMu_M14p5to50": 8,
#   "DYEE_M14p5to50": 8,
#   "DYEE_M10to50": 8
#   }

generator.dic_nJob = { 
  # "WJets": 10,
  # "ttbar": 10,
  # "tW":    1,
  # "tbarW": 1,
  # "WW":    1,
  # "WZ":    1,
  # "ZZ":    1,
  "DYTauTau_M50toInf": 10,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()


