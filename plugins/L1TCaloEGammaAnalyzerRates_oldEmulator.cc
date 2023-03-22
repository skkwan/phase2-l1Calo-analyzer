/*
 *  \file L1TCaloEGammaAnalyzerRates_oldEmulator.cc
 *  Authors S. Kwan, P. Das, I. Ojalvo
 */

// system include files
#include <ap_int.h>
#include <array>
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

// Output tower collection
#include "DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h"
#include "DataFormats/L1TCalorimeterPhase2/interface/CaloTower.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"

#include "L1Trigger/L1CaloTrigger/interface/ParametricCalibration.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "L1Trigger/L1CaloPhase2Analyzer/interface/L1TCaloEGammaAnalyzerRates_oldEmulator.h"
#include "DataFormats/Math/interface/deltaR.h"


// ECAL propagation
#include "CommonTools/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "CommonTools/BaseParticlePropagator/interface/RawParticle.h"


using namespace edm;
using std::cout;
using std::endl;
using std::vector;

L1TCaloEGammaAnalyzerRates_oldEmulator::L1TCaloEGammaAnalyzerRates_oldEmulator(const edm::ParameterSet& cfg) :
  decoderToken_(esConsumes<CaloTPGTranscoder, CaloTPGRecord>(edm::ESInputTag("", ""))),
  caloGeometryToken_(esConsumes<CaloGeometry, CaloGeometryRecord>(edm::ESInputTag("", ""))),
  hbTopologyToken_(esConsumes<HcalTopology, HcalRecNumberingRecord>(edm::ESInputTag("", ""))),
  ecalSrc_(consumes<EcalEBTrigPrimDigiCollection>(cfg.getParameter<edm::InputTag>("ecalDigis"))),
  hcalSrc_(consumes<HcalTrigPrimDigiCollection>(cfg.getParameter<edm::InputTag>("hcalDigis"))),
  gctClustersSrc_(consumes<l1tp2::CaloCrystalClusterCollection >(cfg.getParameter<edm::InputTag>("clusters"))),
  genSrc_ (( cfg.getParameter<edm::InputTag>( "genParticles")))
{
  genToken_ =     consumes<std::vector<reco::GenParticle> >(genSrc_);

    folderName_          = cfg.getUntrackedParameter<std::string>("folderName");
    efficiencyTree = tfs_->make<TTree>("efficiencyTree", "Efficiency Tree");
    
    ////putting bufsize at 32000 and changing split level to 0 so that the branch isn't split into multiple branches
    efficiencyTree->Branch("hcalTPGs", "vector<TLorentzVector>", &allHcalTPGs, 32000, 0); 
    efficiencyTree->Branch("ecalTPGs", "vector<TLorentzVector>", &allEcalTPGs, 32000, 0); 

    efficiencyTree->Branch("gctClusters", "vector<TLorentzVector>", &gctClusters, 32000, 0);
    efficiencyTree->Branch("gctTowers",   "vector<TLorentzVector>", &gctTowers, 32000, 0);
    
    efficiencyTree->Branch("run",    &run,     "run/I");
    efficiencyTree->Branch("lumi",   &lumi,    "lumi/I");
    efficiencyTree->Branch("event",  &event,   "event/I");

    // Gen electron (only highest pT one per event)
    efficiencyTree->Branch("genPt",  &genPt,  "genPt/D");
    efficiencyTree->Branch("genEta", &genEta, "genEta/D");
    efficiencyTree->Branch("genPhi", &genPhi, "genPhi/D");

    // GCT cluster that was matched to the highest pT electron per event
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
    
    nEvents = tfs_->make<TH1F>( "nEvents", "nEvents",  2,  0., 2. );

    // Histograms for rates
    l1eg_pt       = tfs_->make<TH1F>( "l1eg_pt"        , "p_{t}", 150,  0., 150. );
    l1egVLoose_pt = tfs_->make<TH1F>( "l1egVLoose_pt"  , "p_{t}", 150,  0., 150. );
    l1egLoose_pt  = tfs_->make<TH1F>( "l1egLoose_pt"   , "p_{t}", 150,  0., 150. );
    l1egMedium_pt = tfs_->make<TH1F>( "l1egMedium_pt"  , "p_{t}", 150,  0., 150. );
    l1egTight_pt  = tfs_->make<TH1F>( "l1egTight_pt"   , "p_{t}", 150,  0., 150. );

    l1eg_pt_is_iso = tfs_->make<TH1F>( "l1eg_pt_is_iso", "p_{t}", 150,  0., 150. );
    l1eg_pt_is_ss  = tfs_->make<TH1F>( "l1eg_pt_is_ss", "p_{t}", 150,  0., 150. );
    l1eg_pt_is_iso_is_ss = tfs_->make<TH1F>( "l1eg_pt_is_iso_is_ss", "p_{t}", 150,  0., 150. );

    l1eg_pt_is_looseTkiso = tfs_->make<TH1F>( "l1eg_pt_is_looseTkiso", "p_{t}", 150,  0., 150. );
    l1eg_pt_is_looseTkss  = tfs_->make<TH1F>( "l1eg_pt_is_looseTkss", "p_{t}", 150,  0., 150. );
    l1eg_pt_is_looseTkiso_is_looseTkss = tfs_->make<TH1F>( "l1eg_pt_is_looseTkiso_is_looseTkss", "p_{t}", 150,  0., 150. );

  }

