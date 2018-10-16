#include <Include/SimplePlotTools.h>

void Print_SumWeight(TString rootFileName)
{
  TH1D* h_sumWeight = PlotTool::Get_Hist(rootFileName, "h_sumWeight");
  Double_t sumWeight = h_sumWeight->GetBinContent(1);
  printf("[SumWeight] %lf\n", sumWeight);
}