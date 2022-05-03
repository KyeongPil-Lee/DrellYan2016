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
  TString type_;

  vector<TH1D*> vec_hist_;

  TH1D* h_diPt_;
  TH1D* h_diRap_;
  TH1D* h_diM_;

  TH1D* h_pt_;
  TH1D* h_pt_lead_;
  TH1D* h_pt_sub_;

  TH1D* h_eta_;
  TH1D* h_eta_lead_;
  TH1D* h_eta_sub_;

  HistContainer(TString type)
  {
    type_ = type;
    Init();
  }

  void Fill(DrellYan::GenPair& genPair, Double_t weight)
  {
    h_diPt_->Fill( genPair.pt, weight );
    h_diRap_->Fill( genPair.rap, weight );
    h_diM_->Fill( genPair.mass, weight );

    h_pt_->Fill( genPair.first_.pt, weight );
    h_pt_->Fill( genPair.second_.pt, weight );
    h_pt_lead_->Fill( genPair.first_.pt, weight );
    h_pt_sub_->Fill( genPair.second_.pt, weight );

    h_eta_->Fill( genPair.first_.eta, weight );
    h_eta_->Fill( genPair.second_.eta, weight );
    h_eta_lead_->Fill( genPair.first_.eta, weight );
    h_eta_sub_->Fill( genPair.second_.eta, weight );
  }

  void Write(TFile *f_output)
  {
    for(const auto& h : vec_hist_ )
      h->Write();

    // -- delete histogram to avoid "memory leak" warning when multiple HistContainers are used (no leak happens, though)
    // for(const auto& h : vec_hist_ )
    //   delete h;

    cout << "[HistContainer::Write] All histograms with " << type_ << " are saved in " << f_output->GetName() << endl;
    cout << endl;
  }

private:
  void Init()
  {
    h_diPt_  = new TH1D("h_diPt_"+type_,  "", 10000, 0, 10000); vec_hist_.push_back( h_diPt_ );
    h_diRap_ = new TH1D("h_diRap_"+type_, "", 2000, -100, 100); vec_hist_.push_back( h_diRap_ );
    h_diM_   = new TH1D("h_diM_"+type_,   "", 10000, 0, 10000); vec_hist_.push_back( h_diM_ );

    h_pt_      = new TH1D("h_pt_"+type_,      "", 10000, 0, 10000); vec_hist_.push_back( h_pt_ );
    h_pt_lead_ = new TH1D("h_pt_lead_"+type_, "", 10000, 0, 10000); vec_hist_.push_back( h_pt_lead_ );
    h_pt_sub_  = new TH1D("h_pt_sub_"+type_,  "", 10000, 0, 10000); vec_hist_.push_back( h_pt_sub_ );

    h_eta_      = new TH1D("h_eta_"+type_,      "", 2000, -100, 100); vec_hist_.push_back( h_eta_ );
    h_eta_lead_ = new TH1D("h_eta_lead_"+type_, "", 2000, -100, 100); vec_hist_.push_back( h_eta_lead_ );
    h_eta_sub_  = new TH1D("h_eta_sub_"+type_,  "", 2000, -100, 100); vec_hist_.push_back( h_eta_sub_ );
  }
};


class FullHistContainer {
public:
  FullHistContainer() { Init(); }

  void Fill(DrellYan::GenPair& genPair, Double_t weight) {

    Bool_t is2817_ = kFALSE;
    if( genPair.first_.pt > 28 && genPair.second_.pt > 17 ) is2817_ = kTRUE;

    Bool_t is2610_ = kFALSE;
    if( genPair.first_.pt > 26 && genPair.second_.pt > 10 ) is2610_ = kTRUE;

    Bool_t is2015_ = kFALSE;
    if( genPair.first_.pt > 20 && genPair.second_.pt > 15 ) is2015_ = kTRUE;

    Double_t diMuM = genPair.mass;

    hists_0000_->Fill( genPair, weight );
    if( 50 < diMuM && diMuM < 76 )   hists_0000_m50to76_->Fill( genPair, weight );
    if( 76 < diMuM && diMuM < 106 )  hists_0000_m76to106_->Fill( genPair, weight );
    if( 106 < diMuM && diMuM < 170 ) hists_0000_m106to170_->Fill( genPair, weight );
    if( 170 < diMuM )                hists_0000_m170toInf_->Fill( genPair, weight );

    if( is2817_ ) {
      hists_2817_->Fill( genPair, weight );
      if( 50 < diMuM && diMuM < 76 )   hists_2817_m50to76_->Fill( genPair, weight );
      if( 76 < diMuM && diMuM < 106 )  hists_2817_m76to106_->Fill( genPair, weight );
      if( 106 < diMuM && diMuM < 170 ) hists_2817_m106to170_->Fill( genPair, weight );
      if( 170 < diMuM )                hists_2817_m170toInf_->Fill( genPair, weight );
    }

    if( is2610_ ) {
      hists_2610_->Fill( genPair, weight );
      if( 50 < diMuM && diMuM < 76 )   hists_2610_m50to76_->Fill( genPair, weight );
      if( 76 < diMuM && diMuM < 106 )  hists_2610_m76to106_->Fill( genPair, weight );
      if( 106 < diMuM && diMuM < 170 ) hists_2610_m106to170_->Fill( genPair, weight );
      if( 170 < diMuM )                hists_2610_m170toInf_->Fill( genPair, weight );
    }

    if( is2015_ ) {
      hists_2015_->Fill( genPair, weight );
      if( 50 < diMuM && diMuM < 76 )   hists_2015_m50to76_->Fill( genPair, weight );
      if( 76 < diMuM && diMuM < 106 )  hists_2015_m76to106_->Fill( genPair, weight );
      if( 106 < diMuM && diMuM < 170 ) hists_2015_m106to170_->Fill( genPair, weight );
      if( 170 < diMuM )                hists_2015_m170toInf_->Fill( genPair, weight );
    }
  }

