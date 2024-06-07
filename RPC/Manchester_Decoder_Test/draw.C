#include <iostream>
#include <string>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TLine.h>
#include <TPDF.h>
using namespace std;

int draw(float start_ns=-1000, float end_ns=1000, string channel="CH4", int event=0) {

    vector<float> draw_red_vertical = {

    };
    
    vector<float> draw_green_vertical = {

    };

    // ====================================================================== //
    TFile* file = TFile::Open("merged_data.root");
    TTree* tree = (TTree*)file->Get("tree");
    
    vector<float>* clocktime = nullptr;
    tree->SetBranchAddress("time", &clocktime);
    
    vector<float>* ch = nullptr;
    tree->SetBranchAddress(channel.c_str(), &ch);
    
    TGraph *g = new TGraph();
    
    //for (int i = 0; i < 1; ++i) {
       
        tree->GetEntry(event);
        for (size_t k = 0; k < clocktime->size(); k++) {
            
            if (clocktime->at(k) < start_ns*1e-9 || clocktime->at(k) > end_ns*1e-9) continue;
            g->SetPoint(g->GetN(), clocktime->at(k), ch->at(k));
        }
    //}
    
    file->Close();
    g->SetMarkerStyle(21);
    g->SetMarkerSize(0.5);
    TCanvas *canvas = new TCanvas("c", "c", 1500, 500);
    //g->Draw("AL");
    g->Draw();
    
      
    for (auto& x:draw_red_vertical) {
        x = x*1e-9;
    }
    for (auto& x:draw_green_vertical) {
        x = x*1e-9;
    }
    

    for (auto x : draw_red_vertical) {    
        TLine* line = new TLine(x, g->GetYaxis()->GetXmin(), x, g->GetYaxis()->GetXmax());
        line->SetLineColor(kRed);
        line->Draw("same");
    }
    
    for (auto x : draw_green_vertical) {    
        TLine* line = new TLine(x, g->GetYaxis()->GetXmin(), x, g->GetYaxis()->GetXmax());
        line->SetLineColor(kGreen);
        line->Draw("same");
    }

    
    return 0;
}