#include "newATLASstyle/atlasstyle-00-03-05/AtlasStyle.C"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasUtils.C"

/*-----Taking the 4 WP file name for ploting as input-----*/
void CaloScoreWPPlot(string inputWP0, string inputWP1, string inputWP2, string inputWP3){
	
	/*-----ATLAS Style is to setup all ATLAS default plot formatting. Detail in https://atlas-plot-utils.readthedocs.io/en/latest/atlas_style.html-----*/    
    SetAtlasStyle();
    
	/*----- Make a Tfile pointer named "inFileWP0" pointing to the "output_CaloScore.root" file ------------------
	  ----- Make a TH1 pointer named "nEvtsWP0" pointing to the "nEvts" TH1 in"output_CaloScore.root" file -------
	  ----- Make a Ttree pointer named "ntWP0" pointing to the "muonNT" Ttree in"output_CaloScore.root" file -----*/
	/*??? Why do we need output_CaloScore.root ???*/
    TFile* inFileWP0 = new TFile("../user.ptong.UseCaloScore.Zmumu_output.root/output_CaloScore.root");
    TH1* nEvtsWP0 = (TH1*)inFileWP0->Get("nEvts");
    TTree* ntWP0 = (TTree*)inFileWP0->Get("muonNT");


	/*----- printing information to the user -------------------------------------------------
	  ----- A misleading discription that said WP is 0 ---------------------------------------
	  ----- Get the first bin in the nEvtsWP0 ------------------------------------------------
	  ----- in TH1 "nEvts" there is only 1 bin which indicate the total number of event. -----
	  ----- Looking at output_CaloScore.root, they are the same value ------------------------*/
    cout <<"----------------------------------------\n";
    cout << "CaloMuonScore Working Point 0 results:\n";
    cout <<"----------------------------------------\n";
    cout << "Total number of events processed: " << nEvtsWP0->GetBinContent(1) << endl;

	/*----- making TFile pointer pointing to the TFile created from the 4 input file name ----- 
	  ----- the .c_str() finction take the string to a array of char --------------------------*/
    TFile* histWP0 = new TFile(inputWP0.c_str());
    TFile* histWP1 = new TFile(inputWP1.c_str());
    TFile* histWP2 = new TFile(inputWP2.c_str());
    TFile* histWP3 = new TFile(inputWP3.c_str());

    /*----- define a string vector call "WPnames" contain all the different Quality ----- 
	  ----- It named as WP but it is actrually quality ----------------------------------*/	
    const int Nwp = 9;
    std::vector<std::string> WPnames = {"Tight", "Medium", "Loose", "VeryLoose", "HighPt", "LowPt", "LowPtMVA", "NoSelection", "RebuiltCaloScore"};
	//std::vector<std::string> WPnames = {"Loose"};
    //std::vector<std::string> WPnames = {"NoSelection"};


    /*----- define a string vector call "varName" contain all the different variable -----*/	
    const int Nvar = 1;
	//std::vector<std::string> varNames = {"pT","eta","phi","phiMod","z0sintheta","qOpSig","author","type","eLoss","caloScore"};//,"siliconHits","d0","d0sig","pixelHits","SCTHits","TRTHits","author","truthType","truthOrigin","redChi2"};
    std::vector<std::string> varNames = {"pT"};


	/*----- Big loop that loop over all combination of WP and variable, everyloop produce a plot of Quality vs variable for 4 WP -----*/
    for (int var = 0; var < Nvar; var++) {
        for (int wp = 0; wp < Nwp; wp++) {

		/*----- Get the name of the corresponding histograme of quality and variable ----- 
		  ----- save the histograme at pointer hWPx from the input root file -------------*/
        TH1* hWP0 = (TH1*)histWP0->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP1 = (TH1*)histWP1->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP2 = (TH1*)histWP2->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());
        TH1* hWP3 = (TH1*)histWP3->Get(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str());

        // //Scale according to stats
        // hWP1->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP1->GetBinContent(1));
        // hWP2->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP2->GetBinContent(1));
        // hWP3->Scale(nEvtsWP0->GetBinContent(1)/nEvtsWP3->GetBinContent(1));
		
		/*----- Creating canvas using the finction TCanvas(name,title,width,height) -----*/
        TCanvas* canv = new TCanvas(("muon_"+varNames[var]+"_"+WPnames[wp]).c_str(),("muon_"+varNames[var]+"_"+WPnames[wp]).c_str(),1000,850);

		/*----- Creating the Pad for ploting using the function TPad(name,title,xlow,ylow,xup,yup)----------------
		  ----- the xylow and xyup is the coordinate in the normalize space space, which is 1by1 unit square -----
		  ----- Then draw out the histograme and go into edit the histograme -------------------------------------*/
        TPad *npad = new TPad("npad", "", 0.0,0.3, 1, 1.0);
        //npad->SetLogy();
        //npad->SetLogx();

        /*----- add the pads to the canvas -----*/ 
        npad->Draw();

        /*----- select npad for the current pad -----*/
        npad->cd();
		
		/*----- Creating a legend call "leg" using the function TLegned(x1,y1,x2,y2) -----
          ----- bottom-left and top-right corners of the legend
		  ----- the four input are the coordinate of the vertex of the legend ------------
		  ----- four legend is added with function AddEntry(histograme,name,lable) -------
		  ----- lable type L drawing style is using line ---------------------------------------------*/
        TLegend* leg = new TLegend(0.646293,0.740741,0.955912,0.892416);
        leg->AddEntry(hWP0,"CaloMuonScoreWP1","L");
        leg->AddEntry(hWP1,"CaloMuonScoreWP2","L");
        leg->AddEntry(hWP2,"CaloMuonScoreWP3","L");
        leg->AddEntry(hWP3,"CaloMuonScoreWP4","L");

		/*----- Setting the style of the histograme -----*/
        leg->SetFillColor(0);
        leg->SetFillStyle(0);
        leg->SetShadowColor(0);
        leg->SetBorderSize(0);
        
		/*----- Setting the color of the histogrames -----*/
        hWP0->SetLineColor(kRed);
        hWP1->SetLineColor(kBlue);
        hWP2->SetLineColor(kOrange);
        hWP3->SetLineColor(kBlack);

		/*----- Setting the title of the x-axis in the histogrames -----*/
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

		/*??? not realy understand what SetRangeUser is doing ???*/
        if (hWP0->GetMaximum() > hWP1->GetMaximum())
        hWP0->GetYaxis()->SetRangeUser(0,hWP0->GetMaximum()*1.5);
        else
        hWP0->GetYaxis()->SetRangeUser(0,hWP1->GetMaximum()*1.5);

		/*----- draw the histograme out ----------------------------------
		  ----- "hist" is to draw without error bar ----------------------
		  ----- "same" is to draw without replacing previous drawing -----*/
        hWP0->Draw("hist");
        hWP1->Draw("hist same");
        hWP2->Draw("hist same");
        hWP3->Draw("hist same");
        leg->Draw();

		/*----- creat a text box writen the quality that are using as the title name -----*/
        myText(0.25,0.84,1,WPnames[wp].c_str());

		/*----- go back to canv from npad ----------
		  ----- to ready to creat another npad -----*/
        canv->cd();

		/*----- creat another npad located below npad 1 -----
		  ----- go to npad2 and setup grid ------------------*/
        TPad *npad2 = new TPad("npad2", "", 0.00,0.00, 1, 0.3);
        npad2->Draw();
        npad2->cd();
        npad2->SetGridy();
        //npad2->SetLogx();

		/*----- creat a histograme that take the ratio between wp3 and wp2 -----*/
		/*??? why is we compare these two ???*/
        TH1* ratio = (TH1*)hWP3->Clone();
        ratio->Divide(hWP2);

		/*----- do all of the seting in the ratio histograme ------------------------------------
		  ----- The getYaxis() function into SetRangeUser() function set the range of yaxis -----
		  ----- In the function SetTitleSize(size,notknow), size is in %of pad size -------------*/
		/*it is better to see the resulting plot for easier understanding for the plot*/

        //ratio->SetYTitle("With / without");
        ratio->SetYTitle("WP4 / WP3");
        //ratio->SetYTitle("Rel. 22 / 21");
        ratio->SetXTitle(hWP1->GetXaxis()->GetTitle()); 
        ratio->SetLabelSize(0.11,"XY"); //both xy label
        ratio->SetTitleSize(0.0,"X"); //x title
        ratio->SetTitleSize(0.14,"Y"); //y title
        ratio->SetTitleOffset(0.45,"Y"); 
        ratio->GetYaxis()->SetRangeUser(0.6,1.4);
        ratio->SetLineColor(1);

		/*----- draw out the ratio histograme-----*/
        ratio->Draw("hist");

		/*----- print out the number of event ratio in WP1 and WP2 form 0 to end -----*/
		/*??? why is these information ???*/
        cout << WPnames[wp] << ": " << (hWP1->Integral(0,hWP1->GetNbinsX()+1) / hWP0->Integral(0,hWP0->GetNbinsX()+1)) << endl;

		/*----- save the plot as png in directory "plots_4WPs_prompt" -----*/
        canv->SaveAs(("plots_4WPs_prompt/muon_"+varNames[var]+"_"+WPnames[wp]+".png").c_str());
        }
    }
}