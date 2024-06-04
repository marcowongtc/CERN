#include "newATLASstyle/atlasstyle-00-03-05/AtlasStyle.C"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasUtils.C"

void CaloScoreWPPlot(string inputWP0, string inputWP1, string inputWP2, string inputWP3){
    
    SetAtlasStyle();
    
    TFile* inFileWP0 = new TFile("../user.ptong.UseCaloScore.Zmumu_output.root/output_CaloScore.root");
    TH1* nEvtsWP0 = (TH1*)inFileWP0->Get("nEvts");
    TTree* ntWP0 = (TTree*)inFileWP0->Get("muonNT");

    cout <<"----------------------------------------\n";
    cout << "CaloMuonScore Working Point 0 results:\n";
    cout <<"----------------------------------------\n";
    cout << "Total number of events processed: " << nEvtsWP0->GetBinContent(1) << endl;

    TFile* histWP0 = new TFile(inputWP0.c_str());
    TFile* histWP1 = new TFile(inputWP1.c_str());
    TFile* histWP2 = new TFile(inputWP2.c_str());
    TFile* histWP3 = new TFile(inputWP3.c_str());

    
    const int Nwp = 9;
    std::vector<std::string> WPnames = {"Tight", "Medium", "Loose", "VeryLoose", "HighPt", "LowPt", "LowPtMVA", "NoSelection", "RebuiltCaloScore"};
//    std::vector<std::string> WPnames = {"Loose"};
    // std::vector<std::string> WPnames = {"NoSelection"};

    const int Nvar = 1;
//    std::vector<std::string> varNames = {"pT","eta","phi","phiMod","z0sintheta","qOpSig","author","type","eLoss","caloScore"};//,"siliconHits","d0","d0sig","pixelHits","SCTHits","TRTHits","author","truthType","truthOrigin","redChi2"};
    std::vector<std::string> varNames = {"pT"};

    for (int var = 0; var < Nvar; var++) {
        for (int wp = 0; wp < Nwp; wp++) {

        TH1* hWP0 = (TH1*)histWP0->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP1 = (TH1*)histWP1->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP2 = (TH1*)histWP2->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP3 = (TH1*)histWP3->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());

        // //Scale according to stats
        // hWP1->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP1->GetBinContent(1));
        // hWP2->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP2->GetBinContent(1));
        // hWP3->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP3->GetBinContent(1));

        TCanvas* canv = new TCanvas(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str(),("muon_"+varNames[var]+"_"+WPnames[wp]).c_str(),1000,850);

        TPad *npad = new TPad("npad", "", 0.0,0.3, 1, 1.0);
        //npad->SetLogy();
        //npad->SetLogx();
        npad->Draw();
        npad->cd();

        TLegend* leg = new TLegend(0.646293,0.740741,0.955912,0.892416);
        leg->AddEntry(hWP0,"CaloMuonScoreWP1","L");
        leg->AddEntry(hWP1,"CaloMuonScoreWP2","L");
        leg->AddEntry(hWP2,"CaloMuonScoreWP3","L");
        leg->AddEntry(hWP3,"CaloMuonScoreWP4","L");

        leg->SetFillColor(0);
        leg->SetFillStyle(0);
        leg->SetShadowColor(0);
        leg->SetBorderSize(0);
        
        hWP0->SetLineColor(kRed);
        hWP1->SetLineColor(kBlue);
        hWP2->SetLineColor(kOrange);
        hWP3->SetLineColor(kBlack);

        if (var == 0)
        //hWP0->SetXTitle("CaloMuonScore"); //hWP0->SetXTitle("Number of TRT hits");
        hWP0->SetXTitle("Muon p_{T} [GeV]");
        if (var == 1)
        hWP0->SetXTitle("Muon #eta");
        if (var == 2)
        hWP0->SetXTitle("Muon #phi");
        if (var == 3)
        hWP0->SetXTitle("Muon #phi % #pi/4");
        if (var == 4)
        hWP0->SetXTitle("z_{0} sin(#theta) [mm]"); //Number of silicon hits");
        if (var == 5)
        hWP0->SetXTitle("q/p significance"); //Muon d_{0} [mm]");
        if (var == 6)
        hWP0->SetXTitle("Muon author"); //Muon d_{0} significance");
        if (var == 7)
        hWP0->SetXTitle("Muon type"); //"Number of pixel hits");
        if (var == 8)
        hWP0->SetXTitle("Energy loss [GeV]"); //"Number of SCT hits");
        if (var == 9)
        hWP0->SetXTitle("CaloMuonScore"); //hWP0->SetXTitle("Number of TRT hits");
        if (var == 10)
        hWP0->SetXTitle("Muon author");
        if (var == 11)
        hWP0->SetXTitle("Muon truth type");
        if (var == 12)
        hWP0->SetXTitle("Muon truth origin");
        if (var == 13)
        hWP0->SetXTitle("#chi^{2} / N_{dof}");


        //hWP0->SetYTitle("Truth-matched muons");
        //hWP0->SetYTitle("Prompt-matched muons");
        //hWP0->SetYTitle("Non-matched muons");
        //hWP0->SetYTitle("All muons");

        if (hWP0->GetMaximum() > hWP1->GetMaximum())
        hWP0->GetYaxis()->SetRangeUser(0,hWP0->GetMaximum()*1.5);
        else
        hWP0->GetYaxis()->SetRangeUser(0,hWP1->GetMaximum()*1.5);

        hWP0->Draw("hist");
        hWP1->Draw("hist same");
        hWP2->Draw("hist same");
        hWP3->Draw("hist same");
        leg->Draw();

        myText(0.25,0.84,1,WPnames[wp].c_str());

        canv->cd();

        TPad *npad2 = new TPad("npad2", "", 0.00,0.00, 1, 0.3);
        npad2->Draw();
        npad2->cd();
        npad2->SetGridy();
        //npad2->SetLogx();

        TH1* ratio = (TH1*)hWP3->Clone();
        ratio->Divide(hWP2);

        //ratio->SetYTitle("With / without");
        ratio->SetYTitle("WP4 / WP3");
        //ratio->SetYTitle("Rel. 22 / 21");
        ratio->SetXTitle(hWP1->GetXaxis()->GetTitle()); 
        ratio->SetLabelSize(0.11,"XY");
        ratio->SetTitleSize(0.0,"X");
        ratio->SetTitleSize(0.14,"Y");
        ratio->SetTitleOffset(0.45,"Y");
        ratio->GetYaxis()->SetRangeUser(0.6,1.4);
        ratio->SetLineColor(1);

        ratio->Draw("hist");

        cout << WPnames[wp] << ": " << (hWP1->Integral(0,hWP1->GetNbinsX()+1) / hWP0->Integral(0,hWP0->GetNbinsX()+1)) << endl;

        canv->SaveAs(("plots_4WPs_prompt/muon_"+varNames[var]+"_"+WPnames[wp]+".png").c_str());
        }
    }
}