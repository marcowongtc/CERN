//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May  5 08:58:46 2021 by ROOT version 6.12/06
// from TTree muonNT/muonNT
// found on file: ../inputs/output_rel22_1.3Mevts_AOD_addHitsVars.root
//////////////////////////////////////////////////////////

#ifndef muonNTlooper_h
#define muonNTlooper_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class muonNTlooper {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         pT;
   Float_t         eta;
   Float_t         phi;
   Int_t           quality;
   Bool_t          passPresel;
   Bool_t          passIDhits;
   Bool_t          passHighPt;
   Bool_t          passLowPt;
   Bool_t          passLowPtMVA;
   Bool_t          isBadMuon_highPt;
   Bool_t          isBadMuon_other;
   Int_t           truthType;
   Int_t           truthTypeMCTC;
   Int_t           truthOriginMCTC;
   Int_t           muonType;
   Int_t           author;
   vector<Int_t>*           isAuthor;
   Int_t           caloMuonIDTag;
   Float_t         caloMuonScore;
   Int_t           PixHits;
   Int_t           PixDead;
   Int_t           SCTHits;
   Int_t           SCTDead;
   Int_t           PixHoles;
   Int_t           SCTHoles;
   Int_t           TRTHits;
   Int_t           TRTOut;
   Int_t           innerSmallHits;
   Int_t           middleSmallHits;
   Int_t           outerSmallHits;
   Int_t           extendedSmallHits;
   Int_t           innerLargeHits;
   Int_t           middleLargeHits;
   Int_t           outerLargeHits;
   Int_t           extendedLargeHits;
   Int_t           phiLayer1RPCHits;
   Int_t           phiLayer2RPCHits;
   Int_t           phiLayer3RPCHits;
   Int_t           phiLayer4RPCHits;
   Int_t           etaLayer1RPCHits;
   Int_t           etaLayer2RPCHits;
   Int_t           etaLayer3RPCHits;
   Int_t           etaLayer4RPCHits;
   Int_t           phiLayer1Hits;
   Int_t           phiLayer2Hits;
   Int_t           phiLayer3Hits;
   Int_t           phiLayer4Hits;
   Int_t           etaLayer1Hits;
   Int_t           etaLayer2Hits;
   Int_t           etaLayer3Hits;
   Int_t           etaLayer4Hits;
   Float_t         d0;
   Float_t         d0sig;
   Float_t         z0;
   Float_t         rho;
   Float_t         qOverPsignif;
   Float_t         reducedChi2;
   Int_t           nprecisionLayers;
   Int_t           nprecisionHoleLayers;
   Float_t         momentumBalanceSig;
   Float_t         scatteringCurvatureSig;
   Float_t         scatteringNeighbourSig;
   Float_t         energyLoss;
   Int_t         energyLossType;
   Float_t         muonSegmentDeltaEta;
   Float_t         middleHoles;
   Float_t         muonSeg1ChamberIdx;
   Float_t         muonSeg2ChamberIdx;
   Float_t         muonSeg3ChamberIdx;
   Float_t         muonSeg4ChamberIdx;
   Int_t           nSegments;
   Float_t         primary_chi2;
   Float_t         primary_Ndof;
   Float_t         ID_chi2;
   Float_t         ID_Ndof;
   Float_t         ME_chi2;
   Float_t         ME_Ndof;
   Float_t         MS_chi2;
   Float_t         MS_Ndof;
   Float_t         MSonly_chi2;
   Float_t         MSonly_Ndof;
   Float_t         segment1_chi2;
   Float_t         segment1_Ndof;
   Int_t         segment1_nprecisionHits;
   Float_t         segment2_chi2;
   Float_t         segment2_Ndof;
   Int_t         segment2_nprecisionHits;
   Float_t         segment3_chi2;
   Float_t         segment3_Ndof;
   Int_t         segment3_nprecisionHits;


   // List of branches
   TBranch        *b_pT;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_quality;   //!
   TBranch        *b_passPresel;   //!
   TBranch        *b_passIDhits;   //!
   TBranch        *b_passHighPt;   //!
   TBranch        *b_passLowPt;   //!
   TBranch        *b_passLowPtMVA;   //!
   TBranch        *b_isBadMuon_highPt;   //!
   TBranch        *b_isBadMuon_other;   //!
   TBranch        *b_truthType;   //!
   TBranch        *b_truthTypeMCTC;   //!
   TBranch        *b_truthOriginMCTC;   //!
   TBranch        *b_muonType;   //!
   TBranch        *b_author;   //!
   TBranch        *b_isAuthor;   //!
   TBranch        *b_caloMuonIDTag;   //!
   TBranch        *b_caloMuonScore;   //!
   TBranch        *b_PixHits;   //!
   TBranch        *b_PixDead;   //!
   TBranch        *b_SCTHits;   //!
   TBranch        *b_SCTDead;   //!
   TBranch        *b_PixHoles;   //!
   TBranch        *b_SCTHoles;   //!
   TBranch        *b_TRTHits;   //!
   TBranch        *b_TRTOut;   //!
   TBranch        *b_innerSmallHits;   //!
   TBranch        *b_middleSmallHits;   //!
   TBranch        *b_outerSmallHits;   //!
   TBranch        *b_extendedSmallHits;   //!
   TBranch        *b_innerLargeHits;   //!
   TBranch        *b_middleLargeHits;   //!
   TBranch        *b_outerLargeHits;   //!
   TBranch        *b_extendedLargeHits;   //!
   TBranch        *b_phiLayer1RPCHits;   //!
   TBranch        *b_phiLayer2RPCHits;   //!
   TBranch        *b_phiLayer3RPCHits;   //!
   TBranch        *b_phiLayer4RPCHits;   //!
   TBranch        *b_etaLayer1RPCHits;   //!
   TBranch        *b_etaLayer2RPCHits;   //!
   TBranch        *b_etaLayer3RPCHits;   //!
   TBranch        *b_etaLayer4RPCHits;   //!
   TBranch        *b_phiLayer1Hits;   //!
   TBranch        *b_phiLayer2Hits;   //!
   TBranch        *b_phiLayer3Hits;   //!
   TBranch        *b_phiLayer4Hits;   //!
   TBranch        *b_etaLayer1Hits;   //!
   TBranch        *b_etaLayer2Hits;   //!
   TBranch        *b_etaLayer3Hits;   //!
   TBranch        *b_etaLayer4Hits;   //!
   TBranch        *b_d0;   //!
   TBranch        *b_d0sig;   //!
   TBranch        *b_z0;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_qOverPsignif;   //!
   TBranch        *b_reducedChi2;   //!
   TBranch        *b_nprecisionLayers;   //!
   TBranch        *b_nprecisionHoleLayers;   //!
   TBranch        *b_momentumBalanceSig;   //!
   TBranch        *b_scatteringCurvatureSig;   //!
   TBranch        *b_scatteringNeighbourSig;   //!
   TBranch        *b_energyLoss;   //!
   TBranch        *b_energyLossType;   //!
   TBranch        *b_muonSegmentDeltaEta;   //!
   TBranch        *b_middleHoles;   //!
   TBranch        *b_muonSeg1ChamberIdx;   //!
   TBranch        *b_muonSeg2ChamberIdx;   //!
   TBranch        *b_muonSeg3ChamberIdx;   //!
   TBranch        *b_muonSeg4ChamberIdx;   //!
   TBranch        *b_nSegments;   //!
   TBranch        *b_primary_chi2;   //!                                                                                                                           
   TBranch        *b_primary_Ndof;   //!                                                                                                                                               
   TBranch        *b_ID_chi2;   //!                                                                                                                                             
   TBranch        *b_ID_Ndof;   //!                                                                                                                                         
   TBranch        *b_ME_chi2;   //!                                                                                                                                    
   TBranch        *b_ME_Ndof;   //!                                                                                                                                          
   TBranch        *b_MS_chi2;   //!                                                                                                                                    
   TBranch        *b_MS_Ndof;   //!                                                                                                                                          
   TBranch        *b_MSonly_chi2;   //!                                                                                                                                  
   TBranch        *b_MSonly_Ndof;   //!       
   TBranch        *b_segment1_chi2;   //!                                                                                                                                            
   TBranch        *b_segment1_Ndof;   //!                                                                                                                                              
   TBranch        *b_segment1_nprecisionHits;   //!                                                                                                                                 
   TBranch        *b_segment2_chi2;   //!                                                                                                                                                 
   TBranch        *b_segment2_Ndof;   //!                                                                                                                                                   
   TBranch        *b_segment2_nprecisionHits;   //!                                                                                                                             
   TBranch        *b_segment3_chi2;   //!                                                                                                                                     
   TBranch        *b_segment3_Ndof;   //!                                                                                                                               
   TBranch        *b_segment3_nprecisionHits;   //!              

   muonNTlooper(TTree *tree=0);
   /* --- virtual: declare a member function can be overriden with the same name in other class
      --- it is used as polumorphism, exist two / more form in diff class 
      --- eg: draw() for circle and square is different but with sam purpose
   */ 
   virtual ~muonNTlooper();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(string outname, int CaloScore_WP);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef muonNTlooper_cxx
