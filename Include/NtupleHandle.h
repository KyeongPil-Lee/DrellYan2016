#pragma once

#define ArrSize 50000
#include <TTree.h>
#include <TTreeCache.h>
#include <TChain.h>
#include <vector>

// -- for the physical variables, do not use _ at the end of the variable name.
// -- because getter functions for those variables will not be created (too many!)
class NtupleHandle
{
public:
  TChain *chain_;

  // -- Event Informations
  Int_t nVertex;
  Int_t runNum;
  Int_t lumiBlock;
  Int_t evtNum;
  Int_t nPileUp;

  // -- Trigger variables
  Int_t HLT_nTrig;
  Int_t HLT_trigFired[ArrSize];
  vector<string> *HLT_trigName;
  Double_t HLT_trigPt[ArrSize];
  Double_t HLT_trigEta[ArrSize];
  Double_t HLT_trigPhi[ArrSize];

  // -- Generator level information
  Int_t nGenLepton;

  Double_t genEvent_weight;
  Double_t genEvent_QScale;
  Double_t genEvent_x1;
  Double_t genEvent_x2;
  Double_t genEvent_alphaQCD;
  Double_t genEvent_alphaQED;

  Double_t genLepton_px[ArrSize];
  Double_t genLepton_py[ArrSize];
  Double_t genLepton_pz[ArrSize];
  Double_t genLepton_mother[ArrSize];
  Double_t genLepton_pt[ArrSize];
  Double_t genLepton_eta[ArrSize];
  Double_t genLepton_phi[ArrSize];
  Int_t genLepton_charge[ArrSize];
  Int_t genLepton_status[ArrSize];
  Int_t genLepton_ID[ArrSize];
  Int_t genLepton_isPrompt[ArrSize];
  Int_t genLepton_isPromptFinalState[ArrSize];
  Int_t genLepton_isTauDecayProduct[ArrSize];
  Int_t genLepton_isPromptTauDecayProduct[ArrSize];
  Int_t genLepton_isDirectPromptTauDecayProductFinalState[ArrSize];
  Int_t genLepton_isHardProcess[ArrSize];
  Int_t genLepton_isLastCopy[ArrSize];
  Int_t genLepton_isLastCopyBeforeFSR[ArrSize];
  Int_t genLepton_isPromptDecayed[ArrSize];
  Int_t genLepton_isDecayedLeptonHadron[ArrSize];
  Int_t genLepton_fromHardProcessBeforeFSR[ArrSize];
  Int_t genLepton_fromHardProcessDecayed[ArrSize];
  Int_t genLepton_fromHardProcessFinalState[ArrSize];

  Int_t nGenOthers;
  Double_t genOthers_px[ArrSize];
  Double_t genOthers_py[ArrSize];
  Double_t genOthers_pz[ArrSize];
  Double_t genOthers_mother[ArrSize];
  Double_t genOthers_pt[ArrSize];
  Double_t genOthers_eta[ArrSize];
  Double_t genOthers_phi[ArrSize];
  Int_t genOthers_charge[ArrSize];
  Int_t genOthers_status[ArrSize];
  Int_t genOthers_ID[ArrSize];
  Int_t genOthers_isPrompt[ArrSize];
  Int_t genOthers_isPromptFinalState[ArrSize];
  Int_t genOthers_isTauDecayProduct[ArrSize];
  Int_t genOthers_isPromptTauDecayProduct[ArrSize];
  Int_t genOthers_isDirectPromptTauDecayProductFinalState[ArrSize];
  Int_t genOthers_isHardProcess[ArrSize];
  Int_t genOthers_isLastCopy[ArrSize];
  Int_t genOthers_isLastCopyBeforeFSR[ArrSize];
  Int_t genOthers_isPromptDecayed[ArrSize];
  Int_t genOthers_isDecayedLeptonHadron[ArrSize];
  Int_t genOthers_fromHardProcessBeforeFSR[ArrSize];
  Int_t genOthers_fromHardProcessDecayed[ArrSize];
  Int_t genOthers_fromHardProcessFinalState[ArrSize];

  //////////////////////////
  // -- Muon Variables -- //
  //////////////////////////
  Int_t nMuon;

  Double_t muon_pt[ArrSize];
  Double_t muon_eta[ArrSize];
  Double_t muon_phi[ArrSize];
  Double_t muon_px[ArrSize];
  Double_t muon_py[ArrSize];
  Double_t muon_pz[ArrSize];
  Int_t muon_charge[ArrSize]; 
  
