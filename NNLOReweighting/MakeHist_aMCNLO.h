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

class DiPtRapHistContainer
{
public:
  // -- min/max mass: to select appropriate pt/rapidity bin
  Double_t minM_;
  Double_t maxM_;
  vector<Double_t> vec_diPtBinEdge_;
  vector<Double_t> vec_diRapBinEdge_;

  Int_t nDiPtBin_;
  Int_t nDiRapBin_;

  TString massBinInfo_;
  vector<TH1D*> vec_hist_;

  DiPtRapHistContainer(Double_t minM, Double_t maxM, 
                       vector<Double_t> vec_diPtBinEdge, 
                       vector<Double_t> vec_diRapBinEdge)
  {
    minM_ = minM;
    maxM_ = maxM;
    massBinInfo_ = TString::Format("M%.0lfto%.0lf", minM, maxM);

    vec_diPtBinEdge_  = vec_diPtBinEdge;
    nDiPtBin_ = (Int_t)vec_diPtBinEdge_.size() - 1;

    vec_diRapBinEdge_ = vec_diRapBinEdge;
    nDiRapBin_ = (Int_t)vec_diRapBinEdge_.size() - 1;

    Init();
  }

  void Fill(DrellYan::GenPair& genPair, Double_t weight)
  {
    Int_t rapBin = FindRapidityBin( genPair.absRap );
    // printf("[GenPair rapidity: %lf -> bin = %d]\n", genPair.absRap, rapBin);
    if( rapBin == -1 )
    {
      printf("[PtRapHistContainer::Fill]\n");
      printf("  index = -1 -> this event is ignored\n");
      return;
    }

    vec_hist_[rapBin]->Fill( genPair.pt, weight );    
  }

  void Save(TFile *f_output)
  {
    f_output->cd();
    for( const auto& h : vec_hist_ )
      h->Write();
  }

private:
  void Init()
  {
    // -- make array with dilepton pT bin edges to use it for making histogram
    Double_t *arr_diPtBinEdge_ = new Double_t[nDiPtBin_+1];
    for(Int_t i_pt=0; i_pt<nDiPtBin_+1; i_pt++)
      arr_diPtBinEdge_[i_pt] = vec_diPtBinEdge_[i_pt];

    for(Int_t i_rap=0; i_rap<nDiRapBin_; i_rap++)
    {
      Double_t minY = vec_diRapBinEdge_[i_rap];
      Double_t maxY = vec_diRapBinEdge_[i_rap+1];

      TString histName = TString::Format("h_diPt_%s_Y%.1lfto%.1lf", massBinInfo_.Data(), minY, maxY);
      TH1D* h_temp = new TH1D(histName, "", nDiPtBin_, arr_diPtBinEdge_);
      h_temp->Sumw2();

      vec_hist_.push_back( h_temp );
    }

    delete[] arr_diPtBinEdge_;
  }

  Int_t FindRapidityBin( Double_t absRapidity )
  {
    Int_t binIndex = -1;

    if( absRapidity < 0 )
      printf("[PtRapHistContainer::FindRapidityBin] absRapidity = %lf < 0\n", absRapidity);

    if( absRapidity > vec_diRapBinEdge_[nDiRapBin_] )
    {
      printf("[PtRapHistContainer::FindRapidityBin]\n");
      printf("  Rapidity = %lf is larger than the last bin edge (%lf): binIndex = -1 is returned\n", absRapidity, vec_diRapBinEdge_[nDiRapBin_]);
      return binIndex;
    }

    for(Int_t i_rap=0; i_rap<nDiRapBin_; i_rap++)
    {
      Double_t minY = vec_diRapBinEdge_[i_rap];
      Double_t maxY = vec_diRapBinEdge_[i_rap+1];

      if( absRapidity > minY && absRapidity < maxY )
      {
        binIndex = i_rap;
        break;
      }
    }

    if( binIndex == -1 )
      printf("[Rapidity = %lf is not assigned to any rapidity bin! -> binIndex = -1 (may make seg. fault)]\n", absRapidity);

    return binIndex;
  }
};


class HistContainer
{
public:
  Int_t nMassBin_;
  vector<Double_t> vec_massBinEdge_;
  vector<Double_t> vec_diPtBinEdge_;
  vector<Double_t> vec_diRapBinEdge_;

  vector<DiPtRapHistContainer*> vec_diPtRapHist_;

  HistContainer(vector<Double_t> vec_massBinEdge, 
                vector<Double_t> vec_diPtBinEdge, 
                vector<Double_t> vec_diRapBinEdge)
  {
    vec_massBinEdge_  = vec_massBinEdge;
    nMassBin_ = (Int_t)vec_massBinEdge_.size()-1;

    vec_diPtBinEdge_  = vec_diPtBinEdge;
    vec_diRapBinEdge_ = vec_diRapBinEdge;

    Init();
  }

