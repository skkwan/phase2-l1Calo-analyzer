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
 * "mode" must be "isoOnly" or "ssOnly" to specify isolation or shower shape only for the secondary curve, otherwise it defaults to applying both an iso and shower shape.
 */

void makeEfficienciesPlotForOneScheme(TString mode, bool useOwnIsolationFlag, bool useOwnShowerShapeFlag, float acceptancePerBin, int nBinToStartFit, float isoPlateauPt)
{
  gROOT->ProcessLine(".L calculateEfficiency.cpp");


  TString treePath = "l1NtupleProducer/efficiencyTree";
  TString treePath2 = "l1NtupleSingleProducer/efficiencyTree";

  TString rootFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_FlatPt-1To100-gun_oldEmulator.root";
  TString rootFileDirectory2 = "/eos/user/s/skkwan/phase2RCTDevel/analyzer.root";
  TString signalFileDirectory = ""; //eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_partial.root"; // for the parametric curve
  TString outputDirectory = "/eos/user/s/skkwan/phase2RCTDevel/figures/efficiencies/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;

  
  TString isoCut;
  TString ssCut;
  TString outputPlotName;

  if (useOwnIsolationFlag) {
    printf("[WARNING:] Using self-derived isolation flag\n");

    if ((acceptancePerBin > 0) && (nBinToStartFit > 0)) {
      TGraph *tGraph_iso;
      tGraph_iso = getCutoffOfTH2DAsTGraph(fillTH2DIsolationVsPt(getTChainFromSingleFile(signalFileDirectory, "l1NtupleProducer/efficiencyTree")), acceptancePerBin);
      parametricFit paramFitIso = parametricFit(tGraph_iso, isoPlateauPt, nBinToStartFit);
      isoCut = paramFitIso.linearFitStringRepr();
      paramFitIso.tGraphRepr();

    }
  }
  else {
    // Use TDR version
    printf("[INFO:] Using TDR isolation flag\n");
    isoCut = "gct_is_iso";
    outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_iso_and_ss_TDRflags";
  }

  if (useOwnShowerShapeFlag) {
    printf("[WARNING:] Using self-derived shower shape flag\n");

    // In-line modified
    // TDR
    float c0_ss = 0.94, c1_ss = 0.052, c2_ss = 0.044;  
    // Lower the y-intercept by 0.1 (v1) or 0.05 (v2)
    c0_ss = c0_ss - 0.1;
    ssCut.Form("((gct_cPt > 130) || ((gct_et2x5/gct_et5x5) >= (%f + %f* exp(-%f * gct_cPt))) )", c0_ss, c1_ss, c2_ss);
  } 
  else {
    printf("[INFO:] Using TDR shower shape flag\n");

    ssCut = "gct_is_ss";
  }


  // Default behavior: both iso and ss cuts
  TString redCutString = " && " + isoCut + " && " + ssCut;
  TString redLabel = "with iso and shower shape";

  if (useOwnIsolationFlag && useOwnShowerShapeFlag) {
    outputPlotName = "efficiency_genPt_barrel_GCT_isoAccept_" + TString::Format("%.2f", acceptancePerBin) + "_isoFitStartBin_" +  TString::Format("%i", nBinToStartFit) + "_useOwnFlags";
    redLabel = "with custom iso and shape flags";
  }
  else if (useOwnIsolationFlag && !useOwnShowerShapeFlag) {
    outputPlotName = "efficiency_genPt_barrel_GCT_isoAccept_" + TString::Format("%.2f", acceptancePerBin) + "_isoFitStartBin_" + TString::Format("%i", nBinToStartFit) + "_useOwnIsoFlagOnly";
    redLabel = "with custom iso and TDR shape flags";
  }
  else if (!useOwnIsolationFlag && useOwnShowerShapeFlag) {
    outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_useOwnShowerShapeFlagOnly";
    redLabel = "with TDR iso and custom shape flags";
  }
  else {
    outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_useTDRFlags";
    redLabel = "with TDR iso and shape flags";
  }

  if (mode == "isoOnly") {
    redCutString = " && " + isoCut;
    if (useOwnIsolationFlag) { 
      redLabel = "with custom iso flag only";
    }
    else {
      redLabel = "with TDR iso flag only";
    }
  }
  else if (mode == "ssOnly") {
    redCutString = "&& " + ssCut;
    if (useOwnShowerShapeFlag) {
      redLabel = "with custom shape flag only";
    }
    else {
      redLabel = "with TDR shape flag only";
    }
  }

  // Also state the plateau pT if applicable
  if (useOwnIsolationFlag) {
    outputPlotName = outputPlotName + "_isoPlateauPt_" + TString::Format("%.0f", isoPlateauPt);
  }
  /*******************************************************/
  /* efficiency as a function of genPt: GCT              */
  /*******************************************************/

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 40;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 5)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all);
  // vLabels.push_back("TDR emulator w/o WP");
  // vColors.push_back(kGray);

  // TGraphAsymmErrors *tight = calculateEfficiency("genPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight);
  // vLabels.push_back("TDR emulator with standalone WP");
  // vColors.push_back(kBlue);


  // TGraphAsymmErrors *all2 = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2);
  // vLabels.push_back("Phase 2 emulator w/o WP");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight2 = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2);
  // vLabels.push_back("Phase 2 emulator with standalone WP");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Emulators",   
  //                   outputPlotName +  "_standaloneWP_gt5GeV",                                                                
  //                   outputDirectory, "L1 p_{T} > 5 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);    
  

  /*******************************************************/
  /* standalone WP, > 3 GeV           */
  /*******************************************************/


  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 40;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 3)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all_a = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all_a);
  // vLabels.push_back("TDR emulator w/o WP");
  // vColors.push_back(kGray);

  // TGraphAsymmErrors *tight_a = calculateEfficiency("genPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight_a);
  // vLabels.push_back("TDR emulator with standalone WP");
  // vColors.push_back(kBlue);


  // TGraphAsymmErrors *all2_a = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2_a);
  // vLabels.push_back("Phase 2 emulator w/o WP");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight2_a = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2_a);
  // vLabels.push_back("Phase 2 emulator with standalone WP");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Emulators",   
  //                   outputPlotName +  "_standaloneWP_gt3GeV",                                                             
  //                   outputDirectory, "L1 p_{T} > 3 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);  

    /*******************************************************/
  /* standalone WP, full range           */
  /*******************************************************/

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 100;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all_b = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all_b);
  // vLabels.push_back("TDR emulator w/o WP");
  // vColors.push_back(kGray);

  // TGraphAsymmErrors *tight_b = calculateEfficiency("genPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight_b);
  // vLabels.push_back("TDR emulator with standalone WP");
  // vColors.push_back(kBlue);


  // TGraphAsymmErrors *all2_b = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2_b);
  // vLabels.push_back("Phase 2 emulator w/o WP");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight2_b = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2_b);
  // vLabels.push_back("Phase 2 emulator with standalone WP");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Emulators",   
  //                   outputPlotName +  "_standaloneWP_gt25GeV",                                                             
  //                   outputDirectory, "L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);  

  /*******************************************************/
  /* loose Tk > 3 GeV          */
  /*******************************************************/

  // /// Now with loose Tk
  // redCutString = " && gct_is_looseTkiso && gct_is_looseTkss ";

  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 40;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 3)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all_c = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all_c);
  // vLabels.push_back("TDR emulator w/o WP");
  // vColors.push_back(kGray);

  // TGraphAsymmErrors *tight_c = calculateEfficiency("genPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight_c);
  // vLabels.push_back("TDR emulator with loose Tk WP");
  // vColors.push_back(kBlue);


  // TGraphAsymmErrors *all2_c = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2_c);
  // vLabels.push_back("Phase 2 emulator w/o WP");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight2_c = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2_c);
  // vLabels.push_back("Phase 2 emulator with loose Tk WP");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Emulators",   
  //                   outputPlotName + "_looseTkWP_gt3GeV",                                                             
  //                   outputDirectory, "L1 p_{T} > 3 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);  


 /*******************************************************/
  /* loose Tk > 5 GeV          */
  /*******************************************************/

  /// Now with loose Tk
  redCutString = " && gct_is_looseTkiso && gct_is_looseTkss ";

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 40;
  genCut  = "(abs(genEta) < 1.4841)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 5)";
  useVariableBinning = false;

  TGraphAsymmErrors *all_d = calculateEfficiency("genPt", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all_d);
  vLabels.push_back("TDR emulator w/o WP");
  vColors.push_back(kGray);

  TGraphAsymmErrors *tight_d = calculateEfficiency("genPt", treePath, rootFileDirectory,  
                                                  l1Cut + redCutString,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight_d);
  vLabels.push_back("TDR emulator with loose Tk WP");
  vColors.push_back(kBlue);


  TGraphAsymmErrors *all2_d = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all2_d);
  vLabels.push_back("Phase 2 emulator w/o WP");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *tight2_d = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
                                                  l1Cut + redCutString,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight2_d);
  vLabels.push_back("Phase 2 emulator with loose Tk WP");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron p_{T}",
                    "Emulators",   
                    outputPlotName + "_looseTkWP_gt5GeV",                                                             
                    outputDirectory, "L1 p_{T} > 5 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);  

