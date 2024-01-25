# README.md for CRAB anlyzer submission

## Basic steps
Performed on lxplus. There is one single `cmsRun` config file for the old and new emulators' efficiencies, one `cmsRun` config file for just the old emulator's rates, and lastly 
one other `cmsRun` config file for just the new emulator's rates.

1. Edit `datasetConfig.yml` which lists all the input DAS datasets and says
   which cmsRun script to use, the CRAB production tag, etc.

   Edit `crab_template_cfg.py` which contains the parameters passed to CRAB, e.g.
   which T2 storage to use, multi-core options, and so on.

   In `datasetConfig.yml`, it says the signal datasets will use the cmsRun config file `crab_script_signal.py`, so make sure the contents of that file are correct.
2. Now create one CRAB `_cfg.py` per dataset, by running this:
   ```
   cmsenv
   python3 parseYaml.py
   ```
3. Set up
   ```
   voms-proxy-init
   cd crabJobConfigs/2024/
   ```
4. Submit the CRAB jobs from a CMSSW-el7 singularity (since CRAB does not support el9 as of January 25 2024)
   ``` 
   # In the crab/ directory
   cmssw-el7 
   cmsenv
   crab submit -c [path-to-file-in-crabJobConfigs/2024/]
   # May need to type GRID password again even if you did voms-proxy-init earlier
   ```

## Troubleshooting CRAB jobs

* Wall clock time exceeded
  * Could be due to broken paths in /hdfs (e.g. to outdated or duplicate datasets) which
    need to be marked as invalid.

* Multi-threading in CRAB config:
  ```
  config.JobType.maxJobRuntimeMin = 300
  config.JobType.numCores = 8
  config.JobType.maxMemoryMB = 9000
  ```