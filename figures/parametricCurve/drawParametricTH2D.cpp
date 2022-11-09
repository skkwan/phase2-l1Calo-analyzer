#include "cutoffValues.h"
#include "fillTH2D.h"

/*
 * Set title and axis labels of a TH2D. 
 */
void setPlotStyle(TH2D*& myHist, TString title, TString xLabel, TString yLabel) {
    myHist->GetXaxis()->SetTitle(xLabel);
    myHist->GetYaxis()->SetTitle(yLabel);
    myHist->SetTitle(title);

    myHist->GetXaxis()->SetTitleSize(0.04); // default is 0.03                                                                    
    myHist->GetYaxis()->SetTitleSize(0.04); // default is 0.03 

}

/*
 * Set title and axis labels of a TGraph.
 */
void setPlotStyleTGraph(TGraph*& myGraph, TString title, TString xLabel, TString yLabel) {
    if (myGraph != NULL) {
        myGraph->GetXaxis()->SetTitle(xLabel);
        myGraph->GetYaxis()->SetTitle(yLabel);
        myGraph->SetTitle(title);

        myGraph->GetXaxis()->SetTitleSize(0.04); // default is 0.03                                                                    
        myGraph->GetYaxis()->SetTitleSize(0.04); // default is 0.03 
    }
}

/*
 * Set plot style of a parametric line represented as a TGraph.
 */

void setParametricLinePlotStyle(TGraph*& myGraph) {
    if (myGraph != NULL) {
        myGraph->SetLineColor(kRed);
        myGraph->SetLineWidth(6);
    }
}


/*
 * Make canvas larger so side label won't run off the side.
 */

void setPadMargins(TPad*& myPad) {

    myPad->SetRightMargin(0.15);
    myPad->SetLeftMargin(0.15);
}

/*
 * Draw and save the provided h2 to the output name. If h2Cutoff (TH2D) is provided, also draw it.
 */

void drawAndSaveTH2D(TH2D* h2, TString title, TString xLabel, TString yLabel, TString outputName, TGraph* tGraph = NULL) {

    TCanvas *c1 = new TCanvas("c1", "c1", 1900, 1900);
    TPad *pad1 = new TPad("pad1", "The pad", 0, 0, 1, 1);
    setPadMargins(pad1);
    
    gStyle->SetOptStat(0);

    pad1->Draw();
    pad1->cd();
    
    h2->Draw("COLZ 1 SAME");

    if (tGraph != NULL) {
        std::cout << ">>> Cutoff TH2D found!" << std::endl;
        setParametricLinePlotStyle(tGraph);
        tGraph->Draw("L"); 
    }
    else {
        std::cout << ">>> Cutoff TH2D not found" << std::endl;
    }


    setPlotStyle(h2, title, xLabel, yLabel);
    setPlotStyleTGraph(tGraph, title, xLabel, yLabel);
    c1->SaveAs(outputName);

    delete pad1;
    delete c1;
}

/*
 * Produce parametric plots for an input file and processName (description to put in the output file). 
 */
int produceParametric(TString rootFileDirectory, TString processName, TString label, TString plotFolder, TString inputTextFile = "", int startLine = -1, int nLinesToRead = -1) {

    TH2D *h2_iso = fillTH2DIsolationVsPt(rootFileDirectory, "l1NtupleProducer/efficiencyTree", inputTextFile, startLine, nLinesToRead);
    TH2D *h2_ss = fillTH2DShapeVarVsPt(rootFileDirectory, "l1NtupleProducer/efficiencyTree", inputTextFile, startLine, nLinesToRead);
    
    TGraph *tGraph_iso = getCutoffOfTH2DAsTGraph(h2_iso);
    bool countUpwards = false; // just for ss
    TGraph *tGraph_ss = getCutoffOfTH2DAsTGraph(h2_ss, countUpwards);

    drawAndSaveTH2D(h2_iso, label, "Cluster p_{T} [GeV]", "Isolation [GeV]", plotFolder + processName + "_parametric_isolation_vs_clusterPt.pdf",       tGraph_iso);
    drawAndSaveTH2D(h2_ss,  label, "Cluster p_{T} [GeV]", "Et2x5/Et5x5",     plotFolder + processName + "_parametric_et2x5_over_et5x5_vs_clusterPt.pdf", tGraph_ss);

    delete h2_iso, h2_ss;
    delete tGraph_iso, tGraph_ss;

    return 1;
}

/*
 * Main function: Create TH2D of the isolation and et2x5/et5x5 vs. the cluster pT for a given TTree.
 * Also finds and overlays the cut-off points (one per x-axis bin) where 95% of the events fall above (or below) the cut-off point.
 * Saves the plots as pdfs.
 */

int drawParametricTH2D(void) {

    // produceParametric("/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron.root", "DoubleElectron", "Double Electron, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/");
    produceParametric("",                                                                                 "MinBias", "MinBias, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/", "../data/listMinBiasAnalyzerFiles.txt");

    return 1;
}