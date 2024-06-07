///////////////////////////////////////////////////

//CHANNELS MAPS ***********************************************

//CAEN MAP
#define TOP_eta_IN  0       
#define TOP_eta_OUT 15
#define TOP_phi_IN 16
#define TOP_phi_OUT 31

#define MID_eta_IN  32
#define MID_eta_OUT 47
#define MID_phi_IN 48
#define MID_phi_OUT 63

#define BOT_eta_IN  64
#define BOT_eta_OUT 79
#define BOT_phi_IN  80
#define BOT_phi_OUT 95

//BIS7
#define BIS7_TOP_eta_IN  1
#define BIS7_TOP_eta_OUT 32
#define BIS7_TOP_phi_IN  97
#define BIS7_TOP_phi_OUT 128
#define BIS7_TOP_phi_IN_OH  193
#define BIS7_TOP_phi_OUT_OH 224

#define BIS7_MID_eta_IN  33
#define BIS7_MID_eta_OUT 64
#define BIS7_MID_phi_IN  129
#define BIS7_MID_phi_OUT 160
#define BIS7_MID_phi_IN_OH 225
#define BIS7_MID_phi_OUT_OH 256

#define BIS7_BOT_eta_IN  65
#define BIS7_BOT_eta_OUT 96
#define BIS7_BOT_phi_IN  161
#define BIS7_BOT_phi_OUT 192
#define BIS7_BOT_phi_IN_OH  257
#define BIS7_BOT_phi_OUT_OH 288
//BIS8
#define BIS8_TOP_eta_IN  289
#define BIS8_TOP_eta_OUT 320
#define BIS8_TOP_phi_IN  385
#define BIS8_TOP_phi_OUT 416
#define BIS8_TOP_phi_IN_OH 481
#define BIS8_TOP_phi_OUT_OH 512

#define BIS8_MID_eta_IN  321
#define BIS8_MID_eta_OUT 352
#define BIS8_MID_phi_IN  417
#define BIS8_MID_phi_OUT 448
#define BIS8_MID_phi_IN_OH 513
#define BIS8_MID_phi_OUT_OH 544

#define BIS8_BOT_eta_IN  353
#define BIS8_BOT_eta_OUT 384
#define BIS8_BOT_phi_IN  449
#define BIS8_BOT_phi_OUT 480
#define BIS8_BOT_phi_IN_OH 545
#define BIS8_BOT_phi_OUT_OH 576

//****************************************************************
//****************************************************************
#define Signal_Beginning 0
#define Signal_Ending 15000
//****************************************************************
#define Signal_TOP_Beginning 0
#define Signal_TOP_Ending 15000

#define Signal_MID_Beginning 0
#define Signal_MID_Ending 15000

#define Signal_BOT_Beginning 0
#define Signal_BOT_Ending 15000
//****************************************************************
//Parametri delle strip in cm
#define pitcheta 2.5
#define pitchphi 2.5
#define lenghteta 180
#define lenghtphi 110
//****************************************************************
//TDC Time window opened in ns
#define TDCwindow 1500
// distanza di tempo tra due eventi per non considerarli più contigui
#define TimeDistanceCluster 15
#define TimeDistanceTrigger 5
#define ChannelDistance 2
#define WidthReference 30
#define WidthReference2 5
#define Clustersizecut 3


