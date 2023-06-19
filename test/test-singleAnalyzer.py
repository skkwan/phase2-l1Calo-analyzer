import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('REPR',eras.Phase2C9)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D49_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                'file:/eos/user/s/skkwan/phase2RCTDevel/unitTests/Phase2Fall22DRMiniAOD_DoubleElectron_FlatPt-1To100-gun_PU200_125X_mcRun4_realistic_v2-v1_0024ebea-73de-496a-9d75-6f0a7c3b2ba4.root',

                                # Signal events (just want to get one with good agreement)
                                #'root://cmsxrootd.fnal.gov///store/mc/Phase2Fall22DRMiniAOD/DoubleElectron_FlatPt-1To100-gun/GEN-SIM-DIGI-RAW-MINIAOD/PU200_125X_mcRun4_realistic_v2-v1/30000/0024ebea-73de-496a-9d75-6f0a7c3b2ba4.root',


                               #'root://cmsxrootd.fnal.gov///store/user/skkwan/DoubleElectron_FlatPt-1To100-gun/crab_pickEvents/230316_113131/0000/pickevents_1.root',
                                # Disagreements in iso flag, and new rel isolation is less than old rel isolation
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230209_083734_newIsoLessThanOldIso/0000/pickevents_1.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230209_083734_newIsoLessThanOldIso/0000/pickevents_2.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230209_083734_newIsoLessThanOldIso/0000/pickevents_3.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230209_083734_newIsoLessThanOldIso/0000/pickevents_4.root',

                                # Disagrements in iso flag *and* high pT
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_1.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_2.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_3.root',


                                # Only disagreements in iso flag
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_10.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_11.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_12.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_13.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_14.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_15.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_16.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_17.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_18.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_19.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_1.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_20.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_2.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_3.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_4.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_5.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_6.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_7.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_8.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230125_100411/0000/pickevents_9.root',

       
                                                      ),
                            inputCommands = cms.untracked.vstring(
                                "keep *"
                            )
                        )
# process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange("1:594")
#process.source.eventsToProcess = cms.untracked.VEventRange("1:66890")


# --------------------------------------------------------------------------------------------                                                    
#                                                                                                                                                            
# ----   Run the relevant algorithms
# ---- Global Tag :
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '123X_mcRun4_realistic_v3', '')


# Add HCAL Transcoder
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load('CalibCalorimetry.CaloTPG.CaloTPGTranscoder_cfi')


# --------------------------------------------------------------------------------------------
#
# ----   One analyzer to rule them all

process.load('L1Trigger.L1CaloTrigger.l1tPhase2L1CaloEGammaEmulator_cfi')
process.load('L1Trigger.L1CaloTrigger.l1tEGammaCrystalsEmulatorProducer_cfi')
process.load('L1Trigger.L1CaloPhase2Analyzer.l1TCaloEGammaSingleAnalyzer_cfi')

process.pL1EG = cms.Path( process.l1tPhase2L1CaloEGammaEmulator * process.l1tEGammaClusterEmuProducer * process.l1NtupleSingleProducer)

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('singleAnalyzer-test.root')
)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "phase2L1EGammaAnalyzer.root" ),
    outputCommands = cms.untracked.vstring(

        "keep *_Phase2L1CaloEGammaEmulator_*_*",
#        "keep *_TriggerResults_*_*",
#        "keep *_simHcalTriggerPrimitiveDigis_*_*",
#        "keep *_EcalEBTrigPrimProducer_*_*"
    )
)


process.end = cms.EndPath( process.Out )

process.schedule = cms.Schedule(process.pL1EG)

# dump_file = open("dump_file.py", "w")
# dump_file.write(process.dumpPython())
