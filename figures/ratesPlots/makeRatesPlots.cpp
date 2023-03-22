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
  TString rootFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_rates.root";
  TString outputDirectory = "/eos/user/s/skkwan/phase2RCTDevel/figures/rates/";

  TString rootFileDirectory2 = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_rates_oldEmulator.root";


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
  yMin = 1.0;
  yMax = 50000;
  useLogy = true;

  // For old emulator need l1NtupleProducerRates/, for new emulator need l1NtupleProducer/
  TString folder = "l1NtupleProducer/";
  TString evHist = "l1NtupleProducer/nEvents";

  TString folder2 = "l1NtupleProducerRates/";
  TString evHist2 = "l1NtupleProducerRates/nEvents";

  TH1F* egVLoose = calculateRates(folder + "l1eg_pt", evHist, rootFileDirectory);
  TH1F* egStandalone = calculateRates(folder + "l1eg_pt_is_iso_is_ss", evHist, rootFileDirectory);

  TH1F* egVLoose2 = calculateRates(folder2 + "l1eg_pt", evHist2, rootFileDirectory2);
  TH1F* egStandalone2 = calculateRates(folder2 + "l1eg_pt_is_iso_is_ss", evHist2, rootFileDirectory2);

  vHists.push_back(egVLoose2); vLabels.push_back("TDR emulator w/o WP"); vColors.push_back(kGray);
  vHists.push_back(egStandalone2);  vLabels.push_back("TDR emulator with standalone WP"); vColors.push_back(kBlue);


  vHists.push_back(egVLoose); vLabels.push_back("Phase 2 emulator w/o WP"); vColors.push_back(kBlack);
  vHists.push_back(egStandalone);  vLabels.push_back("Phase 2 emulator with standalone WP"); vColors.push_back(kRed);



  // one more color if necessary: kAzure-9
  plotNRates(vHists, vLabels, vColors,
             xMin, xMax, yMin, yMax,
             "ET threshold [GeV]",
             "rates_barrel_overlaid_standaloneWP",
             outputDirectory,
             useLogy);

  vHists.clear();  vLabels.clear();  vColors.clear();

  ////////////////////////////////////////////////////////
  // With loose WP
  //////////////////////////////////////////////////////////////////////

  TH1F* eg = calculateRates(folder + "l1eg_pt", evHist, rootFileDirectory);
  TH1F* egTk = calculateRates(folder + "l1eg_pt_is_looseTkiso_is_looseTkss", evHist, rootFileDirectory);

  TH1F* eg2 = calculateRates(folder2 + "l1eg_pt", evHist2, rootFileDirectory2);
  TH1F* egTk2 = calculateRates(folder2 + "l1eg_pt_is_looseTkiso_is_looseTkss", evHist2, rootFileDirectory2);

  vHists.push_back(eg2); vLabels.push_back("TDR emulator w/o WP"); vColors.push_back(kGray);
  vHists.push_back(egTk2);  vLabels.push_back("TDR emulator with loose Tk WP"); vColors.push_back(kBlue);


  vHists.push_back(eg); vLabels.push_back("Phase 2 emulator w/o WP"); vColors.push_back(kBlack);
  vHists.push_back(egTk);  vLabels.push_back("Phase 2 emulator with loose tk WP"); vColors.push_back(kRed);



  // one more color if necessary: kAzure-9
  plotNRates(vHists, vLabels, vColors,
             xMin, xMax, yMin, yMax,
             "ET threshold [GeV]",
             "rates_barrel_overlaid_looseTkWP",
             outputDirectory,
             useLogy);

  vHists.clear();  vLabels.clear();  vColors.clear();

}
