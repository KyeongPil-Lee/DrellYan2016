from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "GenLevel" # -- used for workspace name
generator.classCodePath = "Investigation/v03_Comp_PtCut/MakeHist_GenLevel.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  # "DYMuMu_M200to400":   2,
  "DYMuMu_M50toInf":   10,
  # "DYMuMu_M200to400_fixed":   2,
  # "ZMuMuPowheg_M200to400":   2,
  }

generator.outputPath = "/data9/Users/kplee/DrellYan2016Job"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()


