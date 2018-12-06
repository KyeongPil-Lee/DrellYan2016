#include <Include/SimplePlotTools.h>

TH1D* ConvertTH1D_withLabel( TH1D* h, vector<Double_t> vec_binEdge )
{
  Int_t nVecBin = (Int_t)vec_binEdge.size() - 1;
  Int_t nBin = h->GetNbinsX();
  if( nBin != nVecBin )
  {
    printf("(nBin, nBinEdge-1) = (%d, %d): different!", nBin, nVecBin);
    return nullptr;
  }
  
  TH1D* h_return = new TH1D(h->GetName(), "", nBin, 0, nBin);

  for(Int_t i=0; i<nBin; i++)
  {
    Int_t i_bin = i+1;

    TString binLabel = TString::Format("[%.0lf, %.0lf]", vec_binEdge[i], vec_binEdge[i+1]);
    h_return->GetXaxis()->SetBinLabel(i_bin, binLabel);

    Double_t binContent = h->GetBinContent(i_bin);
    Double_t binError = h->GetBinError(i_bin);

    h_return->SetBinContent(i_bin, binContent);
    h_return->SetBinError(i_bin, binError);
  }

  return h_return;
}

void DrawPlot_aMCNLO_GivenEdges(vector<Double_t> vec_massBinEdge, vector<Double_t> vec_diPtBinEdge, vector<Double_t> vec_diRapBinEdge)
{
  vector<Int_t> vec_rapColor = {kBlack,   kRed,     kBlue,   kGreen+2, kViolet, 
                                kCyan,    kMagenta, kBlue-9, kGray,    kOrange+2 };

  TString fileName = "ROOTFile_DYMuMu.root";

  Int_t nMassBin = (Int_t)vec_massBinEdge.size() - 1;

  for(Int_t i_mass=0; i_mass<nMassBin; i_mass++)
  {
    Double_t minM = vec_massBinEdge[i_mass];
    Double_t maxM = vec_massBinEdge[i_mass+1];

    TString canvasName = TString::Format("c_diPt_M%.0lftoM%.0lf_aMCNLO", minM, maxM);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);

    // -- to automatically adjust the Y range per plot
    Double_t globalMin = 9999;
    Double_t globalMax = -9999;

    Int_t nDiRapBin = (Int_t)vec_diRapBinEdge.size() - 1;
    for(Int_t i_rap=0; i_rap<nDiRapBin; i_rap++)
    {
      Double_t minY = vec_diRapBinEdge[i_rap];
      Double_t maxY = vec_diRapBinEdge[i_rap+1];

      TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
      printf("Histogram name: %s\n", histName.Data());

      TH1D* h_diPt = PlotTool::Get_Hist(fileName, histName);
      h_diPt = PlotTool::DivideEachBin_ByBinWidth(h_diPt); // -- divide each bin by pT bin width
      h_diPt->Scale( 1.0 / (maxY - minY) ); // -- divide by rapidity bin width also
      if( minM == 1000 && maxM == 3000 )
        h_diPt = ConvertTH1D_withLabel(h_diPt, vec_diPtBinEdge);
      else
        h_diPt = ConvertTH1D_withLabel(h_diPt, vec_diPtBinEdge);

      Double_t localMin = h_diPt->GetMinimum();
      if( localMin < globalMin ) globalMin = localMin;

      Double_t localMax = h_diPt->GetMaximum();
      if( localMax > globalMax ) globalMax = localMax;

      TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
      canvas->Register( h_diPt, legend, vec_rapColor[i_rap]);
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

    if( globalMin < 0 )
    {
      cout << "Negative bin exists" << endl;
    }
  }

}

void DrawPlot_aMCNLO()
{
  vector<Double_t> vec_massBinEdge_belowZPeak = {15, 20, 30, 45, 60};
  vector<Double_t> vec_diPtBinEdge_belowZPeak = {0, 20, 25, 30, 100, 1000};
  vector<Double_t> vec_diRapBinEdge_belowZPeak = {0, 1.5, 2.4, 100};
  DrawPlot_aMCNLO_GivenEdges(vec_massBinEdge_belowZPeak, vec_diPtBinEdge_belowZPeak, vec_diRapBinEdge_belowZPeak);

  vector<Double_t> vec_massBinEdge_ZPeak = {60, 72, 106, 120};
  vector<Double_t> vec_diPtBinEdge_ZPeak = {0, 10, 15, 20, 25, 30, 50, 70, 90, 1000};
  vector<Double_t> vec_diRapBinEdge_ZPeak = {0, 1.2, 1.8, 2.4, 100};
  DrawPlot_aMCNLO_GivenEdges(vec_massBinEdge_ZPeak, vec_diPtBinEdge_ZPeak, vec_diRapBinEdge_ZPeak);

  vector<Double_t> vec_massBinEdge_aboveZPeak = {120, 133, 150, 171, 200, 400, 510, 600, 1000};
  vector<Double_t> vec_diPtBinEdge_aboveZPeak = {0, 10, 15, 20, 25, 30, 40, 60, 80, 1000};
  vector<Double_t> vec_diRapBinEdge_aboveZPeak = {0, 0.9, 1.5, 2.4, 100};
  DrawPlot_aMCNLO_GivenEdges(vec_massBinEdge_aboveZPeak, vec_diPtBinEdge_aboveZPeak, vec_diRapBinEdge_aboveZPeak);

  vector<Double_t> vec_massBinEdge_highMass = {1000, 3000};
  vector<Double_t> vec_diPtBinEdge_highMass = {0, 20, 30, 100, 1000};
  vector<Double_t> vec_diRapBinEdge_highMass = {0, 0.9, 1.5, 100};
  DrawPlot_aMCNLO_GivenEdges(vec_massBinEdge_highMass, vec_diPtBinEdge_highMass, vec_diRapBinEdge_highMass);
}