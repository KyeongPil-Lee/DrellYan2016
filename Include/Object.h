#pragma once
#include <TLorentzVector.h>

//customized header files
#include <Include/NtupleHandle.h>

namespace DrellYan
{

const Double_t M_mu  = 0.1056583715;  // -- GeV -- //
const Double_t M_el  = 0.000510998;  // -- GeV -- //
const Double_t M_tau = 1.77682;      // -- GeV -- //

Bool_t IsTrigMatchedByDR(const TLorentzVector& vecP, NtupleHandle *ntuple, TString HLT)
{
  vector<std::string> *vec_trigName = ntuple->HLT_trigName;
  Int_t hlt_nTrig = ntuple->HLT_nTrig;

  Bool_t isMatched = kFALSE;
  for(Int_t k=0; k<hlt_nTrig; k++)
  {
    if( (vec_trigName->at((unsigned int)k)) == HLT )
    {
      Double_t mass = vecP.M(); // -- mass for trigger object: same with offline object (but this is not used anyway for dR calc.)
      TLorentzVector vecP_trigObj;
      Double_t Trig_Pt = ntuple->HLT_trigPt[k];
      Double_t Trig_eta = ntuple->HLT_trigEta[k];
      Double_t Trig_phi = ntuple->HLT_trigPhi[k];
      vecP_trigObj.SetPtEtaPhiM( Trig_Pt, Trig_eta, Trig_phi, mass );

      Double_t dR = vecP.DeltaR( vecP_trigObj );
      if( dR < 0.2 )
      {
        isMatched = kTRUE;
        break;
      }
    }
  }
  return isMatched;
}


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

    if     ( std::abs(ID) == 11 ) mass = DrellYan::M_el;
    else if( std::abs(ID) == 13 ) mass = DrellYan::M_mu;
    else if( std::abs(ID) == 15 ) mass = DrellYan::M_tau;
    else if( std::abs(ID) == 12 || std::abs(ID) == 14 || std::abs(ID) == 16 ) mass = 0; // -- neutrino
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

class Muon: public Object
{
public:
  Double_t mass;
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t energy;
  Int_t charge;
  
  // -- Cut variables -- //
  Int_t muonType;
  Double_t chi2DOF;
  Int_t nMuonHit;
  Int_t nSegment;
  Int_t nMatchedStation;
  Int_t nTrackerLayer;

  Int_t nPixelHitGLB;
  Int_t nTrackerLayerGLB;
  
  Int_t nPixelHit;
  Double_t dB;
  Double_t dxyVTX;
  Double_t dzVTX;
  Double_t trkIso;
  Int_t isGLB;
  Int_t isPF;
  Int_t isTRK;

  Double_t PFIsoR04_CH;
  Double_t PFIsoR04_NH;
  Double_t PFIsoR04_photon;
  Double_t PFIsoR04_sumPt;

  // -- Various Track Information -- //
  Double_t best_pt;
  Double_t best_ptError;
  Double_t best_px;
  Double_t best_py;
  Double_t best_pz;
  Double_t best_eta;
  Double_t best_phi;

  Double_t inner_pt;
  Double_t inner_ptError;
  Double_t inner_px;
  Double_t inner_py;
  Double_t inner_pz;
  Double_t inner_eta;
  Double_t inner_phi;
  Double_t energy_inner;
  TLorentzVector vecP_inner;

  Double_t outer_pt;
  Double_t outer_ptError;
  Double_t outer_px;
  Double_t outer_py;
  Double_t outer_pz;
  Double_t outer_eta;
  Double_t outer_phi;

  Double_t global_pt;
  Double_t global_ptError;
  Double_t global_px;
  Double_t global_py;
  Double_t global_pz;
  Double_t global_eta;
  Double_t global_phi;

  Double_t tuneP_pt;
  Double_t tuneP_ptError;
  Double_t tuneP_px;
  Double_t tuneP_py;
  Double_t tuneP_pz;
  Double_t tuneP_eta;
  Double_t tuneP_phi;

  Int_t stationMask;
  Int_t nMatchedRPCLayer;

  // -- derived variables
  Double_t relTrkIso;
  Double_t relPFIso;
  Double_t relPFIso_dBeta;

  Bool_t isTight; // -- should be replaced by ID flag in CMSSW
  Bool_t isHighPt; // -- should be replaced by ID flag in CMSSW

  Bool_t isHighPt_noDZ; // -- without dzVTX condition

  Muon()
  {
    Init();
  }

  Muon( NtupleHandle *ntuple, Int_t index ): Muon()
  {
    FillFromNtuple( ntuple, index );
  }

