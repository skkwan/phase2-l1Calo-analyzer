/*
 *  \file L1TCaloEGammaSingleAnalyzer.cc
 *  Authors S. Kwan, P. Das, I. Ojalvo
 */

// system include files
#include <ap_int.h>
#include <array>
#include <bitset>
#include <cmath>
// #include <cstdint>
#include <iostream>
#include <fstream>
#include <memory>

// user include files
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

// ECAL TPs
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

// HCAL TPs
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"

#include "DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/CaloTower.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/CaloPFCluster.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/EmDigiCluster.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/HadDigiCluster.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"

#include "L1Trigger/L1CaloTrigger/interface/ParametricCalibration.h"
#include "L1Trigger/L1CaloTrigger/interface/Phase2L1CaloBarrelToCorrelator.h" // p2eg function definitions
#include "L1Trigger/L1CaloTrigger/interface/Phase2L1CaloEGammaUtils.h"  // p2eg namespace and function definitions
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
 
#include "L1Trigger/L1CaloPhase2Analyzer/interface/L1TCaloEGammaSingleAnalyzer.h" 

#include "DataFormats/Math/interface/deltaR.h"


// ECAL propagation
#include "CommonTools/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "CommonTools/BaseParticlePropagator/interface/RawParticle.h"


using namespace edm;
using std::cout;
using std::endl;
using std::vector;

L1TCaloEGammaSingleAnalyzer::L1TCaloEGammaSingleAnalyzer(const edm::ParameterSet& cfg ) :
  decoderToken_(esConsumes<CaloTPGTranscoder, CaloTPGRecord>(edm::ESInputTag("", ""))),
  caloGeometryToken_(esConsumes<CaloGeometry, CaloGeometryRecord>(edm::ESInputTag("", ""))),
  hbTopologyToken_(esConsumes<HcalTopology, HcalRecNumberingRecord>(edm::ESInputTag("", ""))),
  ecalSrc_(consumes<EcalEBTrigPrimDigiCollection>(cfg.getParameter<edm::InputTag>("ecalDigis"))),
  hcalSrc_(consumes<HcalTrigPrimDigiCollection>(cfg.getParameter<edm::InputTag>("hcalDigis"))),
  rctClustersSrc_(consumes<l1tp2::CaloCrystalClusterCollection >(cfg.getParameter<edm::InputTag>("rctClusters"))),
  gctClustersSrc_(consumes<l1tp2::CaloCrystalClusterCollection >(cfg.getParameter<edm::InputTag>("gctClusters"))),
  rctTowersSrc_(consumes<l1tp2::CaloTowerCollection >(cfg.getParameter<edm::InputTag>("rctTowers"))),
  gctTowersSrc_(consumes<l1tp2::CaloTowerCollection >(cfg.getParameter<edm::InputTag>("gctTowers"))),
  caloPFClustersSrc_(consumes<l1tp2::CaloPFClusterCollection>(cfg.getParameter<edm::InputTag>("PFclusters"))),
  oldClustersSrc_(consumes<l1tp2::CaloCrystalClusterCollection >(cfg.getParameter<edm::InputTag>("oldClusters"))),
  l1EGammasSrc_(consumes<BXVector<l1t::EGamma>>(cfg.getParameter<edm::InputTag>("l1EGammas"))),
  oldL1EGammasSrc_(consumes<BXVector<l1t::EGamma>>(cfg.getParameter<edm::InputTag>("oldL1EGammas"))),
  fullTowersSrc_(consumes<l1tp2::CaloTowerCollection>(cfg.getParameter<edm::InputTag>("gctFullTowers"))),
  digitizedClustersGTSrc_(consumes<l1tp2::DigitizedClusterGTCollection>(cfg.getParameter<edm::InputTag>("digitizedClustersGT"))),
  digitizedClustersCorrelatorSrc_(consumes<l1tp2::DigitizedClusterCorrelatorCollection>(cfg.getParameter<edm::InputTag>("digitizedClustersCorrelator"))),
  digitizedTowersCorrelatorSrc_(consumes<l1tp2::DigitizedTowerCorrelatorCollection>(cfg.getParameter<edm::InputTag>("digitizedTowersCorrelator"))),
  emDigiClustersSrc_(consumes<l1tp2::EmDigiClusterCollection>(cfg.getParameter<edm::InputTag>("emDigiClusters"))),
  hadDigiClustersSrc_(consumes<l1tp2::HadDigiClusterCollection>(cfg.getParameter<edm::InputTag>("hadDigiClusters"))),
  genSrc_ (( cfg.getParameter<edm::InputTag>( "genParticles")))
{
    genToken_ =     consumes<std::vector<reco::GenParticle> >(genSrc_);

    folderName_          = cfg.getUntrackedParameter<std::string>("folderName");
    requireGenMatching_ = cfg.getUntrackedParameter<bool>("requireGenMatching");
    saveOnlyHighestPtCluster_ = cfg.getUntrackedParameter<bool>("saveOnlyHighestPtCluster");
    dispTree = tfs_->make<TTree>("dispTree", "Event Display Tree");
    efficiencyTree = tfs_->make<TTree>("efficiencyTree", "Efficiency Tree");
    
    ////putting bufsize at 32000 and changing split level to 0 so that the branch isn't split into multiple branches
    dispTree->Branch("run",    &run,     "run/I");
    dispTree->Branch("lumi",   &lumi,    "lumi/I");
    dispTree->Branch("event",  &event,   "event/I");

    dispTree->Branch("oldClusters", "vector<TLorentzVector>", &oldClusters, 32000, 0); 
    dispTree->Branch("oldTowers", "vector<TLorentzVector>", &oldTowers, 32000, 0); 
    dispTree->Branch("oldRelIso", "vector<float>", &oldRelIso, 32000, 0);
    dispTree->Branch("oldIsoFlag", "vector<bool>", &oldIsoFlag, 32000, 0);
    dispTree->Branch("oldRawIso", "vector<float>", &oldRawIso, 32000, 0);

    dispTree->Branch("newClusters", "vector<TLorentzVector>", &gctClusters, 32000, 0); 
    dispTree->Branch("newTowers",   "vector<TLorentzVector>", &gctTowers, 32000, 0);
    dispTree->Branch("hcalTPGs", "vector<TLorentzVector>", &allHcalTPGs, 32000, 0); 
    dispTree->Branch("ecalTPGs", "vector<TLorentzVector>", &allEcalTPGs, 32000, 0); 

    
    efficiencyTree->Branch("run",    &run,     "run/I");
    efficiencyTree->Branch("lumi",   &lumi,    "lumi/I");
    efficiencyTree->Branch("event",  &event,   "event/I");
    efficiencyTree->Branch("nvtx",   &nvtx,         "nvtx/I");

    // Gen electrons
    efficiencyTree->Branch("genPt",  &genPt,  "genPt/D");
    efficiencyTree->Branch("genEta", &genEta, "genEta/D");
    efficiencyTree->Branch("genPhi", &genPhi, "genPhi/D");

    // The RCT emulator cluster that was matched to the gen electron
    efficiencyTree->Branch("rct_cPt",  &rct_cPt,  "rct_cPt/D");
    efficiencyTree->Branch("rct_cEta", &rct_cEta, "rct_cEta/D");
    efficiencyTree->Branch("rct_cPhi", &rct_cPhi, "rct_cPhi/D");
    efficiencyTree->Branch("rct_deltaR", &rct_deltaR, "rct_deltaR/D");
    efficiencyTree->Branch("rct_et2x5", &rct_et2x5, "rct_et2x5/D");
    efficiencyTree->Branch("rct_et5x5", &rct_et5x5, "rct_et5x5/D");

    // The GCT emulator cluster that was matched to the gen electron
    efficiencyTree->Branch("gct_cPt",  &gct_cPt,  "gct_cPt/D");
    efficiencyTree->Branch("gct_cEta", &gct_cEta, "gct_cEta/D");
    efficiencyTree->Branch("gct_cPhi", &gct_cPhi, "gct_cPhi/D");
    efficiencyTree->Branch("gct_deltaR", &gct_deltaR, "gct_deltaR/D");
    efficiencyTree->Branch("gct_et2x5", &gct_et2x5, "gct_et2x5/D");
    efficiencyTree->Branch("gct_et5x5", &gct_et5x5, "gct_et5x5/D");
    efficiencyTree->Branch("gct_iso",   &gct_iso,   "gct_iso/D");
    efficiencyTree->Branch("gct_is_ss", &gct_is_ss, "gct_is_ss/I");
    efficiencyTree->Branch("gct_is_looseTkss", &gct_is_looseTkss, "gct_is_looseTkss/I");
    efficiencyTree->Branch("gct_is_iso", &gct_is_iso, "gct_is_iso/I");
    efficiencyTree->Branch("gct_is_looseTkiso", &gct_is_looseTkiso, "gct_is_looseTkiso/I");


    // l1egTree->Branch("run",    &run,     "run/I");
    // l1egTree->Branch("lumi",   &lumi,    "lumi/I");
    // l1egTree->Branch("event",  &event,   "event/I");

    // l1egTree->Branch("gct_l1egPt",  &gct_l1egPt,  "gct_l1egPt/D");
    // l1egTree->Branch("gct_l1egEta", &gct_l1egEta, "gct_l1egEta/D");
    // l1egTree->Branch("gct_l1egPhi", &gct_l1egPhi, "gct_l1egPhi/D");
    // l1egTree->Branch("gct_l1egQual", &gct_l1egQual, "gct_l1egQual/I");

    // l1egTree->Branch("old_l1egPt",  &old_l1egPt,  "old_l1egPt/D");
    // l1egTree->Branch("old_l1egEta", &old_l1egEta, "old_l1egEta/D");
    // l1egTree->Branch("old_l1egPhi", &old_l1egPhi, "old_l1egPhi/D");
    // l1egTree->Branch("old_l1egQual", &old_l1egQual, "old_l1egQual/I");

  }

