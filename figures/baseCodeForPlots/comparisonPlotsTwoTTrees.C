#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TEfficiency.h"
#include "TFile.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TH1.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TLegend.h"
#include "THStack.h"
#include "TMath.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

#include <cmath>
#include <iostream>
#include <math.h>
#include <string>

#include "tdrstyle.C"
#include "CMS_lumi.h"

#ifndef COMPARISON_PLOTS_TWO_TTREES_INCL
#define COMPARISON_PLOTS_TWO_TTREES_INCL
 
/* Apply template style to a TPad* pad1. */
void applyPadStyle(TPad* pad1){
  pad1->SetFillColor(0);
  pad1->Draw();  pad1->cd();  pad1->SetLeftMargin(0.2);  pad1->SetBottomMargin(0.13); pad1->SetRightMargin(0.1);
  //pad1->SetGrid(); 
  pad1->SetGrid(10,10); 
}

/* Apply legend style to a TLegend *leg. */
void applyLegStyle(TLegend *leg){
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetHeader("");
  leg->SetTextSize(0.05);
  leg->SetTextFont(42);
  leg->SetShadowColor(0);
}
 
/* Apply smaller legend style to a TLegend *leg. */
void applySmallerLegStyle(TLegend *leg){
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.044);
  leg->SetTextFont(42);
  leg->SetHeader("");
  leg->SetShadowColor(0);
}

/* Generate a comparison plot of "variable", using the gen-level genCut, and selecting fakes using fakeCut. treePath specifies the tree in the ROOT file to use.
   The ROOT file is located at inputDirectory. The resulting plots are written to outputDirectory, with filename including "name". The histogram has (bins)
   number of bins and ranges from floats low to high. */
int comparisonPlotsTwoTTrees(TString variable1,
			     TString variable2,
			     TString cut1, TString cut2,
			     TString treePath1, TString treePath2,
			     TString inputDirectory1, TString inputDirectory2,
			     TString outputDirectory, TString name, int bins, float low, float high){ 
 
  //gROOT->LoadMacro("CMS_lumi.C");
  //gROOT->ProcessLine(".L ~/Documents/work/Analysis/PhaseIIStudies/2018/tdrstyle.C");
  setTDRStyle();
 
  //int bins = 30;
  //int low = 0;
  //int high = 15;
 
  //  TFile* tauFile = new TFile("dummy");
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 800, 600);
  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);
  //TPad* pad1 = new TPad("pad1","The pad",0,0.0,0.98,1);
  //applyPadStyle(pad1);
 
  TLegend *leg = new TLegend(0.60,0.70,0.9,0.9);
  applyLegStyle(leg);
 
  TFile *file1 = new TFile(inputDirectory1);
  TFile *file2 = new TFile(inputDirectory2); 
 
  if(!file1->IsOpen()||file1==0){
    std::cout<<"ERROR FILE1 "<< inputDirectory1 + name<<" NOT FOUND; EXITING"<<std::endl;
    return 0;
  }
  if(!file2->IsOpen()||file2==0){
    std::cout<<"ERROR FILE2 "<< inputDirectory1 + name<<" NOT FOUND; EXITING"<<std::endl;
    return 0;
  }
 
  //gStyle->SetOptFit(0);
  //gStyle->SetOptStat(0);
  //gStyle->SetEndErrorSize(0);
  //gStyle->SetErrorX(0.5);
 
  TTree* tree1 = (TTree*)file1->Get(treePath1);
  if(tree1 == 0){
    std::cout<<"ERROR Tree1  "<< tree1<<" NOT FOUND; EXITING"<<std::endl;
    return 0;
  }

  TTree* tree2 = (TTree*)file2->Get(treePath2);
  if(tree2 == 0){
    std::cout<<"ERROR Tree2  "<< tree2<<" NOT FOUND; EXITING"<<std::endl;
    return 0;
  }
 
  TH1F *h1 = new TH1F("h1","h1",bins,low,high);
  tree1->Draw(variable1+">>+h1", cut1);
  TH1F *h2 = new TH1F("h2","h2",bins,low,high);
  tree2->Draw(variable2+">>+h2", cut2);  
 
  /* Compute the ratio by cloning the h1 histogram, subtracting the Fake values,
     and dividing it by the Fake value. */
  /*
  TH1F *ratio = (TH1F*)h1->Clone();
  ratio->Add(Fake,-1);
  ratio->Divide(Fake);
  */

  h1->SetMarkerColor(0);
  h1->SetFillStyle(1001);
  h1->SetFillColorAlpha(kBlue+2, 0.1);
  h1->SetLineWidth(1);
  h1->SetLineColor(kBlue+2);

  h2->SetMarkerColor(0);
  h2->SetFillStyle(1001);
  h2->SetFillColorAlpha(kRed+2, 0.1);
  h2->SetLineWidth(1);
  h2->SetLineColor(kRed+2);

  // Fairly confident the yields are very similar, so do not normalize them
  // h2->Scale(1/h2->Integral());

  //  h1->Scale(1/h1->Integral());

  h2->Draw("HIST");  
  h1->Draw("HIST same"); 
  
  h2->GetXaxis()->SetTitle(name);
  h2->GetYaxis()->SetTitle("Events");

  // float maxHeight = 10; 
  // if(h2->GetXaxis()->GetBinCenter( h2->GetMaximumBin() ) > h1->GetXaxis()->GetBinCenter( h1->GetMaximumBin() ) ) { 
  //   maxHeight = h2->GetXaxis()->GetBinCenter(h2->GetMaximumBin()); 
  // } 
  // else { 
  //   maxHeight = h1->GetXaxis()->GetBinCenter(h1->GetMaximumBin()); 
  // } 
   
  // std::cout<<"max: "<<maxHeight<<std::endl; 
  // h2->SetMaximum(maxHeight*2.0); 
  // h1->SetMaximum(maxHeight*2.0); 
  // Tcan->cd(); 

  
  leg->AddEntry(h1,"Current emulator","l");
  leg->AddEntry(h2,"Old emulator","l");
  leg->Draw();
 
  Tcan->cd();

  //TPad* pad2 = new TPad("pad2","The lower pad",0,0,0.98,0.25);
  //applyPadStyle(pad2);
  //pad2->cd();
  //pad2->SetGrid(0,0); 
 
  //ratio->Draw("p");

 
  Tcan->cd();
  Tcan->SetLogy();
  Tcan->SaveAs(outputDirectory+name+".pdf");
 
  delete Tcan;

  return 1;

}

#endif
