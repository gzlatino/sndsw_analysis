#include "sndMCTrackBaseCut.h"
#include "sndEventHeaderBaseCut.h"
#include <stdexcept>

#include "SNDLHCEventHeader.h"

#include "TROOT.h"
#include "TChain.h"

#include <iostream>


namespace snd::analysis_cuts {

MCTrackBaseCut::MCTrackBaseCut() {
    // 1. Find the Tree
    tree = dynamic_cast<TChain*>(gROOT->GetListOfGlobals()->FindObject("rawConv"));
    if (!tree) tree = dynamic_cast<TChain*>(gROOT->GetListOfGlobals()->FindObject("cbmsim"));

    // 2. Find the MCTrack TClonesArray
    mcTracks = dynamic_cast<TClonesArray*>(gROOT->GetListOfGlobals()->FindObject("MCTrack"));
    
    // 3. Find the EventHeader for the weight
    header = dynamic_cast<SNDLHCEventHeader*>(gROOT->GetListOfGlobals()->FindObject("EventHeader"));
    if (!header) header = dynamic_cast<SNDLHCEventHeader*>(gROOT->GetListOfGlobals()->FindObject("EventHeader."));

    if (!mcTracks) {
        std::cerr << "Warning: MCTrack branch not found in Globals!" << std::endl;
    }
}

double MCTrackBaseCut::GetEventWeight() const {
    if (mcTracks) {
      return cbmsim->MCTrack.fW(); 
    }
    return 1.0; // Default weight if header is missing
}

}
