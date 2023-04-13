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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                'root://cmsxrootd.fnal.gov///store/mc/Phase2Fall22DRMiniAOD/DoubleElectron_FlatPt-1To100-gun/GEN-SIM-DIGI-RAW-MINIAOD/PU200_125X_mcRun4_realistic_v2-v1/30000/15db466a-ee8d-41f8-856a-1b6dd5261f13.root'
                                                      ),
                            inputCommands = cms.untracked.vstring(
                                "keep *"
                            )
                        )

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
    fileName = cms.string("singleAnalyzer.root")
)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "phase2L1EGammaAnalyzer.root" ),
    outputCommands = cms.untracked.vstring(
        "drop *"
       # "keep *_Phase2L1CaloEGammaEmulator_*_*",
#        "keep *_TriggerResults_*_*",
#        "keep *_simHcalTriggerPrimitiveDigis_*_*",
#        "keep *_EcalEBTrigPrimProducer_*_*"
    )
)


process.end = cms.EndPath( process.Out )

process.schedule = cms.Schedule(process.pL1EG)

# dump_file = open("dump_file.py", "w")
# dump_file.write(process.dumpPython())
