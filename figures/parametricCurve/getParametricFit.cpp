#include "cutoffValues.h"
#include "fillTH2D.h"
#include "parametricFit.h"

TF1* getIsolationParametricFit(void) {

    TString rootFileDirectory = "/eos/user/s/skkwan/phase2RCTDevel/analyzer_DoubleElectron_partial.root";
    TString treePath = "l1NtupleProducer/efficiencyTree";

    TChain *ch = getTChainFromSingleFile(rootFileDirectory, treePath);
    TH2D *h2_iso = fillTH2DIsolationVsPt(ch);
    TGraph *tGraph_iso = getCutoffOfTH2DAsTGraph(h2_iso);

    parametricFit paramFitIso = parametricFit(tGraph_iso, 70.0);

    return paramFitIso.fitLinearPortion();

}