  // -- Cut variables -- //
  Int_t muon_muonType[ArrSize];
  Double_t muon_chi2DOF[ArrSize];
  Int_t muon_nMuonHit[ArrSize];
  Int_t muon_nSegment[ArrSize];
  Int_t muon_nMatchedStation[ArrSize];
  Int_t muon_nTrackerLayer[ArrSize];

  // Int_t muon_trackerHitsGLB[ArrSize];
  Int_t muon_nPixelHitGLB[ArrSize];
  Int_t muon_nTrackerLayerGLB[ArrSize];
  
  Int_t muon_nPixelHit[ArrSize];
  Double_t muon_dB[ArrSize];
  Double_t muon_dxyVTX[ArrSize];
  Double_t muon_dzVTX[ArrSize];
  Double_t muon_trkIso[ArrSize];
  Int_t muon_isGLB[ArrSize];
  Int_t muon_isPF[ArrSize];
  Int_t muon_isTRK[ArrSize];

  Double_t muon_PFIsoR04_CH[ArrSize];
  Double_t muon_PFIsoR04_NH[ArrSize];
  Double_t muon_PFIsoR04_photon[ArrSize];
  Double_t muon_PFIsoR04_sumPt[ArrSize];

  // -- dimuon vertex variables
  std::vector<double> *diMu_cosAngle_inner;
  std::vector<double> *diMu_vtxChi2_inner;
  std::vector<double> *diMu_vtxProb_inner;
  std::vector<double> *diMu_vtxNDOF_inner;
  std::vector<double> *diMu_vtxPt1_inner;
  std::vector<double> *diMu_vtxPt2_inner;

  std::vector<double> *diMu_cosAngle_TuneP;
  std::vector<double> *diMu_vtxPt1_TuneP;
  std::vector<double> *diMu_vtxPt2_TuneP;
  std::vector<double> *diMu_vtxChi2_TuneP;
  std::vector<double> *diMu_vtxNDOF_TuneP;
  std::vector<double> *diMu_vtxProb_TuneP;

  // -- emu vertex variables
  std::vector<double> *emu_vtxChi2;
  std::vector<double> *emu_vtxProb;
  std::vector<double> *emu_vtxNDOF;
  std::vector<double> *emu_vtxPt1;
  std::vector<double> *emu_vtxPt2;

  // -- Various Track Information -- //
  Double_t muon_best_pt[ArrSize];
  Double_t muon_best_ptError[ArrSize];
  Double_t muon_best_px[ArrSize];
  Double_t muon_best_py[ArrSize];
  Double_t muon_best_pz[ArrSize];
  Double_t muon_best_eta[ArrSize];
  Double_t muon_best_phi[ArrSize];

  Double_t muon_inner_pt[ArrSize];
  Double_t muon_inner_ptError[ArrSize];
  Double_t muon_inner_px[ArrSize];
  Double_t muon_inner_py[ArrSize];
  Double_t muon_inner_pz[ArrSize];
  Double_t muon_inner_eta[ArrSize];
  Double_t muon_inner_phi[ArrSize];

  Double_t muon_outer_pt[ArrSize];
  Double_t muon_outer_ptError[ArrSize];
  Double_t muon_outer_px[ArrSize];
  Double_t muon_outer_py[ArrSize];
  Double_t muon_outer_pz[ArrSize];
  Double_t muon_outer_eta[ArrSize];
  Double_t muon_outer_phi[ArrSize];

  Double_t muon_global_pt[ArrSize];
  Double_t muon_global_ptError[ArrSize];
  Double_t muon_global_px[ArrSize];
  Double_t muon_global_py[ArrSize];
  Double_t muon_global_pz[ArrSize];
  Double_t muon_global_eta[ArrSize];
  Double_t muon_global_phi[ArrSize];

  Double_t muon_tuneP_pt[ArrSize];
  Double_t muon_tuneP_ptError[ArrSize];
  Double_t muon_tuneP_px[ArrSize];
  Double_t muon_tuneP_py[ArrSize];
  Double_t muon_tuneP_pz[ArrSize];
  Double_t muon_tuneP_eta[ArrSize];
  Double_t muon_tuneP_phi[ArrSize];

  Int_t muon_stationMask[ArrSize];
  Int_t muon_nMatchedRPCLayer[ArrSize];

  // -- dielectron vertex variables
  std::vector<double> *diEl_vtxChi2;
  std::vector<double> *diEl_vtxProb;
  std::vector<double> *diEl_vtxNDOF;
  std::vector<double> *diEl_vtxPt1;
  std::vector<double> *diEl_vtxPt2;

