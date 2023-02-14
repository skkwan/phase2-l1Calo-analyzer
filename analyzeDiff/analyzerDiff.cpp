// root -l -b -q analyzerDiff.cpp

// reference: https://root.cern/manual/trees/#widening-a-ttree-through-friends

#include "DataFormats/Math/interface/deltaR.h"


void analyzerDiff() {

    std::unique_ptr<TFile> myFile( TFile::Open("/eos/user/s/skkwan/phase2RCTDevel/analyzeDiff/analyzer_MinBias_discrepancies.root") );
    auto tree = myFile->Get<TTree>("l1NtupleProducer/efficiencyTree");

    std::unique_ptr<TFile> myFriendFile( TFile::Open("/eos/user/s/skkwan/phase2RCTDevel/analyzeDiff/analyzer_oldEmulator_MinBias_discrepancies.root") );
    auto friendTree = myFriendFile->Get<TTree>("l1NtupleProducer/efficiencyTree");

    double gct_cEta, gct_cPhi;
    tree->SetBranchAddress("gct_cEta", &gct_cEta);
    tree->SetBranchAddress("gct_cPhi", &gct_cPhi);

    double old_cEta, old_cPhi;
    friendTree->SetBranchAddress("gct_cEta", &old_cEta);
    friendTree->SetBranchAddress("gct_cPhi", &old_cPhi);

    TH1F* hDeltaR = new TH1F("hDeltaR", "deltaR between old/new emulator clusters", 100, 0, 5);

    // Run over iEntry < tree->GetEntries() to get all the entries
    for (int iEntry = 0; iEntry < tree->GetEntries(); iEntry++) {

        tree->GetEntry(iEntry);
        friendTree->GetEntry(iEntry);

        //printf("%f %f\n", gct_cEta, old_cEta);
        
        float deltaR = reco::deltaR(gct_cEta, gct_cPhi, old_cEta, old_cPhi);
        hDeltaR->Fill(deltaR);
        printf("%f \n", deltaR);
    }

    TFile outFile("/eos/user/s/skkwan/phase2RCTDevel/analyzeDiff/out.root", "RECREATE");
    hDeltaR->Write(); 
    
    TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 600);
    Tcan->cd();     
    Tcan->SetFillColor(0);
    hDeltaR->Draw("HIST");
    Tcan->SaveAs("/eos/user/s/skkwan/phase2RCTDevel/analyzeDiff/deltaR.pdf");

    delete Tcan;
}