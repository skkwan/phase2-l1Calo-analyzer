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
                                        rctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "RCTClusters", "REPR"),
                                        gctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTClusters", "REPR"),
                                        rctTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "RCTTowers", "REPR"),
                                        gctTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTTowers", "REPR"),
                                        PFclusters              = cms.InputTag("l1tPhase2CaloPFClusterEmulator", "GCTPFCluster"),
                                        oldClusters = cms.InputTag("l1tEGammaClusterEmuProducer", "", "REPR"),
                                        l1EGammas = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTEGammas", "REPR"),
                                        oldL1EGammas = cms.InputTag("l1tEGammaClusterEmuProducer", "", "REPR"),
                                        gctFullTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTFullTowers", "REPR"),

                                        digitizedClustersGT = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedClusterToGT", "REPR"),
                                        digitizedClustersCorrelator = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedClusterToCorrelator", "REPR"),
                                        digitizedTowersCorrelator = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedTowerToCorrelator", "REPR"),

                                        gctBarrelDigiClustersToCorrLayer1 = cms.InputTag("l1tPhase2GCTBarrelToCorrelatorLayer1Emulator", "GCTBarrelDigiClustersToCorrLayer1", "REPR")

)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleSingleProducer,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
