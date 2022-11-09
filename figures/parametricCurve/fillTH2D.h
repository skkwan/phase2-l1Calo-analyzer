#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "../baseCodeForPlots/getTChain.h"


#ifndef FILL_TH2D_H_INCL
#define FILL_TH2D_H_INCL

/*
 * Return a TH2D with the isolation vs. cluster pT. Optional arguments to use the input list of files scheme instead. If you use the second scheme then the first argument does not matter.
 */

TH2D* fillTH2DIsolationVsPt(TString rootFileDirectory, TString treePath, TString inputListOfFiles = "", int startLine = -1, int nLinesToRead = -1) {

  TChain *ch;
  if (inputListOfFiles == "") {
    ch = getTChainFromSingleFile(rootFileDirectory, treePath);
  }
  else {
    ch = getTChainFromListOfFiles(inputListOfFiles, treePath);
  }

  // Declare the branches we want to read
  TTreeReader myReader(ch);
  TTreeReaderValue<Double_t> reader_iso(myReader, "gct_iso");
  TTreeReaderValue<Double_t> reader_cPt(myReader, "gct_cPt");

  // Create and fill a 2D histogram
  TH2D *h2 = new TH2D("h2", "h2", 20, 0.0, 100.0, 100, 0.0, 11.0);

  // Loop over all entries of the TChain
  while (myReader.Next()) {
    // Access data as if iso and cPt were iterators (note the '*' in front of them)
    // std::cout << "iso and cpt:" << *reader_iso << " and " << *reader_cPt << std::endl;
    h2->Fill(*reader_cPt, *reader_iso);
  }

  return h2;

}


/*
 * Same but for et2x5/et5x5 vs. pT.
 */

TH2D* fillTH2DShapeVarVsPt(TString rootFileDirectory, TString treePath, TString inputListOfFiles = "", int startLine = -1, int nLinesToRead = -1) {

  TChain *ch;
  if (inputListOfFiles == "") {
    ch = getTChainFromSingleFile(rootFileDirectory, treePath);
  }
  else {
    ch = getTChainFromListOfFiles(inputListOfFiles, treePath);
  }

  // Declare the branches we want to read
  TTreeReader myReader(ch);
  TTreeReaderValue<Double_t> reader_cPt(myReader, "gct_cPt");
  TTreeReaderValue<Double_t> reader_et2x5(myReader, "gct_et2x5");
  TTreeReaderValue<Double_t> reader_et5x5(myReader, "gct_et5x5");

  // Create and fill a 2D histogram
  TH2D *h2 = new TH2D("Shape var", "Shape var", 20, 0.0, 100.0, 50, 0.4, 1.05);

  // Loop over all entries of the TChain
  while (myReader.Next()) {
    h2->Fill(*reader_cPt, *reader_et2x5/ *reader_et5x5);
  }

  return h2;

}


#endif