  NtupleHandle()
  {
    cout << "Default constructor ... nothing happens" << endl;
  }

  NtupleHandle( TChain *chain )
  {
    chain_ = chain;

    chain_->SetBranchStatus("*", 0);
    // TurnOnBranches_GenLepton();
    // TurnOnBranches_GenOthers();
  }

  void GetEvent(Int_t index)
  {
    chain_->GetEntry(index);
  }

  void TurnOnBranches_EventInfo()
  {
    // -- Event Information -- //
    chain_->SetBranchStatus("runNum", 1);
    chain_->SetBranchStatus("lumiBlock", 1);
    chain_->SetBranchStatus("evtNum", 1);
    chain_->SetBranchStatus("nPileUp", 1);
    chain_->SetBranchStatus("nVertices", 1);

    chain_->SetBranchAddress("runNum",    &runNum);
    chain_->SetBranchAddress("lumiBlock", &lumiBlock);
    chain_->SetBranchAddress("evtNum",    &evtNum);
    chain_->SetBranchAddress("nPileUp",   &nPileUp);
    chain_->SetBranchAddress("nVertices", &nVertex);

    // -- Trigger Information -- //
    chain_->SetBranchStatus("HLT_trigName", 1);
    chain_->SetBranchStatus("HLT_ntrig", 1);
    chain_->SetBranchStatus("HLT_trigFired", 1);
    chain_->SetBranchStatus("HLT_trigPt", 1);
    chain_->SetBranchStatus("HLT_trigEta", 1);
    chain_->SetBranchStatus("HLT_trigPhi", 1);

    chain_->SetBranchAddress("HLT_trigName",  &HLT_trigName);
    chain_->SetBranchAddress("HLT_trigFired", &HLT_trigFired);
    chain_->SetBranchAddress("HLT_ntrig",     &HLT_nTrig);
    chain_->SetBranchAddress("HLT_trigPt",    &HLT_trigPt);
    chain_->SetBranchAddress("HLT_trigEta",   &HLT_trigEta);
    chain_->SetBranchAddress("HLT_trigPhi",   &HLT_trigPhi);
  }

