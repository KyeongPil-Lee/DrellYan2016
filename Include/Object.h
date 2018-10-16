#pragma once
#include <TLorentzVector.h>

//customized header files
#include <Include/NtupleHandle.h>

#define M_mu 0.1056583715  // -- GeV -- //
#define M_elec 0.000510998 // -- GeV -- //
#define M_tau 1.77682      // -- GeV -- //

class Object
{
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  TLorentzVector vecP;

  Object() {}
};


class GenLepton : public Object
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t energy;
  Double_t mother;
  Int_t ID;
  Double_t charge;
  Double_t mass;
  Int_t status;

  Int_t isPrompt;
  Int_t isPromptFinalState;
  Int_t isTauDecayProduct;
  Int_t isPromptTauDecayProduct;
  Int_t isDirectPromptTauDecayProductFinalState;
  Int_t isHardProcess;
  Int_t isLastCopy;
  Int_t isLastCopyBeforeFSR;
  Int_t isPromptDecayed;
  Int_t isDecayedLeptonHadron;
  Int_t fromHardProcessBeforeFSR;
  Int_t fromHardProcessDecayed;
  Int_t fromHardProcessFinalState;

  GenLepton()
  {
    Init();
  }

  GenLepton( NtupleHandle *ntuple, Int_t index ): GenLepton()
  {
    FillFromNtuple( ntuple, index );
  }

  void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
  {
    pt      = ntuple->genLepton_pt[index];
    eta     = ntuple->genLepton_eta[index];
    phi     = ntuple->genLepton_phi[index];
    px      = ntuple->genLepton_px[index];
    py      = ntuple->genLepton_py[index];
    pz      = ntuple->genLepton_pz[index];
    mother  = ntuple->genLepton_mother[index];
    ID      = ntuple->genLepton_ID[index];
    charge  = ntuple->genLepton_charge[index];
    status  = ntuple->genLepton_status[index];

    if     ( ID == -11 || ID == 11 ) mass = M_elec;
    else if( ID == -13 || ID == 13 ) mass = M_mu;
    else if( ID == -15 || ID == 15 ) mass = M_tau;
    else if( abs(ID) == 12 || abs(ID) == 14 || abs(ID) == 16 ) mass = 0; // -- neutrino
    else
    {
      printf("ID = %d is not recognized... mass = 0 for 4-momentum\n", ID);
      mass = 0;
    }

    energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    isPrompt                                = ntuple->genLepton_isPrompt[index];
    isPromptFinalState                      = ntuple->genLepton_isPromptFinalState[index];
    isTauDecayProduct                       = ntuple->genLepton_isTauDecayProduct[index];
    isPromptTauDecayProduct                 = ntuple->genLepton_isPromptTauDecayProduct[index];
    isDirectPromptTauDecayProductFinalState = ntuple->genLepton_isDirectPromptTauDecayProductFinalState[index];
    isHardProcess                           = ntuple->genLepton_isHardProcess[index];
    isLastCopy                              = ntuple->genLepton_isLastCopy[index];
    isLastCopyBeforeFSR                     = ntuple->genLepton_isLastCopyBeforeFSR[index];
    isPromptDecayed                         = ntuple->genLepton_isPromptDecayed[index];
    isDecayedLeptonHadron                   = ntuple->genLepton_isDecayedLeptonHadron[index];
    fromHardProcessBeforeFSR                = ntuple->genLepton_fromHardProcessBeforeFSR[index];
    fromHardProcessDecayed                  = ntuple->genLepton_fromHardProcessDecayed[index];
    fromHardProcessFinalState               = ntuple->genLepton_fromHardProcessFinalState[index];
  }

  void Init()
  {
    px = -999;
    py = -999;
    pz = -999;
    energy = -999;
    mother = -999;
    ID = -999;
    charge = -999;
    mass = -999;
    status = -999;

    isPrompt = -999;
    isPromptFinalState = -999;
    isTauDecayProduct = -999;
    isPromptTauDecayProduct = -999;
    isDirectPromptTauDecayProductFinalState = -999;
    isHardProcess = -999;
    isLastCopy = -999;
    isLastCopyBeforeFSR = -999;
    isPromptDecayed = -999;
    isDecayedLeptonHadron = -999;
    fromHardProcessBeforeFSR = -999;
    fromHardProcessDecayed = -999;
    fromHardProcessFinalState = -999;
  }
};

