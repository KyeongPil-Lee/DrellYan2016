// -- namespace with variables, classes for Drell-Yan differential cross section measurements
#pragma once

#include <fstream>
#include <iostream>

#include <TObjString.h>
#include <Include/Object.h>

namespace DrellYan
{

const Int_t nMassBin = 43;
const Int_t nPUBin = 75;

// Double_t lumi = 36459.0 // -- Rereco B-G and prompt H(v2+v3) -- //
// Double_t lumi_afterTkMu50 = 33618.689 // -- Rereco B-G and prompt H(v2+v3) after TkMu50 is implemented (run >= 274954)
// Double_t lumi_BeforeL2Fix = 656.977 // -- integrated luminosity before Run 274094
// Double_t lumi_RunG = 7554.454 // -- integrated luminosity for Run2016G


enum ColorCode
{
  // -- stack plots
  kDY    = kOrange-2,  // -- signal (DY->mumu, DY->ee)
  kTop   = kRed+2,     // -- ttbar + tW + tbarW
  kEW    = kOrange+10, // -- DY->tautau + WW + WZ + ZZ
  kFakes = kViolet-5,  // -- W+jets + QCD

  kttbar = kRed+2,
  ktW    = kRed+1,
  ktbarW = kRed+3,
  kDYTauTau = kBlue-9,
  kWW = kGreen+2,
  kWZ = kGreen-9,
  kZZ = kGreen,
  kWJets = kCyan,
  kQCD   = kViolet,

  // -- for cross section plots
  kStatUnc = kBlack,
  kTotUnc  = kGray+1,
  kFEWZ    = kBlue,
  kaMCNLO  = kRed
};

Bool_t SelectGenEventBySampleType(TString type, NtupleHandle *ntuple)
{
  Bool_t flag = kFALSE;

  if( type.Contains("DYMuMu") || type.Contains("DYEE") || type.Contains("DYTauTau") )
  {
    Int_t requiredID = 0;
    if( type.Contains("DYEE") )     requiredID = 11;
    if( type.Contains("DYMuMu") )   requiredID = 13;
    if( type.Contains("DYTauTau") ) requiredID = 15;

    vector<GenLepton> vec_genLepton;
    for(Int_t i_gen=0; i_gen<ntuple->nGenLepton; i_gen++)
    {
      GenLepton genLepton(ntuple, i_gen);
      if( abs(genLepton.ID) == requiredID && genLepton.isHardProcess )
        vec_genLepton.push_back( genLepton );
    }

    Int_t nGenLeptonHP = (Int_t)vec_genLepton.size();
    if( nGenLeptonHP == 2 )
    {
      TLorentzVector v1 = vec_genLepton[0].vecP;
      TLorentzVector v2 = vec_genLepton[1].vecP;
      Double_t dileptonM = (v1 + v2).M();

      if( type.Contains("_M50to100") )
      {
        if( dileptonM < 100 ) flag = kTRUE;
      }
      else if( type.Contains("_M50to200") )
      {
        if( dileptonM < 200 ) flag = kTRUE;
      }
      else if( type.Contains("_M50to400") )
      {
        if( dileptonM < 400 ) flag = kTRUE;
      }
      else if( type.Contains("_M14p5to50") )
      {
        if( dileptonM > 14.5 && dileptonM < 50 ) flag = kTRUE;
      }
      else if( type.Contains("_M15to50") )
      {
        if( dileptonM > 15 && dileptonM < 50 ) flag = kTRUE;
      }
      else
        flag = kTRUE;
    }
  }
  else // -- if it is not DY sample
    flag = kTRUE;

  return flag;
}

GenLepton LeptonDressingByPhoton(NtupleHandle *ntuple, GenLepton &genLepton_postFSR, Double_t dRCut, vector< GenOthers >* vec_genPhoton)
{
  GenLepton genLepton_dressed = genLepton_postFSR; // -- will be returned. initialization -- //

  TLorentzVector vecP_postFSR = genLepton_postFSR.vecP;

  TLorentzVector sumVecP_photon;
  sumVecP_photon.SetPxPyPzE(0,0,0,0);

  for(Int_t i_gen=0; i_gen<ntuple->nGenOthers; i_gen++)
  {
    GenOthers genPhoton(ntuple, i_gen);

    if( fabs(genPhoton.ID) == 22 && genPhoton.status == 1 ) // -- count only stable photon in final state
    {
      Double_t dR = vecP_postFSR.DeltaR( genPhoton.vecP );

      // -- Sum of all photon's momentum near the post-FSR lepton -- //
      if( dR < dRCut )
      {
        sumVecP_photon  = sumVecP_photon + genPhoton.vecP;
        vec_genPhoton->push_back( genPhoton );
      }
    }
  }

  // -- vecP(pre-FSR) = vecP(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
  genLepton_dressed.vecP   = vecP_postFSR + sumVecP_photon;
  genLepton_dressed.pt     = genLepton_dressed.vecP.Pt();
  genLepton_dressed.energy = genLepton_dressed.vecP.E();
  genLepton_dressed.eta    = genLepton_dressed.vecP.Eta();
  genLepton_dressed.phi    = genLepton_dressed.vecP.Phi();
  genLepton_dressed.px     = genLepton_dressed.vecP.Px();
  genLepton_dressed.py     = genLepton_dressed.vecP.Py();
  genLepton_dressed.pz     = genLepton_dressed.vecP.Pz();

  return genLepton_dressed;
}

struct SampleInfo
{
  TString type;
  TString ntuplePathFile;
  Bool_t isMC;
  Double_t xSec;
  Double_t sumWeight;
  Double_t normFactor;
};

class ClassTemplate
{
public:
  DrellYan::SampleInfo sampleInfo_;
  TStopwatch timer_;

