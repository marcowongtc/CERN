// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TSpectrum.h"
#include "TF1.h"
#include "TMath.h" // for Power (useless)
#include "TVirtualFFT.h"
#include "TMatrix.h"
#include "TMatrixD.h"

// Standard includes
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "TFile.h"
#include "TString.h"
#include <sys/stat.h> 
#include <stdlib.h>
#include <iostream>
#include <sstream>
//#include "stdafx.h"
#include <string>
#include <math.h>

//#include "Config.h"//file di configurazione 
#include "analysis.h"//file con gli algoritmi di analisi

using namespace std;

int main(int argc, char **argv)
{

//*******************************************    ECCEZIONI ED ERRORI DI SETUP ANALISI    ***********************************************
TString ANALYSIS_TYPE=argv[3];
if(ANALYSIS_TYPE == "")
  { 
    cout<<"MISSING ANALYSIS TYPE"<<endl;
    printHelp();
    return -1;
  }

int analysis_check=0;
if (ANALYSIS_TYPE == "BITS")
{
  analysis_check=1;
  cout<<"BITS analysis"<<endl;
}
if (ANALYSIS_TYPE == "WAVES")
{
  analysis_check=2;
  cout<<"WAVES analysis"<<endl;
}
if( analysis_check == 0 )
{
cout<<"WRONG ANALYSIS TYPE"<<endl;
      printHelp();
      return -1;
}

//*******************************************    variabili per la lettura dei parametri    ***********************************************
Int_t nWord=0;
Int_t nrun=0;
string pip;
int bit;
int tempbit;
float timetemp;
float ch1;
float ch2;
float ch3;
float ch4;
std::vector<int> Word;
std::vector<float> Time;
std::vector<float> Amp1;
std::vector<float> Amp2;
std::vector<float> Amp3;
std::vector<float> Amp4;
std::ifstream InputCARD(argv[1]);//imposto il file da leggere
char filename[1024];
if(!InputCARD) cout << "Error: could not find the InputCARD" << endl;
int bit0tran=0;
int bit1tran=0;
int bit2tran=0;
int bit3tran=0;
int bit4tran=0;
int bit5tran=0;
int bit6tran=0;
int bit7tran=0;
//*************************************************************       creo i tree       ***********************************************
  TFile *OutputFile = new TFile(argv[2],"recreate");
  TTree *OutTree = new TTree("analysis","analysis");

////////////////////////////////////////////////////////////////// Analysis branches///////////////////////////////////////////////////
  OutTree->Branch("nrun",&nrun);
  OutTree->Branch("nWord",&nWord);
  OutTree->Branch("bit",&bit);
  OutTree->Branch("Word",&Word);
  OutTree->Branch("Time",&Time);
  OutTree->Branch("Amp1",&Amp1);
  OutTree->Branch("Amp2",&Amp2);
  OutTree->Branch("Amp3",&Amp3);
  OutTree->Branch("Amp4",&Amp4);

//**************************************************** apro while su i singoli file RAW ************************************** 
while(1)
{ 
    OutTree->SetDirectory(OutputFile);
    InputCARD>>filename;
    if(InputCARD.eof())  break;

nrun++;//incremento il run
cout << "Reading====>>>"<< filename<<"  nrun = "<<nrun<<endl;//per controllare che legga bene il lista.list

//**********************************************   READING PARAMETERS IS OVER  ****************************************************


//************************* Inizializzo tutti i valori per una nuova run e Reset delle variabili che devono essere resettate solo all'inizio del run


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                              ///
///                                              INIZIO ANALISI                                                  ///
///                                                                                                              ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::ifstream Data(filename);
//Resettare qui le variabili che devono essere resettate all'inizio di ogni run
int counter=0;
int counter2=0;
//int bit0tran=0;
int bit0=0;
int bit0temp=0;
//int bit1tran=0;
int bit1=0;
int bit1temp=0;
//int bit2tran=0;
int bit2=0;
int bit2temp=0;
//int bit3tran=0;
int bit3=0;
int bit3temp=0;
//int bit4tran=0;
int bit4=0;
int bit4temp=0;
//int bit5tran=0;
int bit5=0;
int bit5temp=0;
//int bit6tran=0;
int bit6=0;
int bit6temp=0;
//int bit7tran=0;
int bit7=0;
int bit7temp=0;
//nWord=0;
if (analysis_check==1)
{
while(1)
   { 
      if(Data.eof())  break;    
      Data>>pip;
    if(pip == "Time")
      { 
        counter++;
      }
    if(pip == "Time" && counter==2)
      { 
        while(2)
        { 
             Data>>pip;
             if(Data.eof())  break;    
             Data>>bit>>pip;
             if(counter2>12 && bit==0)
             {
              counter2=0;
              Word.clear();
              nWord++;
              for(int i=0;i<19;i++)
                {//Resettare qui le variabili che devono essere resettate all'inizio di ogni parola

                  Word.push_back(bit);
                  Data>>pip;
                  if(Data.eof())  break;    
                  Data>>bit>>pip;
                  if(bit==0)tempbit=1;
                  if(bit==1)tempbit=0;
                  bit=tempbit;
                }
                if(Data.eof())  break; 
              bit7temp=Word.at(3);
              bit6temp=Word.at(4);
              bit5temp=Word.at(5);
              bit4temp=Word.at(6);
              bit3temp=Word.at(7);
              bit2temp=Word.at(8);
              bit1temp=Word.at(9);
              bit0temp=Word.at(10);
              if(bit7!=bit7temp){bit7tran++;bit7=bit7temp;}
              if(bit6!=bit6temp){bit6tran++;bit6=bit6temp;}
              if(bit5!=bit5temp){bit5tran++;bit5=bit5temp;}
              if(bit4!=bit4temp){bit4tran++;bit4=bit4temp;}
              if(bit3!=bit3temp){bit3tran++;bit3=bit3temp;}
              if(bit2!=bit2temp){bit2tran++;bit2=bit2temp;}
              if(bit1!=bit1temp){bit1tran++;bit1=bit1temp;}
              if(bit0!=bit0temp){bit0tran++;bit0=bit0temp;}

              OutTree->Fill();
                 
              cout<<"nword  "<<nWord<<" = "<<Word.at(0)<<Word.at(1)<<Word.at(2)<<Word.at(3)<<Word.at(4)<<Word.at(5)<<Word.at(6)<<Word.at(7)<<Word.at(8)<<Word.at(9)<<Word.at(10)<<Word.at(11)<<Word.at(12)<<Word.at(13)<<Word.at(14)<<Word.at(15)<<Word.at(16)<<Word.at(17)<<Word.at(18)<<endl;
             }
          if(bit==1) counter2++;

        }
      } 

   }//finisco il loop su tutti gli eventi di un file
}
if (analysis_check==2)
{
  while(1)
   { 
      if(Data.eof())  break;    
      Data>>pip;
    if(pip == "MATH1")
      { 
        counter++;
      }
    if(pip == "MATH1" && counter==2)
      { 
        while(2)
        { 
          Data>>timetemp>>ch1>>ch2>>ch3>>ch4>>pip>>pip;
          Time.push_back(timetemp);
          Amp1.push_back(ch1);
          Amp2.push_back(ch2);
          Amp3.push_back(ch3);
          Amp4.push_back(ch4);
          if(Data.eof())  break;    
        }
        OutTree->Fill();      
      }
    } 

}//finisco il loop su tutti gli eventi di un file
float amptemp;
if (analysis_check==2)
{
  timetemp=Time.at(0);
  amptemp=0;
  for (int i = 0; i < Time.size(); i++)
  {

    if (Amp1.at(i)>Vth)
    {
      
    }
    
  }
  
  
}

//***********************************************************************************************************************************************

}//finisco la lettura della lista
if (analysis_check==1)
{
   cout<< bit7tran<<" = transizioni bit7"<<endl;
 cout<< bit6tran<<" = transizioni bit6"<<endl;
 cout<< bit5tran<<" = transizioni bit5"<<endl;
 cout<< bit4tran<<" = transizioni bit4"<<endl;
 cout<< bit3tran<<" = transizioni bit3"<<endl;
 cout<< bit2tran<<" = transizioni bit2"<<endl;
 cout<< bit1tran<<" = transizioni bit1"<<endl;
 cout<< bit0tran<<" = transizioni bit0"<<endl;
}



//********************************************************************      FINE DELL'ANALISI       ***********************************************************
  OutputFile->cd();
  OutTree->Write();
  OutputFile->Close();
  return(EXIT_SUCCESS);
}
