#include "sndPMUWeightProcess.h"
#include "TROOT.h"
#include "TGlobal.h"
#include "TInterpreter.h"
#include "ShipMCTrack.h"
#include "FairLogger.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TFile.h"

namespace snd {
  namespace analysis_processes {

    sndPMUWeightProcess::sndPMUWeightProcess(bool isPMU, int nFiles)
      : isPMU_(isPMU), 
        nFiles_(nFiles), // Capture the number of files
        event_weight_(nullptr), 
        event_weight_value_(1.0), 
        p_event_weight_value_(&event_weight_value_)
    {
      processName = "sndPMUWeightProcess";
      
      // Safety check to prevent division by zero
      if (nFiles_ <= 0) {
          LOG(WARNING) << "sndPMUWeightProcess: nFiles is " << nFiles_ << ". Setting to 1 to avoid crash.";
          nFiles_ = 1;
      }

      // Setup Global for histograms
      if (!gROOT->GetListOfGlobals()->FindObject("sndPMU_event_weight")) {
          gInterpreter->Declare("double sndPMU_event_weight = 1.0;");
      }
      auto* globalObj = gROOT->GetListOfGlobals()->FindObject("sndPMU_event_weight");
      if (globalObj) {
        event_weight_ = static_cast<double*>(dynamic_cast<TGlobal*>(globalObj)->GetAddress());
      }
    }

    void sndPMUWeightProcess::process()
    {
      static bool branch_initialized = false;
      if (!branch_initialized) {
        TTree* outTree = nullptr;
        TFile* outFile = nullptr;

        TIter nextFile(gROOT->GetListOfFiles());
        TFile* f = nullptr;
        while ((f = (TFile*)nextFile())) {
          if (f->IsWritable()) {
            outFile = f;
            outTree = (TTree*)f->Get("cbmsim");
            if (outTree) break; 
          }
        }

        if (outTree && outFile) {
          if (!outTree->GetBranch("pmu_weight")) {
            outTree->Branch("pmu_weight", &event_weight_value_, "pmu_weight/D");
            outTree->SetDirectory(outFile); 
            LOG(INFO) << "sndPMUWeightProcess: Branch 'pmu_weight' attached using nFiles = " << nFiles_;
          }
          branch_initialized = true;
        } else {
          branch_initialized = true; 
        }
      }

      event_weight_value_ = 1.0;

      if (!isPMU_) {
        if (event_weight_) *event_weight_ = 1.0;
        return;
      }

      auto* mcTrackCollection = dynamic_cast<TClonesArray*>(gROOT->GetListOfGlobals()->FindObject("MCTrack"));
      
      if (mcTrackCollection && mcTrackCollection->GetEntries() > 0) {
	LOG(WARNING) << "Entry: " << mcTrackCollection->GetEntries();
	
	auto* aTrack = static_cast<ShipMCTrack*>(mcTrackCollection->At(0));
	double weight = (1e5 * 4.0 * aTrack->GetWeight()) / static_cast<double>(nFiles_);
	LOG(WARNING) << "Weight: " << weight;
	
	bool isHadronic = (aTrack->GetMotherId() == 0 &&
			   aTrack->GetStartZ() > 360. &&
			   aTrack->GetStartZ() < 450. &&
			   (aTrack->GetProcID() >= 23 || aTrack->GetProcID() <= 27 || aTrack->GetProcID() == 13 || aTrack->GetProcID() == 46));

	if (isHadronic) {
	  event_weight_value_ = 0.0;
	} else {
	  event_weight_value_ = weight;
	}
      
	LOG(WARNING) << "Calculated Weight: " << event_weight_value_;
      } else {
	LOG(WARNING) << "MCTrack not found! Weight defaulting to 1.0";
      }
      //if (event_weight_) {
        *event_weight_ = event_weight_value_;
	//}
    }
  }
}
