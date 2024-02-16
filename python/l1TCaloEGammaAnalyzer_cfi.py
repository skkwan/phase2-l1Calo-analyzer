import FWCore.ParameterSet.Config as cms


l1NtupleProducer = cms.EDAnalyzer("L1TCaloEGammaAnalyzer",
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
#                                  clusters  = cms.InputTag('L1EGammaClusterEmuProducer')
)



from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleProducer,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
