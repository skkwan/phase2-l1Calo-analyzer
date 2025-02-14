import FWCore.ParameterSet.Config as cms


l1NtupleSingleProducer = cms.EDAnalyzer("L1TCaloEGammaSingleAnalyzer",
                                        folderName              = cms.untracked.string("firstFolder"),
                                        requireGenMatching      = cms.untracked.bool(True),
                                        saveOnlyHighestPtCluster = cms.untracked.bool(False),
                                        genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                        #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                        #pfCands                 = cms.InputTag("particleFlow"),                                                                    
                                        ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis"),
                                        hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis"),
                                        rctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "RCTClusters"),
                                        gctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTClusters"),
                                        rctTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "RCTTowers"),
                                        gctTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTTowers"),
                                        oldClusters = cms.InputTag("l1tEGammaClusterEmuProducer", "", "REPR"),
                                        l1EGammas = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTEGammas"),
                                        oldL1EGammas = cms.InputTag("l1tEGammaClusterEmuProducer", "", "REPR"),
                                        gctFullTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTFullTowers"),

                                        digitizedClustersGT = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedClusterToGT"),
                                        digitizedClustersCorrelator = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedClusterToCorrelator"),
                                        digitizedTowersCorrelator = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedTowerToCorrelator"),



)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleSingleProducer,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
