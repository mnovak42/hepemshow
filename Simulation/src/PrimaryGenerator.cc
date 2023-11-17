
#include "PrimaryGenerator.hh"

#include "G4HepEmTrack.hh"


// set initial values: 10 GeV e- at [0,0,0] pointing to [1,0,0]
PrimaryGenerator::PrimaryGenerator()
: fKinEnergy(1.0E+4),
  fPosition{0,0,0},
  fDirection{1,0,0},
  fCharge(-1.0) {}


void PrimaryGenerator::GenerateOne(G4HepEmTrack& primTrack) {
  primTrack.SetEKin(fKinEnergy);
  primTrack.SetPosition(fPosition);
  primTrack.SetDirection(fDirection);
  primTrack.SetCharge(fCharge);
}


void PrimaryGenerator::SetPosition(double* pos) {
  fPosition[0] = pos[0];
  fPosition[1] = pos[1];
  fPosition[2] = pos[2];
}

void PrimaryGenerator::SetPosition(double x, double y, double z) {
  fPosition[0] = x;
  fPosition[1] = y;
  fPosition[2] = z;
}


void PrimaryGenerator::SetDirection(double* dir) {
  fDirection[0] = dir[0];
  fDirection[1] = dir[1];
  fDirection[2] = dir[2];
}

void PrimaryGenerator::SetDirection(double x, double y, double z) {
  fDirection[0] = x;
  fDirection[1] = y;
  fDirection[2] = z;
}
