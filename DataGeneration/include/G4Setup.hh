
#ifndef G4Setup_HH
#define G4Setup_HH

/**
 * @file    G4Setup.hh
 * @class   G4Setup
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief Provides a pre-initialised `Geant4` detector ready for the physics initialisation.
 *
 * This builds a fake Geant4 geometry having the given list of NIST materials
 * in the geometry with the given production threshold. The corresponding
 * material-cuts couples will be built that are needed for a Geant4
 * initialisation state from which the physics initialisation can be started.
 * This will be the `G4HepEm` data initialisation in our case in the corresponding
 * auxiliary `HepEmShow-DataGeneration` application.
 */

#include <vector>
#include <string>

class G4Material;


/**
  * The method that builds and pre-initialises a `Geant4` detector with the given
  * list of materials and secondary production threshold.
  *
  * The materials, that the detector will contain, are determined by the material
  * names given in the input argument. The secondary production threshold (in
  * [mm] units) can also be given.
  *
  * @param[in] g4NISTMatNames A vector of `Geant4` pre-defined NIST material names specifying the required materials.
  * @param[in] prodcut        The required secondary production threshold or cut value (in length [mm])
  * @param[in] verbose        Possibility of printing some infomation when the verbosity level is higher than zero.
  */
void FakeG4Setup(const std::vector<std::string>& g4NISTMatNames, double prodcut, int verbose=0);

#endif // G4Setup_HH
