
#include "Hist.hh"

#include <iostream>
#include <cstdio>

Hist::Hist(const std::string& fname, double min, double max, int numbin)
: fFileName(fname),
  fMin(min),
  fMax(max),
  fDelta(0.),
  fInvDelta(1.),
  fSum(0.),
  fNumBins(numbin) {
  fDelta    = (fMax - fMin) / fNumBins;
  fInvDelta = 1./fDelta;
  Initialize();
}

Hist::Hist(const std::string& fname, double min, double max, double delta)
: fFileName(fname),
  fMin(min),
  fMax(max),
  fDelta(delta),
  fInvDelta(1.),
  fSum(0.),
  fNumBins(0) {
  fInvDelta = 1./fDelta;
  fNumBins = (int)((fMax - fMin) / (fDelta)) + 1.0;
  Initialize();
}

Hist::Hist()
: fFileName("nothing"),
  fMin(0),
  fMax(10),
  fDelta(0.),
  fInvDelta(1.),
  fSum(0.),
  fNumBins(10) {
  fDelta    = (fMax - fMin) / fNumBins;
  fInvDelta = 1./fDelta;
  Initialize();
}


void Hist::Initialize() {
  fx.resize(fNumBins, 0.0);
  fy.resize(fNumBins, 0.0);
  for (int i = 0; i < fNumBins; ++i) {
    fx[i] = fMin + i * fDelta;
  }
  fSum = 0.0;
}


void Hist::ReSet(const std::string& filename, double min, double max, int numbins) {
  fFileName = filename;
  fMin      = min;
  fMax      = max;
  fNumBins  = numbins;
  fDelta    = (fMax - fMin) / fNumBins;
  fInvDelta = 1./fDelta;
  Initialize();
}


void Hist::Fill(double x) {
  if (x<fMin) return;
  int indx = (x==fMax) ? fNumBins-1 : (int)((x - fMin) * fInvDelta);
/*
  if (indx < 0) {
    std::cerr << "\n ***** ERROR in Hist::FILL  =>  x = "
           << x << " < fMin = " << fMin
           << std::endl;
    exit(1);
  }
*/
  if (indx>-1 && indx<fNumBins) {
    fy[indx] += 1.0;
    fSum     += 1.0;
  }
}


void Hist::Fill(double x, double w) {
  if (x<fMin) return;
  int indx = (x==fMax) ? fNumBins-1 : (int)((x - fMin) * fInvDelta);
/*
  if (indx < 0) {
    std::cerr << "\n ***** ERROR in Hist::FILL  =>  x = "
           << x << " < fMin = " << fMin
           << std::endl;
    exit(1);
  }
*/
  if (indx>-1 && indx<fNumBins) {
    fy[indx] += w; //1.0 * w;
    fSum     += w;
  }
}


void Hist::Scale(double sc) {
  for (int i = 0; i < fNumBins; ++i) {
    fy[i] *= sc;
  }
}


void Hist::WriteToFile(bool isNorm) {
  FILE* f = fopen(fFileName.c_str(), "w");
  if (!f) {
    std::cerr << "\n ***** ERROR in Hist::WriteToFile  "
           << " cannot create the file = " << fFileName
           << std::endl;
    exit(1);
  }
  double norm = 1.0;
  if (isNorm) {
    norm  = 1. / (fSum*fDelta);
  }
  for (int i = 0; i < fNumBins; ++i) {
    fprintf(f, "%d\t%.8g\t%.8g\n", i, fx[i] + 0.5 * fDelta, fy[i] * norm);
  }
  fclose(f);
}


void Hist::WriteToFile(double norm) {
  FILE* f = fopen(fFileName.c_str(), "w");
  if (!f) {
    std::cerr << "\n ***** ERROR in Hist::WriteToFile  "
           << " cannot create the file = " << fFileName
           << std::endl;
    exit(1);
  }
  for (int i = 0; i < fNumBins; ++i) {
    fprintf(f, "%d\t%.8g\t%.8g\n", i, fx[i] + 0.5 * fDelta, fy[i] * norm);
  }
  fclose(f);
}

void Hist::Add(const Hist* hist) {
  if (fNumBins != hist->GetNumBins() ||
      fMin     != hist->GetMin() ||
      fMax     != hist->GetMax()) {
    std::cerr << "\n ***** ERROR in Hist::Add  "
           << " histograms have different dimensions ! "
           << std::endl;
  }
  for (int i = 0; i < fNumBins; ++i) {
    fy[i] += hist->GetY()[i];
  }
  fSum += hist->GetSum();
}
