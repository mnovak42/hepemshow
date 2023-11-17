
#ifndef BOX_HH
#define BOX_HH

/**
 * @file    Box.hh
 * @class   Box
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief A simplified version of the G4Box shape.
 *
 * This is a simple version of the G4Box shape to describe geometry objects and
 * use them in the simulation to calculate distance to their boundary from a
 * point inside. Note, that the calculations include a tolerance, e.g. a point
 * is on the surface if closer to a boundary than 1/2 tolerance (kCarTolerance).
 * The two most important methods, used during this simplified simulation, are:
 *
 * - DistanceToOut(position, direction): distance to boundary from a local
 *     position (inside the box) along the given direction. The boundary is
 *     ignored if the position is closer to it than 1/2 tolerance (i.e. point is
 *     on the surface). The distance to boundary is zero in this case whenever
 *     the direction is pointing outisde (i.e. the particle is moving away/out).
 *
 * - DistanceToOut(postition): this is the `safety`, i.e. the distance to the
 *     nearest boundary from the given local point inside (zero if on the surface
 *     or outside).
 *
 * This version of the Box stores an index to the material that fills the shape
 * (therefore closer to the Geant4 `logical` volume concept than to a shape).
 *
 * Box shapes are constructed for each geometry object in the `Geometry` and
 * the above methods are utilised during the simulation step computation in the
 * `GammaStepper` and `ElectronStepper` (in some case indirectly by calling
 * `Geometry::DistanceToOut` that first locates the point, i.e. finds the Box
 * object which the given global point is located in).
 *
 * NOTE: a point given in local coordiantes can locate
 *  - `inside`  : if deeper inside than `kCarTolerance/2` from any boundary
 *  - `surface` : if within `kCarTolerance/2` from any boundary (in- or outside)
 *  - `outside` : if further away than `kCarTolerance/2` from any boundary outside
 *
 * NOTE: distance to volume boundary from a point along a given direction is
 *  zero when the point is not `inside` and the direction is pointing away.
 *  Therefore, a point located on the surface gives distance to boundary:
 *  - zero    : if the direction is pointing outside of that boundary
 *  - non-zero: if the direction is pointing inside of that boundary
 */

/* Constants used in the Inside() method (not utilised by the simulation)*/
//enum  EInside { kOutside, kSurface, kInside};


#include <string>

class Box {

public:
  /**
   * Constructor.
   *
   * @param[in] name    Name of this volume.
   * @param[in] indxMat Index of the material this volume is filled with.
   * @param[in] pX      Half length of the box along the x-axis.
   * @param[in] pY      Half length of the box along the y-axis.
   * @param[in] pZ      Half length of the box along the z-axis.
   */
  Box (const std::string& name, int indxMat, double pX, double pY, double pZ);

  /** Destructor (nothig to do)*/
 ~Box () {}


  /** Get the name of this volume. */
  const std::string& GetName() const { return fName; }


  /** Set the material this volume is filled with.
    * @param[in]  indx Index of the material.
    */
  void SetMaterialIndx(int indx) { fMaterialIndx = indx; }

  /** Get the material this volume is filled with.
    * @return Index of the material.
    */
  int  GetMaterialIndx() const   { return fMaterialIndx; }


  /** Set the half length of the box along the given axis.
    * @param[in] val Half length in [mm] units.
    * @param[in] idx Encodes the axis along the half length is given
    *                (idx=0 --> x; idx=1 --> y; idx=2 --> z).
    */
  void SetHalfLength(double val, int idx);

  /** Get the half length of the box along the given axis.
    * @param[in] idx Encodes the axis along the half length is required
    *                (idx=0 --> x; idx=1 --> y; idx=2 --> z).
    * @return Half length of this box along the required axis.
    */
  double GetHalfLength(int idx) const;


  /**
    * Calculates distance to the volume boundary from inside along the given
    * direction.
    *
    * Returns the distance along the normalised direction vector `v` to the
    * volume boundary, from the given point `p` inside or on the surface
    * of the box. Intersections with surfaces, when the point is within half
    * tolerance (`kCarTolerance/2`) from a surface, is ignored.
    *
    * @param[in] r 3D position of the point in local coordinates
    * @param[in] v 3D normalised direction
    * @return Distance to the surface boundary from inside (see above).
    */
  double DistanceToOut(double* r, double *v) const;

  /**
    * Calculates the distance to the nearest boundary of a shape from inside (safety).
    *
    * While the above considers the direction, this finds the nearest boundary.
    *
    * @param[in] r 3D position of the point in local coordinates
    * @return Distance to the nearest surface boundary from inside (see above).
    */
  double DistanceToOut(double* r) const;


  // Return whether the given `position` (in local coordinates) is
  // inside(0)/outside(1)/on surface(2), taking into account tolerance.
//  EInside Inside(double rx, double ry, double rz) const;
//  EInside Inside(double* r) const;


private:

  /** Name of this volume. */
  const std::string fName;
  /** Index of the material this volume is filled with.*/
  int    fMaterialIndx;
  /** Half length of the box along the x-axis. */
  double fDx;
  /** Half length of the box along the y-axis. */
  double fDy;
  /** Half length of the box along the z-axis. */
  double fDz;

  /** Value of the tolerance in [mm]. */
  const double kCarTolerance = 1.0E-9;
  /** Half of the above tolerance. */
  double fDelta;

};

#endif // BOX_HH
