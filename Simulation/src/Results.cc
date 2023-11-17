
#include "Results.hh"

#include <cmath>

#include <iostream>
#include <iomanip>


void WriteResults(struct Results& res, int numEvents) {
  // for the histograms, bring them to be mean per event and write
  const double norm = numEvents > 0 ? 1.0/numEvents : 1.0;
  res.fEdepPerLayer.Scale(norm);
  res.fGammaTrackLenghtPerLayer.Scale(norm);
  res.fElPosTrackLenghtPerLayer.Scale(norm);

  res.fEdepPerLayer.WriteToFile(false);
  res.fGammaTrackLenghtPerLayer.WriteToFile(false);
  res.fElPosTrackLenghtPerLayer.WriteToFile(false);

  //
  res.fEdepAbs  = res.fEdepAbs*norm;
  res.fEdepAbs2 = res.fEdepAbs2*norm;
  const double rmsEAbs = std::sqrt(std::abs(res.fEdepAbs2 - res.fEdepAbs*res.fEdepAbs));

  res.fEdepGap  = res.fEdepGap*norm;
  res.fEdepGap2 = res.fEdepGap2*norm;
  const double rmsEGap = std::sqrt(std::abs(res.fEdepGap2 - res.fEdepGap*res.fEdepGap));


  // the secondary type and step number statistics
  std::cout << std::endl;
  std::cout << " --- Results::WriteResults ---------------------------------- " << std::endl;
  std::cout << std::setprecision(6);
  std::cout << " Absorber: mean Edep = " << res.fEdepAbs << " [MeV] and  Std-dev = " << rmsEAbs << " [MeV]"<< std::endl;
  std::cout << " Gap     : mean Edep = " << res.fEdepGap << " [MeV] and  Std-dev = " << rmsEGap << " [MeV]"<< std::endl;

  std::cout << std::endl;
  std::cout << std::setprecision(14);
  std::cout << " Mean number of gamma       " << res.fNumSecGamma*norm    << std::endl;
  std::cout << " Mean number of e-          " << res.fNumSecElectron*norm << std::endl;
  std::cout << " Mean number of e+          " << res.fNumSecPositron*norm << std::endl;

  std::cout << std::endl;
  std::cout << std::setprecision(6)
            << " Mean number of e-/e+ steps " << res.fNumStepsElPos*norm  << std::endl;
  std::cout << " Mean number of gamma steps " << res.fNumStepsGamma*norm  << std::endl;
  std::cout << " ------------------------------------------------------------\n";

}
