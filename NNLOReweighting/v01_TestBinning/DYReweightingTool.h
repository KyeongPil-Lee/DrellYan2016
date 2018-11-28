#include <Include/SimplePlotTools.h>

namespace DYReweight
{
const Int_t nMassBin = 4;
Double_t arr_massBinEdge[nMassBin+1] = {15, 60, 120, 1000, 3000};

const Int_t nDiRapBin = 9;
Double_t arr_diRapBinEdge[nDiRapBin+1] = {0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 100};

const Int_t nDiPtBin = 14;
Double_t arr_diPtBinEdge[nDiPtBin+1] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 1000};

// -- for the highest mass bin
const Int_t nDiPtBin2 = 13;
Double_t arr_diPtBinEdge2[nDiPtBin+1] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 1000};

// -- color for each rapidity range in plots
vector<Int_t> vec_rapColor = {kBlack,   kRed,     kBlue,   kGreen+2, kViolet, 
                              kCyan,    kMagenta, kBlue-9, kGray,    kOrange+2 };


TH1D* ConvertTH1D_withLabel( TH1D* h, Int_t nArrBin, Double_t* arr_BinEdge )
{
  Int_t nBin = h->GetNbinsX();
  if( nBin != nArrBin )
  {
    printf("(nBin, nBinEdge-1) = (%d, %d): different!", nBin, nArrBin);
    return nullptr;
  }
  
  TH1D* h_return = new TH1D(h->GetName(), "", nBin, 0, nBin);

  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    TString binLabel = TString::Format("[%.0lf, %.0lf]", arr_BinEdge[i], arr_BinEdge[i+1]);
    h_return->GetXaxis()->SetBinLabel(i_bin, binLabel);

    Double_t binContent = h->GetBinContent(i_bin);
    Double_t binError = h->GetBinError(i_bin);

    h_return->SetBinContent(i_bin, binContent);
    h_return->SetBinError(i_bin, binError);
  }

  return h_return;
}

TH1D* Convert_dSigmadPtdY(TH1D* h, Double_t minY, Double_t maxY)
{
  TH1D* h_DXSec = (TH1D*)h->Clone();
  h_DXSec = PlotTool::DivideEachBin_ByBinWidth(h_DXSec); // -- divide each bin by pT bin width
  h_DXSec->Scale( 1.0 / (maxY - minY) ); // -- divide by rapidity bin width also

  return h_DXSec;
}

};