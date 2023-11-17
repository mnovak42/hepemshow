
#ifndef GEOMETRY_HH
#define GEOMETRY_HH

/**
 * @file    Geometry.hh
 * @class   Geometry
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief Geometry description for this simple simulation setup.
 *
 * The simulation setup is a **configurable simplified sampling calorimeter** built
 * up from `N` layers of an `absorber` and a `gap` (both by default). The number of
 * layers `N`, the thickness of both the `absorber` and `gap` along the x-axes
 * can be set and changed dynamically.
 *
 * - `layer`:
 *   - number   :   `fNumLayers`
 *   - set/get  :   `SetNumLayers(int)`/`GetNumLayers()`
 *   - thickness:   `fLayerThick` (calculated automatically from the `absorber`
 *                   and `gap` thicknesses)
 *
 * - `absorber`:
 *   - thickness:   `fAbsThick`
 *   - set/get  :   `SetAbsThick(double)`/`GetAbsThick()`
 *   - material :   lead tungstate/atolzite (`"G4_PbWO4"`) with material `index = 1` (by default)
 *
 * - `gap`:
 *   - thickness:   `fGapThick`
 *   - set/get  :   `SetGapThick(double)`/`GetGapThick()`
 *   - material :   liquid argon (`"G4_lAr"`) with material `index = 2` (by default)
 *
 * @note The default length unit is [mm] so all thicknesses and sizes are
 * assumed to be give in [mm] units.
 *
 * @note The `gap` thickness can be set even to zero in which case the
 * `calorimeter` is built up from the given number of `layer`s of `absorber`
 * with the given thickness (i.e. a single material calorimeter sliced by the
 * `layer`s).
 *
 * @note The material indices are determined by the order of the corresponding
 * `Geant4` (predefined NIST) material names listed in the material name vector of
 * the data extraction application (i.e. in `DataGeneration.cc`). This application
 * is used beforehand to extract the material (and cuts) dependent data required
 * during the simulation. The default vector in `DataGeneration.cc`, that was used
 * to extract the provided data files, is
 *
 *     // list of Geant4 (NIST) material names
 *     std::vector<std::string> matList {"G4_Galactic", "G4_PbWO4", "G4_lAr"};
 *
 * hence the above `material - index` mapping.
 * Changing the material name(s) in this above vector (especially at index `1`
 * and/or `2` as the vacuum is always needed to fill the container volumes like
 * the `layer`, `calorimeter` or the `world` volumes),
 * regenerating the data, then executing the same `Simulation` application,
 * corresponds to changing the mateiral (with the given index) in the simulation.
 * A [list of the available predefined Geant4 NIST material names](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html)
 * can be found in the corresponding part of the Geant4 Documentation.
 *
 * All thickness measured along the `x` axes while the `yz` extent is the same
 * both for the `absorber` and `gap` determined by the `fCaloSizeYZ` which can
 * be set dynamically by `SetCaloSizeYZ(val)/GetCaloSizeYZ()`.
 *
 * A single layer is composed from the above `absorber` and `gap` while the entire
 * `calorimeter` is built up form the given number of identical `layer`s shifted
 * along the `x` axes. The `calorimeter` center, i.e. the [0,0,0] position of the
 * corresponding Box shape local coordinate, is at the global origin (i.e. no
 * translation nor rotation is applied). The entire calorimeter is placed inside
 * the `world` that is the limit of our simulation universe. The `layer`,
 * `calorimeter` and `world` is filled with vacuum (very low density hydrogen),
 * so only the `absorber` and the `gap` have non-vacuum like materials.
 *
 * The shape of all objects (`absorber`, `gap`, `layer`, `calorimeter`, `world`)
 * is `Box`. A box object is constructed for each in the constructor by setting
 * the appropriate name and material index fields. Their proper sizes are
 * calculated and updated automatically whenever one of the above setters,
 * affecting any of the thicknesses or sizes, is invoked.
 *
 * The geometry can also provide an apropriate initial `x` position for the
 * primary particles locating in between the `world` and the `calorimeter` on
 * the left hand side (`GetPrimaryXposition()`). The `x` position, where the
 * `calorimeter` starts on the left hand side, can also be obtained
 * (`GetCaloStartXposition()`). These method will always give an appropriate
 * value as the corresponding data are also updated dynamically whenever any of
 * the thicknesses or sizes are modified.
 *
 *
 * The geometry also provides a very simple "navigation" through its
 * `CalculateDistanceToOut(double*, double*, Box**, int*, int*)` method that determines:
 * - the (deepest) volume/box in which the given global `position` is located
 * - the index of the layer (only if the point is located inside the `calorimeter`)
 * - and the index of the absorber: 0 for the `absorber` and 1 for the `gap`
 *   (but only in case the point is inside the calorimeter)
 *
 * At the end, it returns:
 * - a large (1E+20 [mm]) value whenever the `position` and `direction` is such
 *   that the particle is about leaving the `calorimeter` (i.e. going to vacuum
 *   then would hit the boundary of the `world` at the end of that step)
 * - otherwise: the distance to the boundary of the volume in which the given
 *   `position` was located: from the given `position` along the given `direction`.
 *
 * Note, that this distance is computed by using the corresponding box/volume
 * method (namely `Box::DistanceToOut(double*,double*) const`) and the `box` object
 * in which the given `position` was located:
 * - distance to out is 0 if a given `position` is outside of that volume/box
 * - volume boundaries, closer to `position` than half of the
 *   `Box::kCarTolerance` (= 1E-9 [mm]), are ignored when the direction is pointing
 *   out, i.e. these positions are considered to be outside, leading to 0 distance
 *   to the volume boundary from inside. (see more on `inside`, `surface` and
 *   `outside` at the `Box` documentation)
 *
 * A small (e.g. 1E-6 [mm]) push along the current direction might be
 * applied in the steppers when the distance to out was found to be 0.
 *
 * This is because it's assumed, that the 0 distance is due to the employed simple
 * location calculation, that ignors the tolerance, instead of using an apropriate
 * navigator. Namely, the point was located in volume A. but it's actually on its
 * `surface`: closer than `Box::kCarTolerance`/2 to its boundary. Morover, the direction
 * is pointing toward to the next, volume B., that is just on the other side of
 * this boundary. This correctly gives 0 distance to the boundary of volume A as
 * actually the given step will be done in volume B. Therefore, the small push is
 * to overcome the volume boundary, i.e. to push the point to be on the other side
 * of the boundary between volume A and B. Then, the point is calulated to be in
 * volume B now when relocating and as the direction is pointing inside volume B,
 * the expected distance to the next boundary of volume B is computed.
 */

