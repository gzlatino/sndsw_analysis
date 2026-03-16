#pragma once

#include "sndBaseCut.h"
#include "TClonesArray.h"
#include "SNDLHCEventHeader.h"
#include "TChain.h"

namespace snd {
  namespace analysis_cuts {
    class MCTrackBaseCut : public snd::analysis_cuts::baseCut {

      double GetEventWeight() const;

    protected:
      TChain* tree;
      TClonesArray* mcTracks;
      SNDLHCEventHeader* header;

      MCTrackBaseCut();
      ~MCTrackBaseCut(){;}
    };

  }
}
