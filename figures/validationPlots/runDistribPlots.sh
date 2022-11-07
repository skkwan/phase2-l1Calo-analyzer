# runDistribPlot.sh
# Usage: source runDistribPlot.sh


root -l -b -q 'makeDistributionPlots.C("cPt-gt-15_genPt-gt-15",                                             \
                                       "DoubleElectron Flat Pt 2 to 100",                                     \
                                       "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/analyzer-l1egammaCrystals.root", \
                                       "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/oldEmulator/", \
                                       "(gct_cPt > 15) && (genPt > 15) && (abs(genEta) < 1.4841)", "",     \
                                       "L1 p_{T} > 15, Gen p_{T} > 15",                                              \
                                       "1.0")'

# ### The usual reco/gen pT cuts, but with different scale factors

# root -l -b -q 'makeDistributionPlots.C("SF_0.985_cPt-gt-15_genPt-gt-15",                                             \
#                                        "DoubleElectron Flat Pt 2 to 100",
#                                        "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/analyzer_SingleElectron.root", \
#                                        "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/", \
#                                        "(gct_cPt > 15) && (genPt > 15) && (abs(genEta) < 1.4841)", "SF = 1.0",     \
#                                        "L1 p_{T} > 15, Gen p_{T} > 15",                                              \
#                                        "1.0")'

root -l -b -q 'makeDistributionPlots.C("SF_0.985_cPt-gt-15_genPt-gt-15",                                             \
                                       "DoubleElectron Flat Pt 2 to 100",
                                       "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/analyzer_SingleElectron.root", \
                                       "/Users/stephaniekwan/Dropbox/Princeton_G5/Phase2EGamma/analyzer/compare_Nov_01_2022/", \
                                       "(gct_cPt > 15) && (genPt > 15) && (abs(genEta) < 1.4841)", "SF = 0.995",     \
                                       "L1 p_{T} > 15, Gen p_{T} > 15",                                              \
                                       "0.995")'
