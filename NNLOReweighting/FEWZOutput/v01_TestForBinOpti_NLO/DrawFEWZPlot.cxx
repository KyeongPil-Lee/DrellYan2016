#include <Include/SimplePlotTools.h>

void DrawFEWZPlot()
{
  TString fileName = "ROOTFile_FEWZ_TestForBinOpti.root";

  vector<Double_t> vec_massBinEdge = {15, 60, 120, 1000, 3000};
  vector<Double_t> vec_diRapBinEdge = { 0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 3.0, 100 };
  vector<Int_t> vec_color = {kBlack,   kRed,     kBlue,   kGreen+2, kViolet, 
                             kCyan,    kMagenta, kBlue-9, kGray,    kOrange+2 };

  Int_t nMassBin = (Int_t)vec_massBinEdge.size() - 1;
  Int_t nDiRapBin = (Int_t)vec_diRapBinEdge.size() - 1;

  for(Int_t i_mass=0; i_mass<nMassBin; i_mass++)
  {
    Double_t minM = vec_massBinEdge[i_mass];
    Double_t maxM = vec_massBinEdge[i_mass+1];

    TString canvasName = TString::Format("c_diPt_M%.0lftoM%.0lf_FEWZ_NLO", minM, maxM);

    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 1, 0);

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

    canvas->SetLegendPosition( 0.60, 0.15, 0.95, 0.45 );

    canvas->SetRangeX(1e-1, 1000);
    Double_t minFactor = 0.8;
    if( globalMin < 0 ) minFactor = 1.2;
    canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);

   canvas->Draw("EPSAME");
  }

}