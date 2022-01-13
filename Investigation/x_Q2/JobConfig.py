import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "x_Q2" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/HistProducer_x_Q2.h"
generator.className = "HistProducer"

generator.luminosity = 35863.818
generator.jsonName = "SampleInfo_v2p3.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
    "DYMuMu_M15to50": 30,
    "DYMuMu_M50to100": 30,
    "DYMuMu_M100to200_fixed": 5,
    "DYMuMu_M200to400_fixed": 3,
    "DYMuMu_M400to500": 3,
    "DYMuMu_M500to700": 3,
    "DYMuMu_M700to800": 3,
    "DYMuMu_M800to1000": 3,
    "DYMuMu_M1000to1500": 3,
    "DYMuMu_M1500to2000": 3,
    "DYMuMu_M2000to3000": 3,
  }

generator.baseWSPath = "/data9/Users/kplee/DrellYan2016Job"

generator.Generate()
