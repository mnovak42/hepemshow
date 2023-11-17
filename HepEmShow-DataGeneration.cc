/**
 * @file    HepEmShow-DataGeneration.cc
 * @author  M. Novak
 * @date    July 2023
 *
 * @brief The main funtion of the auxiliary `HepEmShow-DataGeneration` application.
 *
 * The physics of the `HepEmShow` simulation application is provided by `G4HepEm`.
 * While the run time of `G4HepEm` is independent from `Geant4`, the `G4HepEm`
 * data initialisation relies and strongly depends on `Geant4`. However, the
 * initialised state, which is manly physics related data such as cross sections,
 * stopping power, range, etc., can be exported into a single file. `G4HepEm` can
 * then be re-initialised solely from this file making possible to skipp its
 * entire `Geant4` dependent initialisation part. This is exactly how `G4HepEm`
 * is initialised in the `HepEmShow` simulation application, i.e. by utilising
 * the provided data file. Therefore, the `HepEmShow` simulation application
 * is independent from `Geant4`. Furthermore, its single dependence, `G4HepEm`
 * can also be built without `Geant4` (optionally, then its `Geant4` dependent
 * initialisation part is not included). This makes possible to build the
 * `HepEmShow` simulation with a `Geant4` independent build of `G4HepEm`.
 *
 * However, the generated and provided `G4HepEm` data file is specific for a
 * given (default) material configuration of the application geometry. The
 * `HepEmShow-DataGeneration` application was used to generate and can be used
 * to re-gerenate such a `G4HepEm` data file for the `HepEmShow` simulation
 * e.g. with a different material configuration if needed (see more in the
 * Geometry and Physics component documentations).
 *
 * @note As the `G4HepEm` data generation requires its initialisation, that heavily
 * depends on `Geant4`, this `HepEmShow-DataGeneration` application requires a
 * complete, `Geant4` dependent build of `G4HepEm`. Moreover, as this data
 * generation application builds and pre-initialises a `Geant4` geometry (with
 * the given list of materials and secondary production threshold value) to be
 * ready for the physics initialisation (using `G4Setup`), the application
 * itself depends on `Geant4`. Therefore, the corresponding executable will be
 * available only if `HepEmShow` show is built with `Geant4` dependence and
 * with a complete, `Geant4` dependent built of `G4HepEm`.
 *
 * See more in the documentation.
 */

// Geant4
#include "G4Setup.hh"
#include "Randomize.hh"

// G4HepEm
#include "G4HepEmDataJsonIO.hh"
#include "G4HepEmState.hh"
#include "G4HepEmData.hh"
#include "G4HepEmParameters.hh"

#include "G4HepEmRunManager.hh"
#include "G4HepEmRandomEngine.hh"

// geant4 types and units
#include "globals.hh"
#include "G4SystemOfUnits.hh"


#include <vector>
#include <string>

int main () {

  // secondary production threshold in length (change the value and regenerate the data)
  const G4double secProdThreshold = 0.7 * mm;

  // list of Geant4 (NIST) material names (change the listed material names and regenerate the data)
  std::vector<std::string> matList {"G4_Galactic", "G4_PbWO4", "G4_lAr"};

  // output, i.e. the G4HepEm data, file name (change the file mane and regenerate the data)
  const G4String fileName    = "../data/hepem_data";
  const G4String g4hepemFile = fileName + ".json";

  // create a fake Geant4 geometry and init to have the material-cuts couples
  FakeG4Setup (matList, secProdThreshold, 1);

  // construct the G4HepEmRunManager, which will fill the data structures
  // on calls to Initialize
  auto* runMgr    = new G4HepEmRunManager(true);
  auto* rngEngine = new G4HepEmRandomEngine(G4Random::getTheEngine());
  runMgr->Initialize(rngEngine, 0);
  runMgr->Initialize(rngEngine, 1);
  runMgr->Initialize(rngEngine, 2);

  G4HepEmState state;
  state.fData       = runMgr->GetHepEmData();
  state.fParameters = runMgr->GetHepEmParameters();
  // Serialize to file
  {
    std::ofstream jsonOS{ g4hepemFile.c_str() };
    if(!G4HepEmStateToJson(jsonOS, &state))
    {
      std::cerr << "Failed to write G4HepEMState to " << g4hepemFile
                << std::endl;
      jsonOS.close();
      return 1;
    }
  }

  FreeG4HepEmData(state.fData);

  return 0;
}
