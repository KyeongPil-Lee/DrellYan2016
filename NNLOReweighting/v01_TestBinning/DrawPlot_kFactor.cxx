#include <Include/SimplePlotTools.h>
#include <NNLOReweighting/DYReweightingTool.h>

void DrawCanvas_aMCNLO_FEWZ(TH1D* h_aMCNLO, TH1D* h_FEWZ, Double_t minM, Double_t maxM, Double_t minY, Double_t maxY );
void CheckCompatibility(TH1D* h_aMCNLO, TH1D* h_FEWZ);
void FindGlobalMinMax(TH1D* h1, TH1D* h2, Double_t& globalMin, Double_t& globalMax );

void DrawPlot_kFactor()
{
  TString rootFile_aMCNLO = "ROOTFile_DYMuMu.root";
  TString rootFile_FEWZ = "./FEWZOutput/v03_TestForBinOpti_NNLO/ROOTFile_FEWZ_TestForBinOpti_NNLO.root";

  for(Int_t i_mass=0; i_mass<DYReweight::nMassBin; i_mass++)
  {
    Double_t minM = DYReweight::arr_massBinEdge[i_mass];
    Double_t maxM = DYReweight::arr_massBinEdge[i_mass+1];

    TString canvasName = TString::Format("c_kFactor_M%.0lftoM%.0lf", minM, maxM);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);

    // -- to automatically adjust the Y range per plot
    Double_t globalMin = 9999;
    Double_t globalMax = -9999;

    vector<TString> vec_FEWZInfo;
    vector<TString> vec_aMCNLOInfo;
    vector<TString> vec_kFactorInfo;

    for(Int_t i_rap=0; i_rap<DYReweight::nDiRapBin; i_rap++)
    {
      Double_t minY = DYReweight::arr_diRapBinEdge[i_rap];
      Double_t maxY = DYReweight::arr_diRapBinEdge[i_rap+1];

      TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
      TH1D* h_diPt_aMCNLO = PlotTool::Get_Hist(rootFile_aMCNLO, histName);
      TH1D* h_diPt_FEWZ   = PlotTool::Get_Hist(rootFile_FEWZ, histName);

      // -- check the binning, negative bins, and so on
      printf("Check CheckCompatibility (%.0lf < M < %.0lf, %.1lf < |Y| < %.1lf)\n", minM, maxM, minY, maxY);
      CheckCompatibility(h_diPt_aMCNLO, h_diPt_FEWZ);

      // -- divided by dilepton pT, rapidity bin size
      TH1D* h_2DXSec_aMCNLO = DYReweight::Convert_dSigmadPtdY(h_diPt_aMCNLO, minY, maxY);
      TH1D* h_2DXSec_FEWZ   = DYReweight::Convert_dSigmadPtdY(h_diPt_FEWZ, minY, maxY);

      if( minM == 1000 && maxM == 3000 )
      {
        h_2DXSec_aMCNLO = DYReweight::ConvertTH1D_withLabel(h_2DXSec_aMCNLO, DYReweight::nDiPtBin2, DYReweight::arr_diPtBinEdge2);
        h_2DXSec_FEWZ   = DYReweight::ConvertTH1D_withLabel(h_2DXSec_FEWZ, DYReweight::nDiPtBin2, DYReweight::arr_diPtBinEdge2);
      }
      else
      {
        h_2DXSec_aMCNLO = DYReweight::ConvertTH1D_withLabel(h_2DXSec_aMCNLO, DYReweight::nDiPtBin, DYReweight::arr_diPtBinEdge);
        h_2DXSec_FEWZ   = DYReweight::ConvertTH1D_withLabel(h_2DXSec_FEWZ, DYReweight::nDiPtBin, DYReweight::arr_diPtBinEdge);
      }

      // -- to individually check the distribution
      DrawCanvas_aMCNLO_FEWZ( h_2DXSec_aMCNLO, h_2DXSec_FEWZ, minM, maxM, minY, maxY );

      TH1D* h_kFactor = (TH1D*)h_2DXSec_aMCNLO->Clone();
      h_kFactor->Divide( h_2DXSec_FEWZ, h_2DXSec_aMCNLO );

      // -- print values
      TString FEWZInfo    = TString::Format("[%.1lf < |Y| < %.1lf] ", minY, maxY);
      TString aMCNLOInfo  = TString::Format("[%.1lf < |Y| < %.1lf] ", minY, maxY);
      TString kFactorInfo = TString::Format("[%.1lf < |Y| < %.1lf] ", minY, maxY);
      Int_t nBin = h_kFactor->GetNbinsX();
      for(Int_t i=0; i<nBin; i++)
      {
        Int_t i_bin = i+1;

        Double_t FEWZ = h_2DXSec_FEWZ->GetBinContent(i_bin);
        FEWZInfo = FEWZInfo + TString::Format("%.5lf  ", FEWZ);

        Double_t aMCNLO = h_2DXSec_aMCNLO->GetBinContent(i_bin);
        aMCNLOInfo = aMCNLOInfo + TString::Format("%.5lf  ", aMCNLO);

        Double_t kFactor = h_kFactor->GetBinContent(i_bin);
        kFactorInfo = kFactorInfo + TString::Format("%.5lf  ", kFactor);
      }
      vec_FEWZInfo.push_back( FEWZInfo );
      vec_aMCNLOInfo.push_back( aMCNLOInfo );
      vec_kFactorInfo.push_back( kFactorInfo );

      Double_t localMin = h_kFactor->GetMinimum();
      if( localMin < globalMin ) globalMin = localMin;

      Double_t localMax = h_kFactor->GetMaximum();
      if( localMax > globalMax ) globalMax = localMax;

      TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
      canvas->Register( h_kFactor, legend, DYReweight::vec_rapColor[i_rap]);
    } // -- end of rapidity iteration

    canvas->SetTitle("p_{T}(ll) [GeV]", "FEWZ/aMC@NLO");

    canvas->Latex_CMSSim();
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Ratio of d#sigma/dp_{T}d|Y| (k-factor)}}");
    TString massInfo = TString::Format("%.0lf < M < %.0lf", minM, maxM);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+massInfo+"}}");

    canvas->SetLegendPosition( 0.65, 0.65, 0.95, 0.95 );

    Double_t minFactor = 0.8;
    // if( globalMin < 0 ) minFactor = 1.2;
    if( globalMin < 0 ) minFactor = 0.0;
    canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);
    canvas->Draw("EPSAME");

    printf("%.0lf < M < %.0lf\n", minM, maxM);

    Int_t nInfo = (Int_t)vec_FEWZInfo.size();

    printf("[FEWZ cross sections]\n");
    for(Int_t i=0; i<nInfo; i++)
      printf("%s\n", vec_FEWZInfo[i].Data() );
    printf("\n");

    printf("[aMCNLO cross sections]\n");
    for(Int_t i=0; i<nInfo; i++)
      printf("%s\n", vec_aMCNLOInfo[i].Data() );
    printf("\n");

    printf("[k-factors]\n");
    for(Int_t i=0; i<nInfo; i++)
      printf("%s\n", vec_kFactorInfo[i].Data() );
    printf("\n");
    printf("\n");
  } // -- end of mass iteration
}