  void FillFromNtuple(NtupleHandle *ntuple, Int_t index)
  {
    // -- kinematics
    pt     = ntuple->muon_pt[index];
    eta    = ntuple->muon_eta[index];
    phi    = ntuple->muon_phi[index];
    px     = ntuple->muon_px[index];
    py     = ntuple->muon_py[index];
    pz     = ntuple->muon_pz[index];
    charge = ntuple->muon_charge[index];

    mass = DrellYan::M_mu;
    energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    // -- muon type
    muonType = ntuple->muon_muonType[index];
    isGLB    = ntuple->muon_isGLB[index];
    isPF     = ntuple->muon_isPF[index];
    isTRK    = ntuple->muon_isTRK[index];
    
    // -- muon system
    chi2DOF          = ntuple->muon_chi2DOF[index];
    nMuonHit         = ntuple->muon_nMuonHit[index];
    nSegment         = ntuple->muon_nSegment[index];
    nMatchedStation  = ntuple->muon_nMatchedStation[index];
    stationMask      = ntuple->muon_stationMask[index];
    nMatchedRPCLayer = ntuple->muon_nMatchedRPCLayer[index];

    // -- inner tracker
    nPixelHit        = ntuple->muon_nPixelHit[index];
    nPixelHitGLB     = ntuple->muon_nPixelHitGLB[index];
    nTrackerLayer    = ntuple->muon_nTrackerLayer[index];
    nTrackerLayerGLB = ntuple->muon_nTrackerLayerGLB[index];

    // -- impact parameters
    dxyVTX = ntuple->muon_dxyVTX[index];
    dzVTX  = ntuple->muon_dzVTX[index];
    dB     = ntuple->muon_dB[index];

    // -- isolation
    trkIso          = ntuple->muon_trkIso[index];
    PFIsoR04_CH     = ntuple->muon_PFIsoR04_CH[index];
    PFIsoR04_NH     = ntuple->muon_PFIsoR04_NH[index];
    PFIsoR04_photon = ntuple->muon_PFIsoR04_photon[index];
    PFIsoR04_sumPt  = ntuple->muon_PFIsoR04_sumPt[index];

    // -- Various Track Information -- //
    best_pt      = ntuple->muon_best_pt[index];
    best_ptError = ntuple->muon_best_ptError[index];
    best_eta     = ntuple->muon_best_eta[index];
    best_phi     = ntuple->muon_best_phi[index];
    best_px      = ntuple->muon_best_px[index];
    best_py      = ntuple->muon_best_py[index];
    best_pz      = ntuple->muon_best_pz[index];

    inner_pt      = ntuple->muon_inner_pt[index];
    inner_ptError = ntuple->muon_inner_ptError[index];
    inner_eta     = ntuple->muon_inner_eta[index];
    inner_phi     = ntuple->muon_inner_phi[index];
    inner_px      = ntuple->muon_inner_px[index];
    inner_py      = ntuple->muon_inner_py[index];
    inner_pz      = ntuple->muon_inner_pz[index];
    energy_inner = sqrt(inner_px*inner_px + inner_py*inner_py + inner_pz*inner_pz + mass*mass);
    vecP_inner.SetPxPyPzE(px, py, pz, energy_inner);

    outer_pt      = ntuple->muon_outer_pt[index];
    outer_ptError = ntuple->muon_outer_ptError[index];
    outer_eta     = ntuple->muon_outer_eta[index];
    outer_phi     = ntuple->muon_outer_phi[index];
    outer_px      = ntuple->muon_outer_px[index];
    outer_py      = ntuple->muon_outer_py[index];
    outer_pz      = ntuple->muon_outer_pz[index];

    global_pt      = ntuple->muon_global_pt[index];
    global_ptError = ntuple->muon_global_ptError[index];
    global_eta     = ntuple->muon_global_eta[index];
    global_phi     = ntuple->muon_global_phi[index];
    global_px      = ntuple->muon_global_px[index];
    global_py      = ntuple->muon_global_py[index];
    global_pz      = ntuple->muon_global_pz[index];

    tuneP_pt      = ntuple->muon_tuneP_pt[index];
    tuneP_ptError = ntuple->muon_tuneP_ptError[index];
    tuneP_eta     = ntuple->muon_tuneP_eta[index];
    tuneP_phi     = ntuple->muon_tuneP_phi[index];
    tuneP_px      = ntuple->muon_tuneP_px[index];
    tuneP_py      = ntuple->muon_tuneP_py[index];
    tuneP_pz      = ntuple->muon_tuneP_pz[index];

    // -- derived variables
    relTrkIso = trkIso / pt;
    relPFIso = (PFIsoR04_CH + PFIsoR04_NH + PFIsoR04_photon) / pt;
    relPFIso_dBeta = ( PFIsoR04_CH + max(0.0, PFIsoR04_NH + PFIsoR04_photon - 0.5*PFIsoR04_sumPt) ) / pt;

    if( isGLB && isPF &&
        chi2DOF < 10 &&
        nMuonHit > 0 &&
        nMatchedStation > 1 &&
        fabs(dxyVTX) < 0.2 && // -- dB is only available in miniAOD: to be safe ...
        fabs(dzVTX) < 0.5 &&
        nPixelHit > 0 &&
        nTrackerLayer > 5 ) isTight = kTRUE;

    if( isGLB &&
        nMuonHit > 0 &&
        nMatchedStation > 1 &&
        (best_ptError / best_pt) < 0.3 &&
        fabs(dxyVTX) < 0.2 && 
        nPixelHit > 0 &&
        nTrackerLayer > 5 )
      {
        isHighPt_noDZ = kTRUE;
        if( fabs(dzVTX) < 0.5 ) isHighPt = kTRUE;
      }
  }