  void TurnOnBranches_GenLepton()
  {
    //Generator level information
    chain_->SetBranchStatus("GENnPair", 1);
    chain_->SetBranchStatus("GENEvt_weight", 1);
    chain_->SetBranchStatus("GENEvt_QScale", 1);
    chain_->SetBranchStatus("GENEvt_x1", 1);
    chain_->SetBranchStatus("GENEvt_x2", 1);
    chain_->SetBranchStatus("GENEvt_alphaQCD", 1);
    chain_->SetBranchStatus("GENEvt_alphaQED", 1);
    chain_->SetBranchStatus("GENLepton_Px", 1);
    chain_->SetBranchStatus("GENLepton_Py", 1);
    chain_->SetBranchStatus("GENLepton_Pz", 1);
    chain_->SetBranchStatus("GENLepton_mother", 1);
    chain_->SetBranchStatus("GENLepton_pT", 1);
    chain_->SetBranchStatus("GENLepton_eta", 1);
    chain_->SetBranchStatus("GENLepton_phi", 1);
    chain_->SetBranchStatus("GENLepton_charge", 1);
    chain_->SetBranchStatus("GENLepton_status", 1);
    chain_->SetBranchStatus("GENLepton_ID", 1);
    chain_->SetBranchStatus("GENLepton_isPrompt", 1);
    chain_->SetBranchStatus("GENLepton_isPromptFinalState", 1);
    chain_->SetBranchStatus("GENLepton_isTauDecayProduct", 1);
    chain_->SetBranchStatus("GENLepton_isPromptTauDecayProduct", 1);
    chain_->SetBranchStatus("GENLepton_isDirectPromptTauDecayProductFinalState", 1);
    chain_->SetBranchStatus("GENLepton_isHardProcess", 1);
    chain_->SetBranchStatus("GENLepton_isLastCopy", 1);
    chain_->SetBranchStatus("GENLepton_isLastCopyBeforeFSR", 1);
    chain_->SetBranchStatus("GENLepton_isPromptDecayed", 1);
    chain_->SetBranchStatus("GENLepton_isDecayedLeptonHadron", 1);
    chain_->SetBranchStatus("GENLepton_fromHardProcessBeforeFSR", 1);
    chain_->SetBranchStatus("GENLepton_fromHardProcessDecayed", 1);
    chain_->SetBranchStatus("GENLepton_fromHardProcessFinalState", 1);

    chain_->SetBranchAddress("GENnPair", &nGenLepton);
    chain_->SetBranchAddress("GENLepton_Px", genLepton_px);
    chain_->SetBranchAddress("GENLepton_Py", genLepton_py);
    chain_->SetBranchAddress("GENLepton_Pz", genLepton_pz);
    chain_->SetBranchAddress("GENLepton_mother", &genLepton_mother);
    chain_->SetBranchAddress("GENLepton_pT", genLepton_pt);
    chain_->SetBranchAddress("GENLepton_eta", genLepton_eta);
    chain_->SetBranchAddress("GENLepton_phi", genLepton_phi);
    chain_->SetBranchAddress("GENLepton_charge", &genLepton_charge);
    chain_->SetBranchAddress("GENLepton_status", &genLepton_status);
    chain_->SetBranchAddress("GENLepton_ID", &genLepton_ID);
    chain_->SetBranchAddress("GENEvt_weight", &genEvent_weight);
    chain_->SetBranchAddress("GENEvt_QScale", &genEvent_QScale);
    chain_->SetBranchAddress("GENEvt_x1", &genEvent_x1);
    chain_->SetBranchAddress("GENEvt_x2", &genEvent_x2);
    chain_->SetBranchAddress("GENEvt_alphaQCD", &genEvent_alphaQCD);
    chain_->SetBranchAddress("GENEvt_alphaQED", &genEvent_alphaQED);

    chain_->SetBranchAddress("GENLepton_isPrompt", &genLepton_isPrompt);
    chain_->SetBranchAddress("GENLepton_isPromptFinalState", &genLepton_isPromptFinalState);
    chain_->SetBranchAddress("GENLepton_isTauDecayProduct", &genLepton_isTauDecayProduct);
    chain_->SetBranchAddress("GENLepton_isPromptTauDecayProduct", &genLepton_isPromptTauDecayProduct);
    chain_->SetBranchAddress("GENLepton_isDirectPromptTauDecayProductFinalState", &genLepton_isDirectPromptTauDecayProductFinalState);
    chain_->SetBranchAddress("GENLepton_isHardProcess", &genLepton_isHardProcess);
    chain_->SetBranchAddress("GENLepton_isLastCopy", &genLepton_isLastCopy);
    chain_->SetBranchAddress("GENLepton_isLastCopyBeforeFSR", &genLepton_isLastCopyBeforeFSR);
    chain_->SetBranchAddress("GENLepton_isPromptDecayed", &genLepton_isPromptDecayed);
    chain_->SetBranchAddress("GENLepton_isDecayedLeptonHadron", &genLepton_isDecayedLeptonHadron);
    chain_->SetBranchAddress("GENLepton_fromHardProcessBeforeFSR", &genLepton_fromHardProcessBeforeFSR);
    chain_->SetBranchAddress("GENLepton_fromHardProcessDecayed", &genLepton_fromHardProcessDecayed);
    chain_->SetBranchAddress("GENLepton_fromHardProcessFinalState", &genLepton_fromHardProcessFinalState);
  }

