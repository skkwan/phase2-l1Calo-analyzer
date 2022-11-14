#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "../baseCodeForPlots/getTChain.h"


#ifndef FILL_TH2D_H_INCL
#define FILL_TH2D_H_INCL

/*
 * Return a TH2D with the relative isolation vs. cluster pT using the provided TChain.
 */

TH2D* fillTH2DIsolationVsPt(TChain *ch, double ymax = 10.0) {

  // Declare the branches we want to read
  TTreeReader myReader(ch);
  TTreeReaderValue<Double_t> reader_iso(myReader, "gct_iso");
  TTreeReaderValue<Double_t> reader_cPt(myReader, "gct_cPt");

  // Create and fill a 2D histogram
  TH2D *h2 = new TH2D("h2", "h2", 20, 0.0, 100.0, 200, 0.0, ymax);

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

TH2D* fillTH2DShapeVarVsPt(TChain *ch) {


  // Declare the branches we want to read
  TTreeReader myReader(ch);
  TTreeReaderValue<Double_t> reader_cPt(myReader, "gct_cPt");
  TTreeReaderValue<Double_t> reader_et2x5(myReader, "gct_et2x5");
  TTreeReaderValue<Double_t> reader_et5x5(myReader, "gct_et5x5");

  // Create and fill a 2D histogram
  TH2D *h2 = new TH2D("Shape var", "Shape var", 20, 0.0, 100.0, 200, 0.4, 1.05);


  // Loop over all entries of the TChain
  while (myReader.Next()) {
    h2->Fill(*reader_cPt, *reader_et2x5/ *reader_et5x5);
  }

  return h2;

}


#endif