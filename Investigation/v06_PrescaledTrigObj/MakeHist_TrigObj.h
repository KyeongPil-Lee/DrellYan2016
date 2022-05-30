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
#include <map>
#include <Include/DYTool.h>


class HistSet { 
public:
  HistSet(TString type) { type_ = type; }

  void Register(TString var, Int_t nBin, Double_t min, Double_t max) {

    TString histName = "h_"+var+"_"+type_;
    if( type_ == "" )
      histName = "h_"+var;

    TH1D* h_temp = new TH1D(histName, "", nBin, min, max);

    map_var_hist_.insert( std::make_pair(var, h_temp) );
  }

  void Fill(TString var, Double_t value, Double_t weight ) {

    auto it_pair = map_var_hist_.find(var);
    if( it_pair == map_var_hist_.end() )
      cout << "no histogram corresponding to " << var << endl;
    else
      it_pair->second->Fill( value, weight );
  }

  void Write() {
    for(auto pair : map_var_hist_ )
      pair.second->Write();
  }

private:
  TString type_;
  map<TString, TH1D*> map_var_hist_;
};

class HistContainer {
public:
  HistContainer(TString type) {
    type_ = type;
    Init();
  }

  void Fill( NtupleHandle* ntuple, Double_t weight ) {
    vector<DrellYan::Muon> vec_muon = DrellYan::GetAllMuons(ntuple);

    for(auto mu : vec_muon) {

      TLorentzVector vecP_HLTObj;
      if( IsTrigMatchedByDR(mu.vecP, ntuple, "HLT_Mu20_v*", vecP_HLTObj) ) {

        cout << "vecP_HLTObj.Pt() = " << vecP_HLTObj.Pt() << endl;

        histSet_->Fill( "pt", vecP_HLTObj.Pt(), weight );
        histSet_->Fill( "eta", vecP_HLTObj.Eta(), weight );
        histSet_->Fill( "phi", vecP_HLTObj.Phi(), weight );
      }
    }
  }

  void Write() {
    histSet_->Write();
  }

private:
  TString type_ = "";
  HistSet* histSet_;

  void Init() {
    histSet_ = new HistSet(type_);

    histSet_->Register("pt",  10000, 0, 10000);
    histSet_->Register("eta", 60, -3, 3);
    histSet_->Register("phi", 80, -4, 4);
  }

  Bool_t IsTrigMatchedByDR(const TLorentzVector& vecP, NtupleHandle *ntuple, TString HLT, TLorentzVector &vecP_HLTObj)
  {
    vector<std::string> *vec_trigName = ntuple->HLT_trigName;
    Int_t hlt_nTrig = ntuple->HLT_nTrig;

    Bool_t isMatched = kFALSE;
    for(Int_t k=0; k<hlt_nTrig; k++)
    {
      if( (vec_trigName->at((unsigned int)k)) == HLT )
      {
        Double_t mass = vecP.M(); // -- mass for trigger object: same with offline object (but this is not used anyway for dR calc.)
        TLorentzVector vecP_trigObj;
        Double_t Trig_Pt = ntuple->HLT_trigPt[k];
        Double_t Trig_eta = ntuple->HLT_trigEta[k];
        Double_t Trig_phi = ntuple->HLT_trigPhi[k];
        vecP_trigObj.SetPtEtaPhiM( Trig_Pt, Trig_eta, Trig_phi, mass );

        Double_t dR = vecP.DeltaR( vecP_trigObj );
        if( dR < 0.2 )
        {
          vecP_HLTObj = vecP_trigObj;
          isMatched = kTRUE;
          break;
        }
      }
    }
    return isMatched;
  }

};


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
    // ntuple->TurnOnBranches_GenLepton();
    ntuple->TurnOnBranches_EventInfo();
    ntuple->TurnOnBranches_Muon();

    // -- histogram declarations
    HistContainer *hist = new HistContainer("");

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DrellYan::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genEvent_weight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;

      // -- trigger check is not done... as we will directly match the trigger objects with the offline muon
      hist->Fill( ntuple, totWeight );

    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = GetOutputFileName("MakeHist_TrigObj");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hist->Write();
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