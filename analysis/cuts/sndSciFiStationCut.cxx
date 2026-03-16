#include "sndSciFiStationCut.h"

#include "sndSciFiTools.h"

#include "TChain.h"

// namespace snd::analysis_cuts{
//   sciFiStationCut::sciFiStationCut(float fraction_threshold, std::vector<int> excluded_stations) : sciFiBaseCut(), fraction_threshold_(fraction_threshold), excluded_stations_(excluded_stations){

//     processName = "Exclude stations";
//     for (int sta : excluded_stations_){
//       processName += " "+std::to_string(sta);
//     }
//     processName += ". Threshold "+std::to_string(fraction_threshold_);

//     shortName = "SciFiStation";
//     for (int sta : excluded_stations_) shortName += "_"+std::to_string(sta);

//     nbins = std::vector<int>{5};
//     range_start = std::vector<double>{1};
//     range_end = std::vector<double>{6};
//     plot_var = std::vector<double>{-1};
//   }

//   void sciFiStationCut::process(){
//     initializeEvent();
    
//     int station = snd::analysis_tools::findScifiStation(*hits_per_plane_horizontal, *hits_per_plane_vertical, fraction_threshold_);
    
//     plot_var[0] = station;

//     if (std::find(excluded_stations_.begin(), excluded_stations_.end(), station) == excluded_stations_.end()){
//       passed_cut = true; return;
//     } else {
//       passed_cut = false; return;
//     }
//   }
// }	     

namespace snd::analysis_cuts {

  // Constructor: Now takes the station number (1, 2, 3...) and the hit limit
  sciFiStationCut::sciFiStationCut(int target_station, int hit_threshold) 
    : sciFiBaseCut(), target_station_(target_station), hit_threshold_(hit_threshold) {

    processName = "Station " + std::to_string(target_station_) + 
                  " Hits < " + std::to_string(hit_threshold_);

    shortName = "SciFiStation" + std::to_string(target_station_) + 
                "_Limit" + std::to_string(hit_threshold_);

    // Adjust plot ranges to fit hit counts (e.g., 0 to 50 hits)
    nbins = std::vector<int>{50};
    range_start = std::vector<double>{0};
    range_end = std::vector<double>{50};
    plot_var = std::vector<double>{-1};
  }

  void sciFiStationCut::process() {
    initializeEvent();

    // Convert Human Station Number (1, 2, 3...) to Vector Index (0, 1, 2...)
    int stationIdx = target_station_ - 1;

    // Safety check: Ensure the station index exists in the data
    if (stationIdx < 0 || stationIdx >= (int)hits_per_plane_horizontal->size()) {
        passed_cut = false; 
        return;
    }

    // Calculate total hits for this specific station
    int totalHits = hits_per_plane_horizontal->at(stationIdx) + 
                    hits_per_plane_vertical->at(stationIdx);

    // Update plotting variable for monitoring
    plot_var[0] = totalHits;

    // The Logic: Pass only if hits are BELOW the threshold
    if (totalHits < hit_threshold_) {
      passed_cut = true;
    } else {
      passed_cut = false;
    }
    
    return;
  }
}
