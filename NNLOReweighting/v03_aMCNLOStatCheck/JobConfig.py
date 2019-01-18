from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "DYReweightHist" # -- used for workspace name
generator.classCodePath = "NNLOReweighting/v03_aMCNLOStatCheck/MakeHist_aMCNLO.h"
generator.className = "HistProducer"

# generator.luminosity = 35863.818
generator.luminosity = 1.0 # -- luminosity is not needed for x-section calculation
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

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

generator.Generate()


