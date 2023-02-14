import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("L1AlgoTest",eras.Phase2C4)

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

                                'root://cms-xrd-global.cern.ch://store/mc/Phase2HLTTDRSummer20ReRECOMiniAOD/SinglePion_PT0to200/GEN-SIM-DIGI-RAW-MINIAOD/NoPU_111X_mcRun4_realistic_T15_v1-v1/120000/6930E519-DD17-DD40-BE77-41A28DEF9278.root'
                                ),
                            inputCommands = cms.untracked.vstring(
                                "keep *",
                                "drop l1tTkPrimaryVertexs_*_*_*",
                            )
                        )

process.source.eventsToProcess = cms.untracked.VEventRange("1:3014")


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
process.load('L1Trigger.L1CaloPhase2Analyzer.l1TCaloEGammaAnalyzer_cfi')

process.pL1EG = cms.Path( process.Phase2L1CaloEGammaEmulatorProducer*process.l1NtupleProducer )

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analyzer_SinglePion_test.root')
)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "phase2L1EGammaAnalyzer_SinglePion.root" ),
    outputCommands = cms.untracked.vstring(
        "drop *"
#        "keep *_Phase2L1CaloEGammaEmulatorProducer_*_*",
#        "keep *_TriggerResults_*_*",
#        "keep *_simHcalTriggerPrimitiveDigis_*_*",
#        "keep *_EcalEBTrigPrimProducer_*_*"
    )
)


process.end = cms.EndPath( process.Out )

process.schedule = cms.Schedule(process.pL1EG, process.end)

dump_file = open("dump_file.py", "w")
dump_file.write(process.dumpPython())
