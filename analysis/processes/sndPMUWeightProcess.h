// #pragma once

// #include "sndFilterProcessBase.h"
// #include "TClonesArray.h"
// #include "TH1D.h"
// #include "TChain.h"

// namespace snd {
//   namespace analysis_processes {

//     class sndPMUWeightProcess : public snd::analysis_core::baseProcess
//     {
//     public:
//       sndPMUWeightProcess(bool isPMU = true);
//       void process();

//     private:
//       bool isPMU_;
//       double event_weight_value_;
//       double* p_event_weight_value_;
//       double* event_weight_;
//       TClonesArray* mcTrackCollection_ = nullptr;
//     };

//   }
// }

#pragma once

#include "sndFilterProcessBase.h"
#include "TClonesArray.h"

namespace snd {
  namespace analysis_processes {

    class sndPMUWeightProcess : public snd::analysis_core::baseProcess
    {
    public:
      // Added nFiles parameter
      sndPMUWeightProcess(bool isPMU = true, int nFiles = 1);
      void process() override;

    private:
      bool isPMU_;
      int nFiles_; // Store the number of files
      double event_weight_value_;
      double* p_event_weight_value_;
      double* event_weight_;
    };

  }
}
