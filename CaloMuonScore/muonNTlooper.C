#define muonNTlooper_cxx
#include "muonNTlooper.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TMath.h"
#include <iostream>
#include <iomanip>
using namespace std;

void muonNTlooper::Loop(string outname, int CaloScore_WP)
{
//   In a ROOT session, you can do:
//      root> .L muonNTlooper.C
//      root> muonNTlooper t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   //double GeV = 1.0e3;
   TH1::SetDefaultSumw2();

   //TH1* m_tightWP_mediumPt_rhoCuts = (TH1*)((new TFile("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/MuonSelectorTools/PreRec2016_2016-04-13/muonSelection_tightWPHisto.root"))->Get("tightWP_mediumPt_rhoCuts"));

   const int Nwp = 9; 
   std::vector<std::string> WPnames = {"Tight", "Medium", "Loose", "VeryLoose", "HighPt", "LowPt", "LowPtMVA", "NoSelection", "RebuiltCaloScore"};


   //Create "padding" strings to facilitate easy table view of results
   std::vector<std::string> padding;
   padding.clear();

   unsigned int maxNameLength = 0;
   for (int wp = 0; wp < Nwp; wp++)
     if (WPnames[wp].size() > maxNameLength)
       maxNameLength = WPnames[wp].size();

   for (int wp = 0; wp < Nwp; wp++) {

     std::string pad = "";
     for (unsigned int i = 0; i < maxNameLength - WPnames[wp].size(); i++)
       pad += " ";

     padding.push_back(pad);
   }


   //Muon counters
   int allMuons = 0;
   // int nPositive = 0;
   // int nNegative = 0;
   int allMuonsUnknown = 0;


   //Summary information - how many muons passed each working point (with and without vetoing bad muons)
   int selectedMuons[Nwp];
   for (int wp = 0; wp < Nwp; wp++)
     selectedMuons[wp] = 0;

   int selectedMuonsNotBad[Nwp];
   for (int wp = 0; wp < Nwp; wp++)
     selectedMuonsNotBad[wp] = 0;

   //pT binning
   const int nxbins = 14;
   double xbins[nxbins+1] = {5., 6., 7., 8., 10., 12., 14., 16., 18., 20., 22., 25., 30., 50., 100.};


   //Histogram form
   TH1* selectedMuons_pt[Nwp];
   TH1* selectedMuonsNotBad_pt[Nwp];
   TH1* selectedMuons_eta[Nwp];
   TH1* selectedMuonsNotBad_eta[Nwp];
   TH1* selectedMuons_phi[Nwp];
   TH1* selectedMuonsNotBad_phi[Nwp];

   TH1* selectedMuons_caloScore[Nwp];
   TH1** selectedMuons_caloScoreBin[Nwp];

   TH1* selectedMuons_absEta[Nwp];
   TH1* selectedMuonsNotBad_absEta[Nwp];

   TH2* selectedMuons_etaPhi[Nwp];
   TH2* selectedMuonsNotBad_etaPhi[Nwp];

   TH1* selectedMuons_phiMod[Nwp];

   TH1* selectedMuons_siliconHits[Nwp];
   TH1* selectedMuons_pixelHits[Nwp];
   TH1* selectedMuons_SCTHits[Nwp];
   TH1* selectedMuons_TRTHits[Nwp];

   TH1* selectedMuons_innerSmallHits[Nwp];
   TH1* selectedMuons_middleSmallHits[Nwp];
   TH1* selectedMuons_outerSmallHits[Nwp];
   TH1* selectedMuons_extendedSmallHits[Nwp];

   TH1* selectedMuons_innerLargeHits[Nwp];
   TH1* selectedMuons_middleLargeHits[Nwp];
   TH1* selectedMuons_outerLargeHits[Nwp];
   TH1* selectedMuons_extendedLargeHits[Nwp];


   TH1* selectedMuons_primary_chi2[Nwp];
   TH1* selectedMuonsNotBad_primary_chi2[Nwp];
   TH1* selectedMuons_primary_Ndof[Nwp];
   TH1* selectedMuonsNotBad_primary_Ndof[Nwp];

   TH1* selectedMuons_primary_chi2nDof[Nwp];

   TH1* selectedMuons_ID_chi2[Nwp];
   TH1* selectedMuonsNotBad_ID_chi2[Nwp];
   TH1* selectedMuons_ID_Ndof[Nwp];
   TH1* selectedMuonsNotBad_ID_Ndof[Nwp];

   TH1* selectedMuons_ME_chi2[Nwp];
   TH1* selectedMuonsNotBad_ME_chi2[Nwp];
   TH1* selectedMuons_ME_Ndof[Nwp];
   TH1* selectedMuonsNotBad_ME_Ndof[Nwp];

   TH1* selectedMuons_MS_chi2[Nwp];
   TH1* selectedMuons_MS_Ndof[Nwp];

   TH1* selectedMuons_MSonly_chi2[Nwp];
   TH1* selectedMuonsNotBad_MSonly_chi2[Nwp];
   TH1* selectedMuons_MSonly_Ndof[Nwp];
   TH1* selectedMuonsNotBad_MSonly_Ndof[Nwp];

   TH1* selectedMuons_MSonly_chi2nDof[Nwp];
   TH1* selectedMuons_segment1_chi2nDof[Nwp];
   TH1* selectedMuons_segment2_chi2nDof[Nwp];
   TH1* selectedMuons_segment3_chi2nDof[Nwp];

   TH1* selectedMuons_nSegments[Nwp];

   TH1* selectedMuons_allSegments_nprecisionHits[Nwp];
   TH1* selectedMuons_allSegments_chi2[Nwp];
   TH1* selectedMuons_allSegments_chi2nDof[Nwp];
   TH1* selectedMuons_allSegments_Ndof[Nwp];

   TH1* selectedMuons_d0[Nwp];
   TH1* selectedMuons_d0sig[Nwp];
   TH1* selectedMuons_z0sintheta[Nwp];

   TH1* selectedMuons_qOpSig[Nwp];
   TH1* selectedMuons_rho[Nwp];
   TH1* selectedMuons_redChi2[Nwp];

   TH1* selectedMuons_eLoss[Nwp];
   TH1* selectedMuons_eLossType[Nwp];

   TH1* selectedMuons_author[Nwp];
   TH1* selectedMuons_type[Nwp];

   TH1* selectedMuons_truthType[Nwp];
   TH1* selectedMuons_truthOrigin[Nwp];

   for (int wp = 0; wp < Nwp; wp++) {

     //variable binning pT
     //selectedMuons_pt[wp] = new TH1D(("muon_pT_"+WPnames[wp]).c_str(),("muon_pT_"+WPnames[wp]).c_str(),nxbins,xbins);

    //  selectedMuons_pt[wp] = new TH1D(("muon_pT_"+WPnames[wp]).c_str(),("muon_pT_"+WPnames[wp]).c_str(),50,0,100);
    //  selectedMuonsNotBad_pt[wp] = new TH1D(("muon_notBad_pT_"+WPnames[wp]).c_str(),("muon_notBad_pT_"+WPnames[wp]).c_str(),50,0,100);
    selectedMuons_pt[wp] = new TH1D(("muon_pT_"+WPnames[wp]).c_str(),("muon_pT_"+WPnames[wp]).c_str(),110,0,110);
    selectedMuonsNotBad_pt[wp] = new TH1D(("muon_notBad_pT_"+WPnames[wp]).c_str(),("muon_notBad_pT_"+WPnames[wp]).c_str(),110,0,110);

    selectedMuons_caloScore[wp] = new TH1D(("muon_caloScore_"+WPnames[wp]).c_str(),("muon_caloScore_"+WPnames[wp]).c_str(),5000,0,1);

    selectedMuons_caloScoreBin[wp] = new TH1*[selectedMuons_pt[wp]->GetNbinsX()];
     for (int i = 1; i <= selectedMuons_pt[wp]->GetNbinsX(); i++)
       selectedMuons_caloScoreBin[wp][i-1] = new TH1D(("muon_caloScore_"+WPnames[wp]+"_bin"+to_string(i)).c_str(),("muon_caloScore_"+WPnames[wp]+"_bin"+to_string(i)).c_str(),5000,0,1);

    //  selectedMuons_eta[wp] = new TH1D(("muon_eta_"+WPnames[wp]).c_str(),("muon_eta_"+WPnames[wp]).c_str(),50,-3.0,3.0);
    //  selectedMuonsNotBad_eta[wp] = new TH1D(("muon_notBad_eta_"+WPnames[wp]).c_str(),("muon_notBad_eta_"+WPnames[wp]).c_str(),50,-3.0,3.0);
    selectedMuons_eta[wp] = new TH1D(("muon_eta_"+WPnames[wp]).c_str(),("muon_eta_"+WPnames[wp]).c_str(),200,-1.0,1.0);
    selectedMuonsNotBad_eta[wp] = new TH1D(("muon_notBad_eta_"+WPnames[wp]).c_str(),("muon_notBad_eta_"+WPnames[wp]).c_str(),200,-1.0,1.0);

     selectedMuons_absEta[wp] = new TH1D(("muon_absEta_"+WPnames[wp]).c_str(),("muon_absEta_"+WPnames[wp]).c_str(),25,0.0,2.5);
     selectedMuonsNotBad_absEta[wp] = new TH1D(("muon_notBad_absEta_"+WPnames[wp]).c_str(),("muon_notBad_absEta_"+WPnames[wp]).c_str(),25,0.0,2.5);

     selectedMuons_phi[wp] = new TH1D(("muon_phi_"+WPnames[wp]).c_str(),("muon_phi_"+WPnames[wp]).c_str(),50,-TMath::Pi(),TMath::Pi());
     selectedMuonsNotBad_phi[wp] = new TH1D(("muon_notBad_phi_"+WPnames[wp]).c_str(),("muon_notBad_phi_"+WPnames[wp]).c_str(),50,-TMath::Pi(),TMath::Pi());

     selectedMuons_etaPhi[wp] = new TH2D(("muon_etaPhi_"+WPnames[wp]).c_str(),("muon_etaPhi_"+WPnames[wp]).c_str(),25,-2.5,2.5,20,-TMath::Pi(),TMath::Pi());
     selectedMuonsNotBad_etaPhi[wp] = new TH2D(("muon_notBad_etaPhi_"+WPnames[wp]).c_str(),("muon_notBad_etaPhi_"+WPnames[wp]).c_str(),50,-2.5,2.5,40,-TMath::Pi(),TMath::Pi());

     selectedMuons_phiMod[wp] = new TH1D(("muon_phiMod_"+WPnames[wp]).c_str(),("muon_phiMod_"+WPnames[wp]).c_str(),50,0.0,TMath::Pi()/4.0);

     selectedMuons_siliconHits[wp] = new TH1D(("muon_siliconHits_"+WPnames[wp]).c_str(),("muon_siliconHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_pixelHits[wp] = new TH1D(("muon_pixelHits_"+WPnames[wp]).c_str(),("muon_pixelHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_SCTHits[wp] = new TH1D(("muon_SCTHits_"+WPnames[wp]).c_str(),("muon_SCTHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_TRTHits[wp] = new TH1D(("muon_TRTHits_"+WPnames[wp]).c_str(),("muon_TRTHits_"+WPnames[wp]).c_str(),20,0,50);

     selectedMuons_innerSmallHits[wp] = new TH1D(("muon_innerSmallHits_"+WPnames[wp]).c_str(),("muon_innerSmallHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_middleSmallHits[wp] = new TH1D(("muon_middleSmallHits_"+WPnames[wp]).c_str(),("muon_middleSmallHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_outerSmallHits[wp] = new TH1D(("muon_outerSmallHits_"+WPnames[wp]).c_str(),("muon_outerSmallHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_extendedSmallHits[wp] = new TH1D(("muon_extendedSmallHits_"+WPnames[wp]).c_str(),("muon_extendedSmallHits_"+WPnames[wp]).c_str(),20,0,20);

     selectedMuons_innerLargeHits[wp] = new TH1D(("muon_innerLargeHits_"+WPnames[wp]).c_str(),("muon_innerLargeHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_middleLargeHits[wp] = new TH1D(("muon_middleLargeHits_"+WPnames[wp]).c_str(),("muon_middleLargeHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_outerLargeHits[wp] = new TH1D(("muon_outerLargeHits_"+WPnames[wp]).c_str(),("muon_outerLargeHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_extendedLargeHits[wp] = new TH1D(("muon_extendedLargeHits_"+WPnames[wp]).c_str(),("muon_extendedLargeHits_"+WPnames[wp]).c_str(),20,0,20);

     selectedMuons_primary_chi2[wp] = new TH1D(("muon_primary_chi2_"+WPnames[wp]).c_str(),("muon_primary_chi2_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_primary_chi2[wp] = new TH1D(("muon_notBad_primary_chi2_"+WPnames[wp]).c_str(),("muon_notBad_primary_chi2_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_primary_Ndof[wp] = new TH1D(("muon_primary_Ndof_"+WPnames[wp]).c_str(),("muon_primary_Ndof_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_primary_Ndof[wp] = new TH1D(("muon_notBad_primary_Ndof_"+WPnames[wp]).c_str(),("muon_notBad_primary_Ndof_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_ID_chi2[wp] = new TH1D(("muon_ID_chi2_"+WPnames[wp]).c_str(),("muon_ID_chi2_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_ID_chi2[wp] = new TH1D(("muon_notBad_ID_chi2_"+WPnames[wp]).c_str(),("muon_notBad_ID_chi2_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_ID_Ndof[wp] = new TH1D(("muon_ID_Ndof_"+WPnames[wp]).c_str(),("muon_ID_Ndof_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_ID_Ndof[wp] = new TH1D(("muon_notBad_ID_Ndof_"+WPnames[wp]).c_str(),("muon_notBad_ID_Ndof_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_ME_chi2[wp] = new TH1D(("muon_ME_chi2_"+WPnames[wp]).c_str(),("muon_ME_chi2_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_ME_chi2[wp] = new TH1D(("muon_notBad_ME_chi2_"+WPnames[wp]).c_str(),("muon_notBad_ME_chi2_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_ME_Ndof[wp] = new TH1D(("muon_ME_Ndof_"+WPnames[wp]).c_str(),("muon_ME_Ndof_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_ME_Ndof[wp] = new TH1D(("muon_notBad_ME_Ndof_"+WPnames[wp]).c_str(),("muon_notBad_ME_Ndof_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_MS_chi2[wp] = new TH1D(("muon_MS_chi2_"+WPnames[wp]).c_str(),("muon_MS_chi2_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuons_MS_Ndof[wp] = new TH1D(("muon_MS_Ndof_"+WPnames[wp]).c_str(),("muon_MS_Ndof_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_MSonly_chi2[wp] = new TH1D(("muon_MSonly_chi2_"+WPnames[wp]).c_str(),("muon_MSonly_chi2_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_MSonly_chi2[wp] = new TH1D(("muon_notBad_MSonly_chi2_"+WPnames[wp]).c_str(),("muon_notBad_MSonly_chi2_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_MSonly_Ndof[wp] = new TH1D(("muon_MSonly_Ndof_"+WPnames[wp]).c_str(),("muon_MSonly_Ndof_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuonsNotBad_MSonly_Ndof[wp] = new TH1D(("muon_notBad_MSonly_Ndof_"+WPnames[wp]).c_str(),("muon_notBad_MSonly_Ndof_"+WPnames[wp]).c_str(),50,0,200);

     selectedMuons_MSonly_chi2nDof[wp] = new TH1D(("muon_MSonly_chi2nDof_"+WPnames[wp]).c_str(),("muon_MSonly_chi2nDof_"+WPnames[wp]).c_str(),50,0,10);

     selectedMuons_nSegments[wp] = new TH1D(("muon_nSegments_"+WPnames[wp]).c_str(),("muon_nSegments_"+WPnames[wp]).c_str(),10,0,10);

     selectedMuons_allSegments_nprecisionHits[wp] = new TH1D(("muon_allSegments_nprecisionHits_"+WPnames[wp]).c_str(),("muon_allSegments_nprecisionHits_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_allSegments_chi2[wp] = new TH1D(("muon_allSegments_chi2_"+WPnames[wp]).c_str(),("muon_allSegments_chi2_"+WPnames[wp]).c_str(),50,0,20);
     selectedMuons_allSegments_chi2nDof[wp] = new TH1D(("muon_allSegments_chi2nDof_"+WPnames[wp]).c_str(),("muon_allSegments_chi2nDof_"+WPnames[wp]).c_str(),50,0,10);
     selectedMuons_allSegments_Ndof[wp] = new TH1D(("muon_allSegments_Ndof_"+WPnames[wp]).c_str(),("muon_allSegments_Ndof_"+WPnames[wp]).c_str(),20,0,20);

     selectedMuons_d0[wp] = new TH1D(("muon_d0_"+WPnames[wp]).c_str(),("muon_d0_"+WPnames[wp]).c_str(),50,-10,10);
     selectedMuons_d0sig[wp] = new TH1D(("muon_d0sig_"+WPnames[wp]).c_str(),("muon_d0sig_"+WPnames[wp]).c_str(),50,-10,10);

     selectedMuons_z0sintheta[wp] = new TH1D(("muon_z0sintheta_"+WPnames[wp]).c_str(),("muon_z0sintheta_"+WPnames[wp]).c_str(),200,-10,10);

     selectedMuons_qOpSig[wp] = new TH1D(("muon_qOpSig_"+WPnames[wp]).c_str(),("muon_qOpSig_"+WPnames[wp]).c_str(),50,0,10);
     selectedMuons_redChi2[wp] = new TH1D(("muon_redChi2_"+WPnames[wp]).c_str(),("muon_redChi2_"+WPnames[wp]).c_str(),50,0,3);
     selectedMuons_rho[wp] = new TH1D(("muon_rho_"+WPnames[wp]).c_str(),("muon_rho_"+WPnames[wp]).c_str(),50,0,1);

     selectedMuons_primary_chi2nDof[wp] = new TH1D(("muon_primary_chi2nDof_"+WPnames[wp]).c_str(),("muon_primary_chi2nDof_"+WPnames[wp]).c_str(),50,0,3);

     selectedMuons_eLoss[wp] = new TH1D(("muon_eLoss_"+WPnames[wp]).c_str(),("muon_eLoss_"+WPnames[wp]).c_str(),50,0,200);
     selectedMuons_eLossType[wp] = new TH1D(("muon_eLossType_"+WPnames[wp]).c_str(),("muon_eLossType_"+WPnames[wp]).c_str(),10,0,10);

     selectedMuons_type[wp] = new TH1D(("muon_type_"+WPnames[wp]).c_str(),("muon_type_"+WPnames[wp]).c_str(),20,0,10);

     selectedMuons_author[wp] = new TH1D(("muon_author_"+WPnames[wp]).c_str(),("muon_author_"+WPnames[wp]).c_str(),12,0,12);
     string authorStrings[12] = {"unknown", "MuidCo", "STACO", "MuTag", "MuTagIMO", "MuidSA", "MuGirl", "MuGirlLowBeta", "CaloTag", "CaloLikelihood", "CaloScore", "ExtrapolateMuonToIP"};
     for (int i = 0; i < 12; i++)
       selectedMuons_author[wp]->GetXaxis()->SetBinLabel(i+1,authorStrings[i].c_str());

     selectedMuons_truthType[wp] = new TH1D(("muon_truthType_"+WPnames[wp]).c_str(),("muon_truthType_"+WPnames[wp]).c_str(),20,0,20);
     selectedMuons_truthOrigin[wp] = new TH1D(("muon_truthOrigin_"+WPnames[wp]).c_str(),("muon_truthOrigin_"+WPnames[wp]).c_str(),40,0,40);
   }


   //Obtain summary information also split by muon type
   const int Ntype = 5;

   std::string typeNames[Ntype];
   for (int type = 0; type < Ntype; type++) {

     if(type == 0)
       typeNames[type] = "combined";
     else if(type == 1)
       typeNames[type] = "stand-alone";
     else if(type == 2)
       typeNames[type] = "segment-tagged";
     else if(type == 3)
       typeNames[type] = "calo-tagged";
     else if(type == 4)
       typeNames[type] = "forward";
     else
       typeNames[type] = "unknown";
   }


   //Muon counters for each type                                                                                                                                                                     
   int allMuonsType[Ntype];
   for (int type = 0; type < Ntype; type++)
     allMuonsType[type] = 0;

   //Muon counters for muons of each type passing each working point                                                                                                                                 
   int selectedMuonsType[Ntype][Nwp];
   for (int type = 0; type < Ntype; type++)
     for (int wp = 0; wp < Nwp; wp++)
       selectedMuonsType[type][wp] = 0;

   int selectedMuonsTypeNotBad[Ntype][Nwp];
   for (int type = 0; type < Ntype; type++)
     for (int wp = 0; wp < Nwp; wp++)
       selectedMuonsTypeNotBad[type][wp] = 0;



   //Obtain summary information also split by muon truth type
   const int NtruthType = 5;

   std::string truthTypeNames[NtruthType] = {"Prompt", "Non-isolated", "Hadron", "Background", "Other"};

   //Muon counters for each truth type
   int allMuonsTruthType[NtruthType];
   for (int truthType = 0; truthType < NtruthType; truthType++)
     allMuonsTruthType[truthType] = 0;

   //Muon counters for muons of each type passing each working point
   int selectedMuonsTruthType[NtruthType][Nwp];
   for (int truthType = 0; truthType < NtruthType; truthType++)
     for (int wp = 0; wp < Nwp; wp++)
       selectedMuonsTruthType[truthType][wp] = 0;

   int selectedMuonsTruthTypeNotBad[NtruthType][Nwp];
   for (int truthType = 0; truthType < NtruthType; truthType++)
     for (int wp = 0; wp < Nwp; wp++)
       selectedMuonsTruthTypeNotBad[truthType][wp] = 0;




   Bool_t passedWP[Nwp];

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (TMath::Abs(eta) > 0.1)
	continue;

      // if (nSegments > 3)
      // 	continue;

      // if (TMath::Abs(eta) > 2.5 || TMath::Abs(eta) < 0.1)
      // 	continue;

      // if (TMath::Abs(d0) > 0.5)
      //  	continue;

      // if (caloMuonIDTag <= 10)
      // 	continue;
      
      // if (truthType == 0 || truthType == 1)
      // 	continue;
      
      if (truthType != 0)
       	continue;

      // if (truthTypeMCTC != 0)
      //  	continue;


      //enum MuonType { Combined, MuonStandAlone, SegmentTagged, CaloTagged, SiliconAssociatedForwardMuon     };

      if (muonType != 3)
       	continue;

      // if (quality != 2)
      // 	continue;
      
      // if (pT > 3.0 || pT < 2.0)
      //  	continue;

      // if (pT > 20.0) // || pT > 100.)
      //  	continue;

      //enum Author {unknown=0, MuidCo, STACO, MuTag, MuTagIMO, MuidSA, MuGirl, MuGirlLowBeta, CaloTag, CaloLikelihood, ExtrapolateMuonToIP, NumberOfMuonAuthors }; R21

      //enum Author {unknown=0, MuidCo, STACO, MuTag, MuTagIMO, MuidSA, MuGirl, MuGirlLowBeta, CaloTag, CaloLikelihood, CaloScore, ExtrapolateMuonToIP, NumberOfMuonAuthors }; R22

      // if (author == 2)
      //  	continue;

      // if (author != 6 && author != 4 && author != 1)
      // 	continue;

      // if (!isAuthor->at(10))  //To choose muons with primary CaloMuonScore
      // 	continue;

      //if (author != 9 or isAuthor->at(10))
      // 	continue;

      // if (energyLossType != 3) //3 = tail, i.e. calo measurement
      // 	continue;


      // if (muonType == 1) {
      // 	cout << "Nauthor = " << isAuthor->size() << endl;
      // 	cout << "isAuthor: ";
      // 	for (Int_t i = 0; i < isAuthor->size(); i++)
      // 	  cout << isAuthor->at(i) << "  ";
      // 	cout << endl;
      // }
      
      //cout << "energyLossType = " << energyLossType << endl;

      allMuons++;
      allMuonsType[muonType]++;
      allMuonsTruthType[truthType]++;

      if (truthTypeMCTC == 0)
	    allMuonsUnknown++;

      float cutValue;
      float a,b,c,d;

      if (CaloScore_WP==0)
        cutValue = 0.92;
              
      else if (CaloScore_WP==1)
        cutValue = 0.59;

      else{
        if (pT > 20.0) //constant cut above 20 GeV
        cutValue = 0.76;
        
        //cut value for pT below 20 GeV
        else if (CaloScore_WP==2)
        {
            a = -1.98e-4;
            b = 6.04e-3;
            c = -6.13e-2;
            d = 1.16;
            cutValue = a*std::pow(pT, 3) + b*std::pow(pT, 2) + c*pT + d;
        }        
        else if (CaloScore_WP==3)
        {
            a = -1.80277888e-4;
            b = 5.01552713e-3;
            c = -4.62271761e-2;
            d = 1.12479350;
            cutValue = a*std::pow(pT, 3) + b*std::pow(pT, 2) + c*pT + d;
        }        
      }
      for (int wp = 0; wp < Nwp; wp++) { //{"Tight", "Medium", "Loose", "VeryLoose", "HighPt", "LowPt", "LowPtMVA", "NoSelection", "RebuiltCaloScore"}

        if (wp == 2){
          if (CaloScore_WP == -1) //for Rel 21
            passedWP[wp] = quality <= wp && passIDhits && passPresel && TMath::Abs(eta) < 2.7;
          else
          passedWP[wp] = (caloMuonScore >= cutValue) && quality <= wp && passIDhits && passPresel && TMath::Abs(eta) < 2.7;
        }
        else if ((wp < 2)|| (wp == 3))
          passedWP[wp] = quality <= wp && passIDhits && passPresel && TMath::Abs(eta) < 2.7;
        else if(wp == 4)
          passedWP[wp] = passHighPt;
        else if (wp == 5)
          passedWP[wp] = passLowPt;
        else if (wp == 6)
          passedWP[wp] = passLowPtMVA;
        else if (wp == 7)
          passedWP[wp] = true;
        else if (wp == 8){
          if (CaloScore_WP == -1) //for Rel 21
            passedWP[wp] = passIDhits && passPresel && TMath::Abs(eta) < 2.7;
          else
          passedWP[wp] = (caloMuonScore >= cutValue) && passIDhits && passPresel && TMath::Abs(eta) < 2.7;
        }
        else
          cout << "ERROR: bad working point!\n";
      }


      for (int wp = 0; wp < Nwp; wp++) {

	if (passedWP[wp]) {
	  selectedMuons[wp]++;
	  selectedMuonsType[muonType][wp]++;
	  selectedMuonsTruthType[truthType][wp]++;

	  selectedMuons_pt[wp]->Fill(pT);
	  selectedMuons_eta[wp]->Fill(eta);
	  selectedMuons_phi[wp]->Fill(phi);

	  selectedMuons_caloScore[wp]->Fill(caloMuonScore);

	  if (selectedMuons_pt[wp]->FindBin(pT) != selectedMuons_pt[wp]->GetNbinsX()+1 && selectedMuons_pt[wp]->FindBin(pT) != 0)
	    selectedMuons_caloScoreBin[wp][selectedMuons_pt[wp]->FindBin(pT)-1]->Fill(caloMuonScore);

	  if (selectedMuons_pt[wp]->FindBin(pT) == 0)
	    cout << "This is the one: pT = " << pT << endl;

	  selectedMuons_absEta[wp]->Fill(TMath::Abs(eta));

	  selectedMuons_phiMod[wp]->Fill(fmod(phi,TMath::Pi()/4.0));

	  selectedMuons_etaPhi[wp]->Fill(eta,phi);

	  selectedMuons_siliconHits[wp]->Fill(PixHits+SCTHits);
	  selectedMuons_pixelHits[wp]->Fill(PixHits);
	  selectedMuons_SCTHits[wp]->Fill(SCTHits);
	  selectedMuons_TRTHits[wp]->Fill(TRTHits);

	  selectedMuons_innerSmallHits[wp]->Fill(innerSmallHits);
          selectedMuons_middleSmallHits[wp]->Fill(middleSmallHits);
          selectedMuons_outerSmallHits[wp]->Fill(outerSmallHits);
          selectedMuons_extendedSmallHits[wp]->Fill(extendedSmallHits);

          selectedMuons_innerLargeHits[wp]->Fill(innerLargeHits);
          selectedMuons_middleLargeHits[wp]->Fill(middleLargeHits);
          selectedMuons_outerLargeHits[wp]->Fill(outerLargeHits);
          selectedMuons_extendedLargeHits[wp]->Fill(extendedLargeHits);


	  selectedMuons_primary_chi2[wp]->Fill(primary_chi2);
          selectedMuons_primary_Ndof[wp]->Fill(primary_Ndof);

	  selectedMuons_primary_chi2nDof[wp]->Fill(primary_chi2/primary_Ndof);

          selectedMuons_ID_chi2[wp]->Fill(ID_chi2);
          selectedMuons_ID_Ndof[wp]->Fill(ID_Ndof);

          selectedMuons_ME_chi2[wp]->Fill(ME_chi2);
          selectedMuons_ME_Ndof[wp]->Fill(ME_Ndof);

          selectedMuons_MS_chi2[wp]->Fill(MS_chi2);
          selectedMuons_MS_Ndof[wp]->Fill(MS_Ndof);

          selectedMuons_MSonly_chi2[wp]->Fill(MSonly_chi2);
          selectedMuons_MSonly_Ndof[wp]->Fill(MSonly_Ndof);
          selectedMuons_MSonly_chi2nDof[wp]->Fill(MSonly_chi2/MSonly_Ndof);

	  selectedMuons_nSegments[wp]->Fill(nSegments);

	  selectedMuons_allSegments_nprecisionHits[wp]->Fill(segment1_nprecisionHits);
          selectedMuons_allSegments_nprecisionHits[wp]->Fill(segment2_nprecisionHits);
          selectedMuons_allSegments_nprecisionHits[wp]->Fill(segment3_nprecisionHits);

          selectedMuons_allSegments_chi2[wp]->Fill(segment1_chi2);
          selectedMuons_allSegments_chi2[wp]->Fill(segment2_chi2);
          selectedMuons_allSegments_chi2[wp]->Fill(segment3_chi2);

          selectedMuons_allSegments_Ndof[wp]->Fill(segment1_Ndof);
          selectedMuons_allSegments_Ndof[wp]->Fill(segment2_Ndof);
          selectedMuons_allSegments_Ndof[wp]->Fill(segment3_Ndof);

	  if (segment1_chi2 != -999.0)
            selectedMuons_allSegments_chi2nDof[wp]->Fill(segment1_chi2/segment1_Ndof);
          if (segment2_chi2 != -999.0)
            selectedMuons_allSegments_chi2nDof[wp]->Fill(segment2_chi2/segment2_Ndof);
          if (segment3_chi2 != -999.0)
            selectedMuons_allSegments_chi2nDof[wp]->Fill(segment3_chi2/segment3_Ndof);

	  selectedMuons_d0[wp]->Fill(d0);
	  selectedMuons_d0sig[wp]->Fill(d0sig);

	  double sintheta = 1.0/TMath::CosH(eta);

	  selectedMuons_z0sintheta[wp]->Fill(z0*sintheta);

	  selectedMuons_qOpSig[wp]->Fill(qOverPsignif);
	  selectedMuons_rho[wp]->Fill(rho);
	  selectedMuons_redChi2[wp]->Fill(reducedChi2);

	  selectedMuons_eLoss[wp]->Fill(energyLoss/1.e3);
	  selectedMuons_eLossType[wp]->Fill(energyLossType);

	  selectedMuons_type[wp]->Fill(muonType);
	  // selectedMuons_author[wp]->Fill(author);
    
    const int Nauthor = 12; 

    for(int i=0; i<Nauthor; i++) 
    selectedMuons_author[wp]->Fill(i, isAuthor->at(i)); //Fill the author flag to the author histogram

	  selectedMuons_truthType[wp]->Fill(truthTypeMCTC);
	  selectedMuons_truthOrigin[wp]->Fill(truthOriginMCTC);

	  Bool_t isBadMuon;
	  if ((wp != 4 && isBadMuon_other) || (wp == 4 && isBadMuon_highPt))
	    isBadMuon = true;
	  else
	    isBadMuon = false;

	  if (!isBadMuon) {
	    selectedMuonsNotBad[wp]++;
	    selectedMuonsTypeNotBad[muonType][wp]++;
	    selectedMuonsTruthTypeNotBad[truthType][wp]++;

	    selectedMuonsNotBad_pt[wp]->Fill(pT);
	    selectedMuonsNotBad_eta[wp]->Fill(eta);
	    selectedMuonsNotBad_absEta[wp]->Fill(TMath::Abs(eta));
	    selectedMuonsNotBad_phi[wp]->Fill(phi);
	    selectedMuonsNotBad_etaPhi[wp]->Fill(eta,phi);

	    selectedMuonsNotBad_primary_chi2[wp]->Fill(primary_chi2);
            selectedMuonsNotBad_primary_Ndof[wp]->Fill(primary_Ndof);

            selectedMuonsNotBad_ID_chi2[wp]->Fill(ID_chi2);
            selectedMuonsNotBad_ID_Ndof[wp]->Fill(ID_Ndof);

            selectedMuonsNotBad_ME_chi2[wp]->Fill(ME_chi2);
            selectedMuonsNotBad_ME_Ndof[wp]->Fill(ME_Ndof);

            selectedMuonsNotBad_MSonly_chi2[wp]->Fill(MSonly_chi2);
            selectedMuonsNotBad_MSonly_Ndof[wp]->Fill(MSonly_Ndof);
	  }
	}
      }

   }

   char* str = new char[200];

   cout << "Total number of muons: " << allMuons << endl;
   cout << "Selected muons by working point (numbers in parenthesis include bad muon veto):\n";
   cout << "--------------------------\n";
   for (int wp = 0; wp < Nwp; wp++) {
     sprintf(str,"%s: %s %i (%i)", WPnames[wp].c_str(), padding[wp].c_str(), selectedMuons[wp], selectedMuonsNotBad[wp]);
     cout << str << endl;
   }
   cout << "--------------------------\n";

   //Make table of selected muons by type and working point
   cout << "Selected muons by type and working point (numbers in parenthesis include bad muon veto):\n";
   cout << "---------------------------------------------------------------------------------------\n";
   for (int l = 0; l < Nwp+2; l++) {

     std::string line = "";
     if (l == 0) { //line with type names
       line += "                    ";
       for (int type = 0; type < Ntype; type++)
         line += typeNames[type] + "         ";
     }
     else if (l == 1) { //line for all muons inclusive
       line += "All muons:             ";
       for (int type = 0; type < Ntype; type++) {
	 std::stringstream ss;
         ss << std::left << std::setw(20) << std::to_string(allMuonsType[type]);
         line += ss.str();
       }
     }
     else { //lines for each of the working points
       int wp = l - 2;
       line += WPnames[wp] + ":" + padding[wp] + "     ";
       for (int type = 0; type < Ntype; type++) {
	 std::stringstream ss;
         ss << std::left << std::setw(20) << (std::to_string(selectedMuonsType[type][wp]) + " (" + std::to_string(selectedMuonsTypeNotBad[type][wp]) + ")");
         line += ss.str();
       }
     }

     cout << line << endl;
   }
   cout << "---------------------------------------------------------------------------------------\n";



   //Make table of selected muons by truth type and working point
   cout << "Selected muons by truth classification and working point (numbers in parenthesis include bad muon veto):\n";
   cout << "---------------------------------------------------------------------------------------\n";
   for (int l = 0; l < Nwp+2; l++) {
     
     std::string line = "";
     if (l == 0) { //line with truth classification labels                                                                                                                                          
       line += "                      ";
       for (int truthType = 0; truthType < NtruthType; truthType++)
	 line += truthTypeNames[truthType] + "           ";
     }
     else if (l == 1) { //line for all muons inclusive                                                                                                                                              
       line += "All muons:             ";
       for (int truthType = 0; truthType < NtruthType; truthType++) {
	 std::stringstream ss;
	 ss << std::left << std::setw(20) << std::to_string(allMuonsTruthType[truthType]);
	 line += ss.str();
       }
     }
     else { //lines for each of the working points 
       int wp = l - 2;
       line += WPnames[wp] + ":" + padding[wp] + "     ";
       for (int truthType = 0; truthType < NtruthType; truthType++) {
	 std::stringstream ss;
	 ss << std::left << std::setw(20) << (std::to_string(selectedMuonsTruthType[truthType][wp]) + " (" + std::to_string(selectedMuonsTruthTypeNotBad[truthType][wp]) + ")");
	 line += ss.str();
       }
     }

     cout << line << endl;
   }
   cout << "---------------------------------------------------------------------------------------\n";
   cout << "Muons classified as unknown by MCTC: " << allMuonsUnknown << endl;
   cout << "---------------------------------------------------------------------------------------\n";

   TFile* fout = new TFile(outname.c_str(),"recreate");
   fout->cd();

   for (int wp = 0; wp < Nwp; wp++) {
     selectedMuons_pt[wp]->Write();
     selectedMuonsNotBad_pt[wp]->Write();
     selectedMuons_eta[wp]->Write();
     selectedMuonsNotBad_eta[wp]->Write();
     selectedMuons_phi[wp]->Write();
     selectedMuonsNotBad_phi[wp]->Write();

     selectedMuons_caloScore[wp]->Write();

     for (int i = 1; i <= selectedMuons_pt[wp]->GetNbinsX(); i++)
       selectedMuons_caloScoreBin[wp][i-1]->Write();

     selectedMuons_absEta[wp]->Write();
     selectedMuonsNotBad_absEta[wp]->Write();

     selectedMuons_phiMod[wp]->Write();

     selectedMuons_etaPhi[wp]->Write();
     selectedMuonsNotBad_etaPhi[wp]->Write();

     selectedMuons_siliconHits[wp]->Write();
     selectedMuons_pixelHits[wp]->Write();
     selectedMuons_SCTHits[wp]->Write();
     selectedMuons_TRTHits[wp]->Write();

     selectedMuons_innerSmallHits[wp]->Write();
     selectedMuons_middleSmallHits[wp]->Write();
     selectedMuons_outerSmallHits[wp]->Write();
     selectedMuons_extendedSmallHits[wp]->Write();

     selectedMuons_innerLargeHits[wp]->Write();
     selectedMuons_middleLargeHits[wp]->Write();
     selectedMuons_outerLargeHits[wp]->Write();
     selectedMuons_extendedLargeHits[wp]->Write();


     selectedMuons_primary_chi2[wp]->Write();
     selectedMuonsNotBad_primary_chi2[wp]->Write();
     selectedMuons_primary_Ndof[wp]->Write();
     selectedMuonsNotBad_primary_Ndof[wp]->Write();

     selectedMuons_primary_chi2nDof[wp]->Write();

     selectedMuons_ID_chi2[wp]->Write();
     selectedMuonsNotBad_ID_chi2[wp]->Write();
     selectedMuons_ID_Ndof[wp]->Write();
     selectedMuonsNotBad_ID_Ndof[wp]->Write();

     selectedMuons_ME_chi2[wp]->Write();
     selectedMuonsNotBad_ME_chi2[wp]->Write();
     selectedMuons_ME_Ndof[wp]->Write();
     selectedMuonsNotBad_ME_Ndof[wp]->Write();

     selectedMuons_MS_chi2[wp]->Write();
     selectedMuons_MS_Ndof[wp]->Write();

     selectedMuons_MSonly_chi2[wp]->Write();
     selectedMuonsNotBad_MSonly_chi2[wp]->Write();
     selectedMuons_MSonly_Ndof[wp]->Write();
     selectedMuonsNotBad_MSonly_Ndof[wp]->Write();

     selectedMuons_MSonly_chi2nDof[wp]->Write();

     selectedMuons_allSegments_nprecisionHits[wp]->Write();
     selectedMuons_allSegments_chi2[wp]->Write();
     selectedMuons_allSegments_chi2nDof[wp]->Write();
     selectedMuons_allSegments_Ndof[wp]->Write();

     selectedMuons_nSegments[wp]->Write();

     selectedMuons_d0[wp]->Write();
     selectedMuons_d0sig[wp]->Write();
     selectedMuons_z0sintheta[wp]->Write();

     selectedMuons_qOpSig[wp]->Write();
     selectedMuons_rho[wp]->Write();
     selectedMuons_redChi2[wp]->Write();
     selectedMuons_eLoss[wp]->Write();
     selectedMuons_eLossType[wp]->Write();
     selectedMuons_author[wp]->Write();
     selectedMuons_type[wp]->Write();

     selectedMuons_truthType[wp]->Write();
     selectedMuons_truthOrigin[wp]->Write();
   }

   fout->Close();
}
