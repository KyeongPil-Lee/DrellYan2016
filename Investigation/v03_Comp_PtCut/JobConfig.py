import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "GenLevel" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MakeHist_GenLevel.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
    "DYMuMu_M50toInf":   30,
  }

generator.baseWSPath = "/data9/Users/kplee/DrellYan2016Job"

generator.Generate()