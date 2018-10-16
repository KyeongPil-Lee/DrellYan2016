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

  //Generator level information
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
};
