#!/bin/bash
cd /eos/home-p/ptong/
source setup.sh
cd /eos/home-p/ptong/rel22validationlooper

# root -l -b -q 'CaloScoreWPPlot.cpp("output_ttbar_CaloScore_WP0_rebin_fake_FillAuthor.root","output_ttbar_CaloScore_WP1_rebin_fake_FillAuthor.root","output_ttbar_CaloScore_WP2_rebin_fake_FillAuthor.root","output_ttbar_CaloScore_WP3_rebin_fake_FillAuthor.root")'

# root -l -b -q 'CaloScoreWPPlot.cpp("output_ttbar_CaloScore_WP0_rebin_prompt_FillAuthor.root","output_ttbar_CaloScore_WP1_rebin_prompt_FillAuthor.root","output_ttbar_CaloScore_WP2_rebin_prompt_FillAuthor.root","output_ttbar_CaloScore_WP3_rebin_prompt_FillAuthor.root")'

# root -l -b -q 'CaloScoreWPPlot.cpp("output_ttbar_CaloScore_WP0_rebin_fake_FillAuthor_merged.root","output_ttbar_CaloScore_WP1_rebin_fake_FillAuthor_merged.root","output_ttbar_CaloScore_WP2_rebin_fake_FillAuthor_merged.root","output_ttbar_CaloScore_WP3_rebin_fake_FillAuthor_merged.root")'

# root -l -b -q 'CaloScoreWPPlot.cpp("output_CaloScore_WP0_rebin_fake_FillAuthor.root","output_CaloScore_WP1_rebin_fake_FillAuthor.root","output_CaloScore_WP2_rebin_fake_FillAuthor.root","output_CaloScore_WP3_rebin_fake_FillAuthor.root")'

root -l -b -q 'CaloScoreWPPlot.cpp("output_CaloScore_WP0_rebin_prompt_FillAuthor.root","output_CaloScore_WP1_rebin_prompt_FillAuthor.root","output_CaloScore_WP2_rebin_prompt_FillAuthor.root","output_CaloScore_WP3_rebin_prompt_FillAuthor.root")'
