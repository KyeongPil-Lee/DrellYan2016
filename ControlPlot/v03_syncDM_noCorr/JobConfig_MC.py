# -- Split jobs & generate script to run split jobs in paralel
# -- python JobConfig.py

from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "MakeHist_DYMuMu" # -- used for workspace name
generator.classCodePath = "ControlPlot/v03_syncDM_noCorr/MakeHist_DYMuMu.h"
generator.className = "HistProducer"

# generator.luminosity = 35863.818
generator.luminosity = 35867 # -- sync. with DM
generator.jsonName = "SampleInfo_v2p3_syncDM.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M50toInf": 25,
  }

generator.outputPath = "/scratch/kplee/DYJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()