void CheckCompatibility(TH1D* h_aMCNLO, TH1D* h_FEWZ)
{
  Int_t nBin_aMCNLO = h_aMCNLO->GetNbinsX();
  Int_t nBin_FEWZ = h_FEWZ->GetNbinsX();
  if( nBin_aMCNLO != nBin_FEWZ )
  {
    printf("[CheckCompatibility] nBin (aMC@NLO, FEWZ) = (%d, %d) ... need to check\n", nBin_aMCNLO, nBin_FEWZ);
    return;
  }

  // -- if nBin is same
  Int_t nBin = nBin_FEWZ;

  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    Double_t minPt_aMCNLO = h_aMCNLO->GetBinLowEdge(i_bin);
    Double_t maxPt_aMCNLO = h_aMCNLO->GetBinLowEdge(i_bin+1);

    Double_t minPt_FEWZ = h_FEWZ->GetBinLowEdge(i_bin);
    Double_t maxPt_FEWZ = h_FEWZ->GetBinLowEdge(i_bin+1);

    if( (minPt_aMCNLO != minPt_FEWZ) || (maxPt_aMCNLO != maxPt_FEWZ) )
    {
      printf("[CheckCompatibility] Dilepton pT range is different in %02d bin!\n", i_bin);
      printf("   aMC@NLO: (%.1lf, %.1lf)\n", minPt_aMCNLO, maxPt_aMCNLO);
      printf("   FEWZ:    (%.1lf, %.1lf)\n", minPt_FEWZ,   maxPt_FEWZ);
      return;
    }

    Double_t minPt = minPt_aMCNLO;
    Double_t maxPt = maxPt_aMCNLO;

    Double_t binContent_aMCNLO = h_aMCNLO->GetBinContent(i_bin);
    Double_t binContent_FEWZ =   h_FEWZ->GetBinContent(i_bin);

    if( binContent_aMCNLO < 0 )
      printf("[CheckCompatibility] [%02d bin] dilepton pT = [%.1lf, %.1lf]: Cross section (aMC@NLO)= %lf < 0\n", i_bin, minPt, maxPt, binContent_aMCNLO);

    if( binContent_FEWZ < 0 )
      printf("[CheckCompatibility] [%02d bin] dilepton pT = [%.1lf, %.1lf]: Cross section (aMC@NLO)= %lf < 0\n", i_bin, minPt, maxPt, binContent_FEWZ);
  }
  printf("\n");
}

