/*
 * Usage: root -l -b -q drawParametricTH2D.cpp
 */

#include "cutoffValues.h"
#include "fillTH2D.h"
#include "parametricFit.h"
#include "tdrHelpers.h"

/*
 * Set plot style, title and axis labels of a TH2D. 
 */
void setPlotStyle(TH2D*& myHist, TString title, TString xLabel, TString yLabel, double plot_ymin, double plot_ymax) {
    myHist->GetXaxis()->SetTitle(xLabel);
    myHist->GetYaxis()->SetTitle(yLabel);
    myHist->SetTitle(title);

    myHist->GetXaxis()->SetTitleSize(0.04); // default is 0.03                                                                    
    myHist->GetYaxis()->SetTitleSize(0.04); // default is 0.03 

    myHist->GetYaxis()->SetRangeUser(plot_ymin, plot_ymax);
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

void drawAndSaveTH2D(TH2D* h2, TString title, TString xLabel, TString yLabel, TString outputName, double plot_ymin, double plot_ymax, TGraph* tGraph = NULL) {

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


    setPlotStyle(h2, title, xLabel, yLabel, plot_ymin, plot_ymax);
    setPlotStyleTGraph(tGraph, title, xLabel, yLabel);
    c1->SaveAs(outputName);

    delete pad1;
    delete c1;
}

/*
 * Produce parametric plots for an input file and processName (description to put in the output file). 
 * Acceptance is the per-bin acceptance to use when getting the points to fit to the parametric curve.
 * nBinsToSkip is the number of bins to skip at the start of the x-axis for the linear fit.
 */
int produceParametric(float acceptancePerBin, int nBinToStartFit, TString rootFileDirectory, TString signalFileDirectory, TString treePath, TString processName, TString label, TString plotFolder, TString inputListOfFiles = "", int startLine = -1, int nLinesToRead = -1, bool overlaySignal = false) {

    // Use same axis limits as TDR
    double plot_iso_ymax = 5.0;
    double plot_iso_ymin = 0.0;

    double plot_ss_ymax = 1.05;
    double plot_ss_ymin = 0.7;

    TChain *ch;
    if (inputListOfFiles == "") {
        std::cout << "Attempting to get TChain from single file..." << std::endl;
        ch = getTChainFromSingleFile(rootFileDirectory, treePath);
    }
    else {
        std::cout << "Attempting to get TChin from list of files..." << std::endl;
        ch = getTChainFromListOfFiles(inputListOfFiles, treePath, startLine, nLinesToRead);
    }

    TH2D *h2_iso = fillTH2DIsolationVsPt(ch);
    TH2D *h2_ss = fillTH2DShapeVarVsPt(ch);
    
    TGraph *tGraph_iso;
    TGraph *tGraph_ss;


    if (!overlaySignal) {
        tGraph_iso = getCutoffOfTH2DAsTGraph(h2_iso, acceptancePerBin);
        bool countUpwards = false; // just for ss
        tGraph_ss = getCutoffOfTH2DAsTGraph(h2_ss, acceptancePerBin, countUpwards);
    }
    else {
        // Get the signal parametric curves (taking care to use the acceptancePerBin argument)
        tGraph_iso = getCutoffOfTH2DAsTGraph(fillTH2DIsolationVsPt(getTChainFromSingleFile(signalFileDirectory, "l1NtupleProducer/efficiencyTree")), acceptancePerBin);
        tGraph_ss =  getCutoffOfTH2DAsTGraph(fillTH2DShapeVarVsPt(getTChainFromSingleFile(signalFileDirectory, "l1NtupleProducer/efficiencyTree")), acceptancePerBin);
    }


    parametricFit paramFitIso = parametricFit(tGraph_iso, 70.0, nBinToStartFit); // start fit at 4th point of the TGraph
    paramFitIso.linearFitStringRepr();
    TGraph tgraphParamFitIso = paramFitIso.tGraphRepr();

    // Needs to be called after paramFitIso is created
    TGraph tgraphTdrIso = getTdrIsoAsTGraph(paramFitIso.getXGraphVals());

    // If you want to plot the un-fitted isolation curve, change the last argument to tGraph_iso (the output of "getCutoffOfTH2DAsTGraph")
    // If you want to plot the fitted/parametrized curve, change the last argument to &tgraphParamFitIso (the output of the parametricFit::tGraphRepr() method)
    // If you want to plot the TDR curve, change the last argument to &tgraphTdrIso

    drawAndSaveTH2D(h2_iso, label, "Cluster p_{T} [GeV]", "Relative isolation", plotFolder + processName + "_parametric_isolation_vs_clusterPt_acceptance_" + std::to_string(acceptancePerBin) + "_fitStartBin_" + std::to_string(nBinToStartFit) + ".pdf",   plot_iso_ymin, plot_iso_ymax, &tgraphParamFitIso);
    // drawAndSaveTH2D(h2_ss,  label, "Cluster p_{T} [GeV]", "Et2x5/Et5x5",     plotFolder + processName + "_parametric_et2x5_over_et5x5_vs_clusterPt.pdf", plot_ss_ymin, plot_ss_ymax, tGraph_ss);
    

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

    TString signalFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_partial.root";

    // Scheme 1 
    produceParametric(0.98, 1, signalFileDirectory, signalFileDirectory, "l1NtupleProducer/efficiencyTree", "DoubleElectron", "Double Electron, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/");
    
    // Scheme 2
    produceParametric(0.98, 2, signalFileDirectory, signalFileDirectory, "l1NtupleProducer/efficiencyTree", "DoubleElectron", "Double Electron, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/");

    // Scheme 3
    produceParametric(0.99, 4, signalFileDirectory, signalFileDirectory, "l1NtupleProducer/efficiencyTree", "DoubleElectron", "Double Electron, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/");

    // Scheme 4
    produceParametric(0.98, 4, signalFileDirectory, signalFileDirectory, "l1NtupleProducer/efficiencyTree", "DoubleElectron", "Double Electron, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/");

    //produceParametric("", signalFileDirectory, "l1NtupleProducer/efficiencyTree", "MinBias", "MinBias, ECAL tower iso", "/eos/user/s/skkwan/phase2RCTDevel/figures/parametricCurves/", "../data/listMinBiasAnalyzerFiles.txt", 0, 100, true);


    return 1;
}