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
  vector<TH1D*> vec_hist_;
  TH1D* h_diM_;
  TH1D* h_diPt_;
  TH1D* h_diRap_;
  TH1D* h_diAbsRap_;

  TH1D* h_pt_;
  TH1D* h_pt_lead_;
  TH1D* h_pt_sub_;

  TH1D* h_eta_;
  TH1D* h_eta_lead_;
  TH1D* h_eta_sub_;

  TH1D* h_phi_;
  TH1D* h_phi_lead_;
  TH1D* h_phi_sub_;

  HistContainer()
  {
    Init();
  }

  void Fill(DrellYan::GenPair& genPair, Double_t weight )
  {
    h_diM_->Fill( genPair.mass );
    h_diPt_->Fill( genPair.pt );
    h_diRap_->Fill( genPair.rap );
    h_diAbsRap_->Fill( genPair.absRap );

    h_pt_->Fill( genPair.first_.pt );
    h_pt_->Fill( genPair.second_.pt );
    h_pt_lead_->Fill( genPair.first_.pt );
    h_pt_sub_->Fill( genPair.second_.pt );

    h_eta_->Fill( genPair.first_.eta );
    h_eta_->Fill( genPair.second_.eta );
    h_eta_lead_->Fill( genPair.first_.eta );
    h_eta_sub_->Fill( genPair.second_.eta );

    h_phi_->Fill( genPair.first_.phi );
    h_phi_->Fill( genPair.second_.phi );
    h_phi_lead_->Fill( genPair.first_.phi );
    h_phi_sub_->Fill( genPair.second_.phi );
  }

  void Save(TFile *f_output)
  {
    f_output->cd();
    for(const auto& h : vec_hist_ )
      h->Write();
  }

private:
  void Init()
  {
    h_diM_      = new TH1D("h_diM",      "", 10000, 0, 10000); vec_hist_.push_back( h_diM_ );
    h_diPt_     = new TH1D("h_diPt",     "", 10000, 0, 10000); vec_hist_.push_back( h_diPt_ );
    h_diRap_    = new TH1D("h_diRap",    "", 2000, -100, 100); vec_hist_.push_back( h_diRap_ );
    h_diAbsRap_ = new TH1D("h_diAbsRap", "", 2000, -100, 100); vec_hist_.push_back( h_diAbsRap_ );

    h_pt_      = new TH1D("h_pt",      "", 10000, 0, 10000); vec_hist_.push_back( h_pt_ );
    h_pt_lead_ = new TH1D("h_pt_lead", "", 10000, 0, 10000); vec_hist_.push_back( h_pt_lead_ );
    h_pt_sub_  = new TH1D("h_pt_sub",  "", 10000, 0, 10000); vec_hist_.push_back( h_pt_sub_ );

    h_eta_      = new TH1D("h_eta",      "", 2000, -100, 100); vec_hist_.push_back( h_eta_ );
    h_eta_lead_ = new TH1D("h_eta_lead", "", 2000, -100, 100); vec_hist_.push_back( h_eta_lead_ );
    h_eta_sub_  = new TH1D("h_eta_sub",  "", 2000, -100, 100); vec_hist_.push_back( h_eta_sub_ );

    h_phi_      = new TH1D("h_phi",      "", 80, -4, 4); vec_hist_.push_back( h_phi_ );
    h_phi_lead_ = new TH1D("h_phi_lead", "", 80, -4, 4); vec_hist_.push_back( h_phi_lead_ );
    h_phi_sub_  = new TH1D("h_phi_sub",  "", 80, -4, 4); vec_hist_.push_back( h_phi_sub_ );

    for(const auto& h : vec_hist_ )
      h->Sumw2();
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
    PrintSampleInfo();
    StartTimer();

    TChain *chain = new TChain("recoTree/DYTree");
    DrellYan::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    HistContainer *hists = new HistContainer();

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
        vector< DrellYan::GenLepton > vec_GenLeptonHP;
        for(Int_t i_gen=0; i_gen<ntuple->nGenLepton; i_gen++)
        {
          DrellYan::GenLepton genLepton( ntuple, i_gen );

          if( fabs(genLepton.ID) == 13 && genLepton.isHardProcess )
            vec_GenLeptonHP.push_back( genLepton );
        }

        if( (Int_t)vec_GenLeptonHP.size() != 2 )
        {
          cout << "vec_GenLeptonHP.size() = " << vec_GenLeptonHP.size() << " != 2" << endl;
          cout << "   Something wrong ... need to check" << endl;
        }

        DrellYan::GenPair genPairHP(vec_GenLeptonHP[0], vec_GenLeptonHP[1]);
        hists->Fill( genPairHP, totWeight );
      }
    }

    TString outputName = TString::Format("ROOTFile_HistProducer_preFSR_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists->Save(f_output);
    f_output->Close();

    PrintRunTime();
  }
};