/*******************************************************************/
/* efficiencyHist.cpp                                              */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "TAxis.h"
#include "TChain.h"

#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

#include <assert.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <TMath.h>
#include <vector>

#include "../baseCodeForPlots/tdrstyle.C"
#include "../baseCodeForPlots/comparisonPlots.C"

#ifndef EFFICIENCY_HIST_CPP_INCL
#define EFFICIENCY_HIST_CPP_INCL

/*******************************************************************/

void plotNEfficiencies(std::vector<TGraphAsymmErrors*> graphs, 
                      std::vector<TString> labels,
                      std::vector<int> colors,
                      TString xAxisLabel,
                      TString legendName,
                      TString outputName,
                      TString outputDir,
                      TString comment = "",
                      float yMin = 0.0,
                      float yMax = 1.1,
                      TString legendPos = "bottomright"
             ) {
  assert((graphs.size() == labels.size()) && (graphs.size() == colors.size()));

  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 1000);
  TLegend *leg;
  if (legendPos == "bottomright") {
    leg = new TLegend(0.40,0.15,0.90,0.45);
  } 
  else if (legendPos == "topright") {
    leg = new TLegend(0.55,0.65,0.90,0.87);
  }
  applySmallerLegStyle(leg);

  Tcan->SetGrid();

  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextColor(kBlack);

  // Set current canvas
  Tcan->cd();  
  Tcan->SetFillColor(0);


  std::vector<TGraphAsymmErrors*>::iterator itGraph;
  std::vector<TString>::iterator itLabel;
  std::vector<int>::iterator itColor;
  
  TGraphAsymmErrors* histDummy;
  for (itGraph = graphs.begin(), itLabel = labels.begin(), itColor = colors.begin();
       itGraph != graphs.end();
       itGraph++, itLabel++, itColor++) {

    // only do this once 
    if (itGraph == graphs.begin()) {
      histDummy = new TGraphAsymmErrors(**itGraph);
    }
      
    // De-reference the iterator to get the TGraphAsymmErrors*
    (*itGraph)->SetMarkerColor(*itColor);
    (*itGraph)->SetMarkerStyle(kFullCircle);
    (*itGraph)->SetLineWidth(2);
    (*itGraph)->SetLineColor(*itColor);
    (*itGraph)->SetMarkerSize(1);
  }

  histDummy->SetMarkerColor(0);
  histDummy->SetLineColor(0);

  histDummy->Draw("");

  for (itGraph = graphs.begin(); itGraph != graphs.end(); itGraph++)  {
    (*itGraph)->Draw("P");
  }

  histDummy->GetXaxis()->SetTitle(xAxisLabel);
  histDummy->GetYaxis()->SetTitle("L1 Efficiency");
  histDummy->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                    
  /* Set y-axis limits */  
  histDummy->GetYaxis()->SetRangeUser(yMin, yMax);
  // histDummy->GetYaxis()->SetRangeUser(0.8, 1.02);

  // Customize legend 

  for (itGraph = graphs.begin(), itLabel = labels.begin();
       itGraph != graphs.end();
       itGraph++, itLabel++)
    {
      leg->AddEntry(*itGraph, "#scale[0.8]{" + *itLabel + "}",  "P");
    }
  leg->Draw();


  // Default to RCT label, use GCT if not
  TString emuLabel = "#scale[1.0]{#bf{CMS}} #scale[0.6]{#it{Phase 2 GCT emulator}}";  
  if (outputName.Contains("RCT")) {
    emuLabel = "#scale[1.0]{#bf{CMS}} #scale[0.6]{#it{Phase 2 RCT emulator}}";  
  }
  latex->DrawLatex(0.16, 0.960, emuLabel); 
  latex->DrawLatex(0.61, 0.960, "#scale[0.6]{200 PU, DoubleEle Pt 1 to 100}"); 

  // Commentary x and y-position
  float commentaryXpos = 0.50;
  
  float offset = 0.0;
  if (legendPos == "bottomright") {
    offset = 0;
  }
  else if (legendPos == "topright") {
    offset = 0.42;
  }
  float yRow1 = 0.540 + offset;
  float yRow2 = 0.480 + offset;
  float yRow3 = 0.420 + offset;

  latex->DrawLatex(commentaryXpos, yRow2, "#scale[0.7]{Phase-2 L1EG (Crystal, Barrel)}");
  latex->DrawLatex(commentaryXpos, yRow3, "#scale[0.7]{" + comment + "}");
  Tcan->Update();


  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName+".pdf");

  Tcan->Close();
  delete histDummy, leg, Tcan;
}
             

/*******************************************************************/

#endif
