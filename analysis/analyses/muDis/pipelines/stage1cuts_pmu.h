#include "TROOT.h"

#include "sndFilterProcessBase.h"

#include "sndSciFiHitFilter.h"

#include "sndStableBeams.h"
#include "sndIP1.h"
#include "sndEventDeltat.h"

#include "sndPMUWeightProcess.h"
#include "sndVetoCut.h"
#include "sndVetoHitsCut.h"
#include "sndSciFiStationCut.h"
#include "sndUSPlanesHit.h"
#include "sndMinSciFiHitsCut.h"
#include "sndUsSpatialAnisotropyCut.h"
#include "sndUSQDCCut.h"

#include "sndCutFlowHist.h"

/* bool isMC = true; */
/* if (gROOT->GetListOfGlobals()->FindObject("rawConv")) isMC = false; */
/*  */
/* bool isMC = true; */
/* if (gROOT->GetListOfGlobals()->FindObject("cbmsim")) isMC = true; */

bool isMC = (gROOT->FindObjectAny("cbmsim") != nullptr);
cout << "[INFO] isMC = " << isMC << endl;

std::vector< snd::analysis_core::baseProcess * > pipeline;

// CUts related to event header
if (not isMC) {
  pipeline.push_back( new snd::analysis_cuts::stableBeamsCut());
  pipeline.push_back( new snd::analysis_cuts::ip1Cut());
  pipeline.push_back( new snd::analysis_cuts::eventDeltatCut(-1, 100)); // J. Previous event more than 100 clock cycles away. To avoid deadtime issues.
 }

pipeline.push_back(new snd::analysis_processes::sndPMUWeightProcess(isMC));
pipeline.push_back(new snd::analysis_processes::sciFiHitFilter(0.5*6.25, 1.2*6.25, 52, 0, 26)); // Select SciFi hits within [-0.5, 1.2] clock cycles of peak time

// Fiducial volume cuts
pipeline.push_back( new snd::analysis_cuts::vetoHitsCut()); // B. Total hits >1; Hits per plane < 2

// SciFi hit selection
//      sciFiHitFilter(float time_lower_range, float time_upper_range, float bins_x, float min_x, float max_x);

pipeline.push_back( new snd::analysis_cuts::sciFiStationCut(1, 6)); // C. <6 hits in the 1st SciFi Stati \
on                                                                                                       
pipeline.push_back( new snd::analysis_cuts::USPlanesHit(std::vector<int>{0, 1, 2, 3, 4})); // All US Pla \
nes active                                                                                               
//pipeline.push_back(new snd::analysis_cuts::minSciFiHits(8));
//pipeline.push_back(new snd::analysis_cuts::usSpatialAnisotropyCut(0.96));

if (isMC) pipeline.push_back( new snd::analysis_cuts::USQDCCut(700)); // Min QDC
 else      pipeline.push_back( new snd::analysis_cuts::USQDCCut(500)); //

// This creates all the cut flow histograms. Must be added after all the cuts.                          
pipeline.push_back( new snd::analysis_processes::cutFlowHist());