// forward
class Box;

class Geometry {

public:

  /** Constructor: sets the default configuration, creates the Box objects for all components.*/
  Geometry();
  /** Destructor: deletes the Box objects that represents the volume of the components.*/
 ~Geometry();

  /** Sets the number of layers the entire calorimeter should be built up.
    * @param[in]  nlayers Number of layers (must be > 0) requested (all parameters are recalculated).
    */
  void   SetNumLayers (int nlayers) {
    if (nlayers > 0) {
      fNumLayers = nlayers;
      UpdateParameters();
    }
  }

  /** Gives the number of layers the calorimeter is built up.
    * @return number of layers.
    */
  int    GetNumLayers ( ) const {
    return fNumLayers;
  }


  /** Gives the thickness of the `calorimeter` (i.e. full size along the x-axis).
    * @return thickness of the `calorimeter` in [mm] units.
    */
  double GetCaloThick ( ) const {
    return fCaloThick;
  }


  /** Sets the required absorber thickness (i.e. full size along the x-axis).
    * @param[in]  thickness Required thickness of the `absorber` in [mm].
    */
  void   SetAbsThick (double thickness) {
    fAbsThick = thickness;
    UpdateParameters();
  }

  /** Gives the thickness of the `absorber` (i.e. full size along the x-axis).
    * @return thickness of the `absorber` in [mm] units.
    */
  double GetAbsThick ( ) const {
    return fAbsThick;
  }

  /** Sets the required `gap` thickness (i.e. full size along the x-axis).
    *
    * Note, that the `gap` thickness can also be set to zero. The calorimeter is
    * built up from a single material layers, i.e. a block of material sliced along
    * the x-axis.
    *
    * @param[in]  thickness Required thickness of the `gap` in [mm] (can be set to 0).
    */
  void   SetGapThick (double thickness) {
    fGapThick = thickness;
    UpdateParameters();
  }

  /** Gives the thickness of the `gap` (i.e. full size along the x-axis).
    * @return thickness of the `gap` in [mm] units.
    */
  double GetGapThick ( ) const {
    return fGapThick;
  }


