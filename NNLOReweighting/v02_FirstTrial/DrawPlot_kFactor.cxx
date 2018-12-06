#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  vector<Double_t> vec_massBinEdge_;
  vector<Double_t> vec_diPtBinEdge_;
  vector<Double_t> vec_diRapBinEdge_;

  Int_t nMassBin_;
  Int_t nDiPtBin_;
  Int_t nDiRapBin_;

  vector<Int_t> vec_rapColor_;

  TString fileName_aMCNLO_;
  TString fileName_FEWZ_;

  PlotProducer( vector<Double_t> vec_massBinEdge, vector<Double_t> vec_diPtBinEdge, vector<Double_t> vec_diRapBinEdge)
  {
    vec_massBinEdge_ = vec_massBinEdge;
    vec_diPtBinEdge_ = vec_diPtBinEdge;
    vec_diRapBinEdge_ = vec_diRapBinEdge;

    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;
    nDiPtBin_ = (Int_t)vec_diPtBinEdge_.size() - 1;
    nDiRapBin_ = (Int_t)vec_diRapBinEdge_.size() - 1;

    vector<Int_t> vec_rapColor = {kBlack,   kRed,     kBlue,   kGreen+2, kViolet, 
                                  kCyan,    kMagenta, kBlue-9, kGray,    kOrange+2 };
    vec_rapColor_ = vec_rapColor;
  }

  void SetFileName( TString fileName_aMCNLO, TString fileName_FEWZ )
  {
    fileName_aMCNLO_ = fileName_aMCNLO;
    fileName_FEWZ_ = fileName_FEWZ;
  }

  void DrawPlots()
  {
    DrawCrossSectionPlot("aMCNLO");
    DrawCrossSectionPlot("FEWZ");
    DrawKFactorPlot();
  }

