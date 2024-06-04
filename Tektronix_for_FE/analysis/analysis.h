void printHelp()
{
  std::cout<<"Example usage: AnalysisTDCUSBIS78.exe NAME_OF_LIST.list NAME_ROOT.root OPTION1 OPTION2"<<std::endl;
  std::cout<<"NAME_OF_LIST.list - name of the .list file containing the paths to the data that you want to analyze"<<std::endl;
  std::cout<<"NAME_ROOT.root - name of the .root file which will be produced by the analysis"<<std::endl;
  std::cout<<"OPTION1 - CAEN to analyze data taken with CAEN TDC; TDCUS to analyze data taken with the vertical slice of BIS78"<<std::endl;
  std::cout<<"OPTION2 - ETA or PHI to analyze data taken when a single layer is on; ETAPHI to analyze data taken when both readout layers are ons"<<std::endl;

}
