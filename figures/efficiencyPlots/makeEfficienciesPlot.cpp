/*******************************************************************/
/* makeEfficienciesPlot.cpp                                        */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlot.cpp                   */
/*******************************************************************/

#include "efficiencyHist.cpp"
#include "calculateEfficiency.cpp"

#include <string>
/*********************************************************************/

/* Plots L1 RCT and GCT EGamma efficiency as a function of gen-level variables. */

void makeEfficienciesPlot(void)
{
  gROOT->ProcessLine(".L calculateEfficiency.cpp");

  TString treePath = "l1NtupleProducer/efficiencyTree";
  TString rootFileDirectory = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/analyzer_SingleElectron.root";
  TString outputDirectory = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;

  // Weird: script cannot correctly produce horizontal error bars if we try to do two sets
  // of the vs. pT plots at the same time (the vs. eta plots are OK to do two at a time)
  // So you need to only make one vs. pT plot at a time, by commenting out the other

  /*******************************************************/
  /* efficiency as a function of genPt: GCT              */
  /*******************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 100;
  genCut  = "(abs(genEta) < 1.4841)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  useVariableBinning = false;

  TGraphAsymmErrors *loose = calculateEfficiency("genPt", treePath, rootFileDirectory,
                l1Cut + "&& gct_cPt > 30",
                genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(loose);
  vLabels.push_back("L1 p_{T} > 30");
  vColors.push_back(kGreen + 2);

  TGraphAsymmErrors *medium = calculateEfficiency("genPt", treePath, rootFileDirectory,
                 l1Cut + "&& gct_cPt > 35",
                 genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(medium);
  vLabels.push_back("L1 p_{T} > 35");
  vColors.push_back(kBlue);
  
  TGraphAsymmErrors *tight = calculateEfficiency("genPt", treePath, rootFileDirectory,
                l1Cut + "&& gct_cPt > 40",
                genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight);
  vLabels.push_back("L1 p_{T} > 40");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *all = calculateEfficiency("genPt", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all);
  vLabels.push_back("No additional cut");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                  "Gen Electron p_{T} [GeV]",
                  "Phase 2 GCT",                                                                
                  "efficiency_genPt_barrel_GCT",        
                  outputDirectory, "");    
  
  /*******************************************************/
  /* efficiency as a function of genPt: GCT, iso and ss   */
  /*******************************************************/

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 100;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25) && gct_is_iso && gct_is_ss";
  // useVariableBinning = false;

  // TGraphAsymmErrors *loose2 = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //               l1Cut + "&& gct_cPt > 30",
  //               genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(loose2);
  // vLabels.push_back("L1 p_{T} > 30");
  // vColors.push_back(kGreen + 2);

  // TGraphAsymmErrors *medium2 = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //                l1Cut + "&& gct_cPt > 35",
  //                genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(medium2);
  // vLabels.push_back("L1 p_{T} > 35");
  // vColors.push_back(kBlue);
  
  // TGraphAsymmErrors *tight2 = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //               l1Cut + "&& gct_cPt > 40",
  //               genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2);
  // vLabels.push_back("L1 p_{T} > 40");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *all2 = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2);
  // vLabels.push_back("No additional cut");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                 "Gen Electron p_{T} [GeV]",
  //                 "Phase 2 GCT, with iso and ss flags",                                                                
  //                 "efficiency_genPt_barrel_GCT_with_ss_iso",        
  //                 outputDirectory,
  //                 ", ss + iso");
  // delete loose2, medium2, tight2, all2;

  /*******************************************************/
  /* efficiency as a function of genEta: GCT             */
  /*******************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = -1.5;
  xMax = 1.5;

  genCut  = "(abs(genEta) < 1.4841)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  useVariableBinning = false;


  TGraphAsymmErrors* loose3 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                  l1Cut + "&& gct_cPt > 30",
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(loose3);
  vLabels.push_back("L1 p_{T} > 30");
  vColors.push_back(kGreen+2);

  TGraphAsymmErrors* medium3 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                    l1Cut + "&& gct_cPt > 35",
                                                    genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(medium3);
  vLabels.push_back("L1 p_{T} > 35");
  vColors.push_back(kBlue);
  
  TGraphAsymmErrors* tight3 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                  l1Cut + "&& gct_cPt > 40",
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight3);
  vLabels.push_back("L1 p_{T} > 40");
  vColors.push_back(kBlack);

  TGraphAsymmErrors* all3 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                l1Cut,
                                                genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all3);
  vLabels.push_back("No additional cut");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron #eta",
                    "Phase 2 GCT, with iso and ss flags",                                                                
                    "efficiency_genEta_barrel",        
                    outputDirectory);    
  

  /************************************************************/
  /* efficiency as a function of genEta: GCT with iso and ss  */
  /************************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = -1.5;
  xMax = 1.5;

  genCut  = "(abs(genEta) < 1.4841)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25) && gct_is_iso && gct_is_ss";
  useVariableBinning = false;


  TGraphAsymmErrors* loose4 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                  l1Cut + "&& gct_cPt > 30",
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(loose4);
  vLabels.push_back("L1 p_{T} > 30");
  vColors.push_back(kGreen+2);

  TGraphAsymmErrors* medium4 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                    l1Cut + "&& gct_cPt > 35",
                                                    genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(medium4);
  vLabels.push_back("L1 p_{T} > 35");
  vColors.push_back(kBlue);
      
  TGraphAsymmErrors* tight4 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                  l1Cut + "&& gct_cPt > 40",
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight4);
  vLabels.push_back("L1 p_{T} > 40");
  vColors.push_back(kBlack);

  TGraphAsymmErrors* all4 = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                l1Cut,
                                                genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all4);
  vLabels.push_back("No additional cut");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron #eta",
                    "Phase 2 GCT, with iso and ss flags",                                                                
                    "efficiency_genEta_barrel_GCT_with_ss_iso",        
                    outputDirectory,
                    ", iso and ss");    
  
  
}
/*********************************************************************/