  void TurnOnBranches_GenOthers()
  {
    chain_->SetBranchStatus("nGenOthers", 1);
    chain_->SetBranchStatus("GenOthers_Px", 1);
    chain_->SetBranchStatus("GenOthers_Py", 1);
    chain_->SetBranchStatus("GenOthers_Pz", 1);
    chain_->SetBranchStatus("GenOthers_mother", 1);
    chain_->SetBranchStatus("GenOthers_pT", 1);
    chain_->SetBranchStatus("GenOthers_eta", 1);
    chain_->SetBranchStatus("GenOthers_phi", 1);
    chain_->SetBranchStatus("GenOthers_charge", 1);
    chain_->SetBranchStatus("GenOthers_status", 1);
    chain_->SetBranchStatus("GenOthers_ID", 1);
    chain_->SetBranchStatus("GenOthers_isPrompt", 1);
    chain_->SetBranchStatus("GenOthers_isPromptFinalState", 1);
    chain_->SetBranchStatus("GenOthers_isTauDecayProduct", 1);
    chain_->SetBranchStatus("GenOthers_isPromptTauDecayProduct", 1);
    chain_->SetBranchStatus("GenOthers_isDirectPromptTauDecayProductFinalState", 1);
    chain_->SetBranchStatus("GenOthers_isHardProcess", 1);
    chain_->SetBranchStatus("GenOthers_isLastCopy", 1);
    chain_->SetBranchStatus("GenOthers_isLastCopyBeforeFSR", 1);
    chain_->SetBranchStatus("GenOthers_isPromptDecayed", 1);
    chain_->SetBranchStatus("GenOthers_isDecayedLeptonHadron", 1);
    chain_->SetBranchStatus("GenOthers_fromHardProcessBeforeFSR", 1);
    chain_->SetBranchStatus("GenOthers_fromHardProcessDecayed", 1);
    chain_->SetBranchStatus("GenOthers_fromHardProcessFinalState", 1);

    chain_->SetBranchAddress("nGenOthers", &nGenOthers);
    chain_->SetBranchAddress("GenOthers_Px", genOthers_px);
    chain_->SetBranchAddress("GenOthers_Py", genOthers_py);
    chain_->SetBranchAddress("GenOthers_Pz", genOthers_pz);
    chain_->SetBranchAddress("GenOthers_mother", &genOthers_mother);
    chain_->SetBranchAddress("GenOthers_pT", genOthers_pt);
    chain_->SetBranchAddress("GenOthers_eta", genOthers_eta);
    chain_->SetBranchAddress("GenOthers_phi", genOthers_phi);
    chain_->SetBranchAddress("GenOthers_charge", &genOthers_charge);
    chain_->SetBranchAddress("GenOthers_status", &genOthers_status);
    chain_->SetBranchAddress("GenOthers_ID", &genOthers_ID);
    chain_->SetBranchAddress("GenOthers_isPrompt", &genOthers_isPrompt);
    chain_->SetBranchAddress("GenOthers_isPromptFinalState", &genOthers_isPromptFinalState);
    chain_->SetBranchAddress("GenOthers_isTauDecayProduct", &genOthers_isTauDecayProduct);
    chain_->SetBranchAddress("GenOthers_isPromptTauDecayProduct", &genOthers_isPromptTauDecayProduct);
    chain_->SetBranchAddress("GenOthers_isDirectPromptTauDecayProductFinalState", &genOthers_isDirectPromptTauDecayProductFinalState);
    chain_->SetBranchAddress("GenOthers_isHardProcess", &genOthers_isHardProcess);
    chain_->SetBranchAddress("GenOthers_isLastCopy", &genOthers_isLastCopy);
    chain_->SetBranchAddress("GenOthers_isLastCopyBeforeFSR", &genOthers_isLastCopyBeforeFSR);
    chain_->SetBranchAddress("GenOthers_isPromptDecayed", &genOthers_isPromptDecayed);
    chain_->SetBranchAddress("GenOthers_isDecayedLeptonHadron", &genOthers_isDecayedLeptonHadron);
    chain_->SetBranchAddress("GenOthers_fromHardProcessBeforeFSR", &genOthers_fromHardProcessBeforeFSR);
    chain_->SetBranchAddress("GenOthers_fromHardProcessDecayed", &genOthers_fromHardProcessDecayed);
    chain_->SetBranchAddress("GenOthers_fromHardProcessFinalState", &genOthers_fromHardProcessFinalState);
  }

