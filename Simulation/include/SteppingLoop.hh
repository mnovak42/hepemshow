
#ifndef STEPPINGLOOP_HH
#define STEPPINGLOOP_HH

/**
 * @file    SteppingLoop.hh
 * @class   SteppingLoop
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief Stepping loops for simulating \f$e^-\f$, \f$e^+\f$ and \f$\gamma\f$
 *        particle histories.
 *
 * The stepping loops can calculate a given \f$\gamma\f$ or \f$e^-/e^+\f$ particle
 * simulation history from their initial state till the end in a step-by-step way
 * (by the `SteppingLoop::GammaStepper(G4HepEmTLData&, G4HepEmState&, TrackStack&,
 * Geometry&, Results&, int)` and `SteppingLoop::ElectronStepper(G4HepEmTLData&,
 * G4HepEmState&, TrackStack&, Geometry&, Results&, int)` respectively). At each
 * step:
 * - the actual step length is calculated (accounting both the geometrical and
 *   the physics related constraints)
 * - the track is moved to its post-step position
 * - all physics related actions, happening along and/or at the post-step point,
 *   are performed on the track
 * - secondary tracks, generated in the given step by a physics interaction (if
 *   any), are insterted into the track stack (by calling the
 *   `SteppingLoop::StackSecondaries(G4HepEmTLData&, TrackStack&, G4HepEmTrack&)`
 *   method)
 * - information (e.g. energy deposit) might be collected at the end of each
 *   simulation step (by calling the `SteppingLoop::SteppingAction(Results&,
 *   const G4HepEmTrack&, const Box*, double, int, int, int, int)` method )
 *
 * **A bit more details**:
 *
 * A simulation history is terminated when:
 * - the particle kinetic energy becomes zero (e.g. an \f$e^-\f$ lost all its
 *   kinetic energy along its last step)
 * - the particle participated in a destructive interaction (e.g. photoelectric
 *   absorption of a \f$\gamma\f$ photon or conversion to \f$e^-/e^+\f$ pairs)
 * - the particle leaves the calorimeter (in a normal `Geant4` simulation the
 *   the history is terminated when the particle leaves the world)
 *
 * The physics related step length constraints as well as the actions (including
 * the secondary track production) are provided by the `G4HepEm` implementation
 * of the EM physics simulation.
 *
 * `G4HepEm` implements two top level methods, in its Gamma and Electron managers:
 * - to provide the information on `HowFar` a given input \f$\gamma\f$ or \f$e^-/e^+\f$
 *   track goes according to their physics related constraints (e.g. till their next
 *   physics interaction takes place or due to other physics related constraints).
 * - to `Perform` all necessary physics related updates on the given input
 *   \f$\gamma\f$ or \f$e^-/e^+\f$ track and produce all secondary tracks in the
 *   given physics interaction (if any).
 *
 * The first (`HowFar`) is invoked at the pre-step point while the second (`Perform`)
 * is at the post-step point of each individual simulation step computation inside
 * the `SteppingLoop::GammaStepper()` and `SteppingLoop::ElectronStepper()`.

 * In `G4HepEm` it's the `G4HepEmTLData` (thread local data) that is used in the
 * top level, two sided communication between the consumer and `G4HepEm`. It
 * encapsulates the (primary and secondary) tracks and the random number generator
 * dedicated for one particular thread. Its **primary** Gamma/Electron track field
 * is used to store the actual state of the \f$\gamma\f$ or \f$e^-/e^+\f$ track
 * that is under tracking. The step limit, imposed by all physics related constraints
 * on the actual track, is calculated at each pre-step point by calling the
 * bove-mentioned `HowFar` top level method provided by `G4HepEm`. Then the `Perform`
 * method needs to be invoked at the post-step point that performs all necessary
 * physics related updates on the input **primary** track while produces all
 * **scondary** tracks related to the given physics interaction (if any). The
 * secondary tracks are delivered back to the caller in the appropriate **seondary**
 * track fields of the `G4HepEmTLData` object.
 *
 * More infomation and details, on how a \f$\gamma\f$ and \f$e^-/e^+\f$ simulation
 * step is computed, might be found by inspecting the implementations of the top
 * level `HowFar` and `Perform` `G4HepEm` methods in the corresponding
 * `G4HepEmGammaManager/G4HepEmElectronManager`.
 */


class G4HepEmTLData;
class G4HepEmState;
class G4HepEmTrack;

class TrackStack;
class Geometry;
class Results;
class Box;

class SteppingLoop {

public:

