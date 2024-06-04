#!/usr/bin/python

### This script is to analyze the output ROOT data files. The following actions are provided

### "plot_ALL_output_distributions": simply plot all the distributions in the decode output without any treatments
### "plot_output_distributions": plot the leading edge, trailing edge and the signal width respectively, discarding bad events
### "bit_analysis": plot the mean and sigma for each bit (leading0-trailing7) after the decoding
### "stack_timeWidth_signalWidth": Stack the histograms of time width and the signal widths
### "stack_leadingns_resetDiscSignalTimeDifference": Stack the histograms of leading_ns and reset_discSignal_time_difference

#============================= Settings ==========================================

decode_output_file = "decode_output.root"

actions = [
           "plot_ALL_output_distributions",
           "plot_output_distributions",
           "bit_analysis",
           "stack_timeWidth_signalWidth",
           "stack_leadingns_resetDiscSignalTimeDifference",
           ]


#==================================================================================

import ROOT
import numpy as np
import ctypes
import math
ROOT.PyConfig.DisableRootLogon = True 

if "plot_ALL_output_distributions" in actions:
    
    output_pdf_name = decode_output_file[:-5]+"_all_distributions.pdf"
    
    c1 = ROOT.TCanvas("c1", "c1", 700,500)
    c1.Print(output_pdf_name + "[")
    
    rootfile = ROOT.TFile.Open(decode_output_file, "READ")
    tree = rootfile.Get("tree")
    
    for branch in tree.GetListOfBranches():
        hist = ROOT.TH1F(branch.GetName()+"_plot", branch.GetName(), 100, 0, 0)
        tree.Draw(branch.GetName() + ">>" + branch.GetName()+"_plot")
        hist.SetMinimum(0)
        c1.cd()
        c1.Clear()
        hist.Draw()
        c1.Print(output_pdf_name)

    c1.Print(output_pdf_name + "]")

