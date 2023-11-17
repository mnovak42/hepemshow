
#include "URandom.hh"

URandom::URandom(int seed) {
  fEngine.seed(seed);
  fDist = new std::uniform_real_distribution<double>(0.0, 1.0);
}

URandom::~URandom() {
  delete fDist;
}

double URandom::flat() {
  return fDist->operator()(fEngine);
}
