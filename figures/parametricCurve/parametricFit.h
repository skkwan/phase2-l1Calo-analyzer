#ifndef PARAMETRIC_FIT_H_INCL
#define PARAMETRIC_FIT_H_INCL



class parametricFit {

    private:
        double xPlateauThreshold; // desired threshold value

        double xPlateauPoint;  // actual threshold point
        double yPlateauPoint;  // actual threshold point
        
        std::vector<double> xInVals, yInVals;  // input values
        std::vector<double> xFitVals, yFitVals; // linear portion to fit
        std::vector<double> xGraphVals, yGraphVals; // all graphing values (includes extrapolated values and plateau)
        TF1 fLinearPortion;

        // Private: Fit linear part of the graph using private members xFitVals and yFitVals
        void fitLinearPortion() {

            // TGraph constructor requires TVectorDs
            TVectorD tXvals(xFitVals.size(), &xFitVals[0]);
            TVectorD tYvals(yFitVals.size(), &yFitVals[0]);

            TGraph *tgraphToFit = new TGraph(tXvals, tYvals);
            tgraphToFit->Fit("pol1"); // use pre-defined function

            // Access the fit results
            TF1 *f1 = tgraphToFit->GetFunction("pol1");

            // Set y-value of plateau point
            yPlateauPoint = f1->Eval(xPlateauThreshold);
            std::cout << "fitLinearPortion: Setting member yPlateauPoint to " << yPlateauPoint << std::endl;


            fLinearPortion = *(f1);

        };

    public:

        /*
         * Constructor
         * xThresholdForPlateau is the x-axis value at which to start the plateau. The y-axis value of the plateau will be the linear function
         * evaluated at xThresholdForPlateau.
         * 
         * iBinFitStart is the bin at which to start adding points to the linear fit, since we often want to skip the 0th, 1st, etc. bins.
         */
        parametricFit(TGraph* tgraphIn, double xThresholdForPlateau, int iBinFitStart) {

            xPlateauThreshold = xThresholdForPlateau;
            xPlateauPoint = -99;  // initialize

            for (int i = 0; i < tgraphIn->GetN(); i++) {

                double xVal = tgraphIn->GetPointX(i);
                double yVal = tgraphIn->GetPointY(i);
                std::cout << "Input point: " << i << "(" << xVal << ", " << yVal << ")" << std::endl;

                // Save each input point
                xInVals.push_back(xVal);
                yInVals.push_back(yVal);

                // Skip the first, or first and second point
                if (i < iBinFitStart) {
                    continue;
                }
                // Save each point in the linear portion separately
                else if (xVal < xPlateauThreshold) {
                    std::cout << "Adding to fitting set: (" << xVal << ", " << yVal << ")" << std::endl;
                    xFitVals.push_back(xVal);
                    yFitVals.push_back(yVal);
                }
                else {
                    // Save the plateau point
                    if (xPlateauPoint == -99) {
                        xPlateauPoint = xVal;
                    }
                }
            }

            fitLinearPortion();
        }

        /*
         * Get the fitting function as a string expression
         */
        TString linearFitStringRepr() {
            double p0 = fLinearPortion.GetParameter(0);
            double p1 = fLinearPortion.GetParameter(1);
            TString strRepr;
            strRepr.Form("( ((gct_cPt >= %.2f) && (gct_iso <= %f)) || ( (gct_cPt < %.2f) && ( gct_iso < ((%f * gct_cPt) + (%f)) ) ) )", xPlateauThreshold, yPlateauPoint, xPlateauThreshold, p1, p0);
            std::cout << strRepr << std::endl;
            return strRepr;
        }

        /*
         * Return a plottable TGraph
         */
        TGraph tGraphRepr() {

            // Also extrapolate to x = 0 for plotting purposes
            xGraphVals.push_back(0.0);
            yGraphVals.push_back(fLinearPortion.Eval(0.0));

            double thres = 70.0;
            std::cout << "Test: at " << thres << " GeV, the function is " << fLinearPortion.Eval(thres) << std::endl;

            for (int i = 0; i < xInVals.size(); i++) {

                std::cout << "Working on point x = " << xInVals[i] << ", ";

                xGraphVals.push_back(xInVals[i]);

                if (xInVals[i] < xPlateauThreshold) {
                    yGraphVals.push_back(fLinearPortion.Eval(xInVals[i]));
                    std::cout << "... pushed back point y (evaluated) = " << fLinearPortion.Eval(xInVals[i]) << std::endl;

                }
                else {
                    yGraphVals.push_back(yPlateauPoint);
                    std::cout << "... pushed back point y (plateau) = " << yPlateauPoint << std::endl;

                }
            }

            // TGraph constructor requires TVectorDs
            TVectorD tXvals(xGraphVals.size(), &xGraphVals[0]);
            TVectorD tYvals(yGraphVals.size(), &yGraphVals[0]);

            TGraph *tgraphPlot = new TGraph(tXvals, tYvals);

            return *tgraphPlot;
        }

        // Getters
        double getXPlateauPoint(void) { return xPlateauPoint; }
        double getYPlateauPoint(void) { return yPlateauPoint; }
        std::vector<double> getXGraphVals(void) { return xGraphVals; }
};



#endif