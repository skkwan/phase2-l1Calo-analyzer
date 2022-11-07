#include "fillTH2D.h"

/*
 *
 */
void setPlotStyle(TH2D*& myHist, TString title, TString xLabel, TString yLabel) {
    myHist->GetXaxis()->SetTitle(xLabel);
    myHist->GetYaxis()->SetTitle(yLabel);
    myHist->SetTitle(title);

    myHist->GetXaxis()->SetTitleSize(0.04); // default is 0.03                                                                    
    myHist->GetYaxis()->SetTitleSize(0.04); // default is 0.03 

}

/*
 * Make canvas larger so side label won't run off the side
 */

void setCanvasMargins(TCanvas*& myCan) {

    myCan->SetRightMargin(0.15);
    myCan->SetLeftMargin(0.15);
}

/*
 * Draw and save the provided h2 to the output name.
 */

void drawAndSaveTH2D(TH2D* h2, TString title, TString xLabel, TString yLabel, TString outputName) {

    TCanvas *c1 = new TCanvas("c1", "c1",1900,1900);
    setCanvasMargins(c1);
    
    gStyle->SetOptStat(0);

    c1->cd();
    h2->Draw("COLZ 1");
    setPlotStyle(h2, title, xLabel, yLabel);

    c1->SaveAs(outputName);

    delete c1;
}

int drawParametricTH2D(void) {

    // Temp: just for developing script
    TString rootFileDirectory = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Oct_25_2022_Iso_7x7_fullECALHCALtowers/analyzer.root";
    TString treePath = "l1NtupleProducer/efficiencyTree";


    TH2D *h2_iso = fillTH2DIsolationVsPt(rootFileDirectory, treePath);
    TH2D *h2_ss = fillTH2DShapeVarVsPt(rootFileDirectory, treePath);

    TString processName = "Single Electron, full tower iso (incorrect scheme)";
    drawAndSaveTH2D(h2_iso, processName, "Cluster p_{T} [GeV]", "Isolation [GeV]", "parametric_isolation_vs_clusterPt.pdf");
    drawAndSaveTH2D(h2_ss,  processName, "Cluster p_{T} [GeV]", "Et2x5/Et5x5",     "parametric_et2x5_over_et5x5_vs_clusterPt.pdf");


    return 1;
}