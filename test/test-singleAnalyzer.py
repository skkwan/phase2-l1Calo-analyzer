import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('REPR',eras.Phase2C17I13M9)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D86Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D86_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

# Using https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TPhase2Instructions#Phase2Fall22_Campaign_125X_sampl
# /DoubleElectron_FlatPt-1To100-gun/Phase2Fall22DRMiniAOD-PU200_125X_mcRun4_realistic_v2-v1/GEN-SIM-DIGI-RAW-MINIAOD
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                # Signal events (just want to get one with good agreement)
                                'root://cms-xrd-global.cern.ch///store/mc/Phase2Spring23DIGIRECOMiniAOD/DoubleElectron_FlatPt-1To100-gun/GEN-SIM-DIGI-RAW-MINIAOD/PU200_Trk1GeV_131X_mcRun4_realistic_v5-v1/30000/0083e06c-959e-4ea5-9190-fd73667fcc00.root'
       
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
process.GlobalTag = GlobalTag(process.GlobalTag, '131X_mcRun4_realistic_v6', '')


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
