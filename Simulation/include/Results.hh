
#ifndef RESULTS_HH
#define RESULTS_HH

/**
 * @file    Results.hh
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief A collection of data that are recorded during the simulation.
 *
 * The following data is recorded during the simulation (mean is per event):
 *  - mean values in the individual layers of the calorimeter for energy deposit,
 *    neutral (gamma) and charged (electron/positron) particle simulation steps
 *  - mean number of energy deposited in the `absorber` and `gap`
 *  - mean number of secondary gamma, electron and positrons produced
 *  - mean number of neutral (gamma) and charged (electron/positron)
 *
 * Quantities, recorded in the individual layers are stored in histograms and
 * written to files at the end of the simulation while the others are reported
 * in the screen. An example looks like
 * ```
 *       --- Results::WriteResults ----------------------------------
 *
 *       Absorber: mean Edep = 6722.95 [MeV] and  Std-dev = 309.636 [MeV]
 *       Gap     : mean Edep = 2571.75 [MeV] and  Std-dev = 118.507 [MeV]
 *
 *       Mean number of gamma       4457.043
 *       Mean number of e-          7957.899
 *       Mean number of e+          428.922
 *
 *       Mean number of e-/e+ steps 36097
 *       Mean number of gamma steps 40436.2
 *       ------------------------------------------------------------
 * ```
 */

#include "Hist.hh"

/**
 * Data that needs to be accumulated during one `event` (the scope is one event):
 * - at the beginning of an `event`: usually reset (to zero)
 * - at the end of an `event`: usually written to the run scope data (see the
 *   `Results` below)
 */
struct ResultsPerEvent {
  double fEdepAbs       { 0.0 }; ///< energy deposit in the absorber during one event
  double fEdepGap       { 0.0 }; ///< energy deposit in the gap during one event
  //
  double fNumSecGamma   { 0.0 }; ///< number of seconday \f$\gamma\f$ particles generated during one event
  double fNumSecElectron{ 0.0 }; ///< number of seconday \f$e^-\f$ particles generated during one event
  double fNumSecPositron{ 0.0 }; ///< number of seconday \f$e^+\f$ particles generated during one event
  //
  double fNumStepsGamma { 0.0 }; ///< number of \f$\gamma\f$ simulation steps during one event
  double fNumStepsElPos { 0.0 }; ///< number of \f$e^-/e^+\f$ simulation steps during one event
};


/**
 * Data that are collected during the entire `run` of the simulation:
 * - at the beginning of the `ru`n: need to be initialised
 * - at the end of an `run`: written out (to file or to the std output)
 * Mean quantities are computed over the simulated events.
 */
struct Results {
  Hist fEdepPerLayer;              ///< mean energy deposit per-layer histogram
  Hist fGammaTrackLenghtPerLayer;  ///< mean number of \f$\gamma\f$ steps per-layer histogram
  Hist fElPosTrackLenghtPerLayer;  ///< mean number of \f$e^-/e^+\f$ steps per-layer histogram
  //
  double fEdepAbs        { 0.0 };  ///< mean energy deposit in the `absorber`
  double fEdepAbs2       { 0.0 };  ///< mean of the squared energy deposit in the `absorber`
  double fEdepGap        { 0.0 };  ///< mean energy deposit in the `gap`
  double fEdepGap2       { 0.0 };  ///< mean of the squared energy deposit in the `gap`
  //
  double fNumSecGamma    { 0.0 };  ///< mean number of the produced secondary \f$\gamma\f$ particles
  double fNumSecGamma2   { 0.0 };  ///< mean of the squared number of produced secondary \f$\gamma\f$ particles
  double fNumSecElectron { 0.0 };  ///< mean number of the produced secondary \f$e^-\f$ particles
  double fNumSecElectron2{ 0.0 };  ///< mean of the squared number of produced secondary \f$e^-\f$ particles
  double fNumSecPositron { 0.0 };  ///< mean number of the produced secondary \f$e^+\f$ particles
  double fNumSecPositron2{ 0.0 };  ///< mean of the squared number of produced secondary \f$e^+\f$ particles
  //
  double fNumStepsGamma  { 0.0 };  ///< mean number of \f$\gamma\f$ steps in the entire calorimeter
  double fNumStepsGamma2 { 0.0 };  ///< mean of the squared number of \f$\gamma\f$ steps in the entire calorimeter
  double fNumStepsElPos  { 0.0 };  ///< mean number of \f$e^-/e^+\f$ steps in the entire calorimeter
  double fNumStepsElPos2 { 0.0 };  ///< mean of the squared number of \f$e^-/e^+\f$ steps in the entire calorimeter
  ResultsPerEvent fPerEventRes;    ///< data structure to accumulate results during a single event
};

/** Writes the final results of the simulation.
 *
 * Writes the 3 histrograms (mean energy deposit, \f$\gamma\f$ and \f$e^-/e^+\f$ steps per-layer) into files
 * while all the other collected data to the screen.*/
void WriteResults(struct Results& res, int numEvents=1);

#endif // RESULTS_HH
