
#ifndef PRIMARYGENERATOR_HH
#define PRIMARYGENERATOR_HH


/**
 * @file    PrimaryGenerator.hh
 * @class   PrimaryGenerator
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief Generates primary particles for an event.
 *
 * This is a simple primary particle generator. The kinetic energy, position,
 * direction and the particle type (through its charge) can be configured.
 * Note, that we simulate only \f$e^-/e^+\f$ and \f$\gamma\f$ particles
 * with -1, +1 and 0 charge respectively.
 *
 * The `GenerateOne()` method is invoked at the beginning of each event. This
 * generates one primary particle/track by setting the properties of the provided
 * `G4HepEmTrack` based on the stored configuration.
 */

// forward
class G4HepEmTrack;

class PrimaryGenerator {

public:

  /** Constructor. */
  PrimaryGenerator();
  /** Destructor (nothing to do). */
 ~PrimaryGenerator() {}

  /**
    * Generates one primary particle into the provided track.
    *
    * The `GenerateOne()` method is invoked at the beginning of each event. This
    * generates one primary particle/track by setting the properties of the provided
    * `G4HepEmTrack` based on the stored configuration.
    *
    * @param[in, out] primTrack a track to fill in the primary particle properties
    */
  void   GenerateOne(G4HepEmTrack& primTrack);


  /** Sets kinetic energy of the primary particle.
    * @param[in] ekin kinetic energy of the primary particle in [MeV] units.*/
  void   SetKinEnergy(double ekin) { fKinEnergy = ekin; }
  /** Privides the kinetic energy of the primary particle.
    * @return kinetic energy in [MeV] units.*/
  double GetKinEnergy()  const     { return fKinEnergy; }

  /** Sets the position of the primary particle.
    * @param[in] pos pointer to a 3D (global) position vector (legth is in [mm])*/
  void   SetPosition(double* pos);

  /** Sets the position of the primary particle.
    * @param[in] x x-coordinate of the positon vector.
    * @param[in] y y-coordinate of the positon vector.
    * @param[in] z z-coordinate of the positon vector. */
  void   SetPosition(double x, double y, double z);

  /** Provides the 3D position vector of the primary particle.
    * @return pointer to a 3D array that stores the x, y and z-coordinates of
    *         the position vector.*/
  const double* GetPosition() const { return fPosition; }


  /** Sets the direction of the primary particle.
    * @param[in] dir pointer to a 3D normalised direction vector*/
  void   SetDirection(double* dir);

  /** Sets the direction of the primary particle.
    * @param[in] x x-coordinate of the normalised direction vector.
    * @param[in] y y-coordinate of the normalised direction vector.
    * @param[in] z z-coordinate of the normalised direction vector. */
  void    SetDirection(double x, double y, double z);

  /** Provides the 3D normalised direction vector of the primary particle.
    * @return pointer to a 3D array that stores the x, y and z-coordinates of
    *         the normalised direction.*/
  const double* GetDirection() const { return fDirection; }


  /** Sets the charge of the primary particle that also determines its type.
    * @param[in] ch the charge in e+ change units:  -1 e-; 0 gamma; +1 e+.*/
  void   SetCharge(double ch) { fCharge = ch; }

  /** Privides the charge of the primary particle.
    * @return charge: -1 e-; 0 gamma; +1 e+.*/
  double GetCharge()  const   { return fCharge; }


private:

  /** Kinetic energy of the primary particle in [MeV] units.*/
  double  fKinEnergy;
  /** Position of the primary particles in (global) coordinates (legth is in [mm]).*/
  double  fPosition[3];
  /** Normalised direction of the primary particles.*/
  double  fDirection[3];
  /** Charge of the primary particle in units of e+ charge: -1 e-; 0 gamma; +1 e+.*/
  double  fCharge;
};

#endif // PRIMARYGENERATOR_HH