void DrawCanvas_aMCNLO_FEWZ(TH1D* h_aMCNLO, TH1D* h_FEWZ, Double_t minM, Double_t maxM, Double_t minY, Double_t maxY )
{
  TString canvasName = TString::Format("c_dSigmadPtdY_aMCNLO_vs_FEWZ_M%.0lftoM%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
  PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 0);
  canvas->Register(h_aMCNLO, "aMC@NLO", kRed);
  canvas->Register(h_FEWZ, "FEWZ (QCD@NNLO + EW@NLO)", kBlue);
  canvas->SetTitle("p_{T}(ll) [GeV]", "d#sigma/dp_{T}d|Y| [pb/GeV]", "FEWZ/aMCNLO");
  canvas->SetLegendPosition( 0.50, 0.75, 0.95, 0.90 );
  canvas->SetRangeRatio(0.4, 1.6);

  Double_t globalMin = 9999;
  Double_t globalMax = -9999;
  FindGlobalMinMax( h_aMCNLO, h_FEWZ, globalMin, globalMax );
  Double_t minFactor = 0.8;
  if( globalMin < 0 ) minFactor = 1.2;
  canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);

  canvas->Latex_CMSSim();
  TString phaseSpaceInfo = TString::Format("%.0lf < M < %.0lf, %.1lf < |Y| < %.1lf", minM, maxM, minY, maxY);
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+phaseSpaceInfo+"}}");

  canvas->Draw("EPSAME");
}

void FindGlobalMinMax(TH1D* h1, TH1D* h2, Double_t& globalMin, Double_t& globalMax )
{
  Double_t localMin1 = h1->GetMinimum();
  Double_t localMax1 = h1->GetMaximum();

  Double_t localMin2 = h2->GetMinimum();
  Double_t localMax2 = h2->GetMaximum();

  if( localMin1 < localMin2 )
    globalMin = localMin1;
  else
    globalMin = localMin2;

  if( localMax1 > localMax2 )
    globalMax = localMax1;
  else
    globalMax = localMax2;
}