class GenOthers : public Object
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t energy;
  Double_t mother;
  Int_t ID;
  Double_t charge;
  Double_t mass;
  Int_t status;

  //GenFlags(after 7_4_X)
  Int_t isPrompt;
  Int_t isPromptFinalState;
  Int_t isTauDecayProduct;
  Int_t isPromptTauDecayProduct;
  Int_t isDirectPromptTauDecayProductFinalState;
  Int_t isHardProcess;
  Int_t isLastCopy;
  Int_t isLastCopyBeforeFSR;
  Int_t isPromptDecayed;
  Int_t isDecayedLeptonHadron;
  Int_t fromHardProcessBeforeFSR;
  Int_t fromHardProcessDecayed;
  Int_t fromHardProcessFinalState;

  GenOthers()
  {
    Init();
  }

  GenOthers( NtupleHandle *ntuple, Int_t index ): GenOthers()
  {
    FillFromNtuple( ntuple, index );
  }

  void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
  {
    pt      = ntuple->genOthers_pt[index];
    eta     = ntuple->genOthers_eta[index];
    phi     = ntuple->genOthers_phi[index];
    px      = ntuple->genOthers_px[index];
    py      = ntuple->genOthers_py[index];
    pz      = ntuple->genOthers_pz[index];
    mother  = ntuple->genOthers_mother[index];
    ID      = ntuple->genOthers_ID[index];
    charge  = ntuple->genOthers_charge[index];
    status  = ntuple->genOthers_status[index];
    
    mass = 0;
    if( abs(ID) == 22 ) // -- Photon -- //
        mass = 0;

    energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    isPrompt                                = ntuple->genOthers_isPrompt[index];
    isPromptFinalState                      = ntuple->genOthers_isPromptFinalState[index];
    isTauDecayProduct                       = ntuple->genOthers_isTauDecayProduct[index];
    isPromptTauDecayProduct                 = ntuple->genOthers_isPromptTauDecayProduct[index];
    isDirectPromptTauDecayProductFinalState = ntuple->genOthers_isDirectPromptTauDecayProductFinalState[index];
    isHardProcess                           = ntuple->genOthers_isHardProcess[index];
    isLastCopy                              = ntuple->genOthers_isLastCopy[index];
    isLastCopyBeforeFSR                     = ntuple->genOthers_isLastCopyBeforeFSR[index];
    isPromptDecayed                         = ntuple->genOthers_isPromptDecayed[index];
    isDecayedLeptonHadron                   = ntuple->genOthers_isDecayedLeptonHadron[index];
    fromHardProcessBeforeFSR                = ntuple->genOthers_fromHardProcessBeforeFSR[index];
    fromHardProcessDecayed                  = ntuple->genOthers_fromHardProcessDecayed[index];
    fromHardProcessFinalState               = ntuple->genOthers_fromHardProcessFinalState[index];
  }

  void Init()
  {
    px = -999;
    py = -999;
    pz = -999;
    energy = -999;
    mother = -999;
    ID = -999;
    charge = -999;
    mass = -999;
    status = -999;

    isPrompt = -999;
    isPromptFinalState = -999;
    isTauDecayProduct = -999;
    isPromptTauDecayProduct = -999;
    isDirectPromptTauDecayProductFinalState = -999;
    isHardProcess = -999;
    isLastCopy = -999;
    isLastCopyBeforeFSR = -999;
    isPromptDecayed = -999;
    isDecayedLeptonHadron = -999;
    fromHardProcessBeforeFSR = -999;
    fromHardProcessDecayed = -999;
    fromHardProcessFinalState = -999;
  }
};


class GenPair: public Object
{
public:
  GenLepton first_;
  GenLepton second_;

  Double_t mass;
  Double_t rap;
  Double_t absRap;

  GenPair() {}

  GenPair(GenLepton genLepton1, GenLepton genLepton2)
  {
    if( genLepton1.pt > genLepton2.pt )
    {
      first_ = genLepton1;
      second_ = genLepton2;
    }
    else
    {
      first_ = genLepton2;
      second_ = genLepton1;
    }

    Assign();
  }

private:
  void Assign()
  {
    vecP = first_.vecP + second_.vecP;

    pt = vecP.Pt();
    eta = vecP.Eta();
    phi = vecP.Phi();

    mass = vecP.M();
    rap = vecP.Rapidity();
    absRap = fabs(rap);
  }
};