
#include "Geometry.hh"

#include "Box.hh"

#include <iostream>

Geometry::Geometry() {
  // default values: 50 layers of 2.3 [mm] absorber (PbWO4) and 5.7 [mm] gap (lAr)
  fNumLayers  =  50;
  fAbsThick   = 2.3; // defult value [mm]
  fGapThick   = 5.7; // defult value [mm]
  fCaloSizeYZ = 400; // defult value [mm]

  // these will be computed automatically in the `UpdateParameters`
  fCaloStartX       = 0.0;
  fPrimaryXPosition = 0.0;

  // crate shapes here for all objects:
  // - their proper size is set when calling `UpdateParameters` below
  // - material index is set to 0, 1 or 2 that corresponds to (using the default
  //   material list) {0 - G4_Galactic; 1 - G4_PbWO4, 2 - G4_lAr}
  fBoxWorld  = new Box("World", 0, 1,1,1); // material index = 0 G4_Galactic
  fBoxCalo   = new Box("Calo" , 0, 1,1,1); // material index = 0 G4_Galactic
  fBoxLayer  = new Box("Layer", 0, 1,1,1); // material index = 0 G4_Galactic
  fBoxAbs    = new Box("Abs"  , 1, 1,1,1); // material index = 1 G4_PbWO4
  fBoxGap    = new Box("Gap"  , 2, 1,1,1); // material index = 2 G4_lAr

  UpdateParameters();
}


Geometry::~Geometry() {
  delete fBoxWorld;
  delete fBoxCalo;
  delete fBoxLayer;
  delete fBoxGap;
  delete fBoxAbs;
}


void Geometry::UpdateParameters() {
  // calculate the layer and calorimeter thicknesses based on the `absorber`,
  // `gap` thinkesses and the number of layers
  fLayerThick = fAbsThick + fGapThick;
  fCaloThick  = fNumLayers*fLayerThick;

  // set/calculate the left hand side x point where the calorimeter starts
  fCaloStartX = -0.5*fCaloThick;
  // set/calculate a world size such that everything fits inside
  const double worldThick = 1.1*fCaloThick;
  // set/calculate the mid-point between the world and calorimeter on the left
  fPrimaryXPosition = -0.25*(worldThick + fCaloThick);

  // half size of all (but the world) along the YZ plane
  const double halfCaloYZ = 0.5*fCaloSizeYZ;

  fBoxWorld->SetHalfLength(0.5*worldThick, 0);
  fBoxWorld->SetHalfLength(1.1*halfCaloYZ, 1);
  fBoxWorld->SetHalfLength(1.1*halfCaloYZ, 2);

  int iMatCalo = 0;
  fBoxCalo->SetMaterialIndx(iMatCalo);
  fBoxCalo->SetHalfLength(0.5*fCaloThick, 0);
  fBoxCalo->SetHalfLength(halfCaloYZ, 1);
  fBoxCalo->SetHalfLength(halfCaloYZ, 2);


  fBoxLayer->SetHalfLength(0.5*fLayerThick, 0);
  fBoxLayer->SetHalfLength(halfCaloYZ, 1);
  fBoxLayer->SetHalfLength(halfCaloYZ, 2);

  fBoxAbs->SetHalfLength(0.5*fAbsThick, 0);
  fBoxAbs->SetHalfLength(halfCaloYZ, 1);
  fBoxAbs->SetHalfLength(halfCaloYZ, 2);

  fBoxGap->SetHalfLength(0.5*fGapThick, 0);
  fBoxGap->SetHalfLength(halfCaloYZ, 1);
  fBoxGap->SetHalfLength(halfCaloYZ, 2);
}


// note: try to keep this more verbose than fast to keep it clear
double Geometry::CalculateDistanceToOut(double* r, double *v, Box** currentVolume, int* indxLayer, int* indxAbs) {
  // init everything to a step in the `world` case
  *currentVolume = fBoxWorld;
  *indxLayer     = -1;
  *indxAbs       = -1;

  // calculate position in the `calorimeter` system:
  // - only x-coordinate is need as everything is centered along the yz
  // - actually its the same as the global: the calorimeter is not translated nor rotated
  const double rx_Calo = r[0];
  const double dToCalo = fBoxCalo->DistanceToOut(r, v);
  // check if about leaving the calorimeter volume: distance to out is zero
  if (dToCalo == 0.0) {
    // currentVolume is already set to `world`
    return 1.0E+20;
  }

  // calculate the position in the `layer` system:
  // - first calculate the index of the `layer` in which the point is located
  const int iLayer = int( (rx_Calo+0.5*fCaloThick)/fLayerThick );
  *indxLayer = iLayer;
  // - then the corresponding translation vector and transform the point
  const double trLayeri = -0.5*fCaloThick + (iLayer+0.5)*fLayerThick;
  const double rx_Layer = rx_Calo - trLayeri;
  r[0] =  rx_Layer;

  // calculate the distance to the `layer` boundary along the given direction
  // why: tolerance and direction was not considered! So to detect here that
  //      the point is actually miss-located (distance is zero in that case.)
  if (fBoxLayer->DistanceToOut(r, v) == 0.0) {
    return 0.0;
    // NOTE: I could also push here and do recursion but keep it clear and push only in the steppers
  }

  // calculate if the point is in the `absorber` or the `gap` part of the `layer`
  if (rx_Layer + 0.5*fLayerThick < fAbsThick || fGapThick == 0) { // in the `absorber`
    // calculate the position in the `absorber` system:
    // - the translation vector and transform the point
    const double trAbs = -0.5*(fLayerThick - fAbsThick);
    r[0] = rx_Layer - trAbs;
    // set what is left and calculate the distance to the `absorber` boundary along
    // the given direction (again, I could push here and do recursion whenever it's zero)
    *currentVolume = fBoxAbs;
    *indxAbs       = 0;
    return fBoxAbs->DistanceToOut(r, v);
  } else { // in the `gap`
    // calculate the position in the `gap` system:
    // - the translation vector and transform the point
    const double  trGap = -0.5*(fLayerThick -fGapThick) + fAbsThick;
    r[0] = rx_Layer - trGap;
    // set what is left and calculate the distance to the `gap` boundary along
    // the given direction (again, I could push here and do recursion whenever it's zero)
    *currentVolume = fBoxGap;
    *indxAbs       = 1;
    return fBoxGap->DistanceToOut(r, v);
  }
}