muonNTlooper::muonNTlooper(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../inputs/output_rel22_1.3Mevts_AOD_addHitsVars.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../inputs/output_rel22_1.3Mevts_AOD_addHitsVars.root");
      }
      f->GetObject("muonNT",tree);

   }
   Init(tree);
}

muonNTlooper::~muonNTlooper()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t muonNTlooper::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t muonNTlooper::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void muonNTlooper::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   isAuthor = 0;

   fChain->SetBranchAddress("pT", &pT, &b_pT);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("quality", &quality, &b_quality);
   fChain->SetBranchAddress("passPresel", &passPresel, &b_passPresel);
   fChain->SetBranchAddress("passIDhits", &passIDhits, &b_passIDhits);
   fChain->SetBranchAddress("passHighPt", &passHighPt, &b_passHighPt);
   fChain->SetBranchAddress("passLowPt", &passLowPt, &b_passLowPt);
   fChain->SetBranchAddress("passLowPtMVA", &passLowPtMVA, &b_passLowPtMVA);
   fChain->SetBranchAddress("isBadMuon_highPt", &isBadMuon_highPt, &b_isBadMuon_highPt);
   fChain->SetBranchAddress("isBadMuon_other", &isBadMuon_other, &b_isBadMuon_other);
   fChain->SetBranchAddress("truthType", &truthType, &b_truthType);
   fChain->SetBranchAddress("truthTypeMCTC", &truthTypeMCTC, &b_truthTypeMCTC);
   fChain->SetBranchAddress("truthOriginMCTC", &truthOriginMCTC, &b_truthOriginMCTC);
   fChain->SetBranchAddress("muonType", &muonType, &b_muonType);
   fChain->SetBranchAddress("author", &author, &b_author);
   fChain->SetBranchAddress("isAuthor", &isAuthor, &b_isAuthor);
   fChain->SetBranchAddress("caloMuonIDTag", &caloMuonIDTag, &b_caloMuonIDTag);
   fChain->SetBranchAddress("caloMuonScore", &caloMuonScore, &b_caloMuonScore);
   fChain->SetBranchAddress("PixHits", &PixHits, &b_PixHits);
   fChain->SetBranchAddress("PixDead", &PixDead, &b_PixDead);
   fChain->SetBranchAddress("SCTHits", &SCTHits, &b_SCTHits);
   fChain->SetBranchAddress("SCTDead", &SCTDead, &b_SCTDead);
   fChain->SetBranchAddress("PixHoles", &PixHoles, &b_PixHoles);
   fChain->SetBranchAddress("SCTHoles", &SCTHoles, &b_SCTHoles);
   fChain->SetBranchAddress("TRTHits", &TRTHits, &b_TRTHits);
   fChain->SetBranchAddress("TRTOut", &TRTOut, &b_TRTOut);
   /* fChain->SetBranchAddress("innerSmallHits", &innerSmallHits, &b_innerSmallHits);  */
   /* fChain->SetBranchAddress("middleSmallHits", &middleSmallHits, &b_middleSmallHits);  */
   /* fChain->SetBranchAddress("outerSmallHits", &outerSmallHits, &b_outerSmallHits);  */
   /* fChain->SetBranchAddress("extendedSmallHits", &extendedSmallHits, &b_extendedSmallHits);  */
   /* fChain->SetBranchAddress("innerLargeHits", &innerLargeHits, &b_innerLargeHits);  */
   /* fChain->SetBranchAddress("middleLargeHits", &middleLargeHits, &b_middleLargeHits);  */
   /* fChain->SetBranchAddress("outerLargeHits", &outerLargeHits, &b_outerLargeHits);  */
   /* fChain->SetBranchAddress("extendedLargeHits", &extendedLargeHits, &b_extendedLargeHits);  */
   /* fChain->SetBranchAddress("phiLayer1RPCHits", &phiLayer1RPCHits, &b_phiLayer1RPCHits); */
   /* fChain->SetBranchAddress("phiLayer2RPCHits", &phiLayer2RPCHits, &b_phiLayer2RPCHits); */
   /* fChain->SetBranchAddress("phiLayer3RPCHits", &phiLayer3RPCHits, &b_phiLayer3RPCHits); */
   /* fChain->SetBranchAddress("phiLayer4RPCHits", &phiLayer4RPCHits, &b_phiLayer4RPCHits); */
   /* fChain->SetBranchAddress("etaLayer1RPCHits", &etaLayer1RPCHits, &b_etaLayer1RPCHits); */
   /* fChain->SetBranchAddress("etaLayer2RPCHits", &etaLayer2RPCHits, &b_etaLayer2RPCHits); */
   /* fChain->SetBranchAddress("etaLayer3RPCHits", &etaLayer3RPCHits, &b_etaLayer3RPCHits); */
   /* fChain->SetBranchAddress("etaLayer4RPCHits", &etaLayer4RPCHits, &b_etaLayer4RPCHits); */
   /* fChain->SetBranchAddress("phiLayer1Hits", &phiLayer1Hits, &b_phiLayer1Hits); */
   /* fChain->SetBranchAddress("phiLayer2Hits", &phiLayer2Hits, &b_phiLayer2Hits); */
   /* fChain->SetBranchAddress("phiLayer3Hits", &phiLayer3Hits, &b_phiLayer3Hits); */
   /* fChain->SetBranchAddress("phiLayer4Hits", &phiLayer4Hits, &b_phiLayer4Hits); */
   /* fChain->SetBranchAddress("etaLayer1Hits", &etaLayer1Hits, &b_etaLayer1Hits); */
   /* fChain->SetBranchAddress("etaLayer2Hits", &etaLayer2Hits, &b_etaLayer2Hits); */
   /* fChain->SetBranchAddress("etaLayer3Hits", &etaLayer3Hits, &b_etaLayer3Hits); */
   /* fChain->SetBranchAddress("etaLayer4Hits", &etaLayer4Hits, &b_etaLayer4Hits); */
   fChain->SetBranchAddress("d0", &d0, &b_d0);
   fChain->SetBranchAddress("d0sig", &d0sig, &b_d0sig);
   fChain->SetBranchAddress("z0", &z0, &b_z0);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("qOverPsignif", &qOverPsignif, &b_qOverPsignif);
   fChain->SetBranchAddress("reducedChi2", &reducedChi2, &b_reducedChi2);
   fChain->SetBranchAddress("nprecisionLayers", &nprecisionLayers, &b_nprecisionLayers);
   fChain->SetBranchAddress("nprecisionHoleLayers", &nprecisionHoleLayers, &b_nprecisionHoleLayers);
   fChain->SetBranchAddress("momentumBalanceSig", &momentumBalanceSig, &b_momentumBalanceSig);
   fChain->SetBranchAddress("scatteringCurvatureSig", &scatteringCurvatureSig, &b_scatteringCurvatureSig);
   fChain->SetBranchAddress("scatteringNeighbourSig", &scatteringNeighbourSig, &b_scatteringNeighbourSig);
   fChain->SetBranchAddress("energyLoss", &energyLoss, &b_energyLoss);
   fChain->SetBranchAddress("energyLossType", &energyLossType, &b_energyLossType);
   fChain->SetBranchAddress("muonSegmentDeltaEta", &muonSegmentDeltaEta, &b_muonSegmentDeltaEta);
   fChain->SetBranchAddress("middleHoles", &middleHoles, &b_middleHoles);
   fChain->SetBranchAddress("muonSeg1ChamberIdx", &muonSeg1ChamberIdx, &b_muonSeg1ChamberIdx);
   fChain->SetBranchAddress("muonSeg2ChamberIdx", &muonSeg2ChamberIdx, &b_muonSeg2ChamberIdx);
   fChain->SetBranchAddress("muonSeg3ChamberIdx", &muonSeg3ChamberIdx, &b_muonSeg3ChamberIdx);
   fChain->SetBranchAddress("muonSeg4ChamberIdx", &muonSeg4ChamberIdx, &b_muonSeg4ChamberIdx);
   fChain->SetBranchAddress("nSegments", &nSegments, &b_nSegments);
   /* fChain->SetBranchAddress("primary_chi2", &primary_chi2, &b_primary_chi2); */
   /* fChain->SetBranchAddress("primary_Ndof", &primary_Ndof, &b_primary_Ndof); */
   /* fChain->SetBranchAddress("ID_chi2", &ID_chi2, &b_ID_chi2); */
   /* fChain->SetBranchAddress("ID_Ndof", &ID_Ndof, &b_ID_Ndof); */
   /* fChain->SetBranchAddress("ME_chi2", &ME_chi2, &b_ME_chi2); */
   /* fChain->SetBranchAddress("ME_Ndof", &ME_Ndof, &b_ME_Ndof); */
   /* fChain->SetBranchAddress("MS_chi2", &MS_chi2, &b_MS_chi2); */
   /* fChain->SetBranchAddress("MS_Ndof", &MS_Ndof, &b_MS_Ndof); */
   /* fChain->SetBranchAddress("MSonly_chi2", &MSonly_chi2, &b_MSonly_chi2); */
   /* fChain->SetBranchAddress("MSonly_Ndof", &MSonly_Ndof, &b_MSonly_Ndof); */
   /* fChain->SetBranchAddress("segment1_chi2", &segment1_chi2, &b_segment1_chi2); */
   /* fChain->SetBranchAddress("segment1_Ndof", &segment1_Ndof, &b_segment1_Ndof); */
   /* fChain->SetBranchAddress("segment1_nprecisionHits", &segment1_nprecisionHits, &b_segment1_nprecisionHits); */
   /* fChain->SetBranchAddress("segment2_chi2", &segment2_chi2, &b_segment2_chi2); */
   /* fChain->SetBranchAddress("segment2_Ndof", &segment2_Ndof, &b_segment2_Ndof); */
   /* fChain->SetBranchAddress("segment2_nprecisionHits", &segment2_nprecisionHits, &b_segment2_nprecisionHits); */
   /* fChain->SetBranchAddress("segment3_chi2", &segment3_chi2, &b_segment3_chi2); */
   /* fChain->SetBranchAddress("segment3_Ndof", &segment3_Ndof, &b_segment3_Ndof); */
   /* fChain->SetBranchAddress("segment3_nprecisionHits", &segment3_nprecisionHits, &b_segment3_nprecisionHits); */

   Notify();
}

Bool_t muonNTlooper::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void muonNTlooper::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t muonNTlooper::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef muonNTlooper_cxx
