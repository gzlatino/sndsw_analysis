// #pragma once

// #include "sndSciFiBaseCut.h"

// #include "TChain.h"
// #include "sndScifiHit.h"

// namespace snd {
//   namespace analysis_cuts {

//     class sciFiStationCut : public snd::analysis_cuts::sciFiBaseCut {
//     private :
//       float fraction_threshold_;
//       std::vector<int> excluded_stations_;
//     public :
//       sciFiStationCut(float threshold, std::vector<int> excluded_stations);
//       ~sciFiStationCut(){;}

//       void process();
//     };

//   }
// }

#pragma once

#include "sndSciFiBaseCut.h"

#include "TChain.h"
#include "sndScifiHit.h"

namespace snd {
  namespace analysis_cuts {

    class sciFiStationCut : public snd::analysis_cuts::sciFiBaseCut {
    private :
      int target_station_;
      int hit_threshold_;
    public :
      sciFiStationCut(int target_station, int hit_threshold);
      ~sciFiStationCut(){;}

      void process();
    };

  }
}