//  /*******************************************************/
//   /* loose Tk > 25 GeV          */
//   /*******************************************************/

  // // Now with loose Tk
  // redCutString = " && gct_is_looseTkiso && gct_is_looseTkss ";
  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 100;
  // genCut  = "(abs(genEta) < 1.4841)";
  // l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25)";
  // useVariableBinning = false;

  // TGraphAsymmErrors *all_e = calculateEfficiency("genPt", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all_e);
  // vLabels.push_back("TDR emulator w/o WP");
  // vColors.push_back(kGray);

  // TGraphAsymmErrors *tight_e = calculateEfficiency("genPt", treePath, rootFileDirectory,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight_e);
  // vLabels.push_back("TDR emulator with loose Tk WP");
  // vColors.push_back(kBlue);


  // TGraphAsymmErrors *all2_e = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(all2_e);
  // vLabels.push_back("Phase 2 emulator w/o WP");
  // vColors.push_back(kBlack);

  // TGraphAsymmErrors *tight2_e = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
  //                                                 l1Cut + redCutString,
  //                                                 genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(tight2_e);
  // vLabels.push_back("Phase 2 emulator with loose Tk WP");
  // vColors.push_back(kRed);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "Gen Electron p_{T}",
  //                   "Emulators",   
  //                   outputPlotName +  "_looseTkWP_gt25GeV",                                                             
  //                   outputDirectory, "L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02);




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

  makeEfficienciesPlotForOneScheme("both", false, false, 0.98, 4, 60);
}
/*********************************************************************/