  void TurnOnBranches_Muon()
  {
    chain_->SetBranchStatus("isPFmuon", 1);
    chain_->SetBranchStatus("isGLBmuon", 1);
    chain_->SetBranchStatus("isTRKmuon", 1);
    chain_->SetBranchStatus("CosAngle", 1);
    chain_->SetBranchStatus("vtxTrkChi2", 1);
    chain_->SetBranchStatus("vtxTrkProb", 1);
    chain_->SetBranchStatus("vtxTrkNdof", 1);
    chain_->SetBranchStatus("vtxTrkCkt1Pt", 1);
    chain_->SetBranchStatus("vtxTrkCkt2Pt", 1);
    chain_->SetBranchStatus("vtxTrkDiEChi2", 1);
    chain_->SetBranchStatus("vtxTrkDiEProb", 1);
    chain_->SetBranchStatus("vtxTrkDiENdof", 1);
    chain_->SetBranchStatus("vtxTrkDiE1Pt", 1);
    chain_->SetBranchStatus("vtxTrkDiE2Pt", 1);
    chain_->SetBranchStatus("vtxTrkEMuChi2", 1);
    chain_->SetBranchStatus("vtxTrkEMuProb", 1);
    chain_->SetBranchStatus("vtxTrkEMuNdof", 1);
    chain_->SetBranchStatus("vtxTrkEMu1Pt", 1);
    chain_->SetBranchStatus("vtxTrkEMu2Pt", 1);

    chain_->SetBranchStatus("CosAngle_TuneP", 1);
    chain_->SetBranchStatus("vtxTrk1Pt_TuneP", 1);
    chain_->SetBranchStatus("vtxTrk2Pt_TuneP", 1);
    chain_->SetBranchStatus("vtxTrkChi2_TuneP", 1);
    chain_->SetBranchStatus("vtxTrkNdof_TuneP", 1);
    chain_->SetBranchStatus("vtxTrkProb_TuneP", 1);

    chain_->SetBranchStatus("nMuon", 1);
    chain_->SetBranchStatus("Muon_pT", 1);
    chain_->SetBranchStatus("Muon_eta", 1);
    chain_->SetBranchStatus("Muon_phi", 1);
    chain_->SetBranchStatus("Muon_muonType", 1);
    chain_->SetBranchStatus("Muon_chi2dof", 1);
    chain_->SetBranchStatus("Muon_muonHits", 1);
    chain_->SetBranchStatus("Muon_nSegments", 1);
    chain_->SetBranchStatus("Muon_nMatches", 1);
    chain_->SetBranchStatus("Muon_trackerLayers", 1);

    // chain_->SetBranchStatus("Muon_trackerHitsGLB", 1);
    chain_->SetBranchStatus("Muon_pixelHitsGLB", 1);
    chain_->SetBranchStatus("Muon_trackerLayersGLB", 1);

    chain_->SetBranchStatus("Muon_pixelHits", 1);
    chain_->SetBranchStatus("Muon_dxyVTX", 1);
    chain_->SetBranchStatus("Muon_dzVTX", 1);
    chain_->SetBranchStatus("Muon_trkiso", 1);
    
    chain_->SetBranchStatus("Muon_Px", 1);
    chain_->SetBranchStatus("Muon_Py", 1);
    chain_->SetBranchStatus("Muon_Pz", 1);

    chain_->SetBranchStatus("Muon_dB", 1);
    
    chain_->SetBranchStatus("Muon_charge", 1);
    
    chain_->SetBranchStatus("Muon_PfChargedHadronIsoR04", 1);
    chain_->SetBranchStatus("Muon_PfNeutralHadronIsoR04" ,1);
    chain_->SetBranchStatus("Muon_PfGammaIsoR04", 1);
    chain_->SetBranchStatus("Muon_PFSumPUIsoR04", 1);

    chain_->SetBranchStatus("Muon_Best_pT", 1);
    chain_->SetBranchStatus("Muon_Best_pTError", 1);
    chain_->SetBranchStatus("Muon_Best_Px", 1);
    chain_->SetBranchStatus("Muon_Best_Py", 1);
    chain_->SetBranchStatus("Muon_Best_Pz", 1);
    chain_->SetBranchStatus("Muon_Best_eta", 1);
    chain_->SetBranchStatus("Muon_Best_phi", 1);

    chain_->SetBranchStatus("Muon_Inner_pT", 1);
    chain_->SetBranchStatus("Muon_Inner_pTError", 1);
    chain_->SetBranchStatus("Muon_Inner_eta", 1);
    chain_->SetBranchStatus("Muon_Inner_phi", 1);
    chain_->SetBranchStatus("Muon_Inner_Px", 1);
    chain_->SetBranchStatus("Muon_Inner_Py", 1);
    chain_->SetBranchStatus("Muon_Inner_Pz", 1);

    chain_->SetBranchStatus("Muon_Outer_pT", 1);
    chain_->SetBranchStatus("Muon_Outer_pTError", 1);
    chain_->SetBranchStatus("Muon_Outer_Px", 1);
    chain_->SetBranchStatus("Muon_Outer_Py", 1);
    chain_->SetBranchStatus("Muon_Outer_Pz", 1);
    chain_->SetBranchStatus("Muon_Outer_eta", 1);
    chain_->SetBranchStatus("Muon_Outer_phi", 1);

    chain_->SetBranchStatus("Muon_GLB_pT", 1);
    chain_->SetBranchStatus("Muon_GLB_pTError", 1);
    chain_->SetBranchStatus("Muon_GLB_Px", 1);
    chain_->SetBranchStatus("Muon_GLB_Py", 1);
    chain_->SetBranchStatus("Muon_GLB_Pz", 1);
    chain_->SetBranchStatus("Muon_GLB_eta", 1);
    chain_->SetBranchStatus("Muon_GLB_phi", 1);

    chain_->SetBranchStatus("Muon_TuneP_pT", 1);
    chain_->SetBranchStatus("Muon_TuneP_pTError", 1);
    chain_->SetBranchStatus("Muon_TuneP_eta", 1);
    chain_->SetBranchStatus("Muon_TuneP_phi", 1);
    chain_->SetBranchStatus("Muon_TuneP_Px", 1);
    chain_->SetBranchStatus("Muon_TuneP_Py", 1);
    chain_->SetBranchStatus("Muon_TuneP_Pz", 1);

    chain_->SetBranchStatus("Muon_stationMask", 1);
    chain_->SetBranchStatus("Muon_nMatchesRPCLayers", 1);


    chain_->SetBranchAddress("nMuon", &nMuon);
    chain_->SetBranchAddress("Muon_pT", muon_pt);
    chain_->SetBranchAddress("Muon_eta", muon_eta);
    chain_->SetBranchAddress("Muon_phi", muon_phi);
    chain_->SetBranchAddress("Muon_muonType", muon_muonType);
    chain_->SetBranchAddress("Muon_chi2dof", muon_chi2DOF);
    chain_->SetBranchAddress("Muon_muonHits", muon_nMuonHit);
    chain_->SetBranchAddress("Muon_nSegments", muon_nSegment);
    chain_->SetBranchAddress("Muon_nMatches", muon_nMatchedStation);
    chain_->SetBranchAddress("Muon_trackerLayers", muon_nTrackerLayer);
    // chain_->SetBranchAddress("Muon_trackerHitsGLB", muon_trackerHitsGLB);
    chain_->SetBranchAddress("Muon_pixelHitsGLB", muon_nPixelHitGLB);
    chain_->SetBranchAddress("Muon_trackerLayersGLB", muon_nTrackerLayerGLB);
    chain_->SetBranchAddress("Muon_pixelHits", muon_nPixelHit);
    chain_->SetBranchAddress("Muon_dxyVTX", muon_dxyVTX);
    chain_->SetBranchAddress("Muon_dzVTX", muon_dzVTX);
    chain_->SetBranchAddress("Muon_trkiso", muon_trkIso);
    chain_->SetBranchAddress("isPFmuon", muon_isPF);
    chain_->SetBranchAddress("isGLBmuon", muon_isGLB);
    chain_->SetBranchAddress("isTRKmuon", muon_isTRK);      
    chain_->SetBranchAddress("Muon_Px", muon_px );
    chain_->SetBranchAddress("Muon_Py", muon_py );
    chain_->SetBranchAddress("Muon_Pz", muon_pz );
    chain_->SetBranchAddress("Muon_dB", muon_dB );
    
    chain_->SetBranchAddress("Muon_charge", muon_charge);
    
    chain_->SetBranchAddress("Muon_PfChargedHadronIsoR04", muon_PFIsoR04_CH);
    chain_->SetBranchAddress("Muon_PfNeutralHadronIsoR04", muon_PFIsoR04_NH);
    chain_->SetBranchAddress("Muon_PfGammaIsoR04", muon_PFIsoR04_photon);
    chain_->SetBranchAddress("Muon_PFSumPUIsoR04", muon_PFIsoR04_sumPt);

    chain_->SetBranchAddress("CosAngle",     &diMu_cosAngle_inner);
    chain_->SetBranchAddress("vtxTrkChi2",   &diMu_vtxChi2_inner);
    chain_->SetBranchAddress("vtxTrkProb",   &diMu_vtxProb_inner);
    chain_->SetBranchAddress("vtxTrkNdof",   &diMu_vtxNDOF_inner);    
    chain_->SetBranchAddress("vtxTrkCkt1Pt", &diMu_vtxPt1_inner);
    chain_->SetBranchAddress("vtxTrkCkt2Pt", &diMu_vtxPt2_inner);

    chain_->SetBranchAddress("vtxTrkDiEChi2", &diEl_vtxChi2);
    chain_->SetBranchAddress("vtxTrkDiEProb", &diEl_vtxProb);
    chain_->SetBranchAddress("vtxTrkDiENdof", &diEl_vtxNDOF);
    chain_->SetBranchAddress("vtxTrkDiE1Pt",  &diEl_vtxPt1);
    chain_->SetBranchAddress("vtxTrkDiE2Pt",  &diEl_vtxPt2);

    chain_->SetBranchAddress("vtxTrkEMuChi2", &emu_vtxChi2);
    chain_->SetBranchAddress("vtxTrkEMuProb", &emu_vtxProb);
    chain_->SetBranchAddress("vtxTrkEMuNdof", &emu_vtxNDOF);
    chain_->SetBranchAddress("vtxTrkEMu1Pt",  &emu_vtxPt1);
    chain_->SetBranchAddress("vtxTrkEMu2Pt",  &emu_vtxPt2);

    chain_->SetBranchAddress("CosAngle_TuneP",   &diMu_cosAngle_TuneP);
    chain_->SetBranchAddress("vtxTrk1Pt_TuneP",  &diMu_vtxPt1_TuneP);
    chain_->SetBranchAddress("vtxTrk2Pt_TuneP",  &diMu_vtxPt2_TuneP);
    chain_->SetBranchAddress("vtxTrkChi2_TuneP", &diMu_vtxChi2_TuneP);
    chain_->SetBranchAddress("vtxTrkNdof_TuneP", &diMu_vtxNDOF_TuneP);
    chain_->SetBranchAddress("vtxTrkProb_TuneP", &diMu_vtxProb_TuneP);
  
    chain_->SetBranchAddress("Muon_Best_pT",      &muon_best_pt);
    chain_->SetBranchAddress("Muon_Best_pTError", &muon_best_ptError);
    chain_->SetBranchAddress("Muon_Best_Px",      &muon_best_px);
    chain_->SetBranchAddress("Muon_Best_Py",      &muon_best_py);
    chain_->SetBranchAddress("Muon_Best_Pz",      &muon_best_pz);
    chain_->SetBranchAddress("Muon_Best_eta",     &muon_best_eta);
    chain_->SetBranchAddress("Muon_Best_phi",     &muon_best_phi);

    chain_->SetBranchAddress("Muon_Inner_pT",      &muon_inner_pt);
    chain_->SetBranchAddress("Muon_Inner_pTError", &muon_inner_ptError);
    chain_->SetBranchAddress("Muon_Inner_Px",      &muon_inner_px);
    chain_->SetBranchAddress("Muon_Inner_Py",      &muon_inner_py);
    chain_->SetBranchAddress("Muon_Inner_Pz",      &muon_inner_pz);
    chain_->SetBranchAddress("Muon_Inner_eta",     &muon_inner_eta);
    chain_->SetBranchAddress("Muon_Inner_phi",     &muon_inner_phi);

    chain_->SetBranchAddress("Muon_Outer_pT",      &muon_outer_pt);
    chain_->SetBranchAddress("Muon_Outer_pTError", &muon_outer_ptError);
    chain_->SetBranchAddress("Muon_Outer_Px",      &muon_outer_px);
    chain_->SetBranchAddress("Muon_Outer_Py",      &muon_outer_py);
    chain_->SetBranchAddress("Muon_Outer_Pz",      &muon_outer_pz);
    chain_->SetBranchAddress("Muon_Outer_eta",     &muon_outer_eta);
    chain_->SetBranchAddress("Muon_Outer_phi",     &muon_outer_phi);

    chain_->SetBranchAddress("Muon_GLB_pT",      &muon_global_pt);
    chain_->SetBranchAddress("Muon_GLB_pTError", &muon_global_ptError);
    chain_->SetBranchAddress("Muon_GLB_Px",      &muon_global_px);
    chain_->SetBranchAddress("Muon_GLB_Py",      &muon_global_py);
    chain_->SetBranchAddress("Muon_GLB_Pz",      &muon_global_pz);
    chain_->SetBranchAddress("Muon_GLB_eta",     &muon_global_eta);
    chain_->SetBranchAddress("Muon_GLB_phi",     &muon_global_phi);

    chain_->SetBranchAddress("Muon_TuneP_pT",      &muon_tuneP_pt);
    chain_->SetBranchAddress("Muon_TuneP_pTError", &muon_tuneP_ptError);
    chain_->SetBranchAddress("Muon_TuneP_Px",      &muon_tuneP_px);
    chain_->SetBranchAddress("Muon_TuneP_Py",      &muon_tuneP_py);
    chain_->SetBranchAddress("Muon_TuneP_Pz",      &muon_tuneP_pz);
    chain_->SetBranchAddress("Muon_TuneP_eta",     &muon_tuneP_eta);
    chain_->SetBranchAddress("Muon_TuneP_phi",     &muon_tuneP_phi);

    chain_->SetBranchAddress("Muon_stationMask", &muon_stationMask);
    chain_->SetBranchAddress("Muon_nMatchesRPCLayers", &muon_nMatchedRPCLayer);
  }
};
