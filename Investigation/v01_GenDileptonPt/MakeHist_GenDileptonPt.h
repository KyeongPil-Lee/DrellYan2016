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
    HistContainer* hists_HP  = new HistContainer("HP");
    HistContainer* hists_HPF = new HistContainer("HPF");

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
        vector<DrellYan::GenLepton> vec_genLeptonHP = DrellYan::GetAllGenLeptons( ntuple, 13, "isHardProcess" );
        if( (Int_t)vec_genLeptonHP.size() != 2 )
        {
          cout << "vec_genLeptonHP.size() = " << vec_genLeptonHP.size() << " != 2" << endl;
          cout << "   Something wrong ... need to check" << endl;
          return;
        }

        DrellYan::GenPair genPairHP(vec_genLeptonHP[0], vec_genLeptonHP[1]);
        Double_t diM_HP = genPairHP.mass;

        if( diM_HP > 200 && diM_HP < 400)
        {
          hists_HP->Fill( genPairHP, totWeight );

          // -- post-FSR distribution
          vector<DrellYan::GenLepton> vec_genLeptonHPF = DrellYan::GetAllGenLeptons( ntuple, 13, "fromHardProcessFinalState" );
          if( (Int_t)vec_genLeptonHPF.size() != 2 )
          {
            cout << "vec_genLeptonHPF.size() = " << vec_genLeptonHPF.size() << " != 2" << endl;
            cout << "   Something wrong ... need to check" << endl;
            return;
          }

          DrellYan::GenPair genPairHPF(vec_genLeptonHPF[0], vec_genLeptonHPF[1]);
          hists_HPF->Fill( genPairHPF, totWeight );
        } // -- end of if( 200 < M < 400 )
      } // -- end of if( DrellYan::SelectGenEventBySampleType )
    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = TString::Format("ROOTFile_MakeHist_GenDileptonPt_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists_HP->Write(f_output);
    hists_HPF->Write(f_output);
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