#include <iostream>
using namespace std;

#include "TROOT.h"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasStyle.C"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasUtils.C"

void Efficiency_Plot(TFile* histR22 , TFile* histR21, TString var_name, TString Xtitle, TString CaloWPname, TString TruthTypeName){
  
  TCanvas* c = new TCanvas("", "", 1000, 1000);
  TPad *npad = new TPad("npad", "", 0.0,0.3, 1, 1.0);
  //npad->SetLogy();
  //npad->SetLogx();
  npad->Draw();
  npad->cd();

  // rebuiltmuonscore
  TString num_name = "muon_"+var_name+"_Loose";
  TString denom_name = "muon_"+var_name+"_NoSelection";

  TH1D* hR22_num = (TH1D*)histR22->Get(num_name);
  TH1D* hR22_denom = (TH1D*)histR22->Get(denom_name);
  
  TH1D* hR21_num = (TH1D*)histR21->Get(num_name);
  TH1D* hR21_denom = (TH1D*)histR21->Get(denom_name);

  if (var_name == "pT")
  {
    hR22_num->Rebin(5);
    hR22_denom->Rebin(5);
    
    hR21_num->Rebin(5);
    hR21_denom->Rebin(5);
  }

  TH1D* hR22 = (TH1D*) hR22_num->Clone();
  TH1D* hR21 = (TH1D*) hR21_num->Clone();

  //***
  hR22->Divide(hR22_denom);
  hR21->Divide(hR21_denom);

  hR22->SetLineColor(kBlue);
  hR21->SetLineColor(kRed);
  
  if(TruthTypeName=="prompt"){
    hR22->SetYTitle("#epsilon_{sig}");
  }

  if(TruthTypeName=="fake"){
    hR22->SetYTitle("#epsilon_{bkg}");
  }
  hR22->SetXTitle(Xtitle);
  if(hR22->GetMaximum()>hR21->GetMaximum()){
//  hR22->GetYaxis()->SetRangeUser(hR22->GetMinimum() ,1.5*hR22->GetMaximum());}     
  hR22->GetYaxis()->SetRangeUser((hR22->GetMinimum()-hR22->GetMaximum())/2 ,1.5*hR22->GetMaximum());}     

  else{
//  hR22->GetYaxis()->SetRangeUser(hR21->GetMinimum(),1.5*hR21->GetMaximum());}   
  hR22->GetYaxis()->SetRangeUser((hR21->GetMinimum()-hR21->GetMaximum())/2 ,1.5*hR21->GetMaximum());}   

  if (var_name == "eta"){
    hR22->GetXaxis()->SetRangeUser(-0.1,0.1);
    //hR22->GetYaxis()->SetRangeUser(0.7,1.1);
  }
  
  //change to 10 start
  if (var_name == "pT"){
    hR22->GetXaxis()->SetRangeUser(0,110.0);
    //hR22->GetYaxis()->SetRangeUser(0.34,1.1);
  }

    
  hR22->Draw("hist");
  hR21->Draw("hist same");


  if(TruthTypeName=="prompt"){
    myText(0.76,0.3,1,"Prompt");
    TLegend* leg = new TLegend(0.266476,0.223629,0.686246,0.339662);
    leg->AddEntry(hR22,"CaloMuonScore","L");
    leg->AddEntry(hR21,"CaloMuonIDTag","L");
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetShadowColor(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.05);
    leg->Draw();
  }    

  if(TruthTypeName=="fake"){
    myText(0.66,0.86,1,"Fakes");
    TLegend* leg = new TLegend(0.202006,0.803797,0.621777,0.917722);
    leg->AddEntry(hR22,"CaloMuonScore","L");
    leg->AddEntry(hR21,"CaloMuonIDTag","L");
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetShadowColor(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.05);
    leg->Draw();
  }


  c->cd();

  TPad *npad2 = new TPad("npad2", "", 0.00,0.00, 1, 0.3);
  npad2->Draw();
  npad2->cd();
  npad2->SetGridy();
  //npad2->SetLogx();

  TH1* ratio = (TH1*)hR22->Clone();
  ratio->Divide(hR21);

  ratio->SetYTitle("#frac{#epsilon_{CaloMuonScore}}{#epsilon_{CaloMuonTag}}");
  ratio->SetXTitle(hR22->GetXaxis()->GetTitle());
  ratio->SetLabelSize(0.11,"XY");
  ratio->SetTitleSize(0.0,"X");
  ratio->SetTitleSize(0.14,"Y");
  ratio->SetTitleOffset(0.45,"Y");
  ratio->GetYaxis()->SetRangeUser(ratio->GetMinimum(), 1.5 * ratio->GetMaximum());
//  ratio->GetYaxis()->SetRangeUser(ratio->GetMinimum(), 10* ratio->GetMaximum());
//  ratio->GetYaxis()->SetRangeUser(0 ,1.5*ratio->GetMaximum());
  //ratio->GetYaxis()->SetRangeUser(0,10);
  ratio->SetLineColor(1);

  ratio->Draw("hist");

  c->SaveAs("EfficiencyPlot_ttbar_"+var_name+"_"+CaloWPname+"_rebin_"+TruthTypeName+".png");
  
 } 

void makeEfficiencyPlot()
{

//Set ATLAS style
// gROOT->LoadMacro("newATLASstyle/atlasstyle-00-03-05/AtlasStyle.C");
// gROOT->LoadMacro("newATLASstyle/atlasstyle-00-03-05/AtlasUtils.C"); 
SetAtlasStyle();
const int NCaloWP = 4;
std::vector<std::string> CaloWPnames = {"WP0", "WP1", "WP2", "WP3"};

std::string TruthTypeName = "prompt";
//std::string TruthTypeName = "fake";

for (int wp = 0; wp < NCaloWP; wp++)
{
  TFile* histR22 = new TFile(("output_ttbar_CaloScore_"+CaloWPnames[wp]+"_rebin_"+TruthTypeName+"_FillAuthor.root").c_str());
  TFile* histR21 = new TFile(("output_ttbar_rel21_rebin_"+TruthTypeName+".root").c_str());

  std::string var_names[4] = {"pT", "eta", "phi", "z0sintheta"};
  std::string Xtitles[4] = {"p_{T} [GeV]", "Muon #eta", "Muon #phi", "z_{0} sin(#theta) [mm]"};

  for(int i =0; i < 3; i++)
  {
    Efficiency_Plot(histR22, histR21, var_names[i], Xtitles[i], CaloWPnames[wp], TruthTypeName);
  }
}



}