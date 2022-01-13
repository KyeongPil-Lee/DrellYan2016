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

class HistContainer {
public:
  HistContainer() {}
  void Init() { isInit_ = kTRUE; Init_Hist(); }

  void Fill(NtupleHandle* ntuple, Double_t weight) {
    if( !isInit_ ) {
      cout << "Init() was not called before Fill()! ... need to check" << endl;
      return;
    }

    vector<DrellYan::GenLepton> vec_genLeptonHPFS = DrellYan::GetAllGenLeptons( ntuple, 13, "fromHardProcessFinalState" );
    DrellYan::GenPair genPairHPF(vec_genLeptonHPFS[0], vec_genLeptonHPFS[1]);

    // -- check the acceptance using post-FSR leptons
    if( !AcceptanceCheck( genPairHPF ) ) return;

    vector< DrellYan::GenOthers > vec_genPhoton_1;
    vector< DrellYan::GenOthers > vec_genPhoton_2;
    DrellYan::GenLepton dressedLepton_1 = DrellYan::LeptonDressingByPhoton( ntuple, genPairHPF.first_, 0.1, &vec_genPhoton_1 );
    DrellYan::GenLepton dressedLepton_2 = DrellYan::LeptonDressingByPhoton( ntuple, genPairHPF.second_, 0.1, &vec_genPhoton_2 );

    DrellYan::GenPair genPair_dressed(dressedLepton_1, dressedLepton_2);

    Double_t mass = genPair_dressed.mass;
    Double_t rap  = genPair_dressed.rap;
    Double_t pt   = genPair_dressed.pt;

    TString tag_mass = GetTag_Mass( mass );
    TString tag_rap  = GetTag_Rap( rap );
    TString tag_pt   = GetTag_Pt( pt );

    Double_t x1 = ntuple->genEvent_x1;
    Double_t x2 = ntuple->genEvent_x2;
    Double_t Q = ntuple->genEvent_QScale;
    Double_t Q2 = Q*Q;

    if( tag_mass != "" ) {
      TString tag = tag_mass;
      map_tag_h2D_[tag]->Fill( x1, Q2, weight );
      map_tag_h2D_[tag]->Fill( x2, Q2, weight );
    }

    if( tag_mass != "" && tag_rap != "" ) {
      TString tag = tag_mass + "_" + tag_rap;
      map_tag_h2D_[tag]->Fill( x1, Q2, weight );
      map_tag_h2D_[tag]->Fill( x2, Q2, weight );
    }

    if( tag_mass != "" && tag_pt != "" ) {
      TString tag = tag_mass + "_" + tag_pt;
      map_tag_h2D_[tag]->Fill( x1, Q2, weight );
      map_tag_h2D_[tag]->Fill( x2, Q2, weight );
    }

    if( tag_mass != "" && tag_rap != "" && tag_pt != "" ) {
      TString tag = tag_mass + "_" + tag_rap + "_" + tag_pt;
      map_tag_h2D_[tag]->Fill( x1, Q2, weight );
      map_tag_h2D_[tag]->Fill( x2, Q2, weight );
    }
  }

  void Write() {
    for(auto& pair : map_tag_h2D_ )
      pair.second->Write();
  }

private:
  Bool_t isInit_ = kFALSE;
  map<TString, TH2D*> map_tag_h2D_;

  TString GetTag_Mass( Double_t mass ) {
    TString tag = "";
    if( 10 < mass && mass < 20 )  tag = "lowM";
    if( 81 < mass && mass < 101 ) tag = "ZPeak";
    if( mass > 1000 )             tag = "highM";

    return tag;
  }

  TString GetTag_Rap( Double_t rap ) {
    TString tag = "";
    if( fabs(rap) < 0.4 ) tag = "lowY";
    if( fabs(rap) > 2.0 ) tag = "highY";

    return tag;
  }

  TString GetTag_Pt( Double_t pt ) {
    TString tag = "";
    if( pt < 1.0 ) tag = "lowPt";
    if( 8 < pt && pt < 12 ) tag = "middlePt";
    if( pt > 100 ) tag = "highPt";

    return tag;
  }

