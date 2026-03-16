#include "sndUsSpatialAnisotropyCut.h"

#include "TClonesArray.h"
#include "TChain.h"
#include "MuFilterHit.h"
#include "TString.h"
#include "TVector3.h"

#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TVectorD.h"


#include <vector>
#include <numeric>
#include <cmath>

namespace snd {
  namespace analysis_cuts {
    
    usSpatialAnisotropyCut::usSpatialAnisotropyCut(double maxA)
      : MuFilterBaseCut(), maxAnisotropy(maxA)
    {
      processName = "US spatial anisotropy";
      shortName   = "usAniso";

      nbins = {100};
      range_start = {0.};
      range_end   = {20.};
      plot_var    = {-1.};
    }

    // void usSpatialAnisotropyCut::process(){

    //   std::vector<double> xs, ys;

    //   for (TObject* obj : *muFilterDigiHitCollection) {
    // 	auto* hit = dynamic_cast<MuFilterHit*>(obj);
    // 	if (!hit) continue;

    // 	if (hit->GetSystem() == 2) continue;

    // 	// TVector3 pos = hit->GetPosition();
    // 	// x.push_back(pos.X());
    // 	// y.push_back(pos.Y());
    // 	// z.push_back(pos.Z());
    // 	TVector3 L, R;
    // 	hit->GetPosition(L, R);
    // 	TVector3 pos = 0.5 * (L + R);

    // 	xs.push_back(pos.X());
    // 	ys.push_back(pos.Y());

    //   };

    //   if (xs.size() < 3) return; // not enough info

    //   auto rms = [](const std::vector<double>& v) {
    // 	double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    // 	double s = 0;
    // 	for (double x : v) s += (x - mean) * (x - mean);
    // 	return std::sqrt(s / v.size());
    //   };

    //   double sx = rms(xs);
    //   double sy = rms(ys);

    //   // double smax = std::max({sx, sy, sz});
    //   // double smin = std::max(1e-6, std::min({sx, sy}));
    //   if (sx <= 0 || sy <= 0) return;

    //   double anisotropy = std::max(sx, sy) / std::min(sx, sy);
    //   plot_var[0] = anisotropy;

    //   if (anisotropy < maxAnisotropy) {
    // 	passed_cut = true;
    //   }
    // }

    void usSpatialAnisotropyCut::process() {
      plot_var[0] = -1;
      passed_cut = false;

      std::vector<TVector3> positions;

      for (TObject* obj : *muFilterDigiHitCollection) {
        MuFilterHit* hit = dynamic_cast<MuFilterHit*>(obj);
        if (!hit) continue;
        if (hit->GetSystem() == 2) continue; // US only

        TVector3 L, R;
        hit->GetPosition(L, R);
        positions.push_back(0.5*(L+R));
      }

      if (positions.size() < 3) return;

      // mean
      TVector3 mean(0,0,0);
      for (auto& p : positions) mean += p;
      mean *= 1.0 / positions.size();

      // covariance
      TMatrixDSym cov(3);
      cov.Zero();
      for (auto& p : positions) {
        TVector3 d = p - mean;
        cov(0,0) += d.X()*d.X();
        cov(0,1) += d.X()*d.Y();
        cov(0,2) += d.X()*d.Z();
        cov(1,1) += d.Y()*d.Y();
        cov(1,2) += d.Y()*d.Z();
        cov(2,2) += d.Z()*d.Z();
      }
      cov *= 1.0 / positions.size();
      cov(1,0) = cov(0,1);
      cov(2,0) = cov(0,2);
      cov(2,1) = cov(1,2);

      // eigenvalues
      TMatrixDSymEigen eig(cov);
      TVectorD ev = eig.GetEigenValues();

      std::vector<double> l = {ev[0], ev[1], ev[2]};
      std::sort(l.begin(), l.end(), std::greater<>());

      double anisotropy = l[0] / (l[0] + l[1] + l[2]);
      plot_var[0] = anisotropy;

      if (anisotropy > 0.8) passed_cut = true;
    }

  }
}
