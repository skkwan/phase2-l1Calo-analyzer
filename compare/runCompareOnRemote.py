# python runCompareSingleFiles.py

# Compare two n-tuples and copy the resulting .pdf and .csv files to the specified EOS area/directory.
# See the documentation in compare.py (stolen from HTT 2015 GitHub: https://github.com/CMS-HTT/2015-sync/blob/master/compare.py) for more command
# line arguments and use cases.

# python runCompareOnRemote.py

import os 

file1 = "davs://cmsxrootd.hep.wisc.edu:1094/store/user/skkwan/Phase2EGammaAnalyzer/MinBias_TuneCP5_14TeV-pythia8/Analyzer_MinBias_TuneCP5_14TeV-pythia8_oldEmulator_Phase2HLTTDRWinter20DIGI/230123_150328/0000/analyzer-MinBias-oldEmulator_1.root"
file2 = "davs://cmsxrootd.hep.wisc.edu:1094/store/user/skkwan/Phase2EGammaAnalyzer/MinBias_TuneCP5_14TeV-pythia8/Analyzer_MinBias_TuneCP5_14TeV-pythia8_Phase2HLTTDRWinter20DIGI/230123_141718/0000/analyzer_1.root"


# Descriptive names of the n-tuples (will show up in the comparison plot legends). Can't have numbers in these.
firstName ="Old-emulator"
secondName = "Current-emulator"

# Destination directory where the .pdfs will be copied to. 
exportDir = "/eos/user/s/skkwan/phase2RCTDevel/compare/"

# Options for compare.py See the original compare.py for a full description.
varslist=["gct_is_iso"]
for var in varslist:
    options = "--no-intersection --diff --var-diff={} --output-dir={} --round-precision=0 --veto-common-branches ecalTPGs hcalTPGs nvtx --titles {} {}".format(var, exportDir, firstName, secondName)
    cmd = "python compare.py {} --files {} {}".format(options, file1, file2)
    print(">> Executing {}".format(cmd))
    os.system(cmd)

    print(">>> If successful: Copying outputs to {}".format(exportDir))
    os.system("cp common.pdf {}{}_common.pdf".format(exportDir, var))
    os.system("cp sync.pdf {}{}_sync.pdf".format(exportDir, var))
    os.system("cp intersect.pdf {}{}_intersect.pdf".format(exportDir, var))
