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

            fLinearPortion = *(f1);

        };

    public:

        /*
         * Constructor
         */
        parametricFit(TGraph* tgraphIn, double xThresholdForPlateau) {

            xPlateauThreshold = xThresholdForPlateau;
            yPlateauPoint = -99;  // initialize

            for (int i = 0; i < tgraphIn->GetN(); i++) {

                double xVal = tgraphIn->GetPointX(i);
                double yVal = tgraphIn->GetPointY(i);
                // std::cout << "Input point: " << i << "(" << xVal << ", " << yVal << ")" << std::endl;

                // Save each input point
                xInVals.push_back(xVal);
                yInVals.push_back(yVal);

                // Skip the first point
                if (i == 0) {
                    continue;
                }
                // Save each point in the linear portion separately
                else if (xVal < xPlateauThreshold) {
                    xFitVals.push_back(xVal);
                    yFitVals.push_back(yVal);
                }
                else {
                    // Save the plateau point
                    if (yPlateauPoint == -99) {
                        xPlateauPoint = xVal;
                        yPlateauPoint = yVal;
                        std::cout << "Set plateau point " << xPlateauPoint << ", " << yPlateauPoint << std::endl;
                    }
                }
            }

            fitLinearPortion();
        }

        // The linear fit function as a string expression
        void linearFitStringRepr() {
            double p0 = fLinearPortion.GetParameter(0);
            double p1 = fLinearPortion.GetParameter(1);

            std::cout << "Parameters are " << p0 << " and " << p1 << std::endl;
        }

        /*
         * Return a plottable TGraph
         */
        TGraph tGraphRepr() {

            // Also extrapolate to x = 0 for plotting purposes
            xGraphVals.push_back(0.0);
            yGraphVals.push_back(fLinearPortion.Eval(0.0));

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
};



#endif