  /** Sets the transverse size (i.e. full size along the yz-axes).
    *
    * Note, this also determines `yz` sizes of the corresponding the `absorber`,
    * `gap` and `layer` volumes/shapes.
    *
    * @param[in]  val Required full transvers size of the `calorimeter` in [mm].
    */
  void   SetCaloSizeYZ (double val) {
    fCaloSizeYZ = val;
    UpdateParameters();
  }

  /** Gives the transverse size of the  `calorimeter` (i.e. full size along the yz-axis).
    * @return full transverse size of the `calorimeter` volume/shape in [mm] units.
    */
  double GetCaloSizeYZ ( ) const {
    return fCaloSizeYZ;
  }

  /** Provides the x-coordinate of the mid-position between the `world` and `calorimeter`
    * boundaries on the left hand side.
    *
    * Note that this is only for the primary generator, the primary tracks should be inside
    * the calorimeter or its boundary but pointing inside (see more at `CalculateDistanceToOut()`)
    *
    * @return the x-coordinate of the mid-point between the `world` and `calorimeter` boundaries on the left.
    */
  double GetPrimaryXposition()   const { return fPrimaryXPosition; }

  /** Provides the x-coordinate on the `calorimeter` boundary on the left hand side.
    *
    * Note this is the initial x-coordinate of each primary track while their direction should
    * point toward the calorimeter (i.e. having positive x-coordiante).
    *
    * @return the x-coordinate of the `calorimeter` boundary on the left hand side.
    */
  double GetCaloStartXposition() const { return fCaloStartX; }


  /**
    * Locates a point in the geometry and calculates the distance till the next boundary.
    *
    * This method is supposed to be called at the pre-step point of the simulation step
    * with the global pre-step ponit coordinates and actual direction in order to:
    * - determine the volume in which this simulation step will be done (and more importantly,
    *   the material as everything depends on that (at least))
    * - the distance to the boundary of that volume along the given diretion (as the material
    *   might change on the other side of that boundary)
    *
    * The pre-step point is supposed to be inside the `calorimeter` volume, i.e. either
    * - `inside`: deeper than `Box::kCarTolerance`/2 form any of its boundaries
    * -  on `surface`: closer to a boundary than `Box::kCarTolerance`/2
    * While the distance to the `calorimeter` volume boundary is > zero in the first case,
    * this depends on the actual direction in the second case:
    * - zero: when the direction is pointing outside of that boundary, i.e. particle is about
    *         leaving the volume
    * - positive: as the particle is about moving in the volume otherwise
    * These are true for all (and not only for the `calorimeter`) volumes!
    *
    * During the simulation, each primary track starts from the `calorimeter` volume
    * boundary with a direction that is pointing inside, i.e. ensured to be inside (on
    * `surface` but pointing in). All tracks are terminated when the particle is about leaving
    * the `calorimeter`, i.e. the particle is on `surface` and pointing out (as the step would
    * be done in the vaccum ending on the boundary of our `world`). Therefore, all step pints,
    * and secondary tracks created at some of these points, are also ensured to be inside the
    * `calorimeter` (as defined above).
    *
    * In order to achive the above, this method returns with a large (1E+20 [mm]) distance to
    * boundary whenever the particle is about leaving the `calorimeter`. The point (the step)
    * is located to be in the `world` volume (`layer` and `absorber` indices are set to -1).
    * Otherwise, the point is located, i.e. the deepest volume inside the `calorimeter` in which
    * the point is located, is determined, the `layer` and `absorber` indices are set.
    *
    * However, this is done based on a simply computation of the `layer` index (based on its
    * thickness) then the same within the layer. In other words, this is done without considering
    * the tolerance or the direction (unlike in Geant4, having a robust but complex navigator for this).
    * Therefore, it might be the case that the point is calculated to be inside a given volume but
    * actually it's on the `surface` while moving out. The corresponding simulation step should actually
    * be performed in the next volume (that is just on the other side of that boundary). This is detected
    * during the simulation step computation, as this method returns zero distance in this case, and:
    * - a small push of 1E-6 [mm] is applied along the current direction (just to push the point to
    *   the other side of the boundary)
    * - this method is called again with the new position: calculated to be in the good volume now
    *
    * The input position, given in global coordinates, always transformed to the local system of the given
    * volume. This is an identity transformation for the `calorimeter` (as not translated nor rotated).
    * Then for the `layer`, the translation vector (having non-zero only its x-component) is determined
    * based on the first layer x-position (where the `calorimeter` starts), the thickness of the
    * `layer` and the current x-position of the point. After transforming the point to `layer` local
    * coordinates, the position is transformed further either to `absorber` or `gap` local coorinates
    * depending on which the point was calculated to be in.
    * At the end, the input position vector contains the position of the point in the local system of the
    * volume/shape that the pint was calculated to be located. Therefore, this local coorinates can be
    * used later directly in any shape (Box) methods, e.g. for computing the safety.
    *
    * @param[in,out] r pointer to a 3D array that stores the x, y and z coorinates of the position in global coordiantes at input.
    *                  These will be updated to be local coordinates in the system of the volume (Box) in which the point was located in.
    * @param[in]     v pointer to a 3D array that stores the x, y and z coorinates of the current normalised direction vector
    * @param[in,out] currentVolume address of a pointer to a Box object that can be anything at input, while at output the corresponding
    *                  pointer is set to the `Box` object that represents the volume in which the given point was calculated to be located
    * @param[in,out] indxLayer pointer to an integer that can be anything at input, while at output it will be the indx of tha `layer`
    *                  in which the given point was calculated to be located (-1 when the particle is leaving the `calorimeter` or no layers)
    * @param[in,out] indxAbs pointer to an integer that can be anything at input, while at output it will be 0 or 1 that corresponds to the
    *                  `absorber` and `gap` depending on which the given point was calculated to be located (-1 when the particle is leaving
    *                  the `calorimeter` or no layers)
    * @return the distance, from the given position along the given direction, to the boundary of the volume in which the given point was
    *         calculated to be located. It might be zero (the step actually shouldn't be done in the located volume) or 1E+20 [mm] (the particle
    *         about leaving the `calorimeter`).
    */
  double CalculateDistanceToOut(double* r, double *v, Box** currentVolume, int* indxLayer, int* indxAbs);




private:

