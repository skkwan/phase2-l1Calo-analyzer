import FWCore.ParameterSet.Config as cms


l1NtupleSingleProducer = cms.EDAnalyzer("L1TCaloEGammaSingleAnalyzer",
                                        folderName              = cms.untracked.string("firstFolder"),
                                        requireGenMatching      = cms.untracked.bool(False),
                                        saveOnlyHighestPtCluster = cms.untracked.bool(False),
                                        genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                        #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                        #pfCands                 = cms.InputTag("particleFlow"),                                                                    
                                        ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis","","HLT"),
                                        hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis","","HLT"),
                                        rctClusters = cms.InputTag("Phase2L1CaloEGammaEmulatorProducer", "RCT"),
                                        gctClusters = cms.InputTag("Phase2L1CaloEGammaEmulatorProducer", "GCT"),
                                        oldClusters = cms.InputTag("L1EGammaClusterEmuProducer", "", "L1AlgoTest")
        #                                  clusters  = cms.InputTag('L1EGammaClusterEmuProducer')
)
