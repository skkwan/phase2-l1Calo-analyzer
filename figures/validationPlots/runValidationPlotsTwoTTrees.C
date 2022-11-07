// Usage: root -l -b -q runValidationsPlotTwoTTrees.C
// Create plots of variables from two different TTrees, overlaying histograms from two cuts

#include "../baseCodeForPlots/comparisonPlotsTwoTTrees.C"


void runValidationPlotsTwoTTrees()
{
  // Load the macro
  gROOT->ProcessLine(".L ../baseCodeForPlots/comparisonPlotsTwoTTrees.C");
 
  TString treePath1 = "l1NtupleProducer/efficiencyTree";
  TString treePath2 = "l1NtupleProducer/efficiencyTree";
  
  TString inputDir1 = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Oct_12_2022/analyzer.root";
  TString inputDir2 = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/analyzer-l1egammaCrystals.root";
  
  TString outputDirectory = "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Oct_12_2022/";

  TString cut1 = "(gct_cPt > 15) && (genPt > 15) && (abs(genEta) < 1.4841)";
  TString cut2 = cut1;

  // Use SF = 0.985 for new emulator cluster pT
  comparisonPlotsTwoTTrees("gct_cPt / 0.985", "gct_cPt",  cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_cPt", 60, 0, 120);
  comparisonPlotsTwoTTrees("gct_cEta",        "gct_cEta", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_cEta", 60, -3, 3);
  comparisonPlotsTwoTTrees("gct_cPhi",        "gct_cPhi", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_cPhi", 60, -4, 4);

  

  comparisonPlotsTwoTTrees("gct_deltaR", "gct_deltaR", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_deltaR", 60, 0, 0.1);
  comparisonPlotsTwoTTrees("gct_et2x5",  "gct_et2x5 * sin(2 * atan(exp(-1 * gct_cEta)))", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_et2x5", 60, 0, 120);
  comparisonPlotsTwoTTrees("gct_et5x5", "gct_et5x5 * sin(2 * atan(exp(-1 * gct_cEta)))", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_et5x5", 60, 0, 120);

  comparisonPlotsTwoTTrees("gct_et2x5/gct_et5x5",  "gct_et2x5/gct_et5x5", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_et2x5_et5x5_ratio", 60, 0.5, 1.05);

  comparisonPlotsTwoTTrees("gct_iso", "gct_iso", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_iso", 60, 0, 0.3);

  comparisonPlotsTwoTTrees("gct_is_ss", "gct_is_ss", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_is_ss", 2, 0, 2);
  comparisonPlotsTwoTTrees("gct_is_looseTkss", "gct_is_looseTkss", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_is_looseTkss", 2, 0, 2);
  comparisonPlotsTwoTTrees("gct_is_iso", "gct_is_iso", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_is_iso", 2, 0, 2);
  comparisonPlotsTwoTTrees("gct_is_looseTkiso","gct_is_looseTkiso", cut1, cut2, treePath1, treePath2, inputDir1, inputDir2, outputDirectory, "gct_is_looseTkiso", 2, 0, 2);
}
