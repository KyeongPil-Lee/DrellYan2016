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

    Bool_t isMu50    = IsTriggered(ntuple, "HLT_Mu50_v*");
    Bool_t isMu20    = IsTriggered(ntuple, "HLT_Mu20_v*");
    Bool_t isIsoMu24 = IsTriggered(ntuple, "HLT_IsoMu24_v*");

    if( isMu50 ) {
      if( isMu20 ) histSet_->Fill( "nEvent_Mu50_Mu20", 1.5, weight ); // -- fill "true" bin
      else         histSet_->Fill( "nEvent_Mu50_Mu20", 0.5, weight ); // -- fill "false" bin
    }

    if( isIsoMu24 ) {
      if( isMu20 ) histSet_->Fill( "nEvent_IsoMu24_Mu20", 1.5, weight ); // -- fill "true" bin
      else         histSet_->Fill( "nEvent_IsoMu24_Mu20", 0.5, weight ); // -- fill "false" bin
    }

    for(auto mu : vec_muon) {

      TLorentzVector vecP_HLTObj_Mu20;
      Bool_t isMatched_Mu20 = IsTrigMatchedByDR(mu.vecP, ntuple, "HLT_Mu20_v*", vecP_HLTObj_Mu20);
      if( isMatched_Mu20 ) {

        histSet_->Fill( "pt", vecP_HLTObj_Mu20.Pt(), weight );
        histSet_->Fill( "eta", vecP_HLTObj_Mu20.Eta(), weight );
        histSet_->Fill( "phi", vecP_HLTObj_Mu20.Phi(), weight );

        if( isMu20 ) {
          histSet_->Fill( "pt_Mu20Fired", vecP_HLTObj_Mu20.Pt(), weight );
          histSet_->Fill( "eta_Mu20Fired", vecP_HLTObj_Mu20.Eta(), weight );
          histSet_->Fill( "phi_Mu20Fired", vecP_HLTObj_Mu20.Phi(), weight );
        }
      }

      TLorentzVector vecP_HLTObj_Mu45Eta2p1;
      Bool_t isMatched_Mu45Eta2p1 = IsTrigMatchedByDR(mu.vecP, ntuple, "HLT_Mu45_eta2p1_v*", vecP_HLTObj_Mu45Eta2p1);

      TLorentzVector vecP_HLTObj_Mu50;
      Bool_t isMatched_Mu50 = IsTrigMatchedByDR(mu.vecP, ntuple, "HLT_Mu50_v*", vecP_HLTObj_Mu50);

      if( isMatched_Mu20 || isMatched_Mu45Eta2p1 ) {
        if( isMatched_Mu20 ) {
          histSet_->Fill( "pt_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu20.Pt(), weight );
          histSet_->Fill( "eta_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu20.Eta(), weight );
          histSet_->Fill( "phi_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu20.Phi(), weight );
        }
        else {
          histSet_->Fill( "pt_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu45Eta2p1.Pt(), weight );
          histSet_->Fill( "eta_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu45Eta2p1.Eta(), weight );
          histSet_->Fill( "phi_Mu20_or_Mu45Eta2p1", vecP_HLTObj_Mu45Eta2p1.Phi(), weight );
        }
      }

      if( isMatched_Mu20 || isMatched_Mu50 ) {
        if( isMatched_Mu20 ) {
          histSet_->Fill( "pt_Mu20_or_Mu50", vecP_HLTObj_Mu20.Pt(), weight );
          histSet_->Fill( "eta_Mu20_or_Mu50", vecP_HLTObj_Mu20.Eta(), weight );
          histSet_->Fill( "phi_Mu20_or_Mu50", vecP_HLTObj_Mu20.Phi(), weight );
        }
        else {
          histSet_->Fill( "pt_Mu20_or_Mu50", vecP_HLTObj_Mu50.Pt(), weight );
          histSet_->Fill( "eta_Mu20_or_Mu50", vecP_HLTObj_Mu50.Eta(), weight );
          histSet_->Fill( "phi_Mu20_or_Mu50", vecP_HLTObj_Mu50.Phi(), weight );
        }
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

    histSet_->Register("nEvent_Mu50_Mu20",    2, 0, 2);
    histSet_->Register("nEvent_IsoMu24_Mu20", 2, 0, 2);

    histSet_->Register("pt",  10000, 0, 10000);
    histSet_->Register("eta", 60, -3, 3);
    histSet_->Register("phi", 80, -4, 4);

    histSet_->Register("pt_Mu20Fired",  10000, 0, 10000);
    histSet_->Register("eta_Mu20Fired", 60, -3, 3);
    histSet_->Register("phi_Mu20Fired", 80, -4, 4);

    histSet_->Register("pt_Mu20_or_Mu45Eta2p1",  10000, 0, 10000);
    histSet_->Register("eta_Mu20_or_Mu45Eta2p1", 60, -3, 3);
    histSet_->Register("phi_Mu20_or_Mu45Eta2p1", 80, -4, 4);

    histSet_->Register("pt_Mu20_or_Mu50",  10000, 0, 10000);
    histSet_->Register("eta_Mu20_or_Mu50", 60, -3, 3);
    histSet_->Register("phi_Mu20_or_Mu50", 80, -4, 4);
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

  Bool_t IsTriggered(NtupleHandle* ntuple, TString HLT) {
    Bool_t isFired = kFALSE;
    Bool_t isFound = kFALSE;

    vector<std::string> *vec_trigName = ntuple->HLT_trigName;
    Int_t nTrig = ntuple->HLT_nTrig;

    for(Int_t k=0; k<nTrig; k++) {
      if( (vec_trigName->at((unsigned int)k)) == HLT ) {
        isFound = kTRUE;
        isFired = ntuple->HLT_trigFired[k] > 0 ? kTRUE : kFALSE;
        break;
      }
    }

    if( !isFound ) {
      cout << "[HistContainer::IsTriggered] HLT = " << HLT << "is not found in the list! ... return false" << endl;
      return kFALSE;
    }

    return isFired;
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