void L1TCaloEGammaAnalyzerRates_oldEmulator::beginJob(const edm::EventSetup & iSetup) {
}

void L1TCaloEGammaAnalyzerRates_oldEmulator::analyze(const edm::Event& evt, const edm::EventSetup& iSetup )
 {

  run = evt.id().run();
  lumi = evt.id().luminosityBlock();
  event = evt.id().event();
  
  nEvents->Fill(1);

  edm::Handle<l1tp2::CaloCrystalClusterCollection> gctCaloCrystalClusters;
  edm::Handle<l1tp2::CaloTowerCollection> gctCaloL1Towers;
  
  edm::Handle<EcalEBTrigPrimDigiCollection> ecalTPGs;
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPGs;  
  edm::Handle<edm::SortedCollection<HcalTriggerPrimitiveDigi> > hbhecoll;
 
  gctClusters->clear();
  gctClusterInfo->clear();
  gctTowers->clear();
  allEcalTPGs->clear(); 
  allHcalTPGs->clear(); 

  // Detector geometry
  caloGeometry_ = &iSetup.getData(caloGeometryToken_);
  ebGeometry = caloGeometry_->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
  hbGeometry = caloGeometry_->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);
  hcTopology_ = &iSetup.getData(hbTopologyToken_);
  HcalTrigTowerGeometry theTrigTowerGeometry(hcTopology_);

  decoder_ = &iSetup.getData(decoderToken_);

  std::cout << "Doing event " << event << "...." << std::endl;

  
  // Get the GCT clusters from the emulator, and sort them by pT
  if(evt.getByToken(gctClustersSrc_, gctCaloCrystalClusters)){
    for(const auto & gctCluster : *gctCaloCrystalClusters){
      Cluster temp ;
      TLorentzVector temp_p4;
      std::cout << "GCT Cluster found: pT " << gctCluster.pt()  << ", "
                << "eta "               << gctCluster.eta() << ", "
                << "phi "               << gctCluster.phi() << ", " 
		            << "iso "               << gctCluster.isolation() << ", " 
	              << "is_ss"              << gctCluster.experimentalParam("standaloneWP_showerShape") << ", "
	              << "is_looseTkss"       << gctCluster.experimentalParam("trkMatchWP_showerShape") 
		            << std::endl;
      temp_p4.SetPtEtaPhiE(gctCluster.pt(),gctCluster.eta(),gctCluster.phi(),gctCluster.pt());

      temp.p4 = temp_p4;
      temp.et2x5 = gctCluster.e2x5();  // see https://cmssdt.cern.ch/lxr/source/DataFormats/L1TCalorimeterPhase2/interface/CaloCrystalCluster.h
      temp.et5x5 = gctCluster.e5x5();
      temp.iso   = gctCluster.isolation();

      temp.is_ss         = gctCluster.experimentalParam("standaloneWP_showerShape");
      temp.is_iso        = gctCluster.experimentalParam("standaloneWP_isolation");
      temp.is_looseTkss  = gctCluster.experimentalParam("trkMatchWP_showerShape");
      temp.is_looseTkiso = gctCluster.experimentalParam("trkMatchWP_isolation");
      std::cout << " with flags: " 
                << "is_ss " << temp.is_ss << ","
                << "is_iso " << temp.is_iso << ", "
                << "is_looseTkss " << temp.is_looseTkss << ", "
                << "is_looseTkiso " << temp.is_looseTkiso << std::endl;
      
      // Save the 4-vector
      gctClusters->push_back(temp_p4);
      // Save the full cluster info
      gctClusterInfo->push_back(temp);
    }
  }
  std::sort(gctClusters->begin(), gctClusters->end(), L1TCaloEGammaAnalyzerRates_oldEmulator::comparePt);
  std::sort(gctClusterInfo->begin(), gctClusterInfo->end(), L1TCaloEGammaAnalyzerRates_oldEmulator::compareClusterPt);

 
  // Build the collections that we need for rates: only use the highest pT GCT cluster in each event
  std::cout << "Building collections for rates: there are " << gctClusters->size() << " GCT clusters in the event" << std::endl;
  if (gctClusterInfo->size() > 0) {

    gct_cPt  = gctClusterInfo->at(0).p4.Pt();
    gct_cEta = gctClusterInfo->at(0).p4.Eta();
    gct_cPhi = gctClusterInfo->at(0).p4.Phi();
    gct_iso   = gctClusterInfo->at(0).iso;
    gct_et2x5 = gctClusterInfo->at(0).et2x5;
    gct_et5x5 = gctClusterInfo->at(0).et5x5; 
    gct_is_looseTkss = gctClusterInfo->at(0).is_looseTkss;
    gct_is_looseTkiso = gctClusterInfo->at(0).is_looseTkiso;
    gct_is_ss = gctClusterInfo->at(0).is_ss;
    gct_is_iso = gctClusterInfo->at(0).is_iso;

    std::cout << "GCT cluster (highest pT in this event): " << gct_cPt<< std::endl;
    
    l1eg_pt->Fill(gct_cPt);
    
    if (gct_cPt> 25) {
      l1egVLoose_pt->Fill(gct_cPt);
      std::cout << "  filled l1egVLoose_pt " ;
    }
    
    if (gct_cPt> 30) {
      l1egLoose_pt->Fill(gct_cPt);
      std::cout << "  filled l1egLoose_pt  ";
    }
    
    if (gct_cPt> 35) {
      l1egMedium_pt->Fill(gct_cPt);
      std::cout << "  filled l1egMedium_pt  ";
    }
    
    if (gct_cPt> 40) {
      l1egTight_pt->Fill(gct_cPt);
      std::cout << " filled l1egTight_pt  ";
    }

    if (gct_is_ss) {
      l1eg_pt_is_ss->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_ss ";
    }

    if (gct_is_iso) {
      l1eg_pt_is_iso->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_iso ";
    }

    if (gct_is_ss && gct_is_iso) {
      l1eg_pt_is_iso_is_ss->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_iso_is_ss ";
    }

    if (gct_is_looseTkiso) {
      l1eg_pt_is_looseTkiso->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_looseTkiso ";
    }

    if (gct_is_looseTkss) {
      l1eg_pt_is_looseTkss->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_looseTkss ";
    }

    if (gct_is_looseTkss && gct_is_looseTkiso) {
      l1eg_pt_is_looseTkiso_is_looseTkss->Fill(gct_cPt);
      std::cout << " filled l1eg_pt_is_looseTkiso_is_looseTkss";
    }
  }
  
 }



void L1TCaloEGammaAnalyzerRates_oldEmulator::endJob() {
}

L1TCaloEGammaAnalyzerRates_oldEmulator::~L1TCaloEGammaAnalyzerRates_oldEmulator(){
}

DEFINE_FWK_MODULE(L1TCaloEGammaAnalyzerRates_oldEmulator);

