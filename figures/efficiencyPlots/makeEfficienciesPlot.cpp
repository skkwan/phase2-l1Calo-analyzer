/*******************************************************************/
/* makeEfficienciesPlot.cpp                                        */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.cpp                   */
/*******************************************************************/

#include "efficiencyHist.cpp"
#include "calculateEfficiency.cpp"

#include "../parametricCurve/cutoffValues.h"
#include "../parametricCurve/fillTH2D.h"
#include "../parametricCurve/parametricFit.h"

#include <string>
/*********************************************************************/

/*
 * Make efficiency plot for one isolation parametrization scheme.
 */

void makeEfficienciesPlotForOneScheme(float acceptancePerBin, int nBinToStartFit)
{
  gROOT->ProcessLine(".L calculateEfficiency.cpp");

  TString treePath = "l1NtupleProducer/efficiencyTree";
  TString rootFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_partial.root";
  TString signalFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_partial.root"; // for the parametric curve
  TString outputDirectory = "/eos/user/s/skkwan/phase2RCTDevel/figures/efficiencies/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;

  
  TString isoCut;
  TString outputPlotName;

  if ((acceptancePerBin > 0) && (nBinToStartFit > 0)) {
    TGraph *tGraph_iso;
    tGraph_iso = getCutoffOfTH2DAsTGraph(fillTH2DIsolationVsPt(getTChainFromSingleFile(signalFileDirectory, "l1NtupleProducer/efficiencyTree")), acceptancePerBin);
    parametricFit paramFitIso = parametricFit(tGraph_iso, 70.0, nBinToStartFit);
    isoCut = paramFitIso.linearFitStringRepr();
    paramFitIso.tGraphRepr();

    outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_" + std::to_string(acceptancePerBin) + "_fitStartBin_" + std::to_string(nBinToStartFit);
  }
  else {
    // Use TDR version
    isoCut = "gct_is_iso";
    outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_TDRflag";
  }

  /*******************************************************/
  /* efficiency as a function of genPt: GCT              */
  /*******************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 100;
  genCut  = "(abs(genEta) < 1.4841)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  useVariableBinning = false;

  TGraphAsymmErrors *all = calculateEfficiency("genPt", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all);
  vLabels.push_back("w/o iso or shower shape");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *tight = calculateEfficiency("genPt", treePath, rootFileDirectory,  
                                                  l1Cut + "&&" + isoCut,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight);
  vLabels.push_back("with isolation only");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron p_{T}",
                    "Phase 2 GCT",   
                    outputPlotName,                                                             
                    outputDirectory, "L1 p_{T} > 25, |#eta^{Gen}| < 1.4841", 0.8, 1.02);    
  

  /*******************************************************/
  /* efficiency as a function of recoPt: GCT              */
  /*******************************************************/


  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 100;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all = calculateEfficiency("gct_cPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all);
  // vLabels.push_back("w/o iso or shower shape");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight = calculateEfficiency("gct_cPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + "&&" + "( ((gct_cPt >= 70) && (gct_iso <= 0.439722)) || ( (gct_cPt < 70) && ( gct_iso < ((-0.013667 * gct_cPt) + (1.396389))  ) ))",
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight);
  // vLabels.push_back("with in-line new iso only");
  // vColors.push_back(kRed);


  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Cluster p_{T}",
  //                   "Phase 2 GCT",                                                                
  //                   "efficiency_recoPt_barrel_GCT_newInLineIsoOnly_test_l1Pt_25",        
  //                   outputDirectory, "L1 p_{T} > 25, |#eta^{Gen}| < 1.4841", 0.8, 1.02);    
  


  // /*******************************************************/
  // /* efficiency as a function of genEta: GCT             */
  // // /*******************************************************/

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = -1.5;
  // xMax = 1.5;

  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *allEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(allEta);
  // vLabels.push_back("w/o iso and shower shape");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tightEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
  //                                                 l1Cut + "&& gct_is_iso && gct_is_ss",
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tightEta);
  // vLabels.push_back("with iso and shower shape");
  // vColors.push_back(kRed);


  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron #eta",
  //                   "Phase 2 GCT",                                                                
  //                   "efficiency_genEta_barrel_GCT",        
  //                   outputDirectory, "L1 p_{T} > 25, |#eta^{Gen}| < 1.4841", 0.0, 1.1);    

  /*******************************************************/
  /* Additional studies                                  */
  /*******************************************************/

  // // This is weird -- needs to be run with the above plots commented out

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 100;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 5)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *allAdditional = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(allAdditional);
  // vLabels.push_back("w/o iso and shower shape");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tightAdditional = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //                                                 l1Cut + "&& gct_is_iso && gct_is_ss",
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tightAdditional);
  // vLabels.push_back("with iso and shower shape");
  // vColors.push_back(kRed);


  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Phase 2 GCT",                                                                
  //                   "efficiency_genPt_barrel_GCT_l1Pt_gt_5GeV",        
  //                   outputDirectory, "L1 p_{T} > 5, |#eta^{Gen}| < 1.4841", 0.8, 1.02);    
  

   
}

void makeEfficienciesPlot(void) {
  
  // // Scheme 1
  // makeEfficienciesPlotForOneScheme(0.98, 1);

  // // Scheme 2
  // makeEfficienciesPlotForOneScheme(0.98, 2);

  // // Scheme 3
  // makeEfficienciesPlotForOneScheme(0.99, 4);

  // Scheme 4
  // makeEfficienciesPlotForOneScheme(0.98, 4);

  // Use TDR
  makeEfficienciesPlotForOneScheme(-1, -1);
}
/*********************************************************************/
