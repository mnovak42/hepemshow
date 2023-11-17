


#include "Physics.hh"

// NOTE: you can edit/change any of these below at their location `G4HepEm-INSTALL-LOCATION/include/G4HepEm`
#include "G4HepEmElectronEnergyLossFluctuation.icc"
#include "G4HepEmElectronInteractionUMSC.icc"
#include "G4HepEmGammaInteractionConversion.icc"
#include "G4HepEmInteractionUtils.icc"
#include "G4HepEmElectronInteractionBrem.icc"
#include "G4HepEmElectronManager.icc"
#include "G4HepEmGammaInteractionPhotoelectric.icc"
#include "G4HepEmPositronInteractionAnnihilation.icc"
#include "G4HepEmElectronInteractionIoni.icc"
#include "G4HepEmGammaInteractionCompton.icc"
#include "G4HepEmGammaManager.icc"
#include "G4HepEmRunUtils.icc"


// implement the missing methods by using our local URandom random number generator
// NOTE: we plug in here our local, uniform random number generator that will
//       be then utilised during the simulation by G4HepEm(Run) through the
//       `G4HepEmRandomEngine` stored in the `G4HepEmTLData` structure
#include "G4HepEmRandomEngine.hh"
#include "URandom.hh"

double G4HepEmRandomEngine::flat() {
  return ((URandom*)fObject)->flat();
}

void G4HepEmRandomEngine::flatArray(const int size, double *vect) {
  for (int i = 0; i < size; i++) vect[i] = ((URandom*)fObject)->flat();
}
