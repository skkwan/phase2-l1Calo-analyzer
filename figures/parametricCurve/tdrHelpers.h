

#ifndef TF1_HELPERS_H_INCL
#define TF1_HELPERS_H_INCL

static constexpr float slideIsoPtThreshold = 80;
static constexpr float a0_80 = 0.85, a1_80 = 0.0080, a0 = 0.21;                        // passes_iso

bool from_oldEmulator_passes_iso(float pt, float iso) {
  bool is_iso = true;
  if (pt < slideIsoPtThreshold) {
    if (!((a0_80 - a1_80 * pt) > iso))
      is_iso = false;
  } else {
    if (iso > a0)
      is_iso = false;
  }
  if (pt > 130)
    is_iso = true;
  return is_iso;
}



/*
 * Get a TGraph representing the isolation curve of the TDR iso function, for a given set of x-values to evaluate the function on.
 */
TGraph getTdrIsoAsTGraph(std::vector<double> xGraphValsIn) {

  static double slideIsoPtThreshold = 80;
  static double a0_80 = 0.85, a1_80 = 0.0080, a0 = 0.21;    

  // X- and Y- values of plots to point
  std::vector<double> xGraphVals;
  std::vector<double> yGraphVals;

  for (unsigned int i = 0; i < xGraphValsIn.size(); i++) {
    double xVal = xGraphValsIn[i];
    double yVal = 0.0;

    if (xVal < 80) {
      yVal = (a0_80 - a1_80 * xVal);
    }
    else {
      yVal = a0;
    }
    if (xVal > 130) {
      yVal = 5.0; // representing no cut on y-axis
    }
    xGraphVals.push_back(xVal);
    yGraphVals.push_back(yVal);

    std::cout << "getTdrIsoAsTGraph: pushing back (" << xVal << ", " << yVal << ")" << std::endl;
  }

  // TGraph constructor requires TVectorDs
  TVectorD tXvals(xGraphVals.size(), &xGraphVals[0]);
  TVectorD tYvals(yGraphVals.size(), &yGraphVals[0]);

  TGraph *tgraphPlot = new TGraph(tXvals, tYvals);

  return *tgraphPlot;

}

#endif