if "plot_output_distributions" in actions:
    
    output_pdf_name = decode_output_file[:-5]+"_leading_trailing_width_distributions.pdf"
    
    c1 = ROOT.TCanvas("plot_output_distributions", "plot_output_distributions", 800,600)
    c1.Print(output_pdf_name + "[")
    
    rootfile = ROOT.TFile.Open(decode_output_file, "READ")
    tree = rootfile.Get("tree")
    
    leading_ns_branch = "leading_ns"
    trailing_ns_branch = "trailing_ns"
    time_width_branch = "time_width"
    need_investigation_branch = "need_investigation"
    outside_range_branch = "outside_range"
    
    disc_signal_leading_ns_branch = "disc_signal_leading_ns"
    disc_signal_trailing_ns_branch = "disc_signal_trailing_ns"
    disc_signal_time_width_branch = "disc_signal_time_width"
    injected_signal_leading_ns_branch = "injected_signal_leading_ns"
    injected_signal_trailing_ns_branch = "injected_signal_trailing_ns"
    injected_signal_time_width_branch = "injected_signal_time_width"
    decoding_signal_match_branch = "decoding_signal_match"
    
    leading_ns = ROOT.vector("float")()
    trailing_ns = ROOT.vector("float")()
    time_width = ROOT.vector("float")()
    need_investigation = ROOT.vector("int")()
    outside_range = ROOT.vector("int")()
    disc_signal_leading_ns = ROOT.vector("float")()
    disc_signal_trailing_ns = ROOT.vector("float")()
    disc_signal_time_width = ROOT.vector("float")()
    injected_signal_leading_ns = ROOT.vector("float")()
    injected_signal_trailing_ns = ROOT.vector("float")()
    injected_signal_time_width = ROOT.vector("float")()
    tree.SetBranchAddress(leading_ns_branch, leading_ns)
    tree.SetBranchAddress(trailing_ns_branch, trailing_ns)
    tree.SetBranchAddress(time_width_branch, time_width)
    tree.SetBranchAddress(need_investigation_branch, need_investigation)
    tree.SetBranchAddress(outside_range_branch, outside_range)
    tree.SetBranchAddress(disc_signal_leading_ns_branch, disc_signal_leading_ns)
    tree.SetBranchAddress(disc_signal_trailing_ns_branch, disc_signal_trailing_ns)
    tree.SetBranchAddress(disc_signal_time_width_branch, disc_signal_time_width)
    tree.SetBranchAddress(injected_signal_leading_ns_branch, injected_signal_leading_ns)
    tree.SetBranchAddress(injected_signal_trailing_ns_branch, injected_signal_trailing_ns)
    tree.SetBranchAddress(injected_signal_time_width_branch, injected_signal_time_width)
    
    hist_leading_ns = ROOT.TH1D("leading_ns", "leading_ns;leading_ns;Events", 15000, 0, 0)
    hist_trailing_ns = ROOT.TH1D("trailing_ns", "trailing_ns;trailing_ns;Events",  15000, 0, 0)
    hist_time_width = ROOT.TH1D("time_width", "time_width;time_width;Events",  15000, 0, 0)
    hist_disc_signal_leading_ns = ROOT.TH1D("disc_signal_leading_ns", "disc_signal_leading_ns;disc_signal_leading_ns;Events", 150, 0, 0)
    hist_disc_signal_trailing_ns = ROOT.TH1D("disc_signal_trailing_ns", "disc_signal_trailing_ns;disc_signal_trailing_ns;Events",  100, 0, 0)
    hist_disc_signal_time_width = ROOT.TH1D("disc_signal_time_width", "disc_signal_time_width;disc_signal_time_width;Events",  15000, 0, 0)
    hist_injected_signal_leading_ns = ROOT.TH1D("injected_signal_leading_ns", "injected_signal_leading_ns;injected_signal_leading_ns;Events", 150, 0, 0)
    hist_injected_signal_trailing_ns = ROOT.TH1D("injected_signal_trailing_ns", "injected_signal_trailing_ns;injected_signal_trailing_ns;Events",  100, 0, 0)
    hist_injected_signal_time_width = ROOT.TH1D("injected_signal_time_width", "injected_signal_time_width;injected_signal_time_width;Events",  15000, 0, 0)

    
    for treenum in range(tree.GetEntries()):
        tree.GetEntry(treenum)
        
        for i in range(need_investigation.size()):
            if need_investigation[i] == 0 and outside_range[i] == 0 and getattr(tree, decoding_signal_match_branch) == 1:
                hist_leading_ns.Fill(leading_ns[i])
                hist_trailing_ns.Fill(trailing_ns[i])
                hist_time_width.Fill(time_width[i])
                
        for i in range(disc_signal_leading_ns.size()):
            if getattr(tree, decoding_signal_match_branch) == 1:
                hist_disc_signal_leading_ns.Fill(disc_signal_leading_ns[i])
                hist_disc_signal_trailing_ns.Fill(disc_signal_trailing_ns[i])
                hist_disc_signal_time_width.Fill(disc_signal_time_width[i])
                hist_injected_signal_leading_ns.Fill(injected_signal_leading_ns[i])
                hist_injected_signal_trailing_ns.Fill(injected_signal_trailing_ns[i])
                hist_injected_signal_time_width.Fill(injected_signal_time_width[i])

    hist_leading_ns.SetLineColor(ROOT.kRed)
    hist_trailing_ns.SetLineColor(ROOT.kRed)
    hist_time_width.SetLineColor(ROOT.kRed)
    hist_disc_signal_leading_ns.SetLineColor(ROOT.kRed)
    hist_disc_signal_trailing_ns.SetLineColor(ROOT.kRed)
    hist_disc_signal_time_width.SetLineColor(ROOT.kRed)
    hist_injected_signal_leading_ns.SetLineColor(ROOT.kRed)
    hist_injected_signal_trailing_ns.SetLineColor(ROOT.kRed)
    hist_injected_signal_time_width.SetLineColor(ROOT.kRed)
    
    c1.cd()
    c1.Clear()
    hist_leading_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_trailing_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_time_width.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_disc_signal_leading_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_disc_signal_trailing_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_disc_signal_time_width.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_injected_signal_leading_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_injected_signal_trailing_ns.Draw()
    c1.Print(output_pdf_name)
    c1.cd()
    c1.Clear()
    hist_injected_signal_time_width.Draw()
    c1.Print(output_pdf_name)
    c1.Print(output_pdf_name + "]")

