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

class HistContainer
{
public:
  vector<TH1D*> vec_hists_;

  // -- basic kinematic distributions
  TH1D* h_pt_;
  TH1D* h_pt_lead_;
  TH1D* h_pt_sub_;

  TH1D* h_eta_;
  TH1D* h_eta_lead_;
  TH1D* h_eta_sub_;

  TH1D* h_phi_;
  TH1D* h_phi_lead_;
  TH1D* h_phi_sub_;

  TH1D* h_diMuM_;
  TH1D* h_diMuRap_;
  TH1D* h_diMuPt_;

  // -- cut variables (to check whether cuts are correctly applied)
  TH1D* h_isGLB_;
  TH1D* h_nMuonHit_;
  TH1D* h_nMatchedStation_;
  TH1D* h_best_relPtError_;
  TH1D* h_dxyVTX_;
  TH1D* h_nPixelHit_;
  TH1D* h_nTrackerLayer_;
  TH1D* h_relTrkIso_;

  // -- dimuon cut variables
  TH1D* h_normVtxChi2_inner_;
  TH1D* h_angle3D_inner_;

  HistContainer()
  {
    Init();
  }

  void Fill( DrellYan::MuonPair& muonPair, Double_t weight )
  {
    Fill_SingleMu( muonPair.first_, weight);
    Fill_SingleMu( muonPair.second_, weight );

    h_pt_lead_->Fill( muonPair.first_.pt, weight );
    h_pt_sub_ ->Fill( muonPair.second_.pt, weight );

    h_eta_lead_->Fill( muonPair.first_.eta, weight );
    h_eta_sub_ ->Fill( muonPair.second_.eta, weight );

    h_phi_lead_->Fill( muonPair.first_.phi, weight );
    h_phi_sub_ ->Fill( muonPair.second_.phi, weight );

    h_diMuM_->Fill( muonPair.mass );
    h_diMuRap_->Fill( muonPair.rap );
    h_diMuPt_->Fill( muonPair.pt );

    h_normVtxChi2_inner_->Fill( muonPair.normVtxChi2_inner );
    h_angle3D_inner_->Fill( muonPair.angle3D_inner );
  }

  void Write(TFile *f_output)
  {
    f_output->cd();
    for(const auto& h: vec_hists_ )
      h->Write();
  }

private:
  void Init()
  {
    h_pt_      = new TH1D("h_pt",      "", 10000, 0, 10000);
    h_pt_lead_ = new TH1D("h_pt_lead", "", 10000, 0, 10000);
    h_pt_sub_  = new TH1D("h_pt_sub",  "", 10000, 0, 10000);

    h_eta_      = new TH1D("h_eta",      "", 60, -3, 3);
    h_eta_lead_ = new TH1D("h_eta_lead", "", 60, -3, 3);
    h_eta_sub_  = new TH1D("h_eta_sub",  "", 60, -3, 3);

    h_phi_      = new TH1D("h_phi",      "", 80, -4, 4);
    h_phi_lead_ = new TH1D("h_phi_lead", "", 80, -4, 4);
    h_phi_sub_  = new TH1D("h_phi_sub",  "", 80, -4, 4);

    h_diMuM_ = new TH1D("h_diMuM_",     "", 10000, 0, 10000);
    h_diMuPt_ = new TH1D("h_diMuPt_",   "", 10000, 0, 10000);
    h_diMuRap_ = new TH1D("h_diMuRap_", "", 60, -3, 3);

    h_isGLB_           = new TH1D("h_isGLB",           "", 2, 0, 2);
    h_nMuonHit_        = new TH1D("h_nMuonHit",        "", 100, 0, 100);
    h_nMatchedStation_ = new TH1D("h_nMatchedStation", "", 10, 0, 10);
    h_best_relPtError_ = new TH1D("h_best_relPtError", "", 50, 0, 0.5);
    h_dxyVTX_          = new TH1D("h_dxyVTX",          "", 60, -0.3, 0.3);
    h_nPixelHit_       = new TH1D("h_nPixelHit",       "", 15, 0, 15);
    h_nTrackerLayer_   = new TH1D("h_nTrackerLayer",   "", 25, 0, 25);
    h_relTrkIso_       = new TH1D("h_relTrkIso",       "", 50, 0, 0.5);

    h_normVtxChi2_inner_ = new TH1D("h_normVtxChi2_inner", "", 300, 0, 30);
    h_angle3D_inner_     = new TH1D("h_angle3D_inner",     "", 40, 0, 4);

    vec_hists_.push_back(h_pt_);
    vec_hists_.push_back(h_pt_lead_);
    vec_hists_.push_back(h_pt_sub_);

    vec_hists_.push_back(h_eta_);
    vec_hists_.push_back(h_eta_lead_);
    vec_hists_.push_back(h_eta_sub_);

    vec_hists_.push_back(h_phi_);
    vec_hists_.push_back(h_phi_lead_);
    vec_hists_.push_back(h_phi_sub_);

    vec_hists_.push_back(h_diMuM_);
    vec_hists_.push_back(h_diMuRap_);
    vec_hists_.push_back(h_diMuPt_);

    vec_hists_.push_back(h_isGLB_);
    vec_hists_.push_back(h_nMuonHit_);
    vec_hists_.push_back(h_nMatchedStation_);
    vec_hists_.push_back(h_best_relPtError_);
    vec_hists_.push_back(h_dxyVTX_);
    vec_hists_.push_back(h_nPixelHit_);
    vec_hists_.push_back(h_nTrackerLayer_);
    vec_hists_.push_back(h_relTrkIso_);

    vec_hists_.push_back(h_normVtxChi2_inner_);
    vec_hists_.push_back(h_angle3D_inner_);

    for(const auto& h: vec_hists_ )
      h->Sumw2();
  }

  void Fill_SingleMu(DrellYan::Muon& mu, Double_t weight)
  {
    h_pt_->Fill( mu.pt, weight );
    h_eta_->Fill( mu.eta, weight );
    h_phi_->Fill( mu.phi, weight );

    h_isGLB_           ->Fill(mu.isGLB, weight );
    h_nMuonHit_        ->Fill(mu.nMuonHit, weight );
    h_nMatchedStation_ ->Fill(mu.nMatchedStation, weight );
    h_best_relPtError_ ->Fill(mu.best_relPtError, weight );
    h_dxyVTX_          ->Fill(mu.dxyVTX, weight );
    h_nPixelHit_       ->Fill(mu.nPixelHit, weight );
    h_nTrackerLayer_   ->Fill(mu.nTrackerLayer, weight );
    h_relTrkIso_       ->Fill(mu.relTrkIso, weight );
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
    ntuple->TurnOnBranches_EventInfo();
    ntuple->TurnOnBranches_GenLepton();
    ntuple->TurnOnBranches_Muon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    HistContainer* hists = new HistContainer();

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
        DrellYan::MuonPair DYMuonPair = DrellYan::EventSelection_MuonChannel(ntuple);
        if( !DYMuonPair.isDummy )
          hists->Fill(DYMuonPair, totWeight);
      }
    }

    TString outputName = TString::Format("ROOTFile_MakeHist_ZMuMu_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists->Write(f_output);    
    f_output->Close();

    PrintRunTime();
  }

};

void Test()
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