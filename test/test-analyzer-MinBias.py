import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("L1AlgoTest",eras.Phase2C4)

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

# Dataset: 
# 200 PU: 
#  /DoubleElectron_FlatPt-1To100/Phase2HLTTDRWinter20DIGI-PU200_110X_mcRun4_realistic_v3-v2/GEN-SIM-DIGI-RAW


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

                                #'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/004299CF-ED4D-9140-BECA-772B15D9FDF4.root',

                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_10.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_11.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_12.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_13.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_14.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_15.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_16.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_17.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_18.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_19.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_1.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_20.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_2.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_3.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_4.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_5.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_6.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_7.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_8.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_9.root',

       
                                                      ),
                            inputCommands = cms.untracked.vstring(
                                "keep *"
                            )
                        )


# process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange("1:3774")
# process.source.eventsToProcess = cms.untracked.VEventRange("1:667914")
# --------------------------------------------------------------------------------------------                                                    
#                                                                                                                                                            
# ----   Run the relevant algorithms

# ---- Global Tag :
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2023_realistic_v1', '')

# Choose a geometry
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')

# Add HCAL Transcoder
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')


# --------------------------------------------------------------------------------------------
#
# ----    Produce the L1EGCrystal clusters using Emulator

process.load('L1Trigger.L1CaloTrigger.Phase2L1CaloEGammaEmulator_cfi')
process.load('L1Trigger.L1CaloPhase2Analyzer.l1TCaloEGammaAnalyzer_ZeroBias_cfi')  # use ZeroBias config file that does not require gen-matching

process.pL1EG = cms.Path( process.Phase2L1CaloEGammaEmulatorProducer*process.l1NtupleProducer )

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analyzer_MinBias_discrepancies.root')
)

# process.Out = cms.OutputModule( "PoolOutputModule",
#     fileName = cms.untracked.string( "phase2L1EGammaAnalyzer_MinBias_noGenMatching.root" ),
#     outputCommands = cms.untracked.vstring(
#         "drop *"
# #        "keep *_Phase2L1CaloEGammaEmulatorProducer_*_*",
# #        "keep *_TriggerResults_*_*",
# #        "keep *_simHcalTriggerPrimitiveDigis_*_*",
# #        "keep *_EcalEBTrigPrimProducer_*_*"
#     )
# )


# process.end = cms.EndPath( process.Out )

process.schedule = cms.Schedule(process.pL1EG)

dump_file = open("dump_file.py", "w")
dump_file.write(process.dumpPython())