if "bit_analysis" in actions:
    
    edge_names = ["leading", "trailing"]
    
    output_pdf_name = decode_output_file[:-5]+"_bit_analysis.pdf"
    c1 = ROOT.TCanvas("bit_analysis", "bit_analysis", 800,600)
    c1.Print(output_pdf_name + "[")

    for edge_name in edge_names:
        rootfile = ROOT.TFile.Open(decode_output_file, "READ")
        tree = rootfile.Get("tree")
        
        edge = []
        for i in range(8):
            edge.append(ROOT.vector("int")())
            tree.SetBranchAddress((edge_name+"{}").format(i), edge[i])
        need_investigation = ROOT.vector("int")()
        outside_range = ROOT.vector("int")()
        tree.SetBranchAddress("need_investigation", need_investigation)
        tree.SetBranchAddress("outside_range", outside_range)
        
        hist_bit_mean = ROOT.TH1D(edge_name+"_bit_mean", edge_name+"_bit_mean;Bit;mean", 8, 0, 8)
        hist_bit_mean.SetStats(0)
        hist_bit_mean.SetOption("E");
        hist_bit_mean.SetMinimum(-0.05)
        hist_bit_mean.SetMaximum(1.05)
        hist_bit_mean.SetMarkerStyle(8)
        hist_bit_sigma = ROOT.TH1D(edge_name+"_bit_sigma", edge_name+"_bit_sigma;Bit;sigma", 8, 0, 8)
        hist_bit_sigma.SetStats(0)
        hist_bit_sigma.SetOption("EH");
        hist_bit_sigma.SetMinimum(-0.05)
        hist_bit_sigma.SetMaximum(1.05)
        hist_bit_sigma.SetMarkerStyle(8)
        
        number_of_events = 0
        
        for i in range(8):
            mean = 0
            sigma = 0
            valid_count = 0
            
            for treenum in range(tree.GetEntries()):
                
                tree.GetEntry(treenum)
                for j in range(len(edge[i])):
                    if need_investigation[j] == 1 or outside_range[j] == 1 or getattr(tree, decoding_signal_match_branch) == 0:
                        continue
                    mean = (mean*valid_count + edge[i][j])/float(valid_count+1)
                    sigma = (sigma*valid_count + edge[i][j]*edge[i][j])/float(valid_count+1)
                    valid_count += 1

            number_of_events = valid_count
            sigma = math.sqrt(sigma-mean*mean)
            if mean == 0:
                mean = 0.00001
            hist_bit_mean.SetBinContent(i+1, mean)
            hist_bit_mean.SetBinError(i+1, sigma/math.sqrt(valid_count))
            hist_bit_sigma.SetBinContent(i+1, sigma)
        
        c1.cd()
        c1.Clear()
        hist_bit_mean.Draw()
        c1.Print(output_pdf_name)
        c1.cd()
        c1.Clear()
        hist_bit_sigma.Draw("PL")
        c1.Print(output_pdf_name)
    
    c1.Print(output_pdf_name + "]")
    
    rootfile.Close()
        
if "stack_timeWidth_signalWidth" in actions:
    
    output_pdf_name = decode_output_file[:-5]+"_time_width_check.pdf"
    
    c1 = ROOT.TCanvas("stack_timeWidth_signalWidth", "stack_timeWidth_signalWidth", 800,600)
    c1.Print(output_pdf_name + "[")
    
    rootfile = ROOT.TFile.Open(decode_output_file, "READ")
    tree = rootfile.Get("tree")
    
    time_width_branch = "time_width"
    need_investigation_branch = "need_investigation"
    outside_range_branch = "outside_range"
    disc_signal_time_width_branch = "disc_signal_time_width"
    injected_signal_time_width_branch = "injected_signal_time_width"
    decoding_signal_match_branch = "decoding_signal_match"
    
    time_width = ROOT.vector("float")()
    need_investigation = ROOT.vector("int")()
    outside_range = ROOT.vector("int")()
    disc_signal_time_width = ROOT.vector("float")()
    injected_signal_time_width = ROOT.vector("float")()
    tree.SetBranchAddress(time_width_branch, time_width)
    tree.SetBranchAddress(need_investigation_branch, need_investigation)
    tree.SetBranchAddress(outside_range_branch, outside_range)
    tree.SetBranchAddress(disc_signal_time_width_branch, disc_signal_time_width)
    tree.SetBranchAddress(injected_signal_time_width_branch, injected_signal_time_width)
    
    hist_time_width = ROOT.TH1D("time_width", "time width vs signal width;ns;Events", 15000, 0, 0)
    hist_disc_signal_time_width = ROOT.TH1D("disc_signal_time_width", "time width vs signal width;ns;Events",  15000, 0, 0)
    hist_injected_signal_time_width = ROOT.TH1D("injected_signal_time_width", "time width vs signal width;ns;Events",  15000, 0, 0)

    for treenum in range(tree.GetEntries()):
    
        tree.GetEntry(treenum)

        for i in range(need_investigation.size()):
            if need_investigation[i] == 0 and outside_range[i] == 0:
                hist_time_width.Fill(time_width[i])
        for i in range(disc_signal_time_width.size()):
            if getattr(tree, decoding_signal_match_branch) == 1 and disc_signal_time_width[i] > 0:
                hist_disc_signal_time_width.Fill(disc_signal_time_width[i])
        for i in range(injected_signal_time_width.size()):
            if getattr(tree, decoding_signal_match_branch) == 1 and injected_signal_time_width[i] > 0:
                hist_injected_signal_time_width.Fill(injected_signal_time_width[i])
                
    hist_time_width.SetLineColor(ROOT.kBlue)
    hist_time_width.SetStats(0)
    hist_disc_signal_time_width.SetLineColor(ROOT.kRed)
    hist_disc_signal_time_width.SetStats(0)
    hist_injected_signal_time_width.SetLineColor(ROOT.kGreen)
    hist_injected_signal_time_width.SetStats(0)

    
    leg = ROOT.TLegend(0.85, 0.55, 0.99, 0.69)
    leg.AddEntry(hist_time_width, "time_width", "l")
    leg.AddEntry(hist_disc_signal_time_width, "disc_signal_time_width", "l")
    leg.AddEntry(hist_injected_signal_time_width, "injected_signal_time_width", "l")
    
    height_maximum = max(hist_time_width.GetMaximum(), hist_disc_signal_time_width.GetMaximum(), hist_injected_signal_time_width.GetMaximum()) * 1.08
    hist_time_width.SetMaximum(height_maximum)
    
    c1.cd()
    c1.Clear()
    hist_time_width.Draw("HIST")
    hist_disc_signal_time_width.Draw("HIST SAME")
    hist_injected_signal_time_width.Draw("HIST SAME")
    leg.Draw("SAME")
    c1.Print(output_pdf_name)
    c1.Print(output_pdf_name + "]")

