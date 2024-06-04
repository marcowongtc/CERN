#include "muonNTlooper.C+"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasStyle.C"
#include "newATLASstyle/atlasstyle-00-03-05/AtlasUtils.C"

/*--- runLooper_CaloScoreWP(path of ntupules, output root file name, WP)  --------------------
  --- (WP = -1 is the WP of CaloMuonIDTag， WP = 0，1，2，3 is the four WPs of CaloMuonScore)--
*/
void runLooper_CaloScoreWP(string inputWP1, string outputWP1, int CaloScore_WP) {

/*--- TFile: file type in root 
  --- TH1: 1D Histogram type (scalar)
  --- TTree: Tree type (Chain > Tree > Branch)
  --------------------------------------------
  --- *is the pointer same as C++
  --- ->f(): method funciton , use a pointer to accesss member funciton, pointer -> f()
  --- .f(): normal method function, object.f()
  --- .f: calling OS command code (eg: .q = quit root, .ls = long list) 
*/
  TFile* inFileWP1 = new TFile(inputWP1.c_str());
  TH1* nEvtsWP1 = (TH1*)inFileWP1->Get("nEvts");
  TTree* ntWP1 = (TTree*)inFileWP1->Get("muonNT");

/*Get the total number of Bin (1D histogram)*/
  cout <<"----------------------------------------\n";
  cout << "CaloMuonScore WP0 results:\n";
  cout <<"----------------------------------------\n";
  cout << "Total number of events processed: " << nEvtsWP1->GetBinContent(1) << endl;

/*Refer to MuoNTlooper, Here define the muonNTlooper object with name ntlWP1 with the input of ntWP1
  method of the class muonNTlooper - loop(name you want to output, WP)
*/
  muonNTlooper ntlWP1(ntWP1);
  ntlWP1.Loop(outputWP1.c_str(),CaloScore_WP);

}