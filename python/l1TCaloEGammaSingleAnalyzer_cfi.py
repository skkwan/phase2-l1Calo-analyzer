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
                                        rctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "RCT", "REPR"),
                                        gctClusters = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCT", "REPR"),
                                        oldClusters = cms.InputTag("l1tEGammaClusterEmuProducer", "", "REPR"),
                                        l1EGammas = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTEGammas", "REPR"),
                                        gctFullTowers = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTFullTowers", "REPR"),
                                        digitizedClusterCorrelator = cms.InputTag("l1tPhase2L1CaloEGammaEmulator", "GCTDigitizedToCorrelator", "REPR"),

                                        
)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleSingleProducer,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
