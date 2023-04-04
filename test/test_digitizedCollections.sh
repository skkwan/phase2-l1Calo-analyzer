#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

F1=${CMSSW_BASE}/src/L1Trigger/L1CaloPhase2Analyzer/test/test-digitizedCollections.py
(cmsRun $F1 ) || die "Failure using $F1" $?
