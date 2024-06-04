#!/bin/bash
lsetup "root 6.20.06-x86_64-centos7-gcc8-opt"

##################################################
###### runLooper_CaloScoreWP.C(ntuple_path, ntuple_name, WP)    (WP = -1 is the WP of CaloMuonIDTag， WP = 0，1，2，3 is the four WPs of CaloMuonScore)
##################################################

# root -l -b -q 'runLooper_CaloScoreWP.C("/eos/home-p/ptong/user.ptong.UseCaloScore.Zmumu_output.root/output_CaloScore.root", "output_CaloScore_WP2_rebin_prompt_FillAuthor.root", 2)'

# root -l -b -q 'runLooper_CaloScoreWP.C("/eos/home-p/ptong/user.ptong.Zmumu_Rel21_output.root/output_caloMuonIDTag.root", "output_rel21_rebin_fake.root", -1)'

# root -l -b -q 'runLooper_CaloScoreWP.C("/eos/home-p/ptong/user.ptong.ttbar_Rel21_output.root/output_ttbar_Rel21.root", "output_ttbar_rel21_rebin_fake.root", -1)'

# root -l -b -q 'runLooper_CaloScoreWP.C("/eos/home-p/ptong/user.ptong.ttbar_Rel21_output.root/output_ttbar_Rel21.root", "output_ttbar_rel21_rebin_prompt.root", -1)'

root -l -b -q 'runLooper_CaloScoreWP.C("/eos/home-p/ptong/user.ptong.UseCaloScore.ttbar_output.root/output_ttbar_CaloScore.root","output_ttbar_CaloScore_WP2_rebin_prompt.root", 2)'