void L1TCaloEGammaSingleAnalyzer::beginJob(const edm::EventSetup &iSetup) {
}

void L1TCaloEGammaSingleAnalyzer::analyze(const Event& evt, const EventSetup& iSetup ){  
 {

  run = evt.id().run();
  lumi = evt.id().luminosityBlock();
  event = evt.id().event();

  edm::Handle<l1tp2::CaloCrystalClusterCollection> oldCaloCrystalClusters;
  edm::Handle<l1tp2::CaloTowerCollection> oldCaloL1Towers;

  edm::Handle<l1tp2::CaloCrystalClusterCollection> rctCaloCrystalClusters;
  edm::Handle<l1tp2::CaloTowerCollection> rctCaloL1Towers;
  
  edm::Handle<l1tp2::CaloCrystalClusterCollection> gctCaloCrystalClusters;
  edm::Handle<l1tp2::CaloTowerCollection> gctCaloTowers;

  edm::Handle<l1tp2::CaloPFClusterCollection> PFClusters;

  edm::Handle<l1tp2::DigitizedClusterCorrelatorCollection> digitizedClustersCorrelator;
  edm::Handle<l1tp2::EmDigiClusterCollection> emDigiClusters; 
  edm::Handle<l1tp2::HadDigiClusterCollection> hadDigiClusters;

  edm::Handle<BXVector<l1t::EGamma>> l1EGammas;
  edm::Handle<BXVector<l1t::EGamma>> oldL1EGammas;
  edm::Handle<l1tp2::CaloTowerCollection> fullTowers;
  
  edm::Handle<EcalEBTrigPrimDigiCollection> ecalTPGs;
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPGs;  
  edm::Handle<edm::SortedCollection<HcalTriggerPrimitiveDigi> > hbhecoll;
 
  std::vector<Cluster> rctClustersMatched;
  std::vector<Cluster> gctClustersMatched;

  std::map<std::string, float> experimentalParams;

  rctClusters->clear(); 
  rctClusterInfo->clear();
  rctTowers->clear();
  gctClusters->clear();
  gctClusterInfo->clear();
  gctTowers->clear();
  caloPFClusters->clear();
  allEcalTPGs->clear(); 
  allHcalTPGs->clear(); 

  oldClusters->clear();
  oldClusterInfo->clear();

  oldRawIso->clear();
  oldRelIso->clear();
  oldIsoFlag->clear();

  newRawIso->clear();
  newRelIso->clear();
  newIsoFlag->clear();

  oldRawIso->clear();
  oldRelIso->clear();
  oldIsoFlag->clear();

  newRawIso->clear();
  newRelIso->clear();
  newIsoFlag->clear();

  newFullTowers->clear();
  newGCTTowers->clear();

  gct_l1eg->clear();
  // old_l1eg->clear();

  // Detector geometry
  caloGeometry_ = &iSetup.getData(caloGeometryToken_);
  ebGeometry = caloGeometry_->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
  hbGeometry = caloGeometry_->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);
  hcTopology_ = &iSetup.getData(hbTopologyToken_);
  HcalTrigTowerGeometry theTrigTowerGeometry(hcTopology_);

  decoder_ = &iSetup.getData(decoderToken_);


  std::cout << "Doing event " << event << "....: require gen matching? " <<  requireGenMatching_ << std::endl;

  // Check that L1EG objects exist
  if (evt.getByToken(l1EGammasSrc_, l1EGammas)){
    int size = 0;
    for (const auto & l1eg : *l1EGammas ) {
      printf("L1EG object found with pT %f, eta %f, phi %f, ", l1eg.p4().pt(), l1eg.p4().eta(), l1eg.p4().phi());
      ap_uint<3> qual = l1eg.hwQual() & 0b111;
      std::cout << "qual " << std::bitset<3>{qual} << std::endl;
      l1egInfo myL1eg = l1egInfo(l1eg);
      if (l1eg.p4().pt() > 0) {
        gct_l1eg->push_back(myL1eg);
      }
      size++;
    }
    if (size == 0) {
      printf("[WARNING:] NO ENTRIES FOUND IN L1EGAMMAS\n");
    }
  }


  // Check that old L1EG objects exist
  if (evt.getByToken(oldL1EGammasSrc_, oldL1EGammas)){
    int size = 0;
    for (const auto & l1eg : *oldL1EGammas ) {
      printf("Previous emulator L1EG object found with pT %f, eta %f, phi %f, ", l1eg.p4().pt(), l1eg.p4().eta(), l1eg.p4().phi());
      ap_uint<3> qual = l1eg.hwQual() & 0b111;
      std::cout << "qual " << std::bitset<3>{qual} << std::endl;      // l1egInfo myL1eg = l1egInfo(l1eg);
      // if (l1eg.p4().pt() > 0) {
      //   old_l1eg->push_back(myL1eg);
      // }
    }
    if (size == 0) {
      printf("[WARNING:] NO ENTRIES FOUND IN OLD EMULATOR L1EGAMMAS\n");
      size++;
    }
  }
  

  // Unit tests for full towers
  if (evt.getByToken(fullTowersSrc_, fullTowers)) {
    for (const auto & t : *fullTowers ) {
      // printf("GCT full tower found with ET %f, ieta %i, iphi %i, eta %f, phi %f\n", t.ecalTowerEt(), t.towerIEta(), t.towerIPhi(), t.towerEta(), t.towerPhi());
      Tower tower;
      TLorentzVector temp_p4;
      temp_p4.SetPtEtaPhiE(t.ecalTowerEt(), t.towerEta(), t.towerPhi(), t.ecalTowerEt());
      tower.p4 = temp_p4;
      tower.idxEta = t.towerIEta();
      tower.idxPhi = t.towerIPhi();
      newFullTowers->push_back(tower);
    }
  }
  bool allowOverlapInFullTowers = true;
  assert(passesTowerSizeTestWithOverlap(*newFullTowers, allowOverlapInFullTowers));
  assert(passesTowerIndexCoverage(*newFullTowers, allowOverlapInFullTowers));

  // Unit tests for GCT towers
  if (evt.getByToken(gctTowersSrc_, gctCaloTowers)) {
    for (const auto & t : *gctCaloTowers ) {
      // printf("GCT full tower found with ET %f, ieta %i, iphi %i, eta %f, phi %f\n", t.ecalTowerEt(), t.towerIEta(), t.towerIPhi(), t.towerEta(), t.towerPhi());
      Tower tower;
      TLorentzVector temp_p4;
      temp_p4.SetPtEtaPhiE(t.ecalTowerEt(), t.towerEta(), t.towerPhi(), t.ecalTowerEt());
      tower.p4 = temp_p4;
      tower.idxEta = t.towerIEta();
      tower.idxPhi = t.towerIPhi();
      newGCTTowers->push_back(tower);
    }
  }
  std::cout << "GCT towers size: " << newGCTTowers->size() << std::endl;
  bool allowOverlapInGCT = false;
  assert(passesTowerSizeTestWithOverlap(*newGCTTowers, allowOverlapInGCT));
  assert(passesTowerIndexCoverage(*newGCTTowers, allowOverlapInGCT));

  // Get the old emulator clusters from the emulator and sort them by pT
  if(evt.getByToken(oldClustersSrc_, oldCaloCrystalClusters)){
    for(const auto & oldCluster : *oldCaloCrystalClusters){ 
      //fill vector                                                                             
      Cluster temp ;
      TLorentzVector temp_p4;
      std::cout << "Old emulator cluster found: pT " << oldCluster.pt()  << ", "
                << "eta "               << oldCluster.eta() << ", "
                << "phi "               << oldCluster.phi() << ", " 
		            << "iso "               << oldCluster.isolation() << ", " 
                << "raw isolation "     << oldCluster.hovere() 
                << "raw isolation "     << oldCluster.hovere() 
		            << std::endl;
      temp_p4.SetPtEtaPhiE(oldCluster.pt(),oldCluster.eta(),oldCluster.phi(),oldCluster.pt());

      temp.p4 = temp_p4;
      temp.et2x5 = oldCluster.e2x5();  // see https://cmssdt.cern.ch/lxr/source/DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h
      temp.et5x5 = oldCluster.e5x5();
      temp.iso   = oldCluster.isolation();
      temp.rawIso = oldCluster.hovere(); // This is really wrong because for debugging we are saving the raw isolation sum (no division by cluster pT) in the H over e field
      temp.rawIso = oldCluster.hovere(); // This is really wrong because for debugging we are saving the raw isolation sum (no division by cluster pT) in the H over e field

      temp.is_ss         = oldCluster.experimentalParam("standaloneWP_showerShape");
      temp.is_iso        = oldCluster.experimentalParam("standaloneWP_isolation");
      temp.is_looseTkss  = oldCluster.experimentalParam("trkMatchWP_showerShape");
      temp.is_looseTkiso = oldCluster.experimentalParam("trkMatchWP_isolation");
      std::cout << " with flags: " 
                << "is_ss " << temp.is_ss << ","
                << "is_iso " << temp.is_iso << ", "
                << "is_looseTkss " << temp.is_looseTkss << ", "
                << "is_looseTkiso " << temp.is_looseTkiso << std::endl;
      
      // Save the 4-vector
      oldClusters->push_back(temp_p4);
      // Save the full cluster info
      oldClusterInfo->push_back(temp);
    }
  }

  std::sort(oldClusters->begin(), oldClusters->end(), L1TCaloEGammaSingleAnalyzer::comparePt);
  std::sort(oldClusterInfo->begin(), oldClusterInfo->end(), L1TCaloEGammaSingleAnalyzer::compareClusterPt);

  // Old emulator clusters are now sorted; pull the isolation information from them for the event display
  for (const auto & oldCluster : *oldClusterInfo) {
    oldRawIso->push_back(oldCluster.rawIso);
    oldRelIso->push_back(oldCluster.iso);
    oldIsoFlag->push_back(oldCluster.is_iso);
  }

  // Get the RCT clusters from the emulator and sort them by pT
  if(evt.getByToken(rctClustersSrc_, rctCaloCrystalClusters)){
    for(const auto & rctCluster : *rctCaloCrystalClusters){

    //   std::cout << "RCT Cluster found: pT " << rctCluster.pt()  << ", "
		// << "eta "                   << rctCluster.eta() << ", "
		// << "phi "                   << rctCluster.phi() << ", " 
		// << "et 2x5"                 << rctCluster.e2x5() << ", "
		// << "et 5x5"                 << rctCluster.e5x5() << ", "
		// << "is_ss"                  << rctCluster.experimentalParam("standaloneWP_showerShape") << ", "
		// << "is_looseTkss"           << rctCluster.experimentalParam("trkMatchWP_showerShape") << std::endl;
      
      Cluster temp ;
      TLorentzVector temp_p4;
      temp_p4.SetPtEtaPhiE(rctCluster.pt(),rctCluster.eta(),rctCluster.phi(),rctCluster.pt());

      temp.p4 = temp_p4;

      temp.et2x5 = rctCluster.e2x5();
      temp.et5x5 = rctCluster.e5x5();
      temp.is_ss         = rctCluster.experimentalParam("standaloneWP_showerShape");
      temp.is_looseTkss  = rctCluster.experimentalParam("trkMatchWP_showerShape");

      // Save the 4-vector
      rctClusters->push_back(temp_p4);
      // Save the full cluster info 
      rctClusterInfo->push_back(temp);

    }
  }

  std::sort(rctClusters->begin(), rctClusters->end(), L1TCaloEGammaSingleAnalyzer::comparePt);
  std::sort(rctClusterInfo->begin(), rctClusterInfo->end(), L1TCaloEGammaSingleAnalyzer::compareClusterPt);

  // Get the RCT towers from the emulator 
  if(evt.getByToken(rctTowersSrc_, rctCaloL1Towers)) {
    for (const auto & rctTower : *rctCaloL1Towers){
      TLorentzVector temp;
      float totalEt = rctTower.ecalTowerEt() + rctTower.hcalTowerEt();
      if (totalEt > 0) {
	// std::cout << "Tower found: ECAL ET " << rctTower.ecalTowerEt()  << ", "
	// 	  << "HCAL ET " << rctTower.hcalTowerEt()  << ", "
	// 	  << "iEta, iPhi " << rctTower.towerIEta() << " " << rctTower.towerIPhi() << ", "
	// 	  << "eta "             << rctTower.towerEta() << ", "
	// 	  << "phi "             << rctTower.towerPhi() << std::endl;
	temp.SetPtEtaPhiE(totalEt,
			  rctTower.towerEta(), rctTower.towerPhi(),
			  totalEt);
	rctTowers->push_back(temp);
      }
    }
  }
  
  // Get the GCT clusters from the emulator, and sort them by pT
  if(evt.getByToken(gctClustersSrc_, gctCaloCrystalClusters)){
    for(const auto & gctCluster : *gctCaloCrystalClusters){
      //fill vector                                                                             
      Cluster temp ;
      TLorentzVector temp_p4;
      std::cout << "GCT Cluster found: pT " << gctCluster.pt()  << ", "
                << "eta "               << gctCluster.eta() << ", "
                << "phi "               << gctCluster.phi() << ", " 
		            << "iso "               << gctCluster.isolation() << ", " 
                << "raw iso (unscaled) " << gctCluster.hovere() 
		            << std::endl;
      temp_p4.SetPtEtaPhiE(gctCluster.pt(),gctCluster.eta(),gctCluster.phi(),gctCluster.pt());

      temp.p4 = temp_p4;
      temp.et2x5 = gctCluster.e2x5();  // see https://cmssdt.cern.ch/lxr/source/DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h
      temp.et5x5 = gctCluster.e5x5();
      temp.iso   = gctCluster.isolation();
      temp.rawIso = gctCluster.hovere(); // this is really wrong but we need the raw isolaion sum for debugging

      temp.is_ss         = gctCluster.experimentalParam("standaloneWP_showerShape");
      temp.is_iso        = gctCluster.experimentalParam("standaloneWP_isolation");
      temp.is_looseTkss  = gctCluster.experimentalParam("trkMatchWP_showerShape");
      temp.is_looseTkiso = gctCluster.experimentalParam("trkMatchWP_isolation");
      // std::cout << " with flags: " 
      //           << "is_ss " << temp.is_ss << ","
      //           << "is_iso " << temp.is_iso << ", "
      //           << "is_looseTkss " << temp.is_looseTkss << ", "
      //           << "is_looseTkiso " << temp.is_looseTkiso << std::endl;
      
      // Save the 4-vector
      gctClusters->push_back(temp_p4);
      // Save the full cluster info
      gctClusterInfo->push_back(temp);
    }
  }
  std::sort(gctClusters->begin(), gctClusters->end(), L1TCaloEGammaSingleAnalyzer::comparePt);
  std::sort(gctClusterInfo->begin(), gctClusterInfo->end(), L1TCaloEGammaSingleAnalyzer::compareClusterPt);

  // GCT clusters are now sorted; pull the isolation information from them for the event display
  for (const auto & newCluster : *gctClusterInfo) {
    newRawIso->push_back(newCluster.rawIso);
    newRelIso->push_back(newCluster.iso);
    newIsoFlag->push_back(newCluster.is_iso);
  }

  // Sort the L1EG info
  std::sort(gct_l1eg->begin(), gct_l1eg->end(), L1TCaloEGammaSingleAnalyzer::compareL1EGPt);


  // Get the PFClusters
  if (evt.getByToken(caloPFClustersSrc_, PFClusters)) {
    for (const auto & PFCluster : *PFClusters) {
      std::cout << "PF cluster found: ET " << PFCluster.clusterEt()  << ", "
                << "iEta, iPhi " << PFCluster.clusterIEta() << " " << PFCluster.clusterIPhi() << ", "
                << "eta, phi " << PFCluster.clusterEta() << " " << PFCluster.clusterPhi() << std::endl;
      TLorentzVector temp;
      temp.SetPtEtaPhiE(PFCluster.clusterEt(), PFCluster.clusterEta(), PFCluster.clusterPhi(), PFCluster.clusterEt());
      caloPFClusters->push_back(temp);
    }
  }
  else {
    std::cout << "[ERROR: ] Did not find any PF Clusters" << std::endl;
  }

  // Cross-check: check the DigitizedClusterCorrelator
  if (evt.getByToken(digitizedClustersCorrelatorSrc_, digitizedClustersCorrelator)) {
    for (const auto & cluster: *digitizedClustersCorrelator) {
      std::cout << "DigitizedClusterCorrelator: pT " << cluster.pt() * cluster.ptLSB()
        << " crystal iEta, iPhi " << cluster.eta() << ", " << cluster.phi() 
        << " real eta, real phi " << cluster.realEta() << ", " << cluster.realPhi() 
        << " iso flags " << cluster.isoFlags() << " shape flags " << cluster.shapeFlags() << std::endl;
    }
  }

  // Get the clusters going from GCT barrel to correlator layer 1
  float slrCentersInDegrees[6] = {10.0, 70.0, 130.0, -170.0, -110.0, -50.0};

  if (evt.getByToken(emDigiClustersSrc_, emDigiClusters)) {
    for (int iLink = 0; iLink < 12; iLink++) {
      // in order: GCT1 SLR1 positive eta, GCT SLR1 negative eta, GCT1 SLR3 positive eta, etc. 
      int iSLR = (iLink/2);
      // Eta: positive or negative depends on the link. If iLink is even, it is in positive eta
      bool isNegativeEta = (iLink % 2);

        int i = 0;
        for (const auto & cluster : (*emDigiClusters).at(iLink)) {
          i++;
          // Example code of how to get to the real eta and phi from the position 
          // Add offset of half-crystal size to get center of crystal in eta
          // Eta is stored as an unsigned quantity, the sign is inferred from the link
          float realEta = p2eg::half_crystal_size + (cluster.eta() * (p2eg::ECAL_eta_range / (p2eg::n_towers_cardEta * p2eg::CRYSTALS_IN_TOWER_ETA)));
          if (isNegativeEta) {
            realEta *= -1;
          }
          // else {
          //   // positive eta: add offset when getting real eta (so that iEta = 0 in negative eta and iEta = 0 in positive eta map to different real eta)
          //   realEta += ((cluster.eta() / std::abs(cluster.eta())) * p2eg::half_crystal_size);
          // }
          // Phi is signed. Units are in degrees. Add half a degree for offset. Use the p2eg::wrappedPhi helper to make sure it returns a value between -180 and +180, and lastly convert to radians
          float realPhi = p2eg::wrappedPhiInDegrees(slrCentersInDegrees[iSLR] + cluster.phi() + 0.5) * (M_PI / 180.);

          std::cout << "EG to Correlator Layer 1, SLR " << iSLR << ", link " << iLink
                    << ": pT " << cluster.ptFloat()
                    << " crystal iEta, iPhi " << cluster.eta() << "," << cluster.phi() 
                    << " real eta, phi " << realEta << ", " << realPhi
                    << " iso flags " << cluster.isoFlags() << " shape flags " << cluster.shapeFlags() << std::endl;
          if (cluster.clusterRef().isNonnull()) {
            std::cout << "\t ... Access underlying float cluster pT " << cluster.clusterRef()->pt() 
                      << " eta, phi " << cluster.clusterRef()->eta() << ", " << cluster.clusterRef()->phi()
                      << " with standalone WP shower shape " << cluster.clusterRef()->experimentalParam("standaloneWP_showerShape")
                      << " with standalone WP isolation " << cluster.clusterRef()->experimentalParam("standaloneWP_isolation") 
                      << std::endl;
          }
          if (cluster.digiClusterRef().isNonnull()) {
                      std::cout << "\t ... Access underlying digitized cluster pT " << cluster.digiClusterRef()->ptFloat() 
                      << " eta, phi " << cluster.digiClusterRef()->realEta() << ", " << cluster.digiClusterRef()->realPhi() << std::endl;
          }
          else {
            std::cout << "\t ... Base cluster reference is null, continuing" << std::endl;
          }
      }
      std::cout << ">>> I found " << i << " EM Digi clusters in iLink " << iLink << std::endl;

    }
  }
  else {
    std::cout << "[ERROR: ] Did not find any EM Digi clusters for correlator layer 1" << std::endl;
  }

  if (evt.getByToken(hadDigiClustersSrc_, hadDigiClusters)) {
    for (int iLink = 0; iLink < 12; iLink++) {
      int i = 0;
      // in order: GCT1 SLR1 positive eta, GCT SLR1 negative eta, GCT1 SLR3 positive eta, etc. 
      int iGCT = (iLink/2);
      // Eta: positive or negative depends on the link. If iLink is even, it is in positive eta
      bool isNegativeEta = (iLink % 2);

      for (const auto & cluster : (*hadDigiClusters).at(iLink)) {
        i++;

        // Example code of how to get to the real eta and phi from the position 
        float realEta;
        float realPhi;

        // Add offset of half-crystal size to get center of crystal in eta and phi
        realEta = (cluster.eta() + 0.5) * (p2eg::ECAL_eta_range / (p2eg::n_towers_cardEta * p2eg::CRYSTALS_IN_TOWER_ETA));
        if (isNegativeEta) {
          realEta *= -1;
        }
        // Add 0.5 to get offset of half a crystal (0.5 degrees in phi)
        realPhi = p2eg::wrappedPhiInDegrees(slrCentersInDegrees[iGCT] + cluster.phi() + 0.5) * (M_PI / 180.);

        std::cout << "PF to Correlator Layer 1, GCT " << iGCT << ", SLR " << iLink
                  << ": pT " << cluster.ptFloat()
                  << " crystal iEta, iPhi " << cluster.eta() << "," << cluster.phi() 
                  << " real eta, phi " << realEta << ", " << realPhi  << std::endl;

          if (cluster.clusterRef().isNonnull()) {
                  std::cout << "\t ... Underlying float cluster pT " << cluster.clusterRef()->clusterEt() << " at " 
                            << cluster.clusterRef()->clusterEta() << ", " << cluster.clusterRef()->clusterPhi() << std::endl;
          }
          else {
            std::cout << "\t ... No associated cluster" << std::endl;
          }
      }
      std::cout << ">>> I found " << i << " digitized PF clusters in iLink " << iLink << std::endl;
    }
  }
  else {
    std::cout << "[ERROR: ] Did not find any HadDigiClusters" << std::endl;
  }


  // get the ECAL inputs (i.e. ECAL crystals)
  if(!evt.getByToken(ecalSrc_, ecalTPGs))
    std::cout<<"ERROR GETTING THE ECAL TPGS"<<std::endl;
  else
    for (const auto& hit : *ecalTPGs.product()) {
      if (hit.encodedEt() > 0)  // hit.encodedEt() returns an int corresponding to 2x the crystal Et
	{
	  // Et is 10 bit, by keeping the ADC saturation Et at 120 GeV it means that you have to divide by 8
	  float et = hit.encodedEt() / 8.;
	  
	  if (et < 0.5)
	    continue;  // keep the 500 MeV ET Cut
	  
	  // std::cout << "ECAL hit et: " << et << std::endl;
	  
	  // Get cell coordinates and info
	  auto cell = ebGeometry->getGeometry(hit.id());
	  
	  // std::cout << "Found ECAL cell/hit with coordinates " << cell->getPosition().x() << "," 
	  // 	  << cell->getPosition().y() << "," 
	  // 	  << cell->getPosition().z() << " and ET (GeV) " 
	  // 	  << et << std::endl;
	  
	  GlobalVector position=GlobalVector(cell->getPosition().x(), cell->getPosition().y(), cell->getPosition().z());
	  float eta = position.eta();
	  float phi = position.phi();
	  TLorentzVector temp ;
	  temp.SetPtEtaPhiE(et,eta,phi,et);
	  
	  // int cc = 28;
	  // if (getCrystal_phiID(position.phi()) <= getPhiMax_card(cc) &&
	  //     getCrystal_phiID(position.phi()) >= getPhiMin_card(cc) &&
	  //     getCrystal_etaID(position.eta()) <= getEtaMax_card(cc) &&
	  //     getCrystal_etaID(position.eta()) >= getEtaMin_card(cc)){

	  //   // TEMP: only use ECAL hits in Card 28                                                                             
	  //   allEcalTPGs->push_back(temp); 

	  //   if ((getCrystal_etaID(position.eta()) > 29) && (getCrystal_etaID(position.eta()) < 35)) {
	  //     std::cout << "[CARD " << cc << "]: Found ECAL cell/hit with eta/phi "
	  // 		<< position.eta() << ", "
	  // 		<< position.phi() << ", and in-detector phiID and etaID "
	  // 		<< getCrystal_phiID(position.phi()) << ", "
	  // 		<< getCrystal_etaID(position.eta()) << ", and ET (GeV) "
	  // 		<< et << std::endl;
	  //   }
	  // }
	  // TEMP: Debugging purposes only: only add ECAL hits in Card 28
	  allEcalTPGs->push_back(temp);
	}
    }
  
  if(!evt.getByToken(hcalSrc_, hcalTPGs))
    std::cout<<"ERROR GETTING THE HCAL TPGS"<<std::endl;
  else
  for (const auto& hit : *hcalTPGs.product()) {
    float et = decoder_->hcaletValue(hit.id(), hit.t0());
    ap_uint<10> encodedEt = hit.t0().compressedEt(); 
    // same thing as SOI_compressedEt() in HcalTriggerPrimitiveDigi.h///
    if (et <= 0)
      continue;
    
    if (!(hcTopology_->validHT(hit.id()))) {
      LogError("Phase2L1CaloEGammaEmulator")
  	<< " -- Hcal hit DetID not present in HCAL Geom: " << hit.id() << std::endl;
      throw cms::Exception("Phase2L1CaloEGammaEmulator");
      continue;
    }
    const std::vector<HcalDetId>& hcId = theTrigTowerGeometry.detIds(hit.id());
    if (hcId.empty()) {
      LogError("Phase2L1CaloEGammaEmulator")
  	<< "Cannot find any HCalDetId corresponding to " << hit.id() << std::endl;
      throw cms::Exception("Phase2L1CaloEGammaEmulator");
      continue;
    }
    if (hcId[0].subdetId() > 1)
      continue;
    GlobalVector hcal_tp_position = GlobalVector(0., 0., 0.);
    for (const auto& hcId_i : hcId) {
      if (hcId_i.subdetId() > 1)
        continue;
      // get the first HCAL TP/ cell
      auto cell = hbGeometry->getGeometry(hcId_i);
      if (cell == nullptr)
  	continue;
      GlobalVector tmpVector = GlobalVector(cell->getPosition().x(), cell->getPosition().y(), cell->getPosition().z());
      hcal_tp_position = tmpVector;
      
      // std::cout << "Found HCAL cell/TP with coordinates " << cell->getPosition().x() << ","
      //  		<< cell->getPosition().y() << ","
      //  		<< cell->getPosition().z() << " and ET (GeV) " << et
      // 		<< ", encoded Et " << encodedEt << std::endl;
      
      break;
    }
  
    float eta = hcal_tp_position.eta();
    float phi = hcal_tp_position.phi();
    TLorentzVector temp ;
    temp.SetPtEtaPhiE(et,eta,phi,et);
    allHcalTPGs->push_back(temp);
  }

  // Get genParticles and build a vector of genElectrons
  edm::Handle<GenParticleCollectionType> genParticleHandle;
  if(!evt.getByToken(genToken_,genParticleHandle))
    std::cout<<"No gen Particles Found "<<std::endl;
  else
    std::cout<<"Gen Particles size "<<genParticleHandle->size()<<std::endl;
  
  std::vector<reco::GenParticle> genElectrons;
  std::vector<reco::GenParticle> genParticles;
  
  for (unsigned int i = 0; i< genParticleHandle->size(); ++i){
    edm::Ptr<reco::GenParticle> ptr(genParticleHandle, i);
    genParticles.push_back(*ptr);

    printf("my pdg id is %i and my eta is %f\n", ptr->pdgId(), ptr->eta());

    
    // Get gen electrons in barrel + overlap
    if ( (abs(ptr->pdgId()) == 11) && ( abs(ptr->eta()) < 1.4841 )) {
      genElectrons.push_back(*ptr);
      // Check isLastCopy() and isLastCopyBeforeFSR()
      std::cout << "isLastCopy: " << ptr->isLastCopy()  << ", "
		<< "isLastCopyBeforeFSR: " << ptr->isLastCopyBeforeFSR() << std::endl;
      
      std::cout << "Added genElectron " << ptr->pt() << std::endl;
    }
  }

  //************************************************************************************/
  // ECAL propagation of gen electrons
  //************************************************************************************/
  std::vector<TLorentzVector> propagatedGenElectrons;

  for (auto genElectron : genElectrons) {
    RawParticle particle(genElectron.p4());
    particle.setVertex(genElectron.vertex().x(), genElectron.vertex().y(), genElectron.vertex().z(), 0.);
    if (fabs(genElectron.pdgId())==11) particle.setMass(.511);
    else particle.setMass(0.);
    
    int pdgId = genElectron.pdgId();
    if (pdgId > 0)  particle.setCharge( -1.0 ); 
    if (pdgId < 0)  particle.setCharge( 1.0 ); 

    float field_z = 4;
    BaseParticlePropagator prop(particle, 0., 0., field_z);
    prop.propagateToEcalEntrance();
    if( prop.getSuccess() != 0 ) {
      // Not sure what this is in the code snippet
      // TLorentzVector trueElectron = TLorentzVector(prop.particle().E()*sin(prop.particle().vertex().theta()),
      // 						   prop.particle().vertex().eta(),
      // 						   prop.particle().vertex().phi(),
      // 						   0.);
      GlobalPoint ecal_pos(prop.particle().vertex().x(), prop.particle().vertex().y(), prop.particle().vertex().z());
      TLorentzVector corrGenElectron;
      // corrGenElectron.SetPtEtaPhiM(prop.particle().Pt(),
      // 				   prop.particle().eta(),
      // 				   prop.particle().phi(),
      // 				   prop.particle().mass());

      corrGenElectron.SetPtEtaPhiM(prop.particle().Pt(),
				   ecal_pos.eta(),
				   ecal_pos.phi(),
				   prop.particle().mass());
      
      std::cout << ">>> Corrected gen electron pt/eta/phi: (" 
		<< genElectron.pt() << ", " << genElectron.eta() << ", " << genElectron.phi() 
		<< ")"
		<< " to " 
		<< corrGenElectron.Pt() << ", " << corrGenElectron.Eta() << ", " << corrGenElectron.Phi()
		<< " with ecal_pos eta/phi "
		<< ecal_pos.eta() << ", " << ecal_pos.phi()
		<< std::endl;
      propagatedGenElectrons.push_back(corrGenElectron);
    }
  }

  //************************************************************************************/ 
  // Loop through the gen-level clusters and match to the RCT clusters, then the GCT clusters.
  //************************************************************************************/ 
  if (requireGenMatching_) {

    for (auto genElectron : propagatedGenElectrons) {

      genPt = genElectron.Pt();
      genEta = genElectron.Eta();
      genPhi = genElectron.Phi();

      rctClustersMatched.clear();
      gctClustersMatched.clear();

      rct_cPt    = 0;    rct_cEta   = -999;    rct_cPhi   = -999;
      rct_deltaR = 999;

      // Loop through the RCT clusters which are already sorted in decreasing pT, and check for
      // the first cluster within deltaR < 0.5. 
      std::cout << "Event " << event << ": check that RCT clusters are sorted!" << std::endl;
    
      for (size_t i = 0; i < rctClusterInfo->size(); ++i) {

        float this_rct_deltaR = reco::deltaR(rctClusterInfo->at(i).p4.Eta(), rctClusterInfo->at(i).p4.Phi(),
              genElectron.Eta(), genElectron.Phi());
        // std::cout << "   Comparing "<< this_rct_deltaR << " to current rct_deltaR " << rct_deltaR << std::endl;

        if (this_rct_deltaR < 0.5) {

          // std::cout << "rctClusterInfo pT " << rctClusterInfo->at(i).Pt() 
          // 	  << " eta " << rctClusterInfo->at(i).Eta()
          // 	  << " phi " << rctClusterInfo->at(i).Phi() << std::endl;
          
          TLorentzVector temp_p4;
          temp_p4.SetPtEtaPhiE(rctClusterInfo->at(i).p4.Pt(), rctClusterInfo->at(i).p4.Eta(),
                  rctClusterInfo->at(i).p4.Phi(), rctClusterInfo->at(i).p4.M());
          Cluster temp;
          temp.p4 = temp_p4;
          temp.et2x5 = rctClusterInfo->at(i).et2x5;
          temp.et5x5 = rctClusterInfo->at(i).et5x5;
          temp.is_ss = rctClusterInfo->at(i).is_ss;
          temp.is_looseTkss = rctClusterInfo->at(i).is_looseTkss;
          rctClustersMatched.push_back(temp);
        }
      }
      
      // For this gen electron, sort the matched clusters by pT, and only save the highest pT one
      std::sort(rctClustersMatched.begin(), rctClustersMatched.end(), L1TCaloEGammaSingleAnalyzer::compareClusterPt);
      if (rctClustersMatched.size() > 0) {
        rct_cPt  = rctClustersMatched.at(0).p4.Pt();
        rct_cEta = rctClustersMatched.at(0).p4.Eta();
        rct_cPhi = rctClustersMatched.at(0).p4.Phi();
        rct_deltaR = reco::deltaR(rct_cEta, rct_cPhi,
          genElectron.Eta(), genElectron.Phi());
        rct_et2x5 = rctClustersMatched.at(0).et2x5;
        rct_et5x5 = rctClustersMatched.at(0).et5x5; 

        std::cout << "--> Matched RCT cluster " << rct_cPt 
                  << " eta: " << rct_cEta
                  << " phi: " << rct_cPhi
                  << " with genElectron " << genPt
                  << " eta: " << genEta
                  << " phi: " << genPhi 
                  << " et2x5: " << rct_et2x5 
                  << " et5x5: " << rct_et5x5 << std::endl;
      }
      
      //************************************************************************************/
      // Loop through the GCT cluster 4-vectors, and gen-match. Only take the first 
      // GCT cluster which has deltaR < 0.5.
      //************************************************************************************/ 
      std::cout << ">>> Doing GCT clusters..." << std::endl;
      
      gct_cPt   = 0;    gct_cEta   = -999;   gct_cPhi  = -999;
      gct_deltaR = 999;
      gct_iso = 0;
      gct_et2x5 = 0; gct_et5x5 = 0;
      gct_is_ss = 0; gct_is_looseTkss = 0;
      gct_is_iso = 0; gct_is_looseTkiso = 0;

      for (size_t i = 0; i < gctClusterInfo->size(); ++i) {
        // std::cout << " gctClusterInfo pT " << gctClusterInfo->at(i).p4.Pt() 
        //           << " eta "               << gctClusterInfo->at(i).p4.Eta()
        //           << " phi "               << gctClusterInfo->at(i).p4.Phi() << std::endl;
        float this_gct_deltaR = reco::deltaR(gctClusterInfo->at(i).p4.Eta(), gctClusterInfo->at(i).p4.Phi(),
                                             genElectron.Eta(), genElectron.Phi());
        if (this_gct_deltaR < 0.5) {
          TLorentzVector temp_p4;
          temp_p4.SetPtEtaPhiE(gctClusterInfo->at(i).p4.Pt(), gctClusterInfo->at(i).p4.Eta(),
                               gctClusterInfo->at(i).p4.Phi(), gctClusterInfo->at(i).p4.M());

          Cluster myTemp;
          myTemp.p4    = temp_p4;
          myTemp.iso   = gctClusterInfo->at(i).iso;
          myTemp.et2x5 = gctClusterInfo->at(i).et2x5;
          myTemp.et5x5 = gctClusterInfo->at(i).et5x5;
          myTemp.is_ss = gctClusterInfo->at(i).is_ss;
          myTemp.is_looseTkss = gctClusterInfo->at(i).is_looseTkss;
          myTemp.is_iso = gctClusterInfo->at(i).is_iso;
          myTemp.is_looseTkiso = gctClusterInfo->at(i).is_looseTkiso;

          gctClustersMatched.push_back(myTemp);

        }
      }
    
    
      // For this gen electron, sort the matched clusters by pT, and only save the highest pT one     
      std::sort(gctClustersMatched.begin(), gctClustersMatched.end(), L1TCaloEGammaSingleAnalyzer::compareClusterPt);

      if (gctClustersMatched.size() > 0) {
        gct_cPt  = gctClustersMatched.at(0).p4.Pt();
        gct_cEta = gctClustersMatched.at(0).p4.Eta();
        gct_cPhi = gctClustersMatched.at(0).p4.Phi();
        gct_deltaR = reco::deltaR(gct_cEta, gct_cPhi, genElectron.Eta(), genElectron.Phi());
        gct_iso   = gctClustersMatched.at(0).iso;
        gct_et2x5 = gctClustersMatched.at(0).et2x5;
        gct_et5x5 = gctClustersMatched.at(0).et5x5; 
        gct_is_ss = gctClustersMatched.at(0).is_ss;
        gct_is_looseTkss = gctClustersMatched.at(0).is_looseTkss;
        gct_is_iso = gctClustersMatched.at(0).is_iso;
        gct_is_looseTkiso = gctClustersMatched.at(0).is_looseTkiso;
        
        std::cout << "--> Matched GCT cluster " << gct_cPt
                  << " eta: " << gct_cEta
                  << " phi: " << gct_cPhi
                  << " with genElectron " << genPt
                  << " eta: " << genEta
                  << " phi: " << genPhi  << ". "
                  << " iso: "   << gct_iso
                  << " et2x5: " << gct_et2x5 
                  << " et5x5: " << gct_et5x5 
                  << " is_ss: " << gct_is_ss 
                  << " is_looseTkss: " << gct_is_looseTkss 
                  << " is_iso: " << gct_is_iso 
                  << " is_looseTkiso: " << gct_is_looseTkiso 
                  << std::endl;
      }
      
      efficiencyTree->Fill();

    } // end of loop over gen electrons
  } // end of requireGenMatching = true block
  else {

    // Do not require gen matching: not an event-level consideration, save all the clusters we can get
    std::cout << ">>> [!!!] Saving all GCT Clusters without gen-matching..." << std::endl;
      
      gct_cPt   = 0;    gct_cEta   = -999;   gct_cPhi  = -999;
      gct_deltaR = 999;
      gct_iso = 0;
      gct_et2x5 = 0; gct_et5x5 = 0;
      gct_is_ss = 0; gct_is_looseTkss = 0;
      gct_is_iso = 0; gct_is_looseTkiso = 0;

      old_cPt   = 0;    old_cEta   = -999;   old_cPhi  = -999;
      old_deltaR = 999;
      old_iso = 0;
      old_et2x5 = 0; old_et5x5 = 0;
      old_is_ss = 0; old_is_looseTkss = 0;
      old_is_iso = 0; old_is_looseTkiso = 0;


      size_t maxToSave_newEmulator;
      if (saveOnlyHighestPtCluster_) {
        std::cout << "[WARNING:] Only saving highest pT cluster..." << std::endl;
        maxToSave_newEmulator = 1;
      }
      else {
        maxToSave_newEmulator = gctClusterInfo->size();
        std::cout << "[NOTE:] Saving all clusters..." << std::endl;

      }

      for (size_t i = 0; i < maxToSave_newEmulator; ++i) {
        std::cout << " gctClusterInfo pT " << gctClusterInfo->at(i).p4.Pt() 
                  << " eta "               << gctClusterInfo->at(i).p4.Eta()
                  << " phi "               << gctClusterInfo->at(i).p4.Phi() << std::endl;

        gct_cPt  = gctClusterInfo->at(i).p4.Pt();
        gct_cEta = gctClusterInfo->at(i).p4.Eta();
        gct_cPhi = gctClusterInfo->at(i).p4.Phi();
        gct_iso   = gctClusterInfo->at(i).iso;
        gct_et2x5 = gctClusterInfo->at(i).et2x5;
        gct_et5x5 = gctClusterInfo->at(i).et5x5; 
        gct_is_ss = gctClusterInfo->at(i).is_ss;
        gct_is_looseTkss = gctClusterInfo->at(i).is_looseTkss;
        gct_is_iso = gctClusterInfo->at(i).is_iso;
        gct_is_looseTkiso = gctClusterInfo->at(i).is_looseTkiso;

        efficiencyTree->Fill();

      }
      
  }  
 }

  //************************************************************************************/ 
  // Unit test: Check that each L1EG is matched to a GCT cluster (ordinarily, should be trivial because one L1EG is created for each GCT)
  //************************************************************************************/ 
  for (const auto & newl1eg : *gct_l1eg) {
    std::cout << "L1 eg at eta, phi " << newl1eg.eta << ", " << newl1eg.phi << " with pT " << newl1eg.pt << std::endl;
  }
  for (const auto & gctCluster : *gctClusterInfo) {
    std::cout << "GCT cluster at eta, phi " << gctCluster.p4.Eta() << ", " << gctCluster.p4.Phi() << " with pT " << gctCluster.p4.Pt() << std::endl;
  }
  // Only fill once per event
  dispTree->Fill();


  for (const auto & newl1eg : *gct_l1eg) {

    
    bool deltaRmatched = false;
    bool hasSimilarPt = false;
    for (const auto & gctCluster : *gctClusterInfo) {
      if (reco::deltaR(newl1eg.eta, newl1eg.phi, gctCluster.p4.Eta(), gctCluster.p4.Phi()) < 0.05) {
        deltaRmatched = true;
        // If pT difference is greater than 20%, throw an error
        if (!hasSimilarPt){
          hasSimilarPt = ( (abs(newl1eg.pt - gctCluster.p4.Pt()) / gctCluster.p4.Pt()) < 0.20);
        }
      } 
    }
    if (!deltaRmatched) {
      LogError("Phase2L1CaloEGammaEmulator")
  	        << " -- No GCT match found for a L1EG object at " << newl1eg.eta << ", " << newl1eg.phi << std::endl;
      throw cms::Exception("Phase2L1CaloEGammaEmulator");
      continue;
    }
    if (!hasSimilarPt) {
      LogError("Phase2L1CaloEGammaEmulator")
  	        << " -- pT mismatch for L1EG object at " << newl1eg.eta << ", " << newl1eg.phi << std::endl;
      throw cms::Exception("Phase2L1CaloEGammaEmulator");
      continue;
    }

  }

}
void L1TCaloEGammaSingleAnalyzer::endJob() { 
    
}

L1TCaloEGammaSingleAnalyzer::~L1TCaloEGammaSingleAnalyzer(){
}

DEFINE_FWK_MODULE(L1TCaloEGammaSingleAnalyzer);
