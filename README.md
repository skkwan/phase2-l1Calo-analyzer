# Phase 2 L1 Calo Emulator and Analyzer

## Description

   This emulator is for the Phase 2 RCT and GCT firmware-based emulators. It corresponds to PR#1069 in cms-l1t-offline (https://github.com/cms-l1t-offline/cmssw/pull/1069) and PR#41224 in cms-sw/cmssw (https://github.com/cms-sw/cmssw/pull/41224).


## Setup emulator (do only once)
   Set up CMSSW release for the emulators (only do once, and word of warning: because of `cms-merge-topic`, opening PRs to CMSSW may be bad)
   ```
   cmsrel CMSSW_12_5_2_patch1
   cd CMSSW_12_5_2_patch1/src
   cmsenv
   git cms-init
   git cms-merge-topic -u cms-l1t-offline:l1t-phase2-v56
   scram b -j 8
   git cms-rebase-topic -u skkwan:devel-phase2egStandaloneBarrelEmulator
   git cms-addpkg L1Trigger/L1CaloTrigger
   git cms-addpkg Configuration/Geometry
   scram b -j 8
   ```

## To run the Phase 2 emulator only
   
   To run the Phase 2 emulator:
   ```
   cmsenv
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloTrigger/test
   cmsRun test_Phase2L1CaloEGammaEmulator.py
   ```

## To set up the analyzer (which calls the old and new emulator)

1. Get the analyzer repository: git clone the analyzer repo and then manually move it:
   ```
   cmsenv
   cd ${CMSSW_BASE}/src/L1Trigger/
   git clone https://github.com/skkwan/phase2-l1Calo-analyzer.git
   mv phase2-l1Calo-analyzer/ L1CaloPhase2Analyzer/ 
   ```


## To run the analyzer (calls both the old and new emulator)

1. To run the analyzer:
   ```
   cmsenv
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test
   cmsRun test-singleAnalyzer.py
   ```
2. To run the rates analyzer:
   ```
   cmsenv
   cd ${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test

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

## (Developers only) To run the emulator and analyzer to make a TTree for distributions, efficiencies, rates, etc.

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