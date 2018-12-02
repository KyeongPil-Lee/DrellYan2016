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

    // -- turn on necessary branches here
    // ---- e.g. ntuple->TurnOnBranches_GenLepton();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    // -- initiate histograms here

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
        // -- fill your histograms here
      }
    }

    TString outputName = TString::Format("ROOTFile_AnalyzerTemplate_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    // -- save your results here
    
    f_output->Close();

    PrintRunTime();
  }
};

void AnalyzerTemplate_TEST()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M50";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/ExampleROOTFile/TestROOTFile.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}