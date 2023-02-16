import FWCore.ParameterSet.Config as cms


l1NtupleProducerRates = cms.EDAnalyzer("L1TCaloEGammaAnalyzerRates_oldEmulator",
                                  folderName              = cms.untracked.string("firstFolder"),
                                  genParticles     = cms.InputTag("genParticles", "", "HLT"),
                                  #packedPfCands           = cms.InputTag("packedPFCandidates"),
                                  #pfCands                 = cms.InputTag("particleFlow"),
                                  ecalDigis = cms.InputTag("simEcalEBTriggerPrimitiveDigis"),
                                  hcalDigis = cms.InputTag("simHcalTriggerPrimitiveDigis"),
                                  clusters = cms.InputTag("L1EGammaClusterEmuProducer", "", "L1AlgoTest")

)

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(l1NtupleProducerRates,
    ecalDigis = cms.InputTag("DMEcalEBTriggerPrimitiveDigis"),
    hcalDigis = cms.InputTag("DMHcalTriggerPrimitiveDigis"),
)
