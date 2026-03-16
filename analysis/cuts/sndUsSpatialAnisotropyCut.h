#pragma once

#include "sndMuFilterBaseCut.h"

#include "TChain.h"

namespace snd{
  namespace analysis_cuts {

    class usSpatialAnisotropyCut : public MuFilterBaseCut {
    private:
      double maxAnisotropy;
    public:
      usSpatialAnisotropyCut(double maxA = 5.0);
      ~usSpatialAnisotropyCut(){;}
      void process();
    };

  }
}