  Bool_t IsTrigMatched(NtupleHandle* ntuple, TString HLT)
  {
    return DrellYan::IsTrigMatchedByDR(vecP, ntuple, HLT);
  }

private:
  void Init()
  {
    px = -999;
    py = -999;
    pz = -999;
    charge = -999; 
    
    // -- Cut variables -- //
    muonType = -999;
    chi2DOF = -999;
    nMuonHit = -999;
    nSegment = -999;
    nMatchedStation = -999;
    nTrackerLayer = -999;

    nPixelHitGLB = -999;
    nTrackerLayerGLB = -999;
    
    nPixelHit = -999;
    dB = -999;
    dxyVTX = -999;
    dzVTX = -999;
    trkIso = -999;
    isGLB = kFALSE;
    isPF = kFALSE;
    isTRK = kFALSE;

    PFIsoR04_CH = -999;
    PFIsoR04_NH = -999;
    PFIsoR04_photon = -999;
    PFIsoR04_sumPt = -999;

    // -- Various Track Information -- //
    best_pt = -999;
    best_ptError = -999;
    best_px = -999;
    best_py = -999;
    best_pz = -999;
    best_eta = -999;
    best_phi = -999;

    inner_pt = -999;
    inner_ptError = -999;
    inner_px = -999;
    inner_py = -999;
    inner_pz = -999;
    inner_eta = -999;
    inner_phi = -999;

    outer_pt = -999;
    outer_ptError = -999;
    outer_px = -999;
    outer_py = -999;
    outer_pz = -999;
    outer_eta = -999;
    outer_phi = -999;

    global_pt = -999;
    global_ptError = -999;
    global_px = -999;
    global_py = -999;
    global_pz = -999;
    global_eta = -999;
    global_phi = -999;

    tuneP_pt = -999;
    tuneP_ptError = -999;
    tuneP_px = -999;
    tuneP_py = -999;
    tuneP_pz = -999;
    tuneP_eta = -999;
    tuneP_phi = -999;

    stationMask = -999;
    nMatchedRPCLayer = -999;

    // -- derived variables
    relTrkIso = -999;
    relPFIso = -999;
    relPFIso_dBeta = -999;

    isTight = kFALSE;
    isHighPt = kFALSE;
    isHighPt_noDZ = kFALSE;
  }
};

class MuonPair: public Object
{
public:
  Muon first_;
  Muon second_;

  Double_t mass;
  Double_t rap;
  Double_t absRap;

  Double_t angle3D;
  Double_t angle3D_inner;
  Bool_t isOS;

  Double_t vtxProb_inner;
  Double_t normVtxChi2_inner;

  Bool_t isDummy; // -- it becomes false when Assign() function is called 

  MuonPair()
  {
    Init();
  }

  MuonPair(Muon muon1, Muon muon2, NtupleHandle *ntuple): MuonPair()
  {
    if( muon1.pt > muon2.pt )
    {
      first_  = muon1;
      second_ = muon2;
    }
    else
    {
      first_  = muon2;
      second_ = muon1;
    }

    Assign(ntuple);
  }

  void Print()
  {
    printf("  [Leading muon]     (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", first_.pt, first_.eta, first_.phi, first_.charge);
    printf("  [Sub-leading muon] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", second_.pt, second_.eta, second_.phi, second_.charge);
    printf("    Dimuon (Mass, pT, rapidity, vtxProb(inner), normVtxChi2(inner) ) = (%10.5lf, %10.5lf, %10.5lf, %10.5lf, %10.5lf)\n", mass, pt, rap, vtxProb_inner, normVtxChi2_inner);
  }

