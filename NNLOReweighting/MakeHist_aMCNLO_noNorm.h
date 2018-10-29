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
#include <NNLOReweighting/DYReweightingTool.h>

class PtRapHistContainer
{
public:
  // -- min/max mass: to select appropriate pt/rapidity bin
  Double_t minM_;
  Double_t maxM_;
  TString massBinInfo_;

  // -- dilepton pt & rapidity bins: assigned according to minM and maxM
  Int_t nDiPtBin_;
  Double_t* arr_diPtBinEdge_;

  Int_t nDiRapBin_;
  Double_t* arr_diRapBinEdge_;

  vector<TH1D*> vec_hist_;

  PtRapHistContainer( Double_t minM, Double_t maxM )
  {
    minM_ = minM;
    maxM_ = maxM;
    massBinInfo_ = TString::Format("M%.0lfto%.0lf", minM, maxM);

    Init();
  }

  void Fill(GenPair& genPair, Double_t weight)
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
    SelectDiPtRapBinEdge();

    for(Int_t i_rap=0; i_rap<DYReweight::nDiRapBin; i_rap++)
    {
      Double_t minY = arr_diRapBinEdge_[i_rap];
      Double_t maxY = arr_diRapBinEdge_[i_rap+1];

      TString histName = TString::Format("h_diPt_%s_Y%.1lfto%.1lf", massBinInfo_.Data(), minY, maxY);
      TH1D* h_temp = new TH1D(histName, "", nDiPtBin_, arr_diPtBinEdge_);

      vec_hist_.push_back( h_temp );
    }
  }

  Int_t FindRapidityBin( Double_t absRapidity )
  {
    Int_t binIndex = -1;

    if( absRapidity < 0 )
      printf("[PtRapHistContainer::FindRapidityBin] absRapidity = %lf < 0\n", absRapidity);

    if( absRapidity > arr_diRapBinEdge_[nDiRapBin_] )
    {
      printf("[PtRapHistContainer::FindRapidityBin]\n");
      printf("  Rapidity = %lf is larger than the last bin edge (%lf): binIndex = -1 is returned\n", absRapidity, arr_diRapBinEdge_[nDiRapBin_]);
      return binIndex;
    }

    for(Int_t i_rap=0; i_rap<nDiRapBin_; i_rap++)
    {
      Double_t minY = arr_diRapBinEdge_[i_rap];
      Double_t maxY = arr_diRapBinEdge_[i_rap+1];

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

  void SelectDiPtRapBinEdge()
  {
    -- dilepton pT bin edge
    if(minM_ == 1000 and maxM_ == 3000)
    {
      nDiPtBin_ = DYReweight::nDiPtBin2;
      MakeArrayAndCopy(DYReweight::nDiPtBin2, DYReweight::arr_diPtBinEdge2, arr_diPtBinEdge_);
    }
    else
    {
      nDiPtBin_ = DYReweight::nDiPtBin;
      MakeArrayAndCopy(DYReweight::nDiPtBin, DYReweight::arr_diPtBinEdge, arr_diPtBinEdge_);
    }
    
    // -- dilepton rapidity bin edge
    nDiRapBin_ = DYReweight::nDiRapBin;
    MakeArrayAndCopy(DYReweight::nDiRapBin, DYReweight::arr_diRapBinEdge, arr_diRapBinEdge_);

    // for(Int_t i_rap=0; i_rap<nDiRapBin_+1; i_rap++)
    // {
    //   printf("[%02d array component] rapidity bin edge = %lf\n", i_rap, arr_diRapBinEdge_[i_rap]);
    // }
    // printf("\n");

  }

  void MakeArrayAndCopy(Int_t nBin, Double_t* arr_original, Double_t*& arr_target)
  {
    arr_target = new Double_t[nBin+1]; // -- # bin edge = nBin + 1
    for(Int_t i=0; i<nBin+1; i++)
      arr_target[i] = arr_original[i];
  }
};

// -- contains all histograms
class HistContainer
{
public:
  vector< PtRapHistContainer* > vec_ptRapHist_;

  HistContainer()
  {
    Init();
  }

  void Fill(GenPair& genPair, Double_t weight )
  {
    Int_t massBin = FindMassBin( genPair.mass );
    // printf("[GenPair mass: %lf -> bin = %d]\n", genPair.mass, massBin);
    if( massBin == -1 )
    {
      printf("[HistContainer::Fill]\n");
      printf("  index = -1 -> this event is ignored\n");
      return;
    }

    vec_ptRapHist_[massBin]->Fill( genPair, weight );
  }

  void Save(TFile *f_output)
  {
    for(const auto& ptRapHist : vec_ptRapHist_ )
      ptRapHist->Save(f_output);
  }
private:
  void Init()
  {
    for(Int_t i=0; i<DYReweight::nMassBin; i++)
    {
      Double_t minM = DYReweight::arr_massBinEdge[i];
      Double_t maxM = DYReweight::arr_massBinEdge[i+1];

      PtRapHistContainer *ptRapHist_temp = new PtRapHistContainer(minM, maxM);
      vec_ptRapHist_.push_back( ptRapHist_temp );
    }
  }

  Int_t FindMassBin( Double_t mass )
  {
    Int_t binIndex = -1;

    if( mass > DYReweight::arr_massBinEdge[DYReweight::nMassBin] )
    {
      printf("[HistContainer::FindMassBin]\n");
      printf("  mass = %lf is larger than the last bin edge: binIndex = -1 is returned\n", mass);
      return binIndex;
    }

    for(Int_t i_mass=0; i_mass<DYReweight::nMassBin; i_mass++)
    {
      Double_t minM = DYReweight::arr_massBinEdge[i_mass];
      Double_t maxM = DYReweight::arr_massBinEdge[i_mass+1];

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
    HistContainer* hists = new HistContainer();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DrellYan::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genEvent_weight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;

      // // -- only DY->mumu or DY->ee events according to its name -- //
      if( DrellYan::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector< GenLepton > vec_GenLeptonHP;
        for(Int_t i_gen=0; i_gen<ntuple->nGenLepton; i_gen++)
        {
          GenLepton genLepton( ntuple, i_gen );

          if( fabs(genLepton.ID) == 13 && genLepton.isHardProcess )
            vec_GenLeptonHP.push_back( genLepton );
        }

        if( (Int_t)vec_GenLeptonHP.size() != 2 )
        {
          cout << "vec_GenLeptonHP.size() = " << vec_GenLeptonHP.size() << " != 2" << endl;
          cout << "   Something wrong ... need to check" << endl;
        }

        GenPair genPairHP(vec_GenLeptonHP[0], vec_GenLeptonHP[1]);
        hists->Fill( genPairHP, totWeight );
      }
    }

    TString outputName = TString::Format("ROOTFile_aMCNLO_DiPtRap_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    hists->Save( f_output );

    f_output->Close();
  }
};