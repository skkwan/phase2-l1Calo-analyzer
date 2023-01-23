#!/bin/bash

# Compare two n-tuples and copy the resulting .pdf and .csv files to the specified EOS area/directory.
# See the documentation in compare.py (stolen from HTT 2015 GitHub: https://github.com/CMS-HTT/2015-sync/blob/master/compare.py) for more command
# line arguments and use cases.

# Example command:                                                                                                                                         
#      python compare.py /eos/user/s/skkwan/hToAA/syncNanoAOD/102X_RunIIAutumn18/sync_ntuples/skim_vbf_2018_stephanie_jetPUid_80X.root /eos/user/s/skkwan/hToAA/syncNanoAOD/102X_RunIIAutumn18/sync_ntuples/skim_vbf_2018_stephanie_jetPUid_102X.root -t EightyX,HundredTwoX 

FIRST_NTUP="/afs/cern.ch/work/s/skkwan/private/phase2RCTDev/CMSSW_11_1_7/src/L1Trigger/L1CaloPhase2Analyzer/test/analyzer_oldEmulator_MinBias_noGenMatching.root"

SECOND_NTUP="/afs/cern.ch/work/s/skkwan/private/phase2RCTDev/CMSSW_11_1_7/src/L1Trigger/L1CaloPhase2Analyzer/test/analyzer_MinBias_noGenMatching.root"


# Descriptive names of the n-tuples (will show up in the comparison plot legends). Can't have numbers in these.
FIRST_NAME="Old-emulator"
SECOND_NAME="Current-emulator"

# Destination directory where the .pdfs will be copied to. 
EXPORTDIR="/eos/user/s/skkwan/phase2RCTDevel/compare/"
mkdir -p ${EXPORTDIR}

# Options for compare.py See the original compare.py for a full description.
varslist=("gct_is_iso")
for var in ${varslist[*]}; do
   OPTIONS="--no-intersection --diff --var-diff="${var}" --output-dir="${EXPORTDIR}" --round-precision=0 --veto-common-branches ecalTPGs hcalTPGs nvtx --titles "${FIRST_NAME}" "${SECOND_NAME}""
   cmd="python compare.py ${OPTIONS} --files "${FIRST_NTUP}" "${SECOND_NTUP}""
   echo ">> Executing ${cmd}"
   eval ${cmd}

   # echo ">>> If successful: Copying outputs and README.md to ${EXPORTDIR}."
   # cp README.md ${EXPORTDIR}
   # cp common.pdf "${EXPORTDIR}${var}_common.pdf"
   # cp sync.pdf "${EXPORTDIR}${var}_sync.pdf"
   # cp intersect.pdf "${EXPORTDIR}${var}_intersect.pdf"
   # cp ${FIRST_NAME}.csv "${EXPORTDIR}${var}_${FIRSTNAME}.csv"
   # cp ${SECOND_NAME}.csv "${EXPORTDIR}${var}_${SECONDNAME}.csv"
done

