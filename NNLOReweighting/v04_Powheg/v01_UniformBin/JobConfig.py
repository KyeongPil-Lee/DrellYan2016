from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "DYReweightHist" # -- used for workspace name
generator.classCodePath = "NNLOReweighting/v04_Powheg/v01_UniformBin/MakeHist_Powheg_uniformBin.h"
generator.className = "HistProducer"

# generator.luminosity = 35863.818
generator.luminosity = 1.0 # -- luminosity is not needed for x-section calculation
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "ZMuMuPowheg_M50to120":    5,
  "ZMuMuPowheg_M120to200":   2,
  "ZMuMuPowheg_M200to400":   2,
  "ZMuMuPowheg_M400to800":   2,
  "ZMuMuPowheg_M800to1400":  2,
  "ZMuMuPowheg_M1400to2300": 2,
  "ZMuMuPowheg_M2300to3500": 2,
  "ZMuMuPowheg_M3500to4500": 2,
  "ZMuMuPowheg_M4500to6000": 2,
  "ZMuMuPowheg_M6000toInf":  2,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()


