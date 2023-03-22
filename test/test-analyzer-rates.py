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

# Dataset: 
# file block=/MinBias_TuneCP5_14TeV-pythia8/Phase2HLTTDRWinter20DIGI-PU200_110X_mcRun4_realistic_v3-v3/GEN-SIM-DIGI-RAW#2f4f0ee3-5dd4-4a4b-82cc-3da8406e3119
process.source = cms.Source("PoolSource",

                            fileNames = cms.untracked.vstring(
                                cms.untracked.vstring(
                                # Disagrements in iso flag *and* high pT
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_1.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_2.root',
                                # 'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_3.root',


                                'root://cmsxrootd.fnal.gov///store/mc/Phase2Fall22DRMiniAOD/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_125X_mcRun4_realistic_v2-v1/30000/001ec7cf-71d4-4eb4-9002-078d21560b2f.root',

                                )

                            ),
                            inputCommands = cms.untracked.vstring(
                                "keep *"
                            )
                        )


#process.source.eventsToProcess = cms.untracked.VEventRange("1:644030", "1:613784")
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
# ----    Produce the L1EGCrystal clusters using Emulator

process.load('L1Trigger.L1CaloTrigger.l1tPhase2L1CaloEGammaEmulator_cfi')
process.load('L1Trigger.L1CaloPhase2Analyzer.l1TCaloEGammaAnalyzerRates_cfi')

process.pL1EG = cms.Path( process.l1tPhase2L1CaloEGammaEmulator*process.l1NtupleProducer )

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('analyzer-rates.root')
)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "phase2L1EGammaAnalyzerRates.root" ),
                                outputCommands = cms.untracked.vstring(
         "drop *"                           
#        "keep *_Phase2L1CaloEGammaEmulatorProducer_*_*",
#        "keep *_TriggerResults_*_*",
#        "keep *_simHcalTriggerPrimitiveDigis_*_*",
#        "keep *_EcalEBTrigPrimProducer_*_*"
    )
)


process.end = cms.EndPath( process.Out )

process.schedule = cms.Schedule(process.pL1EG)
#process.schedule = cms.Schedule(process.pL1EG, process.end)     

dump_file = open("dump_file.py", "w")
dump_file.write(process.dumpPython())
