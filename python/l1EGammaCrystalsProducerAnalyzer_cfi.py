import FWCore.ParameterSet.Config as cms


l1NtupleProducer = cms.EDAnalyzer("L1EGammaCrystalsProducerAnalyzer",
                                  folderName              = cms.untracked.string("firstFolder"),
                                  requireGenMatching      = cms.untracked.bool(True),
                                  saveOnlyHighestPtCluster = cms.untracked.bool(False),
                                  genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                  #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                  #pfCands                 = cms.InputTag("particleFlow"),
                                  ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis", "", "HLT"),
                                  hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis", "", "HLT"),
                                  clusters = cms.InputTag("L1EGammaClusterEmuProducer", "", "L1AlgoTest")
)
