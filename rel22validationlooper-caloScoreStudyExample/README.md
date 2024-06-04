# Introduction

This code is a simple "looper" to loop over muons in input ntuples and create histograms of pT, eta, phi, ... for muons
passing the different selection working points.

The code has primarily been used for rel. 22 validation, and is set up to process a "test file" and a "reference file",
creating plots comparing the two samples. The original use for rel. 22 vs. rel. 21 comparisons is reflected in some
file names etc., but in this branch we demonstrate how to use it for a comparison of CaloMuonIDTag (the old
calorimeter-tagging algorithm) vs. CaloMuonScore (a new NN-based tagging developed for rel. 22).

# Producing ntuples

The ntuples are produced using a modified version of the MuonSelectorToolsTester found 
[here](https://gitlab.cern.ch/mkbugge/athena/-/blob/master-rel22validationTest/PhysicsAnalysis/MuonID/MuonSelectorTools/util/MuonSelectorToolsTester.cxx).
The following commands can be used to get this up and running (although other methods based on e.g. git atlas with a sparse checkout
could also be used). In a fresh directory, do:

```
asetup AnalysisBase,22.2.53
git clone https://:@gitlab.cern.ch:8443/mkbugge/athena.git
cd athena/
git checkout master-rel22validationTest
mkdir ../build
cd ../build/
cmake -DATLAS_PACKAGE_FILTER_FILE=../athena/package_filter_MuonSelectorTools.txt ../athena/Projects/WorkDir/
source x86_64*/setup.sh
make
MuonSelectorToolsTester mc20_13TeV.361107.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zmumu.deriv.DAOD_PHYS.e3601_s3681_r13167_r13146_p4856/DAOD_PHYS.27450451._000015.pool.root.1
```

Here we ran over just one file from the dataset mc20_13TeV.361107.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zmumu.deriv.DAOD_PHYS.e3601_s3681_r13167_r13146_p4856.
The code is set up to run over multiple files by specifying a directory as input argument instead of a file. Processing of large samples
can be a bit cumbersome, as the code is not currently set up to run on the grid.

After the successful execution of MuonSelectorToolsTester, there will be an output ntuple in the working directory, "output.root".
Running the code out of the box like this, the Loose working point will include calorimeter-tagged muons passing the CaloMuonIDTag
identification. In order to compare between CaloMuonIDTag and CaloMuonScore, we can rename the output file to something useful,
like "output_caloMuonIDTag.root". Then we can make a corresponding file using a preliminary CaloMuonScore working point. This
is achieved by commenting in [this line](https://gitlab.cern.ch/mkbugge/athena/-/blob/master-rel22validationTest/PhysicsAnalysis/MuonID/MuonSelectorTools/util/MuonSelectorToolsTester.cxx#L105)
and then compiling and running again. The resulting output file can be renamed e.g. "output_caloScore.root".

The option "UseCaloScore" is not available in any release as of now (Dec. 16th 2021), but is available in the above discussed branch
(and also [here](https://gitlab.cern.ch/mkbugge/athena/-/tree/master-addCaloScoreWP)). The exact working point in place there
has been developed by Ricardo Woelker, and is meant to give a flat rejection of fake muons as function of pT. This is achieved
with a pT-dependent cut on the Neural Network discriminant, specified [here](https://gitlab.cern.ch/mkbugge/athena/-/blob/master-rel22validationTest/PhysicsAnalysis/MuonID/MuonSelectorTools/Root/MuonSelectionTool.cxx#L1377).

# Producing ntuples on the grid

Running the ntuple production on the grid can be achieved with the following commands in a fresh directory:
```
lsetup git
git atlas init-workdir https://:@gitlab.cern.ch:8443/mkbugge/athena.git
cd athena
git atlas addpkg MuonSelectorTools
git atlas addpkg MuonAnalysisInterfaces
git checkout master-rel22validationTest
rm -rf .git/
cd ..
asetup AnalysisBase,22.2.53
lsetup panda
prun --athenaTag "AnalysisBase,22.2.53" --bexec athena/PhysicsAnalysis/MuonID/MuonSelectorTools/scripts/build.sh --exec "athena/PhysicsAnalysis/MuonID/MuonSelectorTools/scripts/run.sh %IN"  --outputs "output.root" --outDS user.<USERNAME>.<NTUPLENAME> --inDS mc20_13TeV.361107.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zmumu.deriv.DAOD_PHYS.e3601_s3681_r13167_r13146_p5057
```
Note the use of git atlas to do a sparse checkout and the removal of the .git/ subfolder, both of which are measures to minimize the
working directory size, since the working directory is zipped and sent to the grid. In the output dataset name (--outDS), replace
<USERNAME> with your grid user name. The <NTUPLENAME> is a free-format dataset name for your own bookkeeping.
## Downloading the ntuples from the grid
```
rucio get user.<USERNAME>.<NTUPLENAME>_output.root/
hadd <MERGED_NTUPLE_NAME>.root user.<USERNAME>.<NTUPLENAME>_output.root/user.<USERNAME>.*  # Merging the output files into a single root file
```

# Running over the ntuples

The processing of the ntuples produced following the above description is done using the code in this repository. 

```
git clone https://gitlab.cern.ch/ptong/rel22validationlooper.git
cd rel22validationlooper/
git checkout caloScoreStudyExample
source setup.sh
source runLooper.sh # Run over the ntuples. Make sure that you already modified the file names in the corresponding "runLooper*.C" scripts
```
# Comparison between 4 WPs of CaloMuonScore using muon variable distributions

```
mkdir plots_4WPs_prompt 
```
Change the name as you like for your bookkeeping, and use the same name of the output path [here](https://gitlab.cern.ch/ptong/rel22validationlooper/-/blob/caloScoreStudyExample/CaloScoreWPPlot.cpp#L170)
```
source plot.sh
```

You may notice the text output of the above command, giving some working point selection results in the same style
as MuonSelectorToolsTester itself. Running out of the box, you will notice that the code selects only
[prompt muons](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L374)
of type [calorimeter-tagged](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L383)
in [|eta| < 0.1](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L356).
The region |eta| < 0.1 corresponds to the "crack region" with poor MS coverage, and is where calorimeter-tagged muons
are used for the Loose working point.

The "acceptance" or "efficiency", defined
in this context as the number of muons passing the working point divided by the total number of muons can be displayed
by running

```
root -l -b -q makeEfficiencyPlot.cpp
```

Note that this is not the actual efficiency wrt. truth particles, only wrt. the total number of reconstructed muons.
It can still be useful e.g. for comparisons between CaloMuonScore and CaloMuonTag, as the denominator will be the same
in both cases.

To make the same plots for fake muons instead of prompt, you can reverse the logic 
[here](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L374) to select
everything except prompt muons ("IsoMuon"), or use something like [this](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L371)
to reject both "IsoMuon" and "NonIsoMuon", and hence define the "fakes" as "Hadron", "Background", and "Other".
To get the correct label on the efficiency plot, you have to edit [here](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/makeEfficiencyPlot.cpp#L56).

## Retuning the CaloScore cut at the ntuple level

The cut on the CaloMuonScore can be modified at the ntuple level. Instead of using the Loose working point, one would then
use the "working point" called ["RebuiltCaloScore"](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L45),
which uses a cut on the CaloMuonScore variable as defined [here](https://gitlab.cern.ch/mkbugge/rel22validationlooper/-/blob/caloScoreStudyExample/muonNTlooper.C#L442).
In the current implementation, this is exactly the same cut definition as used in the relevant version of MuonSelectionTool, and the results
of "RebuiltCaloScore" should therefore be identical to Loose within the considered eta region.