  Bool_t IsWithinAcc( Double_t ptCut_lead, Double_t ptCut_sub, Double_t etaCut_lead, Double_t etaCut_sub )
  {
    Bool_t flag = kFALSE;

    // -- first: leading, second: sub-leading -- //
    if( first_.pt  > ptCut_lead && fabs(first_.eta) < etaCut_lead && 
        second_.pt > ptCut_sub  && fabs(second_.eta) < etaCut_sub )
        flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate(NtupleHandle* ntuple)
  {
    Bool_t flag = kFALSE;

    Double_t ptCut_lead = 28.0;
    Double_t ptCut_sub = 17.0;
    Double_t etaCut_lead = 2.4;
    Double_t etaCut_sub = 2.4;

    Bool_t isWithinAcc = IsWithinAcc(ptCut_lead, ptCut_sub, etaCut_lead, etaCut_sub);

    Bool_t isGoodMuon = kFALSE;
    if( (first_.isHighPt_noDZ  && first_.relTrkIso < 0.10) &&
        (second_.isHighPt_noDZ && second_.relTrkIso < 0.10) )
      isGoodMuon = kTRUE;

    Bool_t isTrigMatched = kFALSE;
    if( first_.IsTrigMatched(ntuple, "HLT_IsoMu24_v*")  || first_.IsTrigMatched(ntuple, "HLT_IsoTkMu24_v*") ||
        second_.IsTrigMatched(ntuple, "HLT_IsoMu24_v*") || second_.IsTrigMatched(ntuple, "HLT_IsoTkMu24_v*") )
      isTrigMatched = kTRUE;

    if( isOS && isWithinAcc && isGoodMuon && isTrigMatched && 
        mass > 15.0 &&
        normVtxChi2_inner < 20.0 &&
        angle3D_inner < TMath::Pi()-0.005 ) flag = kTRUE;

    return flag;
  }


private:
  void Init()
  {
    mass = -999;
    rap = -999;
    absRap = -999;

    angle3D = -999;
    angle3D_inner = -999;
    isOS = 0;

    vtxProb_inner = -999;
    normVtxChi2_inner = 999;

    isDummy = kTRUE;
  }

  void Assign(NtupleHandle *ntuple)
  {
    isDummy = kFALSE;

    vecP = first_.vecP + second_.vecP;

    pt  = vecP.Pt();
    eta = vecP.Eta();
    phi = vecP.Phi();

    mass   = vecP.M();
    rap    = vecP.Rapidity();
    absRap = fabs(rap);

    angle3D       = first_.vecP.Angle( second_.vecP.Vect() );
    angle3D_inner = first_.vecP_inner.Angle( second_.vecP_inner.Vect() );

    isOS = first_.charge != second_.charge ? kTRUE : kFALSE;

    // -- vertex values (from inner track)
    vector<double> *vec_pt1     = ntuple->diMu_vtxPt1_inner;
    vector<double> *vec_pt2     = ntuple->diMu_vtxPt2_inner;
    vector<double> *vec_vtxProb = ntuple->diMu_vtxProb_inner;

    Int_t nPt1 = (Int_t)vec_pt1->size();
    Int_t nPt2 = (Int_t)vec_pt2->size();
    Int_t nVtxProb = (Int_t)vec_vtxProb->size();

    if( nPt1 != nPt2 || nPt2 != nVtxProb || nPt1 != nVtxProb ) 
      cout << "nPt1: " << nPt1 << " nPt2: " << nPt2 << " nVtxProb: " << nVtxProb << endl;

    Double_t pt1 = first_.inner_pt;
    Double_t pt2 = second_.inner_pt;
    for(Int_t i=0; i<nVtxProb; i++)
    {
      // cout << "\tvec_pt1->at("<< i << "): " << vec_pt1->at(i) << " vec_pt2->at("<< i << "): " << vec_pt2->at(i) << endl;
      if( ( vec_pt1->at(i) == pt1 && vec_pt2->at(i) == pt2 ) || ( vec_pt1->at(i) == pt2 && vec_pt2->at(i) == pt1 ) )
      {
        vtxProb_inner     = vec_vtxProb->at(i);
        normVtxChi2_inner = ntuple->diMu_vtxChi2_inner->at(i) / ntuple->diMu_vtxNDOF_inner->at(i);
        break;
      }
    }
  }

};

}; // -- end of DrellYan namespace