private:
  void DrawCrossSectionPlot(TString type)
  {
    cout << "[PlotProducer::DrawCrossSectionPlot] type = " << type << endl;

    TString fileName = "";
    if( type == "aMCNLO" ) fileName = fileName_aMCNLO_;
    if( type == "FEWZ" )   fileName = fileName_FEWZ_;

    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];

      TString canvasName = TString::Format("c_%s_diPt_M%.0lftoM%.0lf", type.Data(), minM, maxM);
      PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);


      vector<TH1D*> vec_DXSecHist; // -- for calculate global min & max
      for(Int_t i_rap=0; i_rap<nDiRapBin_; i_rap++)
      {
        Double_t minY = vec_diRapBinEdge_[i_rap];
        Double_t maxY = vec_diRapBinEdge_[i_rap+1];

        TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
        printf("Histogram name: %s\n", histName.Data());

        TH1D* h_diPt = PlotTool::Get_Hist(fileName, histName);
        CheckNegativeBin(h_diPt);

        TH1D* h_DXSec = Convert_dSigmadPtdY(h_diPt, minY, maxY);
        h_DXSec = ConvertTH1D_withLabel(h_DXSec, vec_diPtBinEdge_);
        vec_DXSecHist.push_back( h_DXSec );

        TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
        canvas->Register( h_DXSec, legend, vec_rapColor_[i_rap]);
      }

      Double_t globalMin, globalMax;
      FindMinMax(vec_DXSecHist, globalMin, globalMax);

      canvas->SetTitle("p_{T}(ll) [GeV]", "d#sigma/dp_{T}d|Y| [pb/GeV]");

      canvas->Latex_CMSSim();
      if( type == "aMCNLO") canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{aMC@NLO}}");
      if( type == "FEWZ")   canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{FEWZ 3.1 (QCD@NNLO, EWK@NLO)}}");
      TString massInfo = TString::Format("%.0lf < M < %.0lf GeV", minM, maxM);
      canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+massInfo+"}}");

      canvas->SetLegendPosition( 0.65, 0.65, 0.95, 0.95 );

      Double_t minFactor = 0.8;
      if( globalMin < 0 ) minFactor = 1.2;
      canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);
      canvas->Draw("EPSAME");

      cout << endl;
    }
  }

  void DrawKFactorPlot()
  {
    cout << "[PlotProducer::DrawKFactorPlot]" << endl;
    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];

      TString canvasName = TString::Format("c_kFactor_diPt_M%.0lftoM%.0lf", minM, maxM);
      PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, 0, 0);

      vector<TH1D*> vec_kFactorHist; // -- for calculate global min & max
      for(Int_t i_rap=0; i_rap<nDiRapBin_; i_rap++)
      {
        Double_t minY = vec_diRapBinEdge_[i_rap];
        Double_t maxY = vec_diRapBinEdge_[i_rap+1];

        TString histName = TString::Format("h_diPt_M%.0lfto%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
        printf("Histogram name: %s\n", histName.Data());

        TH1D* h_diPt_aMCNLO = PlotTool::Get_Hist(fileName_aMCNLO_, histName);
        TH1D* h_DXSec_aMCNLO = Convert_dSigmadPtdY(h_diPt_aMCNLO, minY, maxY);
        h_DXSec_aMCNLO = ConvertTH1D_withLabel(h_DXSec_aMCNLO, vec_diPtBinEdge_);

        TH1D* h_diPt_FEWZ = PlotTool::Get_Hist(fileName_FEWZ_, histName);
        TH1D* h_DXSec_FEWZ = Convert_dSigmadPtdY(h_diPt_FEWZ, minY, maxY);
        h_DXSec_FEWZ = ConvertTH1D_withLabel(h_DXSec_FEWZ, vec_diPtBinEdge_);

        // -- direct comparison in each range
        DrawCanvas_aMCNLOvsFEWZ( h_DXSec_aMCNLO, h_DXSec_FEWZ, minM, maxM, minY, maxY );

        // -- k-factor calculation
        TH1D* h_kFactor = (TH1D*)h_DXSec_FEWZ->Clone();
        h_kFactor->Reset("ICES");
        h_kFactor->Divide( h_DXSec_FEWZ, h_DXSec_aMCNLO );
        vec_kFactorHist.push_back( h_kFactor );

        TString legend = TString::Format("%.1lf < |Y| < %.1lf", minY, maxY);
        canvas->Register( h_kFactor, legend, vec_rapColor_[i_rap]);
      }

      Double_t globalMin, globalMax;
      FindMinMax(vec_kFactorHist, globalMin, globalMax);

      canvas->SetTitle("p_{T}(ll) [GeV]", "FEWZ/aMC@NLO");

      canvas->Latex_CMSSim();
      canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{Ratio of d#sigma/dp_{T}d|Y| (k-factor)}}");
      TString massInfo = TString::Format("%.0lf < M < %.0lf GeV", minM, maxM);
      canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+massInfo+"}}");

      canvas->SetLegendPosition( 0.65, 0.65, 0.95, 0.95 );

      Double_t minFactor = 0.8;
      if( globalMin < 0 ) minFactor = 1.2;
      canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);
      canvas->Draw("EPSAME");

      cout << endl;
    }
  }

  void DrawCanvas_aMCNLOvsFEWZ(TH1D* h_aMCNLO, TH1D* h_FEWZ, Double_t minM, Double_t maxM, Double_t minY, Double_t maxY)
  {
    TString canvasName = TString::Format("c_dSigmadPtdY_aMCNLOvsFEWZ_M%.0lftoM%.0lf_Y%.1lfto%.1lf", minM, maxM, minY, maxY);
    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 0);
    canvas->Register(h_aMCNLO, "aMC@NLO", kRed);
    canvas->Register(h_FEWZ, "FEWZ (QCD@NNLO + EW@NLO)", kBlue);
    canvas->SetTitle("p_{T}(ll) [GeV]", "d#sigma/dp_{T}d|Y| [pb/GeV]", "FEWZ/aMCNLO");
    canvas->SetLegendPosition( 0.50, 0.75, 0.95, 0.90 );
    canvas->SetRangeRatio(0.4, 1.6);

    Double_t globalMin, globalMax;
    vector<TH1D*> vec_hist = {h_aMCNLO, h_FEWZ};
    FindMinMax(vec_hist, globalMin, globalMax);

    Double_t minFactor = 0.8;
    if( globalMin < 0 ) minFactor = 1.2;
    canvas->SetRangeY(globalMin*minFactor , globalMax*1.5);

    canvas->Latex_CMSSim();
    TString phaseSpaceInfo = TString::Format("%.0lf < M < %.0lf, %.1lf < |Y| < %.1lf", minM, maxM, minY, maxY);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{"+phaseSpaceInfo+"}}");

    canvas->Draw("EPSAME");
  }

  TH1D* Convert_dSigmadPtdY(TH1D* h, Double_t minY, Double_t maxY)
  {
    TH1D* h_DXSec = (TH1D*)h->Clone();
    h_DXSec = PlotTool::DivideEachBin_ByBinWidth(h_DXSec); // -- divide each bin by pT bin width
    h_DXSec->Scale( 1.0 / (maxY - minY) ); // -- divide by rapidity bin width also

    return h_DXSec;
  }

  void CheckNegativeBin(TH1D* h)
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binContent = h->GetBinContent(i_bin);

      if( binContent < 0 )
      {
        Double_t binEdgeDown = h->GetBinLowEdge(i_bin);
        Double_t binEdgeUp = h->GetBinLowEdge(i_bin+1);
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "[CheckNegativeBin] Negative bin is found!" << endl;
        cout << "   [" << binEdgeDown << ", " << binEdgeUp << "] value = " << binContent << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      }
    }
  }

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

  void FindMinMax(vector<TH1D*> vec_hist, Double_t &globalMin, Double_t &globalMax)
  {
    globalMin = 9999;
    globalMax = -9999;
    for(const auto& h : vec_hist )
    {
      Double_t localMin = h->GetMinimum();
      Double_t localMax = h->GetMaximum();
      if( localMin < globalMin ) globalMin = localMin;
      if( localMax > globalMax ) globalMax = localMax;
    }
  }
};

