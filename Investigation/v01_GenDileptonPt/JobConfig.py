from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "GenDileptonPt" # -- used for workspace name
generator.classCodePath = "Investigation/v01_GenDileptonPt/MakeHist_GenDileptonPt.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M200to400":   2,
  "DYMuMu_M50":   10,
  "DYMuMu_M200to400_fixed":   2,
  "ZMuMuPowheg_M200to400":   2,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()