  /** Stepping loop for simulating the entire history of a \f$\gamma\f$ track.
   *
   * The initial state of the \f$\gamma\f$ track is provided in the `G4HepEmGammaTrack` field of `theTLData` input argument by the caller.
   * The history is simulated then till the end, the state of the \f$\gamma\f$ track is updated while secondary tracks, produced in the
   * physics interactions, are pushed to `theTrackStack` (if any) and the required simulation results are collected/updated into
   * `theResult` structure after each individual simulation step.
   *
   * @param theTLData a `G4HepEm` specific (thread local) object primarily used to obtain all physics related information from `G4HepEm` needed to compute a simulation step
   * @param theState a `G4HepEm` specific object that stores pointers to the top level `G4HepEm` data structure and parameters that are used by `G4HepEm` to provide all physics related infomation needed to compute a simulation step
   * @param theTrackStack the track stack that is used to store the secondary tracks produced while simulating the entire history o fthe input \f$\gamma\f$ track
   * @param theGeometry the geometry of the application in which the input track history is simulated
   * @param theResult the data structure that holds all the infomation needs to be collected during the simulation. It might be updated after each simulation step by calling the `SteppingAction` method.
   * @param eventID ID of the currently simulated event, i.e. the one to which the given input \f$\gamma\f$ track belongs to
   */
  static void GammaStepper(G4HepEmTLData& theTLData, G4HepEmState& theState, TrackStack& theTrackStack, Geometry& theGeometry, Results& theResult, int eventID);

  /** Stepping loop for simulating the entire history of a \f$e^-/e^+\f$ track.
   *
   * The initial state of the \f$e^-/e^+\f$ track is provided in the `G4HepEmGammaTrack` field of `theTLData` input argument by the caller.
   * The history is simulated then till the end, the state of the \f$e^-/e^+\f$ track is updated while secondary tracks, produced in the
   * physics interactions, are pushed to `theTrackStack` (if any) and the required simulation results are collected/updated into
   * `theResult` structure after each individual simulation step.
   *
   * @param theTLData a `G4HepEm` specific (thread local) object primarily used to obtain all physics related information from `G4HepEm` needed to compute a simulation step
   * @param theState a `G4HepEm` specific object that stores pointers to the top level `G4HepEm` data structure and parameters that are used by `G4HepEm` to provide all physics related infomation needed to compute a simulation step
   * @param theTrackStack the track stack that is used to store the secondary tracks produced while simulating the entire history o fthe input \f$e^-/e^+\f$ track
   * @param theGeometry the geometry of the application in which the input track history is simulated
   * @param theResult the data structure that holds all the infomation needs to be collected during the simulation. It might be updated after each simulation step by calling the `SteppingAction` method.
   * @param eventID ID of the currently simulated event, i.e. the one to which the given input \f$e^-/e^+\f$ track belongs to
   */
  static void ElectronStepper(G4HepEmTLData& theTLData, G4HepEmState& theState, TrackStack& theTrackStack, Geometry& theGeometry, Results& theResult, int eventID);


private:
  SteppingLoop() = delete;

  /** Auxiliary method that pushes the secondary track(s), produced by physics interactions at the post-step point (if any), into the track stack.
   *
   * @param theTLData the `G4HepEm` specific (thread local) object that is used by `G4HepEm` to deliver the secondary tracks to the caller after calling the its `Perform` top level method
   * @param theTrackStack the track stack that is used to store the secondary tracks produced while simulating the entire history of the input track in the steppers
   * @param thePrimary the primary track, in its post interaction state (after calling `G4HepEm` top level `Perform` method), i.e. the one that underwent the physics interaction
   */
  static void StackSecondaries(G4HepEmTLData& theTLData, TrackStack& theTrackStack, G4HepEmTrack& thePrimary);

  /** This method is called at the end of each simulation steps to collect some data during the simulation.
   *
   * This method provides the possibility of collecting some data after each simulation steps (e.g. energy deposit or length of the step).
   * Among the `Geant4` user actions this corresponds to the `G4UserSteppingAction`
   *
   * @param theResult the data structure that holds all the infomation needs to be collected during the simulation (some fields might be updated)
   * @param theTrack the primary track, in its post interaction state, i.e. at the end of the step
   * @param currentVolume pointer to the volume (`absorber`/`gap`) in which the simulation step was done
   * @param currentPhysStepLength real (physical) length of the step
   * @param indxLayer index of the layer in which the step was done
   * @param indxAbsorber indicates if the step was done in the `absorber` (0) or in the `gap` (1)
   * @param eventID ID of the event to which the particle under tracking belongs to
   * @param stepID ID of this step that was just performed, i.e. number of steps cmpleted so far with with the current track
   */
  static void SteppingAction(Results& theResult, const G4HepEmTrack& theTrack, const Box* currentVolume, double currentPhysStepLength, int indxLayer, int indxAbsorber, int eventID, int stepID);


  // some utilities to modify 3vectors
  static void Set3Vect(double* v, double to);
  static void Set3Vect(double* v, const double* to);
  static void AddTo3Vect(double* v, const double* u, double scale=1.0);

};

#endif // STEPPINGLOOP_HH
