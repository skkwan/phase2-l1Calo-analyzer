import FWCore.ParameterSet.Config as cms


l1NtupleSingleProducer = cms.EDAnalyzer("L1TCaloEGammaSingleAnalyzer",
                                        folderName              = cms.untracked.string("firstFolder"),
                                        requireGenMatching      = cms.untracked.bool(False),
                                        saveOnlyHighestPtCluster = cms.untracked.bool(False),
                                        genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                        #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                        #pfCands                 = cms.InputTag("particleFlow"),                                                                    
                                        ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis"),
                                        hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis"),
                                        rctClusters = cms.InputTag("Phase2L1CaloEGammaEmulator", "RCT"),
                                        gctClusters = cms.InputTag("Phase2L1CaloEGammaEmulator", "GCT"),
                                        oldClusters = cms.InputTag("L1EGCrystalClusterEmulatorProducer")
                                        
)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleSingleProducer,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
