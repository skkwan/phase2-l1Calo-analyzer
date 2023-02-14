

#ifndef CUTOFF_VALUES_H
#define CUTOFF_VALUES_H

/* 
 * For a given x-axis bin of a TH2D, return the y-axis value, below which 95% of the events fall. If countUpwards
 * is false, counts downwards from large values of the y-axis.
 * acceptancePerBin is e.g. 0.98 or 0.99.
 */

double getCutoffValue(TH2D* h2, int iBinX, float acceptancePerBin, bool countUpwards = true) {

    int nBinsYToScan = h2->GetNbinsY();

    double nEvents = 0;

    for (int iBinY = 0; iBinY < nBinsYToScan; iBinY++) {
        // std::cout << h2->GetBinContent(iBinX, iBinY) << std::endl;

        nEvents += h2->GetBinContent(iBinX, iBinY);
    }

    // 99% cutoff
    double nEventsBelowCutoff = nEvents * acceptancePerBin;

    std::cout << "nEvents and nEvents below cut-off: " << nEvents << ", " << nEventsBelowCutoff << std::endl;

    double nEventsCounting = 0;
    double yCutoff = 0.0;

    if (countUpwards) {
        for (int iBinY = 0; (iBinY < nBinsYToScan); iBinY++) {
            if (nEventsCounting >= nEventsBelowCutoff) {
                yCutoff = h2->GetYaxis()->GetBinCenter(iBinY);
                // std::cout << ">>> Cut-off reached at iBinY " << iBinY << " and cut-off " << yCutoff << ", breaking out of loop" << std::endl;

                break;
            }
            else {
                nEventsCounting += h2->GetBinContent(iBinX, iBinY);
                // std::cout << ">>> Cut-off not reached, adding iBinY " << iBinY << " with content " << h2->GetBinContent(iBinX, iBinY)
                //           << " to get a running total of " << nEventsCounting << std::endl;

            }
        }
    }
    else {
        // Count downwards
        for (int iBinY = nBinsYToScan; iBinY >= 0; iBinY--) {
            if (nEventsCounting >= nEventsBelowCutoff) {
                yCutoff = h2->GetYaxis()->GetBinCenter(iBinY);
//                std::cout << ">>> Cut-off reached at iBinY " << iBinY << " and cut-off " << yCutoff << ", breaking out of loop" << std::endl;

                break;
            }
            else {
                nEventsCounting += h2->GetBinContent(iBinX, iBinY);
                // std::cout << ">>> Cut-off not reached, adding iBinY " << iBinY << " with content " << h2->GetBinContent(iBinX, iBinY)
                //         << " to get a running total of " << nEventsCounting << std::endl;

            }
        }
    }

    return yCutoff;
    
}


/*
 * Return a TGraph representing the cut-off points of a TH2D per the above definition. If countUpwards is false,
 * integrate in each bin towards smaller values of the y-axis.
 */
TGraph* getCutoffOfTH2DAsTGraph(TH2D* h2, float acceptancePerBin, bool countUpwards = true) {
    
    std::vector<double> xVals, yVals;

    // Need to add 2 to get full coverage to overflow bin
    int nBinsXToScan = h2->GetNbinsX() + 2;

    for (int iBinX = 1; iBinX < nBinsXToScan; iBinX++) {

        double thisXValue = h2->GetXaxis()->GetBinCenter(iBinX);
        double thisCutoff = getCutoffValue(h2, iBinX, acceptancePerBin, countUpwards);

        std::cout << ">>>> Pushing back " << thisXValue << "," << thisCutoff << std::endl;
        
        xVals.push_back(thisXValue);
        yVals.push_back(thisCutoff);
        
    }

    // TGraph constructor requires TVectorDs
    TVectorD tXvals(xVals.size(), &xVals[0]);
    TVectorD tYvals(yVals.size(), &yVals[0]);

    TGraph *tGraph = new TGraph(tXvals, tYvals);

    return tGraph;
}

#endif