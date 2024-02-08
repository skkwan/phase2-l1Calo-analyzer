/*******************************************************************/
/* makeEfficienciesPlotDPnote.cpp                                  */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlotDPNote.cpp             */
/*        Uncomment code blocks labeled "Plot #1", #2,...          */
/*        at a time                                                */
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

  // Use TDR version
  printf("[INFO:] Using TDR isolation flag\n");
  isoCut = "gct_is_iso";
  outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_iso_and_ss_TDRflags";


  printf("[INFO:] Using TDR shower shape flag\n");
  ssCut = "gct_is_ss";

  // Default behavior: both iso and ss cuts
  TString redCutString = " && " + isoCut + " && " + ssCut;
  TString redLabel = "with iso and shower shape";


  outputPlotName = "efficiency_genPt_barrel_GCT_acceptance_useTDRFlags";
  redLabel = "with TDR iso and shape flags";

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


  /**********************************************************************************************************/
  /* (Plot #1) eff. as a function of gen pT, standalone WP, GCT cluster pT > 25 GeV, genPt > 30 GeV         */
  /**********************************************************************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 100;
  genCut  = "(abs(genEta) < 1.4841) && (genPt > 30)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25) && (genPt > 30)";
  useVariableBinning = false;

  TGraphAsymmErrors *all_b = calculateEfficiency("genPt", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all_b);
  vLabels.push_back("TDR emulator w/o WP");
  vColors.push_back(kGray);

  TGraphAsymmErrors *tight_b = calculateEfficiency("genPt", treePath, rootFileDirectory,  
                                                  l1Cut + redCutString,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight_b);
  vLabels.push_back("TDR emulator with standalone WP");
  vColors.push_back(kBlue);


  TGraphAsymmErrors *all2_b = calculateEfficiency("genPt", treePath2, rootFileDirectory2,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(all2_b);
  vLabels.push_back("Phase 2 emulator w/o WP");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *tight2_b = calculateEfficiency("genPt", treePath2, rootFileDirectory2,  
                                                  l1Cut + redCutString,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tight2_b);
  vLabels.push_back("Phase 2 emulator with standalone WP");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron p_{T}",
                    "Emulators",   
                    outputPlotName +  "_standaloneWP_l1Ptgt25GeV_genPtgt30GeV",                                                             
                    outputDirectory, "L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  


  /***********************************************************************************/
  /* (Plot #2) efficiency as a function of genEta: GCT > 25 GeV pT, gen pT > 30 GeV  */
  /***********************************************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = -1.5;
  xMax = 1.5;

  genCut  = "(abs(genEta) < 1.4841) && (genPt > 30)";
  l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25) && (genPt > 30)";
  useVariableBinning = false;

  TGraphAsymmErrors *allEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(allEta);
  vLabels.push_back("TDR emulator w/o WP");
  vColors.push_back(kGray);

  TGraphAsymmErrors *tightEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
                                                  l1Cut + redCutString,
                                                  genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tightEta);
  vLabels.push_back("TDR emulator with standalone WP");
  vColors.push_back(kBlue);

  TGraphAsymmErrors *allEta2 = calculateEfficiency("genEta", treePath2, rootFileDirectory2,
                                                   l1Cut,
                                                   genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(allEta2);
  vLabels.push_back("Phase 2 emulator w/o WP");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *tightEta2 = calculateEfficiency("genEta", treePath2, rootFileDirectory2,
                                                   l1Cut + redCutString,
                                                   genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(tightEta2);
  vLabels.push_back("Phase 2 emulator with standalone WP");
  vColors.push_back(kRed); 

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "Gen Electron #eta",
                    "Emulators",                                                                
                    "efficiency_genEta_barrel_l1Ptgt25GeV_genPtgt30GeV",
                    outputDirectory, "L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");

}



void makeEfficienciesPlotsDPNote(void) {

  makeEfficienciesPlotForOneScheme("both", false, false, 0.98, 4, 60);
}
/*********************************************************************/