void DrawPlot_kFactor()
{
  TString fileName_FEWZ = "/Users/KyeongPil_Lee/Physics/FEWZTool/v01_Reweighting/Workspace/v13_ROOTHistForNNLOXSec_Merge20GeV/ROOTFile_FEWZ_NNLO_forDYReweighting.root";

  vector<Double_t> vec_massBinEdge_belowZPeak = {15, 20, 30, 45, 60};
  vector<Double_t> vec_diPtBinEdge_belowZPeak = {0, 20, 25, 30, 100, 1000};
  vector<Double_t> vec_diRapBinEdge_belowZPeak = {0, 1.5, 2.4, 100};
  PlotProducer *producer_belowZPeak = new PlotProducer(vec_massBinEdge_belowZPeak, vec_diPtBinEdge_belowZPeak, vec_diRapBinEdge_belowZPeak);  
  producer_belowZPeak->SetFileName("ROOTFile_DYMuMu.root", fileName_FEWZ);
  producer_belowZPeak->DrawPlots();


  vector<Double_t> vec_massBinEdge_ZPeak = {60, 72, 106, 120};
  vector<Double_t> vec_diPtBinEdge_ZPeak = {0, 10, 15, 20, 25, 30, 50, 70, 90, 1000};
  vector<Double_t> vec_diRapBinEdge_ZPeak = {0, 1.2, 1.8, 2.4, 100};
  PlotProducer *producer_ZPeak = new PlotProducer(vec_massBinEdge_ZPeak, vec_diPtBinEdge_ZPeak, vec_diRapBinEdge_ZPeak);  
  producer_ZPeak->SetFileName("ROOTFile_DYMuMu.root", fileName_FEWZ);
  producer_ZPeak->DrawPlots();


  vector<Double_t> vec_massBinEdge_aboveZPeak = {120, 133, 150, 171, 200, 400, 510, 600, 1000};
  vector<Double_t> vec_diPtBinEdge_aboveZPeak = {0, 10, 15, 20, 25, 30, 40, 60, 80, 1000};
  vector<Double_t> vec_diRapBinEdge_aboveZPeak = {0, 0.9, 1.5, 2.4, 100};
  PlotProducer *producer_aboveZPeak = new PlotProducer(vec_massBinEdge_aboveZPeak, vec_diPtBinEdge_aboveZPeak, vec_diRapBinEdge_aboveZPeak);  
  producer_aboveZPeak->SetFileName("ROOTFile_DYMuMu.root", fileName_FEWZ);
  producer_aboveZPeak->DrawPlots();


  vector<Double_t> vec_massBinEdge_highMass = {1000, 3000};
  vector<Double_t> vec_diPtBinEdge_highMass = {0, 20, 30, 100, 1000};
  vector<Double_t> vec_diRapBinEdge_highMass = {0, 0.9, 1.5, 100};
  PlotProducer *producer_highMass = new PlotProducer(vec_massBinEdge_highMass, vec_diPtBinEdge_highMass, vec_diRapBinEdge_highMass);  
  producer_highMass->SetFileName("ROOTFile_DYMuMu.root", fileName_FEWZ);
  producer_highMass->DrawPlots();
}