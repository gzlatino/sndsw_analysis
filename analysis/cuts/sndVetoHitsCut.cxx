#include "sndVetoHitsCut.h"

#include "TClonesArray.h"
#include "TChain.h"
#include "MuFilterHit.h"

namespace snd::analysis_cuts {

  vetoHitsCut::vetoHitsCut() : MuFilterBaseCut() {
    processName = "Total Veto Hits >1; Hits per plane <2";

    shortName = "Clean Muon Veto Cut";
    nbins = std::vector<int>{16};
    range_start = std::vector<double>{0};
    range_end = std::vector<double>{16};
    plot_var = std::vector<double>{-1};

  }

  void vetoHitsCut::process(){
    plot_var[0] = 0;
    
    int totalHits = 0;
    int hitsPerPlane[2] = {0,0};

    for (TObject *obj : *muFilterDigiHitCollection)
      {
        MuFilterHit * hit = dynamic_cast<MuFilterHit*>(obj);
        if (!hit) continue;
        if (hit->GetSystem() == 1)
          {
            totalHits++;
            int plane = hit->GetPlane();
            if (plane >= 0 && plane <2)
              hitsPerPlane[plane]++;
          }
      }

    if (totalHits > 1 && hitsPerPlane[0] <= 2 && hitsPerPlane[1] <= 2)
      {
        plot_var[0] += 1;
        passed_cut = true;
        return;
      }

  }

}