if "stack_leadingns_resetDiscSignalTimeDifference" in actions:
    
    output_pdf_name = decode_output_file[:-5]+"_leading_edge_check.pdf"
    
    c1 = ROOT.TCanvas("stack_leadingns_resetDiscSignalTimeDifference", "stack_leadingns_resetDiscSignalTimeDifference", 800,600)
    c1.Print(output_pdf_name + "[")
    
    rootfile = ROOT.TFile.Open(decode_output_file, "READ")
    tree = rootfile.Get("tree")
    
    leading_ns_branch = "leading_ns"
    need_investigation_branch = "need_investigation"
    outside_range_branch = "outside_range"
    reset_discSignal_time_difference_branch = "reset_discSignal_time_difference"
    decoding_signal_match_branch = "decoding_signal_match"
    
    leading_ns = ROOT.vector("float")()
    need_investigation = ROOT.vector("int")()
    outside_range = ROOT.vector("int")()
    reset_discSignal_time_difference = ROOT.vector("float")()
    tree.SetBranchAddress(leading_ns_branch, leading_ns)
    tree.SetBranchAddress(need_investigation_branch, need_investigation)
    tree.SetBranchAddress(outside_range_branch, outside_range)
    tree.SetBranchAddress(reset_discSignal_time_difference_branch, reset_discSignal_time_difference)
    
    hist_leading_ns = ROOT.TH1D("leading_ns", "leading_ns vs reset_signal_time_diff;ns;Events", 150, 0, 0)
    hist_reset_discSignal_time_difference = ROOT.TH1D("reset_discSignal_time_difference", "leading_ns vs reset_signal_time_diff;ns;Events",  150, 0, 0)

    for treenum in range(tree.GetEntries()):
    
        tree.GetEntry(treenum)

        for i in range(need_investigation.size()):
            if need_investigation[i] == 0 and outside_range[i] == 0 and getattr(tree, decoding_signal_match_branch) == 1:
                hist_leading_ns.Fill(leading_ns[i])
        
        for i in range(reset_discSignal_time_difference.size()):
            
            if getattr(tree, decoding_signal_match_branch) == 1 and reset_discSignal_time_difference[i] > 0:
                hist_reset_discSignal_time_difference.Fill(reset_discSignal_time_difference[i])

    hist_leading_ns.SetLineColor(ROOT.kBlue)
    hist_leading_ns.SetStats(0)
    hist_reset_discSignal_time_difference.SetLineColor(ROOT.kRed)
    hist_reset_discSignal_time_difference.SetStats(0)

    
    leg = ROOT.TLegend(0.85, 0.55, 0.99, 0.69)
    leg.AddEntry(hist_leading_ns, "leading_ns", "l")
    leg.AddEntry(hist_reset_discSignal_time_difference, "reset_signal_time_diff", "l")
    
    height_maximum = max(hist_leading_ns.GetMaximum(), hist_reset_discSignal_time_difference.GetMaximum()) * 1.08
    hist_leading_ns.SetMaximum(height_maximum)
    
    c1.cd()
    c1.Clear()
    hist_leading_ns.Draw("HIST")
    hist_reset_discSignal_time_difference.Draw("HIST SAME")
    leg.Draw("SAME")
    c1.Print(output_pdf_name)
    c1.Print(output_pdf_name + "]")
