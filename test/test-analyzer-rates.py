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
#                            fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/s/skkwan/public/phase2RCT/RelValElectronGunPt2To100_190EDE9F-770B-174A-8BA6-F7814FC67FD4.root'),
                            fileNames = cms.untracked.vstring(
                                cms.untracked.vstring(
                                # Disagrements in iso flag *and* high pT
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_1.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_2.root',
                                'root://cmsxrootd.fnal.gov///store/user/skkwan/MinBias_TuneCP5_14TeV-pythia8/crab_pickEvents/230206_094406/0000/pickevents_3.root',



                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/004299CF-ED4D-9140-BECA-772B15D9FDF4.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/00494C5E-BC6A-1E43-A41E-DEDC290A46CB.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0067A78C-C66F-ED4E-A543-1CC1C5ACA2D4.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0092A32D-718D-0E45-88B2-80A50037934A.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/02975B99-0AFC-FD42-9A42-40AE39345194.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/05460D4A-ED9A-D541-B47B-ADCDFCF9ABD9.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/05BB40AD-467D-0B4A-A556-C6717BD2F51A.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/05F67FEC-6D21-374D-8299-B366A9737642.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/080BCB66-D269-1043-95BE-55CBA7E51651.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/09C64E96-9936-AB45-B8AE-18C50B520BB3.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0AB76C1A-80F7-FE42-9C95-115DAB71BF97.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0B0B4946-A08A-E14D-9BE1-0DBF25F98CEB.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0BEEF4B3-38D6-8745-B4C9-2BF9934249C2.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0D112947-F184-1C45-A951-ED90EE261D89.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0D6F9C62-9EC5-FA46-8E83-33E6D036CA6D.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0DC1C618-1E58-5A45-9F5E-4865D66E1888.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0E47B247-D6A8-0548-9B62-9E830A45583E.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0E817C30-1EAA-EB4D-8157-8CCA49736F7B.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/0FD5913A-DF61-8D4B-BE50-8059244F4EF4.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/141CBD63-B60F-8B4A-BF27-F07BC9AC31C8.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/157728FF-0D67-4B4D-8E6D-994268F4ADB3.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1645EFFA-9E99-C241-BD3E-D23B391A1EB4.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/166D4277-6573-C14A-92A7-A4B551E3168A.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/16EDF9D3-BDE5-1F45-948E-4B184D7D87A1.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/17D239C6-14D9-8342-8EE8-A34D4EF8A881.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1802EEA3-FEC8-AD41-9E26-132F6A15E125.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/18447DD3-9E9C-9C44-9CB5-4EFBE86A7A9C.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/185392EC-A57E-5143-A526-D1980E66DEC6.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/18BB3187-E4FD-4D4F-A09C-DEA33EAFC179.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/18C3DD19-B7C7-3C4E-B0A9-875831A7E01F.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/191327E4-0635-344E-A45D-36E54DF1C429.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/192D313F-DA0F-6E47-9CA1-2469F1DA80D0.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1AA6ACA6-DDD4-684B-9601-6700D05BA9C4.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1C9ACC94-2E0C-4240-875E-E4287B4208F1.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1CFFC548-98FB-B84B-A733-BF236ACFDEA9.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1D342B74-88EC-5441-B083-161E73A50614.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1D692227-7DDF-3847-A6DF-E901CFF07FCD.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1D79C619-EB67-644A-B07C-10920DB3FCAE.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1F81F316-0D38-444B-922C-AE4B57E00D08.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1F870D34-B053-BC45-AFE4-6F68F3B16466.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/1FC71542-216B-A542-8D36-E9D2C133BA78.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/20491C29-A9F4-A24B-AF3A-863DB8334249.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/2069F9F2-E933-E347-BF13-292155CAED04.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/21048958-B597-134E-AD13-626C391E8C42.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/220FF751-CCC3-1447-A07A-767A426ABFA3.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/23683615-0F88-F747-9507-67A9BF17FD67.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/23A3F27E-F782-AD48-83B4-D5CB7195D5C8.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/23C5FF5A-E0C9-0440-ACCB-B733DDA3C942.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/23F8E60E-F84C-4E46-8AFF-0227CD5088B5.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/24C80665-3A4C-674C-8ED2-BB4B695FF8E7.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/258E2584-B553-8C40-93B0-1EB2C2E9AC8A.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/26529E4B-D757-B64C-8D94-23B0D1447D38.root',
                                    # 'root://cms-xrd-global.cern.ch///store/mc/Phase2HLTTDRWinter20DIGI/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW/PU200_110X_mcRun4_realistic_v3-v3/10000/268340D3-1A14-9C40-9833-F9D6A85BC140.root

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
