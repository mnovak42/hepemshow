
#include "Box.hh"


#include <iostream>
#include <sstream>
#include <cmath>


Box::Box (const std::string& name, int indxMat, double pX, double pY, double pZ)
: fName(name),
  fMaterialIndx(indxMat),
  fDx(pX),
  fDy(pY),
  fDz(pZ) {
  fDelta = 0.5*kCarTolerance;
  // check minimum size
//  if (pX < 2*kCarTolerance ||
//      pY < 2*kCarTolerance ||
//      pZ < 2*kCarTolerance)  {
//    std::ostringstream message;
//    message << "Dimensions too small for Solid: " << GetName() << "!" << std::endl
//            << "     hX, hY, hZ = " << pX << ", " << pY << ", " << pZ;
//    std::cout << message.str();
//  }
}


void Box::SetHalfLength(double val, int idx) {
   // limit to thickness of surfaces
  if (val > 2*kCarTolerance) {
    switch (idx) {
      case 0: fDx = val;
            break;
      case 1: fDy = val;
              break;
      case 2: fDz = val;
              break;
    };
  } else {
//    std::ostringstream message;
//    message << "Dimension too small for solid: " << GetName() << "!"
//            << std::endl
//            << "      val = " << val << std::endl
//            << "      idx = " << idx;
//    std::cout << message.str();
  }
}

double Box::GetHalfLength(int idx) const {
  switch (idx) {
    case 0: return fDx;
    case 1: return fDy;
    case 2: return fDz;
  };
  return 0;
}


// p should be in local coordinates
// returns zero if p is outside of the box or within tolerance
double Box::DistanceToOut(double* p, double *v) const {
  // Check if point is not inside and traveling away: zero
  // Note: eitehr in surafece or outside
  if ((std::abs(p[0]) - fDx) >= -fDelta && p[0]*v[0] > 0) {
    return 0.0;
  }
  if ((std::abs(p[1]) - fDy) >= -fDelta && p[1]*v[1] > 0) {
    return 0.0;
  }
  if ((std::abs(p[2]) - fDz) >= -fDelta && p[2]*v[2] > 0) {
    return 0.0;
  }
  // Find intersection
  //
  const double vx = v[0];
  const double tx = (vx == 0) ? 1.0E+20 : (std::copysign(fDx,vx) - p[0])/vx;
  //
  const double vy = v[1];
  const double ty = (vy == 0) ? tx : (std::copysign(fDy,vy) - p[1])/vy;
  const double txy = std::min(tx,ty);
  //
  const double vz = v[2];
  const double tz = (vz == 0) ? txy : (std::copysign(fDz,vz) - p[2])/vz;
  const double tmax = std::min(txy,tz);
  //
  return tmax;
}


double Box::DistanceToOut(double* p) const {
  double dist = std::min( std::min(
                   fDx-std::abs(p[0]),
                   fDy-std::abs(p[1]) ),
                   fDz-std::abs(p[2]) );
  return (dist > 0) ? dist : 0.0;
}


/*
EInside Box::Inside(double rx, double ry, double rz) const {
  double dist = std::max ( std::max (
                  std::abs(rx)-fDx,
                  std::abs(ry)-fDy),
                  std::abs(rz)-fDz);
  return (dist > fDelta) ? kOutside : ((dist > -fDelta) ? kSurface : kInside);
}
EInside Box::Inside(double* r) const {
  double dist = std::max ( std::max (
                  std::abs(r[0])-fDx,
                  std::abs(r[1])-fDy),
                  std::abs(r[2])-fDz);
  return (dist > fDelta) ? kOutside : ((dist > -fDelta) ? kSurface : kInside);
}
*/