  Bool_t AcceptanceCheck( DrellYan::GenPair genPair ) {
    Double_t ptCut_lead = 28;
    Double_t ptCut_sub  = 17;
    Double_t etaCut = 2.4;

    Double_t mass = genPair.mass;
    if( mass < 60 ) { // -- scouting data
      ptCut_lead = 5;
      ptCut_sub  = 5;
    }

    Bool_t flag = kFALSE;
    if( genPair.first_.pt  > ptCut_lead && fabs(genPair.first_.eta)  < etaCut && 
        genPair.second_.pt > ptCut_sub  && fabs(genPair.second_.eta) < etaCut)
      flag = kTRUE;

    return flag;
  }

  void Init_Hist() {
    Double_t nBin_x = 1000;
    Double_t min_x = 1e-5;
    Double_t max_x = 1;
    Double_t nBin_Q2 = 100;
    Double_t min_Q2 = 1;
    Double_t max_Q2 = 10000;

    vector<TString> vec_tag_mass = {"lowM", "ZPeak", "highM"};
    vector<TString> vec_tag_rap  = {"lowY", "highY"};
    vector<TString> vec_tag_pt   = {"lowPt", "middlePt", "highPt"};

    for(auto& tag_mass : vec_tag_mass) {
      TH2D* h2D_x_Q2_mass = new TH2D("h2D_x_Q2_"+tag_mass, "", nBin_x, min_x, max_x, nBin_Q2, min_Q2, max_Q2);
      map_tag_h2D_[tag_mass] = h2D_x_Q2_mass;

      for(auto& tag_rap : vec_tag_rap) {
        TString tag_mass_rap = tag_mass+"_"+tag_rap;
        TH2D* h2D_x_Q2_mass_rap = new TH2D("h2D_x_Q2_"+tag_mass_rap, "", nBin_x, min_x, max_x, nBin_Q2, min_Q2, max_Q2);
        map_tag_h2D_[tag_mass_rap] = h2D_x_Q2_mass_rap;

        for(auto& tag_pt : vec_tag_pt) {
          TString tag_mass_rap_pt = tag_mass+"_"+tag_rap+"_"+tag_pt;
          TH2D* h2D_x_Q2_mass_rap_pt = new TH2D("h2D_x_Q2_"+tag_mass_rap_pt, "", nBin_x, min_x, max_x, nBin_Q2, min_Q2, max_Q2);
          map_tag_h2D_[tag_mass_rap_pt] = h2D_x_Q2_mass_rap_pt;
        } // -- iteration over tag_pt
      } // -- iteration over tag_rap
    } // -- iteration over tag_mass

    for(auto& tag_mass : vec_tag_mass) {
      for(auto& tag_pt : vec_tag_pt) {
        TString tag_mass_pt = tag_mass+"_"+tag_pt;
        TH2D* h2D_x_Q2_mass_pt = new TH2D("h2D_x_Q2_"+tag_mass_pt, "", nBin_x, min_x, max_x, nBin_Q2, min_Q2, max_Q2);
        map_tag_h2D_[tag_mass_pt] = h2D_x_Q2_mass_pt;
      } // -- iter over tag_pt
    } // -- iter over tag_mass
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

    // -- histogram
    HistContainer* hist = new HistContainer();
    hist->Init();

    TChain *chain = new TChain("recoTree/DYTree");
    DrellYan::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();
    ntuple->TurnOnBranches_GenOthers(); // -- for the dressed leptons

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
      if( DrellYan::SelectGenEventBySampleType(sampleInfo_.type, ntuple) ) {
        hist->Fill( ntuple, totWeight );
      } // -- end of if( DrellYan::SelectGenEventBySampleType )
    } // -- end of event iteration

    // -- save histogram: only one root file is allowed as an output (if not, merging step will be screwed up!)
    TString outputName = TString::Format("ROOTFile_MakeHist_x_Q2_%s.root", sampleInfo_.type.Data());
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