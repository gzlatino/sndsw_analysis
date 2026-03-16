#pragma once

#include "sndMuFilterBaseCut.h"

#include "TChain.h"

namespace snd {
  namespace analysis_cuts {

    class vetoHitsCut : public snd::analysis_cuts::MuFilterBaseCut {
    public :
      vetoHitsCut();
      ~vetoHitsCut(){;}
      void process();
    };

  }
}
