import FWCore.ParameterSet.Config as cms


l1NtupleProducerRates = cms.EDAnalyzer("L1TCaloEGammaAnalyzerRates_oldEmulator",
                                  folderName              = cms.untracked.string("firstFolder"),
                                  genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                  #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                  #pfCands                 = cms.InputTag("particleFlow"),
                                  
                                  ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis","","HLT"),
                                  hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis","","HLT"),
                                  clusters = cms.InputTag("L1EGammaClusterEmuProducer", "", "L1AlgoTest")

)
