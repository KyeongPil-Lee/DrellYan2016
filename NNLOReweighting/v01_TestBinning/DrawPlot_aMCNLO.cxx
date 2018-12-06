#include <Include/SimplePlotTools.h>
#include <NNLOReweighting/DYReweightingTool.h>

void DrawPlot_aMCNLO()
{
  TString fileName = "ROOTFile_DYMuMu.root";

  for(Int_t i_mass=0; i_mass<DYReweight::nMassBin; i_mass++)
  {
    Double_t minM = DYReweight::arr_massBinEdge[i_mass];
    Double_t maxM = DYReweight::arr_massBinEdge[i_mass+1];

    TString canvasName = TString::Format("c_diPt_M%.0lftoM%.0lf_aMCNLO", minM, maxM);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);

    // -- to automatically adjust the Y range per plot
    Double_t globalMin = 9999;
    Double_t globalMax = -9999;

    for(Int_t i_rap=0; i_rap<DYReweight::nDiRapBin; i_rap++)
    {
      Double_t minY = DYReweight::arr_diRapBinEdge[i_rap];
      Double_t maxY = DYReweight::arr_diRapBinEdge[i_rap+1];

      TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
      printf("Histogram name: %s\n", histName.Data());

      TH1D* h_diPt = PlotTool::Get_Hist(fileName, histName);
      h_diPt = PlotTool::DivideEachBin_ByBinWidth(h_diPt); // -- divide each bin by pT bin width
      h_diPt->Scale( 1.0 / (maxY - minY) ); // -- divide by rapidity bin width also
      if( minM == 1000 && maxM == 3000 )
        h_diPt = DYReweight::ConvertTH1D_withLabel(h_diPt, DYReweight::nDiPtBin2, DYReweight::arr_diPtBinEdge2);
      else
        h_diPt = DYReweight::ConvertTH1D_withLabel(h_diPt, DYReweight::nDiPtBin, DYReweight::arr_diPtBinEdge);

      Double_t localMin = h_diPt->GetMinimum();
      if( localMin < globalMin ) globalMin = localMin;

      Double_t localMax = h_diPt->GetMaximum();
      if( localMax > globalMax ) globalMax = localMax;

      TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
      canvas->Register( h_diPt, legend, DYReweight::vec_rapColor[i_rap]);
    }

    canvas->SetTitle("p_{T}(ll) [GeV]", "d#sigma/dp_{T}d|Y| [pb/GeV]");

    canvas->Latex_CMSSim();
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{aMC@NLO}}");
    TString massInfo = TString::Format("%.0lf < M < %.0lf", minM, maxM);
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+massInfo+"}}");

    canvas->SetLegendPosition( 0.65, 0.65, 0.95, 0.95 );

    Double_t minFactor = 0.8;
    if( globalMin < 0 ) minFactor = 1.2;
    canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);
    canvas->Draw("EPSAME");
  }

}
