
#ifndef PHYSICS_HH
#define PHYSICS_HH

/**
 * @file    Physics.hh
 * @class   Physics
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief The entire physics of the simulation is provided by `G4HepEm` @cite g4hepem and
 *        pulled-in to the `HepEmShow` application by the `Physics.hh` and
 *        `Physics.cc` files.
 *
 * The `Physics.hh` header file includes the `G4HepEmRun` headers that give the
 * complete set of run-time functioinalities required for the EM physics modelling.
 * The corresponding implementations are pulled-in all together in the`Physics.cc`
 * implentation file.
 *
 * The only ingredient of `G4HepEmRun`, that a client application (such as
 * `HepEmShow`), needs to provide is an implementation of a uniform random
 * number generator. An object from such genertor must be plugged-in to the
 * `G4HepEmRandomEngine` by implementing the two missing `G4HepEmRandomEngine::flat()`
 * and `G4HepEmRandomEngine::flatArray(const int, double *)` methods. This is
 * also done in the `Physics.cc` implementation file that completes the
 * implementation of `G4HepEmRun`.
 *
 * `URandom` is the uniform random number generator implemented in `HepEmShow`
 * based on the 64-bit verson of the Mersenne Twister generator provided by c++11.
 * An obejct of this is utilised in the `Physics.cc` file to complete the
 * implementation of the `G4HepEmRandomEngine` as mentioned above. Then the
 * actual uniform random number generator and the random engine objects are
 * constructed (and set to the `G4HepEmTLData` obejct) in the `HepEmShow.cc`
 * main function of the application.
 *
 *
 * `G4HepEm` implements two top level methods, `HowFar` and `Perform`, in
 * its `G4HepEmGammaManager` and `G4HepEmElectronManager`:
 * - to provide the information on `HowFar` a given input \f$\gamma\f$ or \f$e^-/e^+\f$
 *   track goes according to their physics related constraints (e.g. till their next
 *   physics interaction takes place or other physics related constraints).
 * - to `Perform` all necessary physics related updates on the given input
 *   \f$\gamma\f$ or \f$e^-/e^+\f$ track, including the production of secondary
 *   tracks in the given physics interaction (if any).
 *
 * The first (`HowFar`) is invoked at the pre-step point while the second (`Perform`)
 * is at the post-step point of each individual simulation step computation inside
 * the `SteppingLoop::GammaStepper()` and `SteppingLoop::ElectronStepper()`.
 *
 */

// g4hepem run-time headers: provides all EM physics related functionality for the simualtion
#include "G4HepEmElectronEnergyLossFluctuation.hh"
#include "G4HepEmElectronInteractionUMSC.hh"
#include "G4HepEmGammaInteractionConversion.hh"
#include "G4HepEmInteractionUtils.hh"
#include "G4HepEmElectronInteractionBrem.hh"
#include "G4HepEmElectronManager.hh"
#include "G4HepEmGammaInteractionPhotoelectric.hh"
#include "G4HepEmPositronInteractionAnnihilation.hh"
#include "G4HepEmElectronInteractionIoni.hh"
#include "G4HepEmGammaInteractionCompton.hh"
#include "G4HepEmGammaManager.hh"
#include "G4HepEmRunUtils.hh"

#endif // PHYSICS_HH
