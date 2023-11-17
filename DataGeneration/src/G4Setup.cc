
#include "G4Setup.hh"

// Geant4 includes
#include "G4SystemOfUnits.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4String.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4Proton.hh"

#include "G4DataVector.hh"
#include "G4ProductionCuts.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ProductionCutsTable.hh"
#include "G4EmParameters.hh"

#include "G4TransportationManager.hh"

// builds a fake Geant4 geometry with all materials given just to be able to produce material-cuts couple
void FakeG4Setup (const std::vector<std::string>& g4NISTMatNames, double prodcut, int verbose) {
  if (g4NISTMatNames.size() < 1) return;
  //
  // --- Geometry definition: create the word and use the very first material to fill in
  const G4double wDimX = 0.6*mm;
  const G4double wDimY = 0.6*mm;
  const G4double wDimZ = 0.6*mm;
  G4Material*      mat = G4NistManager::Instance()->FindOrBuildMaterial(g4NISTMatNames[0]);
  G4Box*            sW = new G4Box ("Box",wDimX, wDimY, wDimZ);
  G4LogicalVolume*  lW = new G4LogicalVolume(sW,mat,"Box",0,0,0);
  G4PVPlacement*    pW = new G4PVPlacement(0,G4ThreeVector(),"Box",lW,0,false,0);
  // set the world volume for the GetTransportationManager::G4Navigator
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetWorldVolume(pW);
  //
  // --- Build all remanining NIST materials and set a logical volume for each
  const G4int    numMat = g4NISTMatNames.size();
  std::vector<G4LogicalVolume*> theLogicVolumes(numMat);
  const G4double halfX  =  0.5/numMat;  // half width of one material-box
  const G4double    x0  = -0.5+halfX;   // start x-position of the first material-box
  for (int im=0; im<numMat; ++im) {
    mat  = G4NistManager::Instance()->FindOrBuildMaterial(g4NISTMatNames[im]);
    if (mat==nullptr) {
      std::cerr << "  *** G4Setup::FakeG4Setup(): unknown Geant4 NIST material = " << g4NISTMatNames[im]
                << std::endl;
      exit(-1);
    }
//    std::cout << mat->GetName() << " : " << theCutInAbsLenght[im] << " " << theCutInRange[im] << std::endl;
    G4Box*           ss = new G4Box ("Box", halfX, 0.5, 0.5);
    G4LogicalVolume* ll = new G4LogicalVolume(ss, mat, "Box", 0, 0, 0);
    theLogicVolumes[im] = ll;
    new G4PVPlacement(0, G4ThreeVector(x0+im*halfX , 0, 0), "Box", ll, pW, false, 0);
  }
  //
  // --- Create particles that has secondary production threshold
  G4Gamma::Gamma();
  G4Electron::Electron();
  G4Positron::Positron();
  G4Proton::Proton();
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  partTable->SetReadiness();
  //
  // --- Create a detector region with the desired production cut, create couples
  //     and add everything to it
  G4ProductionCuts*    prodCut = new G4ProductionCuts();
  prodCut->SetProductionCut(prodcut);
  G4Region* reg = new G4Region("Det-Region");
  reg->AddRootLogicalVolume(lW);
  reg->UsedInMassGeometry(true);
  reg->SetProductionCuts(prodCut);
  //
  for (G4int im=0; im<numMat; ++im) {
    G4Material*  mat = G4NistManager::Instance()->GetMaterial(im);
    G4MaterialCutsCouple* couple = new G4MaterialCutsCouple(mat, prodCut);
    couple->SetIndex(im);
    reg->RegisterMaterialCouplePair(mat, couple);
  }
  //
  // --- Update the couple tables
  G4ProductionCutsTable* theCoupleTable = G4ProductionCutsTable::GetProductionCutsTable();
  theCoupleTable->UpdateCoupleTable(pW);
  if (verbose>0) {
    theCoupleTable->DumpCouples();
  }
  //
  // --- Set MSC range factor 
  G4EmParameters *param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetMscRangeFactor(0.04);

}