  ClassTemplate()
  {
    sampleInfo_.type = "";
    sampleInfo_.ntuplePathFile = "";
    sampleInfo_.isMC = kFALSE;
    sampleInfo_.xSec = 0;
    sampleInfo_.sumWeight = 0;
    sampleInfo_.normFactor = 0;
  }

  virtual void Run() {}

  void CheckSampleInfo()
  {
    if( sampleInfo_.type == "" )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.type is not assigned\n");

    if( sampleInfo_.ntuplePathFile == "" )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.ntuplePathFile is not assigned\n");

    if( sampleInfo_.xSec == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.xSec is not assigned\n");

    if( sampleInfo_.sumWeight == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.sumWeight is not assigned\n");

    if( sampleInfo_.normFactor == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.normFactor is not assigned\n");
  }

  void PrintSampleInfo()
  {
    cout << "[ClassTemplate::CheckSampleInfo]" << endl;
    cout << "  sampleInfo_.type =           " << sampleInfo_.type << endl;
    cout << "  sampleInfo_.ntuplePathFile = " << sampleInfo_.ntuplePathFile << endl;
    cout << "  sampleInfo_.isMC =           " << sampleInfo_.isMC << endl;
    cout << "  sampleInfo_.xSec =           " << sampleInfo_.xSec << endl;
    cout << "  sampleInfo_.sumWeight =      " << sampleInfo_.sumWeight << endl;
    cout << "  sampleInfo_.normFactor =     " << sampleInfo_.normFactor << endl;
    cout << endl;
  }

  void StartTimer()
  {
    timer_.Start();
  }

  void PrintRunTime()
  {
    Double_t cpuTime = timer_.CpuTime();
    Double_t realTime = timer_.RealTime();

    cout << "************************************************" << endl;
    cout << "Total real time: " << realTime << " (seconds)" << endl;
    cout << "Total CPU time:  " << cpuTime << " (seconds)" << endl;
    cout << "  CPU time / real time = " << cpuTime / realTime << endl;
    cout << "************************************************" << endl;
  }

  // -- Give different name for each given text file
  TString GetOutputFileName(TString tag)
  {
    TString textFilePath = sampleInfo_.ntuplePathFile;
    TObjArray *tokens = textFilePath.Tokenize("/");
    TString textFileName = ((TObjString*)tokens->Last())->GetString();
    textFileName = textFileName.ReplaceAll(".txt", "");
    TString outputName = TString::Format("ROOTFile_%s_%s_%s.root", tag.Data(), sampleInfo_.type.Data(), textFileName.Data());

    return outputName;
  }

};

void AddNtupleToChain(TChain* chain, TString textFileName)
{
  ifstream openFile( textFileName.Data() );
  cout << "===============================" << endl;
  cout << "Read " << textFileName << endl;
  cout << "===============================" << endl;

  if( openFile.is_open() )
  {
    string line;
    while(getline(openFile, line))
    {
      cout << line << endl;
      chain->Add(line.data());
    }
    openFile.close();
  }

  cout << "==================================" << endl;
  cout << "All ROOT Files are put into TChain" << endl;
  cout << "==================================" << endl;
}

vector<DrellYan::GenLepton> GetAllGenLeptons(NtupleHandle *ntuple, Int_t pdgID, TString genFlagName)
{
  vector< DrellYan::GenLepton > vec_genLepton;
  vec_genLepton.clear();

  if( genFlagName != "isHardProcess" && 
      genFlagName != "fromHardProcessFinalState" )
  {
    cout << "[DYTool::GetAllGenMuons_GenFlag] genFlagName = " << genFlagName << " is not supported -> empty vector is returned" << endl;
    cout << "  Update the code if you want to use " << genFlagName << endl;
    return vec_genLepton;
  }

  if( pdgID != 11 && pdgID != 13 )
  {
    {
      cout << "[DYTool::GetAllGenMuons_GenFlag] pdgID = " << pdgID << "is not supported -> empty vector is returned" << endl;
      cout << "  Update the code if you want to use " << pdgID << endl;
      return vec_genLepton;
    }

  }

  for(Int_t i_gen=0; i_gen<ntuple->nGenLepton; i_gen++)
  {
    DrellYan::GenLepton genLepton( ntuple, i_gen );

    if( fabs(genLepton.ID) == pdgID )
    {
      if( genFlagName == "isHardProcess" )
      {
        if( genLepton.isHardProcess ) 
          vec_genLepton.push_back( genLepton );
      }
      else if( genFlagName == "fromHardProcessFinalState" )
      {
        if( genLepton.fromHardProcessFinalState ) 
          vec_genLepton.push_back( genLepton );
      }
    }
  }

  return vec_genLepton;
}


vector<Muon> GetAllMuons(NtupleHandle *ntuple)
{
  vector<Muon> vec_muon;
  for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
  {
    Muon mu(ntuple, i_mu);
    vec_muon.push_back( mu );
  }

  return vec_muon;
}

vector<MuonPair> GetAllMuonPairs(NtupleHandle *ntuple)
{
  vector<MuonPair> vec_muonPair;

  vector<Muon> vec_muon = DrellYan::GetAllMuons(ntuple);

  for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
  {
    Muon mu1 = vec_muon[i_mu];
    for(Int_t j_mu=i_mu+1; j_mu<ntuple->nMuon; j_mu++) // -- iterate from i_mu+1 to avoid duplications
    {
      Muon mu2 = vec_muon[j_mu];

      MuonPair muonPair(mu1, mu2, ntuple);
      vec_muonPair.push_back( muonPair );
    }
  }

  return vec_muonPair;
}

Bool_t CompareMuonPair_SmallerVtxChi2( MuonPair pair1, MuonPair pair2 )
{
  // -- the pair with "smallest" vertex chi2 will be the first element -- //
  return pair1.normVtxChi2_inner < pair2.normVtxChi2_inner; 
}

MuonPair EventSelection_MuonChannel(NtupleHandle *ntuple)
{
  vector<MuonPair> vec_muonPair = DrellYan::GetAllMuonPairs(ntuple);

  vector<MuonPair> vec_goodMuonPair;
  for(auto& pair : vec_muonPair )
  {
    if( pair.IsDYCandidate(ntuple) )
      vec_goodMuonPair.push_back( pair );
  }

  if( (Int_t)(vec_goodMuonPair.size()) == 0 )
  {
    MuonPair muonPair_dummy; // -- isDummy = kTRUE
    return muonPair_dummy;
  }
  else
  {
    std::sort(vec_goodMuonPair.begin(), vec_goodMuonPair.end(), DrellYan::CompareMuonPair_SmallerVtxChi2);
    return vec_goodMuonPair[0]; // -- pair with smallest vertex chi2 value
  }
}

static inline void loadBar(int x, int n, int r, int w)
{
    // Only update r times.
    if( x == n )
      cout << endl;

    if ( x % (n/r +1) != 0 ) return;

 
    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;
 
    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );
 
    // Show the load bar.
    for (int x=0; x<c; x++) cout << "=";
 
    for (int x=c; x<w; x++) cout << " ";
 
    // ANSI Control codes to go back to the
    // previous line and clear it.
  cout << "]\r" << flush;

}

}; // -- end of DrellYan namespace