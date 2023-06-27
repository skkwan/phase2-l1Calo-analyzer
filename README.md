p# Phase 2 L1 Calo Emulator and Analyzer

## Description

   This emulator is for the Phase 2 RCT and GCT firmware-based emulators for the standalone barrel EGamma algorithm. It corresponds to PR#1069 in cms-l1t-offline (https://github.com/cms-l1t-offline/cmssw/pull/1069) (to be updated) and PR#41224 in cms-sw/cmssw (https://github.com/cms-sw/cmssw/pull/41224) (merged to CMSSW master branch as of June 16, 2023).


## Setting up 
   1. Set up CMSSW release and check out the emulator branch that was merged into CMSSW.

   ```bash
   % cmsrel https://github.com/cms-sw/cmssw/releases/tag/CMSSW_13_2_X_2023-06-26-1100
   % cd https://github.com/cms-sw/cmssw/releases/tag/CMSSW_13_2_X_2023-06-26-1100/src/
   % cmsenv && git cms-init
   % git cms-addpkg L1Trigger/L1CaloTrigger
   % git cms-addpkg DataFormats/L1TCalorimeterPhase2
   ```

   n.b. In general, if you want someone's changes that are not officially part of the cms-l1t-offline repo, at the step noted above, you would do `git cms-rebase-topic -u username:branchname`. For instance if you do some development and you need to pass it on to someone else, they should run `git cms-rebase-topic -u yourUsername:yourBranchName`.

   2. Set up the analyzer (to make n-tuples, plots, etc.)

   ```bash
   % cmsenv
   % cd ${CMSSW_BASE}/src/L1Trigger/
   % git clone https://github.com/skkwan/phase2-l1Calo-analyzer.git
   % mv phase2-l1Calo-analyzer/ L1CaloPhase2Analyzer/ 
   % scram b -j 8
   ```

   So now in `${CMSSW_BASE}/src/L1Trigger`, there should be two folders `L1CaloTrigger/` (where the actual emulator lives) and `L1CaloPhase2Analyzer/` (where the analyzer to call the emulator and make CMSSW collections and n-tuples lives).

## Running the analyzers interactively (calls both the old and new emulator)

   1. To run the efficiency analyzer, which reads in ECAL and HCAL crystal-level hits, and runs the old emulator and new emulator separately. Each emulator produces a collection of EGamma objects and tower objects. Then the analyzer gets the generator-level particles, and matches the emulators' output EGamma objects with generator-level electrons. The leading matched clusters are saved to an n-tuple.
   ```bash
   % cmsenv
   % cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test
   % voms-proxy-init --voms cms 
   % cmsRun test-singleAnalyzer.py
   ```
   2. To run the rates analyzer, which fills histograms to calculate the rates:
   ```bash
   % cmsenv
   % cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test
   % voms-proxy-init --voms cms 
   % cmsRun test-analyzer-rates.py 
   ```

## To develop additional code

   1. To develop code in the emulators, you need to have your own branch and fork of CMSSW. If you have never forked CMSSW, go to https://github.com/cms-sw/cmssw and click "Fork" in the top right corner.
   For instance, my fork is https://github.com/skkwan/cmssw. 
   Now in the working area (e.g. on lxplus), we need to create a new local branch, and then set up a remote branch on our fork of CMSSW to track it (standard GitHub stuff):
   ```bash
   % cmsenv
   % cd ${CMSSW_BASE}/src/
   % # If you just did the setup steps, running "git branch" should show that you are on the `phase2-l1t-integration-1252patch1` branch
   % git checkout -b devel-testCheckoutInstructions  # or replace "devel-testCheckoutInstructions" with whatever you want it to be named
   % # It should say "Switched to a new branch 'devel-testCheckoutInstructions'".
   % # Next, add your fork of CMSSW as a remote, replacing the URL with whatever your fork is. Can also call it something other than my-cmssw.
   % git remote add my-cmssw git@github.com:skkwan/cmssw.git  
   ```
   Now if you change any code in the emulator (`L1CaloTrigger/` or `DataFormats/` directories for instance), commit and push to your branch
   ```
   git push my-cmssw devel-testCheckoutInstructions
   ```

   2. To develop code in the analyzers, fork this repository on GitHub, add the GitHub repo as a remote, create a branch locally, and whenever you change your code, push to your remote.
   ```bash
   % cmsenv
   % cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/
   % git checkout -b devel-Branchname
   % git remote add my-analyzer [link to your fork on GitHub]
   % git push my-analyzer devel-Branchname
   ```
   

## (Developers only) To identify pathological MinBias events

0. Run CRAB jobs to make MinBias n-tuples for the old and new analyzers, stored on T2.
1. In the `test/compare` directory, change `compare.py` to have the event pre-selection you want (e.g. disagreement in isolation flag).
2. Interactively (maybe in the background) run `runCompareOnT2Dataset.py` to scan these n-tuples for said differences.
3. This should build a file like `eventsWithDiff_gct_is_iso.txt` with a list of run:lumi:events.
4. Generate the CRAB config file:
   ```
   cd ../
   cd pickEvent
   # Edit runPickEventCRAB.sh to point to the right .txt file!
   source runPickEventCRAB.sh
   ```
5. Submit the resulting CRAB config file and wait for skimmed n-tuples to show up in T2.
6. Then run the single analyzer and the event display (housed in a different directory, `/afs/cern.ch/work/s/skkwan/private/phase2RCTDev/eventDisplay`.

## (Developers only) To run the emulator and analyzer in CRAB to make a TTree for distributions, efficiencies, rates, etc.

1. CRAB to the rescue:
   ```
   cmsenv
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test/crab
   cat datasetConfig.yml
   ```

2. Edit `datasetConfig.yml` to point to the correct datasets and template CRAB files. (Edit the template CRAB file to point to your own T2/T3 storage.)
3. Edit `parseYaml.py` so that the line
   ```
   for eraType in ["signal_singleEmulator", "signal_oldEmulator", "signal", "rates", "rates_oldEmulator"]:
   ```
   has all the entries in the `.yml` that we want to process.
4. Then `python parseYaml.py` which should create one CRAB config file per `eraType`.
5. Submit the crab config file as normal, e.g. `crab submit crabJobConfigs/2023crabTest_DoubleElectron_FlatPt-1To100-gun_singleEmulator_Phase2Fall22DRMiniAOD_cfg.py`.
6. On the T2/T3 data storage, once the CRAB jobs have completed, `hadd -f -j -k` the files and continue. 

## (Developers only) To make distributions and efficiencies 

1. To make efficiencies,
   ```
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/figures
   cd efficiencyPlots/
   ``` 
   Check that `runDistribPlots.sh` points to the right input file, and check that `runDistributionPlots.C` points to a valid output directory to write the .pngs and .pdfs.

2. To make distributions,
   ```
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/figures
   cd validationPlots/
   ``` 
   Check that the files point to the right input/output locations, and run `runDistribPlots.sh`.

## (Developers only) To run the rates 

   ```
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/figures
   cd ratesPlots/
   ``` 
   Run `runRatesPlots.sh`.

## (Developers only) To run the unit tests for the digitized collections

In the `devel-digitizedCollections` branch of the CMSSW release, at the top-level, run `scram b runtests`.