  void Fill(DrellYan::GenPair& genPair, Double_t weight )
  {
    Int_t massBin = FindMassBin( genPair.mass );
    // printf("[GenPair mass: %lf -> bin = %d]\n", genPair.mass, massBin);
    if( massBin == -1 )
    {
      printf("[HistContainer::Fill]\n");
      printf("  index = -1 -> this event is ignored\n");
      return;
    }

    vec_diPtRapHist_[massBin]->Fill( genPair, weight );
  }

  void Save(TFile *f_output)
  {
    for(const auto& diPtRapHist : vec_diPtRapHist_ )
      diPtRapHist->Save(f_output);
  }

private:
  void Init()
  {
    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];

      DiPtRapHistContainer* diPtRapHist_temp = new DiPtRapHistContainer(minM, maxM, vec_diPtBinEdge_, vec_diRapBinEdge_);
      vec_diPtRapHist_.push_back( diPtRapHist_temp );
    }
  }

  Int_t FindMassBin( Double_t mass )
  {
    Int_t binIndex = -1;

    if( mass > vec_massBinEdge_[nMassBin_] )
    {
      printf("[HistContainer::FindMassBin]\n");
      printf("  mass = %lf is larger than the last bin edge: binIndex = -1 is returned\n", mass);
      return binIndex;
    }

    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];

      if( mass > minM && mass < maxM )
      {
        binIndex = i_mass;
        break;
      }
    }

    if( binIndex == -1 )
    {
      printf("[HistContainer::FindMassBin]\n");
      printf("[mass = %lf is not assigned to any mass bin! -> binIndex = -1 (may make seg. fault)]\n", mass);
    }

    return binIndex;
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

    TStopwatch totaltime;
    totaltime.Start();

    TChain *chain = new TChain("recoTree/DYTree");
    DrellYan::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    NtupleHandle *ntuple = new NtupleHandle( chain );
    ntuple->TurnOnBranches_GenLepton();

    // -- histogram initialization
    vector<Double_t> vec_massBinEdge_belowZPeak = {15, 20, 30, 45, 60};
    vector<Double_t> vec_diPtBinEdge_belowZPeak = {0, 20, 25, 30, 100, 1000};
    vector<Double_t> vec_diRapBinEdge_belowZPeak = {0, 1.5, 2.4, 100};
    HistContainer* hist_belowZPeak 
      = new HistContainer(vec_massBinEdge_belowZPeak, 
                          vec_diPtBinEdge_belowZPeak, 
                          vec_diRapBinEdge_belowZPeak);

    vector<Double_t> vec_massBinEdge_ZPeak = {60, 72, 106, 120};
    vector<Double_t> vec_diPtBinEdge_ZPeak = {0, 10, 15, 20, 25, 30, 50, 70, 90, 1000};
    vector<Double_t> vec_diRapBinEdge_ZPeak = {0, 1.2, 1.8, 2.4, 100};
    HistContainer* hist_ZPeak 
      = new HistContainer(vec_massBinEdge_ZPeak, 
                          vec_diPtBinEdge_ZPeak, 
                          vec_diRapBinEdge_ZPeak);

    vector<Double_t> vec_massBinEdge_aboveZPeak = {120, 133, 150, 171, 200, 400, 510, 600, 1000};
    vector<Double_t> vec_diPtBinEdge_aboveZPeak = {0, 10, 15, 20, 25, 30, 40, 60, 80, 1000};
    vector<Double_t> vec_diRapBinEdge_aboveZPeak = {0, 0.9, 1.5, 2.4, 100};
    HistContainer* hist_aboveZPeak 
      = new HistContainer(vec_massBinEdge_aboveZPeak, 
                          vec_diPtBinEdge_aboveZPeak, 
                          vec_diRapBinEdge_aboveZPeak);

    vector<Double_t> vec_massBinEdge_highMass = {1000, 3000};
    vector<Double_t> vec_diPtBinEdge_highMass = {0, 20, 30, 100, 1000};
    vector<Double_t> vec_diRapBinEdge_highMass = {0, 0.9, 1.5, 100};
    HistContainer* hist_highMass 
      = new HistContainer(vec_massBinEdge_highMass, 
                          vec_diPtBinEdge_highMass, 
                          vec_diRapBinEdge_highMass);


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
        Double_t diMass = genPairHP.mass;
        if( diMass < 60 )                         hist_belowZPeak->Fill( genPairHP, totWeight );
        else if( 60   < diMass && diMass < 120 )  hist_ZPeak->Fill( genPairHP, totWeight );
        else if( 120  < diMass && diMass < 1000 ) hist_aboveZPeak->Fill( genPairHP, totWeight );
        else if( 1000 < diMass && diMass < 3000 ) hist_highMass->Fill( genPairHP, totWeight );
      }
    }

    TString outputName = TString::Format("ROOTFile_aMCNLO_DiPtRap_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    hist_belowZPeak->Save( f_output );
    hist_ZPeak->Save( f_output );
    hist_aboveZPeak->Save( f_output );
    hist_highMass->Save( f_output );

    f_output->Close();
  }
};