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
  "DYMuMu_M15to50":     9,
  "DYMuMu_M50to100":    9,
  "DYMuMu_M100to200":   2,
  "DYMuMu_M200to400":   1,
  "DYMuMu_M400to500":   1,
  "DYMuMu_M500to700":   1,
  "DYMuMu_M700to800":   1,
  "DYMuMu_M800to1000":  1,
  "DYMuMu_M1000to1500": 1,
  "DYMuMu_M1500to2000": 1,
  "DYMuMu_M2000to3000": 1,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()


