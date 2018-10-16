#include <Include/SimplePlotTools.h>

TH1D* ConvertTH1D_withLabel( TH1D* h, vector<Double_t> vec_diPtBinEdge );

void DrawFEWZPlot()
{
  TString fileName = "ROOTFile_FEWZ_TestForBinOpti_v2.root";

  vector<Double_t> vec_massBinEdge = {15, 60, 120, 1000, 3000};
  vector<Double_t> vec_diPtBinEdge = { 0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 1000 };
  vector<Double_t> vec_diRapBinEdge = { 0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 100 };
  vector<Int_t> vec_color = {kBlack,   kRed,     kBlue,   kGreen+2, kViolet, 
                             kCyan,    kMagenta, kBlue-9, kGray,    kOrange+2 };

  Int_t nMassBin = (Int_t)vec_massBinEdge.size() - 1;
  Int_t nDiRapBin = (Int_t)vec_diRapBinEdge.size() - 1;

  for(Int_t i_mass=0; i_mass<nMassBin; i_mass++)
  {
    Double_t minM = vec_massBinEdge[i_mass];
    Double_t maxM = vec_massBinEdge[i_mass+1];

    TString canvasName = TString::Format("c_diPt_M%.0lftoM%.0lf_FEWZ_NLO", minM, maxM);

    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);

    // -- to automatically adjust the Y range per plot
    Double_t globalMin = 9999;
    Double_t globalMax = -9999;

    for(Int_t i_rap=0; i_rap<nDiRapBin; i_rap++)
    {
      Double_t minY = vec_diRapBinEdge[i_rap];
      Double_t maxY = vec_diRapBinEdge[i_rap+1];

      TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_diRap%.1lfto%.1lf", minM, maxM, minY, maxY);
      printf("Histogram name: %s\n", histName.Data());

      TH1D* h_diPt = PlotTool::Get_Hist(fileName, histName);
      h_diPt = PlotTool::DivideEachBin_ByBinWidth(h_diPt); // -- divide each bin by pT bin width
      h_diPt->Scale( 1.0 / (maxY - minY) ); // -- divide by rapidity bin width also
      h_diPt = ConvertTH1D_withLabel(h_diPt, vec_diPtBinEdge);

      Double_t localMin = h_diPt->GetMinimum();
      if( localMin < globalMin ) globalMin = localMin;

      Double_t localMax = h_diPt->GetMaximum();
      if( localMax > globalMax ) globalMax = localMax;

      TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
      canvas->Register( h_diPt, legend, vec_color[i_rap]);
    }

    canvas->SetTitle("p_{T}(ll) [GeV]", "d#sigma/dp_{T}d|Y| [pb/GeV]");

    canvas->Latex_CMSSim();
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{FEWZ 3.1 (QCD@NLO, EWK@LO)}}");

    canvas->SetLegendPosition( 0.65, 0.65, 0.95, 0.95 );

    Double_t minFactor = 0.8;
    if( globalMin < 0 ) minFactor = 1.2;
    canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);

   canvas->Draw("EPSAME");
  }

}

TH1D* ConvertTH1D_withLabel( TH1D* h, vector<Double_t> vec_diPtBinEdge )
{
  Int_t nBin = h->GetNbinsX();
  if( nBin != ((Int_t)vec_diPtBinEdge.size() - 1) )
  {
    printf("(nBin, nBinEdge-1) = (%d, %d): different!", nBin, (Int_t)vec_diPtBinEdge.size() - 1);
    return nullptr;
  }
  
  TH1D* h_return = new TH1D(h->GetName(), "", nBin, 0, nBin);

  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    TString binLabel = TString::Format("[%.0lf, %.0lf]", vec_diPtBinEdge[i], vec_diPtBinEdge[i+1]);
    h_return->GetXaxis()->SetBinLabel(i_bin, binLabel);

    Double_t binContent = h->GetBinContent(i_bin);
    Double_t binError = h->GetBinError(i_bin);

    h_return->SetBinContent(i_bin, binContent);
    h_return->SetBinError(i_bin, binError);
  }

  return h_return;
}