/*******************************************************************/
/* makeRatesPlots.cpp                                              */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeRatesPlots.cpp                         */
/*******************************************************************/

#include "calculateRates.cpp"
#include "plotNRates.cpp"
#include "../baseCodeForPlots/CMS_lumi.h"
#include "../baseCodeForPlots/tdrstyle.C"

void plotHists(TH1F* h1, TString h1Label, 
          TH1F* h2, TString h2Label,
          TH1F* h3, TString h3Label,
          TString filename,
               TString outputDir);

void plotFiveRates(TH1F* h1, TString h1Label, int c1,
         TH1F* h2, TString h2Label, int c2,
         TH1F* h3, TString h3Label, int c3,
         TH1F* h4, TString h4Label, int c4,
         TH1F* h5, TString h5Label, int c5,
         float xMin, float xMax,
         float yMin, float yMax,
         TString legendTitle,
         TString filename,
         TString outputDir);

/*********************************************************************/

/* 
 * Create rates plots.
 */

void makeRatesPlots(void)
{
  gROOT->ProcessLine(".L calculateRates.cpp");

  // Input file and output directory
  TString rootFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer-rates_MinBias.root";
  TString outputDirectory = "/eos/user/s/skkwan/phase2RCTDevel/figures/rates/";

  float xMin, xMax;
  float yMin, yMax;
  std::vector<TH1F*> vHists;
  std::vector<TString> vLabels;
  std::vector<int> vColors;
  bool useLogy;

  vHists.clear();  vLabels.clear();  vColors.clear();

  // Rates as a function of l1Pt    
  xMin = 0;
  xMax = 60.0;
  yMin = 10.0;
  yMax = 50000;
  useLogy = true;

  TString folder= "l1NtupleProducer/";
  TString evHist = "l1NtupleProducer/nEvents";
  TH1F* egVLoose = calculateRates(folder + "l1eg_pt", evHist, rootFileDirectory);
  TH1F* eg_is_ss = calculateRates(folder + "l1eg_pt_is_ss", evHist, rootFileDirectory);
  TH1F* eg_is_iso = calculateRates(folder + "l1eg_pt_is_iso", evHist, rootFileDirectory);
  TH1F* eg_is_ss_iso = calculateRates(folder + "l1eg_pt_is_iso_is_ss", evHist, rootFileDirectory);

  vHists.push_back(egVLoose); vLabels.push_back("No additional cuts"); vColors.push_back(kBlack);
  vHists.push_back(eg_is_ss);  vLabels.push_back("Shower shape only"); vColors.push_back(kTeal-8);
  vHists.push_back(eg_is_iso); vLabels.push_back("Isolation only"); vColors.push_back(kAzure-9);
  vHists.push_back(eg_is_ss_iso);  vLabels.push_back("Shower shape and isolation"); vColors.push_back(kRed);

  // one more color if necessary: kAzure-9
  
  plotNRates(vHists, vLabels, vColors,
             xMin, xMax, yMin, yMax,
             "ET threshold [GeV]",
             "rates_barrel_GCT",
             outputDirectory,
             useLogy);

  vHists.clear();  vLabels.clear();  vColors.clear();

}