  /** Privite method that clculates the apropriate positions and volume/shape sizes whever any related parameters is updated.*/
  void   UpdateParameters();


// data members
private:

  /** Number of layers the calorimeter is built up.
    * Can be set (even to zero: calorimeter is just a single volume/box)*/
  int    fNumLayers;

  /** `Absorber` thickness measured along the `x`-axis in [mm] (can be set)*/
  double fAbsThick;

  /** `Gap` thickness measured along the `x`-axis in [mm] (can be set; even to
    *  zero: single material `calorimeter` sliced by layers along the `x`-axis)*/
  double fGapThick;

  /** `Layer` thickness measured along the `x` axes in [mm].
    *  Computed automatically (whenever the `absorber` or `gap` thickness is
    *  updated*/
  double fLayerThick;

  /** The thickness of the entire `calorimeter` measured along the `x` axes in [mm]
    * Computed automatically whenever the `layer` thickness (i.e. absorber and/or
    * gap thickness) or number of layer is updated. */
  double fCaloThick;

  /** The transverse size (i.e. full size along the `yz` axes) of the `calorimeter`
    * in [mm] units (same for `asborber`, `gap` and `layer` volumes/shapes)*/
  double fCaloSizeYZ;


  /** The `x`-coordinate of the `calorimeter` boundary on the left hand size.
    * Calculated automatically (whenever the related parameters are updated) */
  double fCaloStartX;

  /** The `x`-coordinate of the mid-point between the `calorimeter` and `world`
    * boundaries on the left hand size.
    * Calculated automatically (whenever the related parameters are updated) */
  double fPrimaryXPosition;


  // pointers to box shape objects representing each elements of the geometry
  /** Pointer to the `Box` shape representing the `world` volume.*/
  Box*   fBoxWorld;

  /** Pointer to the `Box` shape representing the `calorimeter` volume.*/
  Box*   fBoxCalo;

  /** Pointer to the `Box` shape representing the `layer` volume.*/
  Box*   fBoxLayer;

  /** Pointer to the `Box` shape representing the `absorber` volume.*/

  Box*   fBoxAbs;

  /** Pointer to the `Box` shape representing the `gap` volume.*/
  Box*   fBoxGap;
};

#endif // GEOMETRY_HH
