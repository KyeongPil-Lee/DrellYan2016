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
    PrintSampleInfo();
    StartTimer();

    TStopwatch totaltime;
    totaltime.Start();

    TChain *chain = new TChain("recoTree/DYTree");
    DrellYan::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();

    // -- histogram initialization
    TH1D* h_sumWeight = new TH1D("h_sumWeight", "", 1, 0, 1);
    Double_t sumWeight = 0;

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DrellYan::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genEvent_weight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;

      // // -- only DY->mumu or DY->ee events according to its name -- //
      if( DrellYan::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        sumWeight = sumWeight + genWeight;
      }
    }

    h_sumWeight->SetBinContent(1, sumWeight);
    h_sumWeight->SetBinError(1, 0);

    TString outputName = TString::Format("ROOTFile_SumWeight_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_sumWeight->Write();
    f_output->Close();

    PrintRunTime();
  }
};