  void Write(TFile *f_output) {
    hists_0000_->Write(f_output);
    hists_0000_m50to76_->Write(f_output);
    hists_0000_m76to106_->Write(f_output);
    hists_0000_m106to170_->Write(f_output);
    hists_0000_m170toInf_->Write(f_output);

    hists_2817_->Write(f_output);
    hists_2817_m50to76_->Write(f_output);
    hists_2817_m76to106_->Write(f_output);
    hists_2817_m106to170_->Write(f_output);
    hists_2817_m170toInf_->Write(f_output);

    hists_2610_->Write(f_output);
    hists_2610_m50to76_->Write(f_output);
    hists_2610_m76to106_->Write(f_output);
    hists_2610_m106to170_->Write(f_output);
    hists_2610_m170toInf_->Write(f_output);

    hists_2015_->Write(f_output);
    hists_2015_m50to76_->Write(f_output);
    hists_2015_m76to106_->Write(f_output);
    hists_2015_m106to170_->Write(f_output);
    hists_2015_m170toInf_->Write(f_output);
  }

private:
  HistContainer* hists_0000_;
  HistContainer* hists_0000_m50to76_;
  HistContainer* hists_0000_m76to106_;
  HistContainer* hists_0000_m106to170_;
  HistContainer* hists_0000_m170toInf_;

  HistContainer* hists_2817_;
  HistContainer* hists_2817_m50to76_;
  HistContainer* hists_2817_m76to106_;
  HistContainer* hists_2817_m106to170_;
  HistContainer* hists_2817_m170toInf_;

  HistContainer* hists_2610_;
  HistContainer* hists_2610_m50to76_;
  HistContainer* hists_2610_m76to106_;
  HistContainer* hists_2610_m106to170_;
  HistContainer* hists_2610_m170toInf_;

  HistContainer* hists_2015_;
  HistContainer* hists_2015_m50to76_;
  HistContainer* hists_2015_m76to106_;
  HistContainer* hists_2015_m106to170_;
  HistContainer* hists_2015_m170toInf_;

  void Init() {
    hists_0000_ = new HistContainer("0000");
    hists_0000_m50to76_ = new HistContainer("0000_m50to76");
    hists_0000_m76to106_ = new HistContainer("0000_m76to106");
    hists_0000_m106to170_ = new HistContainer("0000_m106to170");
    hists_0000_m170toInf_ = new HistContainer("0000_m170toInf");

    hists_2817_ = new HistContainer("2817");
    hists_2817_m50to76_ = new HistContainer("2817_m50to76");
    hists_2817_m76to106_ = new HistContainer("2817_m76to106");
    hists_2817_m106to170_ = new HistContainer("2817_m106to170");
    hists_2817_m170toInf_ = new HistContainer("2817_m170toInf");

    hists_2610_ = new HistContainer("2610");
    hists_2610_m50to76_ = new HistContainer("2610_m50to76");
    hists_2610_m76to106_ = new HistContainer("2610_m76to106");
    hists_2610_m106to170_ = new HistContainer("2610_m106to170");
    hists_2610_m170toInf_ = new HistContainer("2610_m170toInf");

    hists_2015_ = new HistContainer("2015");
    hists_2015_m50to76_ = new HistContainer("2015_m50to76");
    hists_2015_m76to106_ = new HistContainer("2015_m76to106");
    hists_2015_m106to170_ = new HistContainer("2015_m106to170");
    hists_2015_m170toInf_ = new HistContainer("2015_m170toInf");
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
    ntuple->TurnOnBranches_GenLepton();

    // -- histogram declarations
    FullHistContainer* fullHist = new FullHistContainer();

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
        // vector<DrellYan::GenLepton> vec_genLeptonHP = DrellYan::GetAllGenLeptons( ntuple, 13, "isHardProcess" );
        vector<DrellYan::GenLepton> vec_genLeptonHPF = DrellYan::GetAllGenLeptons( ntuple, 13, "fromHardProcessFinalState" );
        if( (Int_t)vec_genLeptonHPF.size() != 2 )
        {
          cout << "vec_genLeptonHPF.size() = " << vec_genLeptonHPF.size() << " != 2" << endl;
          cout << "   Something wrong ... need to check" << endl;
          return;
        }

        DrellYan::GenPair genPairHPF(vec_genLeptonHPF[0], vec_genLeptonHPF[1]);
        fullHist->Fill( genPairHPF, totWeight );

      } // -- end of if( DrellYan::SelectGenEventBySampleType )
    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = TString::Format("ROOTFile_MakeHist_GenDileptonPt_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    fullHist->Write(f_output);
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