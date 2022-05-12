#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>

#include <vector>
#include <Include/DYTool.h>


class HistProducer: public DrellYan::ClassTemplate
{
public:
  HistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    TChain *chain = new TChain("recoTree/DYTree");
    DrellYan::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();
    ntuple->TurnOnBranches_GenOthers(); // -- for the dressed leptons

    // -- histogram declarations
    const Int_t nMassBin = 34;
    Double_t arr_massBinEdge[nMassBin+1] = {
      50, 55, 60, 64, 68, 72, 76, 81, 86, 91,
      96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000
    };
    TH1D* h_diMu_mass_full = new TH1D("h_diMu_mass_full", "", nMassBin, arr_massBinEdge); // -- full phase space
    TH1D* h_diMu_mass      = new TH1D("h_diMu_mass",      "", nMassBin, arr_massBinEdge);

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DrellYan::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genEvent_weight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DrellYan::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector<DrellYan::GenLepton> vec_genLeptonHPFS = DrellYan::GetAllGenLeptons( ntuple, 13, "fromHardProcessFinalState" );
        DrellYan::GenPair genPairHPF(vec_genLeptonHPFS[0], vec_genLeptonHPFS[1]);

        vector< DrellYan::GenOthers > vec_genPhoton_1;
        vector< DrellYan::GenOthers > vec_genPhoton_2;
        DrellYan::GenLepton dressedLepton_1 = DrellYan::LeptonDressingByPhoton( ntuple, genPairHPF.first_, 0.1, &vec_genPhoton_1 );
        DrellYan::GenLepton dressedLepton_2 = DrellYan::LeptonDressingByPhoton( ntuple, genPairHPF.second_, 0.1, &vec_genPhoton_2 );

        DrellYan::GenPair genPair_dressed(dressedLepton_1, dressedLepton_2);

        Double_t diMu_mass = genPair_dressed.mass;
        h_diMu_mass_full->Fill(diMu_mass, totWeight);

        Bool_t pass_acc = kFALSE;
        if( genPair_dressed.first_.pt  > 20 && fabs(genPair_dressed.first_.eta)  < 2.4 &&
            genPair_dressed.second_.pt > 15 && fabs(genPair_dressed.second_.eta) < 2.4)
          pass_acc = kTRUE;

        if( !pass_acc ) continue;

        h_diMu_mass->Fill(diMu_mass, totWeight);

      } // -- end of if( DrellYan::SelectGenEventBySampleType )
    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = TString::Format("ROOTFile_MakeHist_DiffXSec_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_diMu_mass_full->Write();
    h_diMu_mass->Write();
    f_output->Close();

    PrintRunTime();
  }
};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "Test_DYMuMu_M50";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/ExampleROOTFile/TestROOTFile.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}