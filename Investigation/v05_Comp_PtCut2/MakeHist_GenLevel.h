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

// -- HistContainers for each lepton pT cut
class Hists_LeptonPt {
public:
  Hists_LeptonPt(TString type, Double_t ptCut_lead, Double_t ptCut_sub) {
    type_       = type;
    ptCut_lead_ = ptCut_lead;
    ptCut_sub_  = ptCut_sub;

    Init();
  }

  void Fill( DrellYan::GenPair& genPair, Double_t weight ) {
    if( !(genPair.first_.pt > ptCut_lead_ && genPair.second_.pt > ptCut_sub_) ) return;

    hists_->Fill( genPair, weight );

    Double_t diMuM = genPair.mass;
    if( 50 < diMuM && diMuM < 76 )   hists_m50to76_->Fill( genPair, weight );
    if( 76 < diMuM && diMuM < 106 )  hists_m76to106_->Fill( genPair, weight );
    if( 106 < diMuM && diMuM < 170 ) hists_m106to170_->Fill( genPair, weight );
    if( 170 < diMuM )                hists_m170toInf_->Fill( genPair, weight );
  }

  void Write(TFile *f_output) {
    hists_->Write(f_output);
    hists_m50to76_->Write(f_output);
    hists_m76to106_->Write(f_output);
    hists_m106to170_->Write(f_output);
    hists_m170toInf_->Write(f_output);
  }

private:
  TString type_ = "";
  Double_t ptCut_lead_ = 0;
  Double_t ptCut_sub_ = 0;

  HistContainer* hists_;
  HistContainer* hists_m50to76_;
  HistContainer* hists_m76to106_;
  HistContainer* hists_m106to170_;
  HistContainer* hists_m170toInf_;

  void Init() {
    hists_ = new HistContainer(type_);
    hists_m50to76_   = new HistContainer(type_+"_m50to76");
    hists_m76to106_  = new HistContainer(type_+"_m76to106");
    hists_m106to170_ = new HistContainer(type_+"_m106to170");
    hists_m170toInf_ = new HistContainer(type_+"_m170toInf");
  }
};

class FullHistContainer {
public:
  FullHistContainer(TString type) { 
    type_ = type;
  }

  void Register(TString tag, Double_t ptCut_lead, Double_t ptCut_sub) {

    TString fullTag = TString::Format("%s_%s", tag.Data(), type_.Data()); // -- e.g. 2817_ee
    Hists_LeptonPt* hists = new Hists_LeptonPt(fullTag, ptCut_lead, ptCut_sub);
    vec_hists_.push_back(hists);
  }

  void Fill(DrellYan::GenPair& genPair, Double_t weight) {

    Bool_t passEtaCut = kFALSE;
    if( fabs(genPair.first_.eta) < 2.4 && fabs(genPair.second_.eta) < 2.4 ) passEtaCut = kTRUE;

    if( !passEtaCut ) return;

    for(auto hist : vec_hists_ ) {
      hist->Fill( genPair, weight );
    }
  }

  void Write(TFile* f_output) {
    for(auto hist : vec_hists_ ) {
      hist->Write( f_output );
    }
  }

private:
  TString type_ = "";

  vector<Hists_LeptonPt*> vec_hists_;
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
    FullHistContainer* FullHistContainer_mm = new FullHistContainer("mm");
    FullHistContainer_mm->Register("0000", 0, 0);
    FullHistContainer_mm->Register("2817", 28, 17);
    FullHistContainer_mm->Register("2610", 26, 10);
    FullHistContainer_mm->Register("2015", 20, 15);
    FullHistContainer_mm->Register("2010", 20, 10);

    FullHistContainer* FullHistContainer_ee = new FullHistContainer("ee");
    FullHistContainer_ee->Register("0000", 0, 0);
    FullHistContainer_ee->Register("2817", 28, 17);
    FullHistContainer_ee->Register("2520", 25, 20);
    FullHistContainer_ee->Register("2515", 25, 15);

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DrellYan::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genEvent_weight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;

      vector<DrellYan::GenLepton> vec_genLeptonHPF_ee = DrellYan::GetAllGenLeptons( ntuple, 11, "fromHardProcessFinalState" );
      vector<DrellYan::GenLepton> vec_genLeptonHPF_mm = DrellYan::GetAllGenLeptons( ntuple, 13, "fromHardProcessFinalState" );

      if( vec_genLeptonHPF_ee.size() == 2 ) {
        DrellYan::GenPair genPairHPF(vec_genLeptonHPF_ee[0], vec_genLeptonHPF_ee[1]);
        FullHistContainer_ee->Fill(genPairHPF, genWeight);
      }

      if( vec_genLeptonHPF_mm.size() == 2 ) {
        DrellYan::GenPair genPairHPF(vec_genLeptonHPF_mm[0], vec_genLeptonHPF_mm[1]);
        FullHistContainer_mm->Fill(genPairHPF, genWeight);
      }

    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = GetOutputFileName("MakeHist_GenLevel");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    FullHistContainer_ee->Write(f_output);
    FullHistContainer_mm->Write(f_output);
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