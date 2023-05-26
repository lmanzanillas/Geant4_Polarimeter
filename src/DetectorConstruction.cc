#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SiliconPlateConstruction.hh"
#include "CollimatorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4MaterialTable.hh"
#include "SoleilMaterials.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4Hype.hh"

#include "G4Transform3D.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include <G4UserLimits.hh>

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"
#include <G4VPrimitiveScorer.hh>

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VoxelLimits.hh"

#include "G4MTRunManager.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

#include "G4GDMLParser.hh"

#include <G4VisAttributes.hh>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
Constructs DetectorConstruction, defines default values.
*/

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),  geDetectorTube(nullptr), 
logicCollimatorLEAPS(nullptr), 
logicSample(nullptr), 
geLogicVolume(nullptr) 
{
  fDetectorMessenger = new DetectorMessenger(this);
  halfSizeDarkBoxX = halfSizeDarkBoxY = halfSizeDarkBoxZ = 1.*m;
  fSetupName = "Polar";
  fDataType = "csv";
  detectorInnerRadius = 62.2*mm;
  detectorThickness = 2.0*mm;
  detectorHeight = 11.5*mm;
  sampleInnerRadius = 0.0*mm;
  sampleThickness = 1.615*mm;
  sampleHeight = 10.0*mm;
  beWindowRadius = 8.0*mm;
  CollimatorThickness = 20.0*mm;
  distanceCollimatorDetector = 5.0*mm;
  ContactThickness = 300.0*nm;
  halfSizeCameraX = 50.0*mm;
  halfSizeCameraY = 50.0*mm;
  halfSizeCameraZ = 0.15*mm;
  nSamples = 1;
  fDetectorType = 0;
  fDetectorName = "Polarimeter";
  fVolName = "World";
  materialConstruction = new SoleilMaterials;
  DefineMaterials();
  data_output_directory = "./";  
  fCollimatorMaterial = G4Material::GetMaterial("titanium_grade1");
  fTargetMaterial = G4Material::GetMaterial("G4_Ge");
  fSampleMaterial = G4Material::GetMaterial("G4_POLYETHYLENE");
  fWorldMaterial = G4Material::GetMaterial("G4_Galactic");
  materialGeContainer = G4Material::GetMaterial("Aluminium");
  materialGeContainerCoating = G4Material::GetMaterial("Aluminium");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){
  //delete physicWorldBox;
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::GetPhysicalVolumeByName(const G4String &name)
{
  // access the store of physical volumes
  G4PhysicalVolumeStore * pvs= G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pv;
  G4int npv= pvs->size();
  G4int ipv;
  for (ipv=0; ipv<npv; ipv++) {
    pv= (*pvs)[ipv];
    if (!pv)
      break;
    //G4cout<<" pv->GetName() "<<pv->GetName()<<G4endl;
    if (pv->GetName() == name)
      return pv;
  }
  return NULL;
}




/*
Sets which detector geometry is used.
*/
void DetectorConstruction::SetDetectorType(G4int value){
  fDetectorType=value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::SetNumberOfTargetSamples(G4int value){
  nSamples = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //UpdateGeometry();
}

//Sets dimmensions of target, thickness corresponds to the Z coordinate, Length to x.
void DetectorConstruction::SetGeDetectorLength(G4double value){
  detectorInnerRadius = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetGeDetectorThickness(G4double value){
  detectorThickness = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetGeDetectorWidth(G4double value){
  detectorHeight = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetContactThickness(G4double value){
  ContactThickness = (value/1.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//Set sample dimmensions
void DetectorConstruction::SetSampleLength(G4double value){
  sampleInnerRadius = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetSampleThickness(G4double value){
  sampleThickness = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetSampleWidth(G4double value){
  sampleHeight = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetBeWindowRadius(G4double value){
  beWindowRadius = value*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//Collimator thickness
void DetectorConstruction::SetCollimatorThickness(G4double value){
  CollimatorThickness = value*mm;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//Collimator to detector distance
void DetectorConstruction::SetDistanceCollimatorDetector(G4double value){
  distanceCollimatorDetector = value*mm;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//Sets material of target.
void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    fTargetMaterial = pttoMaterial;
    if(geLogicVolume)geLogicVolume->SetMaterial(fTargetMaterial);
    G4cout<<" material: "<<fTargetMaterial->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

//Sets material of Ge Container
void DetectorConstruction::SetGeContainerMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    materialGeContainer = pttoMaterial;
    if(logicGeContainer)logicGeContainer->SetMaterial(materialGeContainer);
    G4cout<<" material container: "<<materialGeContainer->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

//Sets material of Ge Container coating
void DetectorConstruction::SetGeContainerMaterialCoating(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    materialGeContainerCoating = pttoMaterial;
    if(logicGeContainerCoating)logicGeContainerCoating->SetMaterial(materialGeContainerCoating);
    G4cout<<" material container coating: "<<materialGeContainerCoating->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::SetDetectorName(G4String detectorNAME)
{
	fDetectorName = detectorNAME;
}

void DetectorConstruction::SetSetupName(G4String setupNAME)
{
	fSetupName = setupNAME;
}
void DetectorConstruction::SetDataType(G4String dataType)
{
	fDataType = dataType;
}
/*
Sets material of sample.
*/
void DetectorConstruction::SetSampleMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    fSampleMaterial = pttoMaterial;
    if(logicSample)logicSample->SetMaterial(fSampleMaterial);
    G4cout<<" material "<<fSampleMaterial->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}



void DetectorConstruction::SetOutputDirectory(G4String output_directory)
{
    data_output_directory = output_directory;  
}
/*
Sets material of world volume.
*/
void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) {
    fWorldMaterial = pttoMaterial;
    if ( logicWorldBox ) { logicWorldBox->SetMaterial(fWorldMaterial); }
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::SetCollimatorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) {
    fCollimatorMaterial = pttoMaterial;
    if ( logicCollimatorLEAPS ) { logicCollimatorLEAPS->SetMaterial(fCollimatorMaterial); }
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}


/*
Defines materials used in simulation. Sets material properties for PEN and other optical components.
*/
void DetectorConstruction::DefineMaterials(){
  // ============================================================= Materials =============================================================
  //materialConstruction = new PenMaterials;
  materialConstruction-> Construct();
  materialAir = G4Material::GetMaterial("Air");
  materialBialkali = G4Material::GetMaterial("Bialkali");
  fGlass = G4Material::GetMaterial("BorosilicateGlass");
  PenMaterial = G4Material::GetMaterial("PEN");
  PVTMaterial = G4Material::GetMaterial("PVT_structure");
  materialSi = G4Material::GetMaterial("G4_Si");
  materialGe = G4Material::GetMaterial("G4_Ge");
  materialTriggerFoilEJ212 = G4Material::GetMaterial("EJ212");
  Pstyrene = G4Material::GetMaterial("Polystyrene");
  materialPMMA = G4Material::GetMaterial("PMMA");
  fVacuum = G4Material::GetMaterial("G4_Galactic");
  materialGreaseEJ550 = G4Material::GetMaterial("Grease");
  materialTeflon = G4Material::GetMaterial("G4_TEFLON");
  materialVikuiti = G4Material::GetMaterial("Vikuiti");
  materialTitanium = G4Material::GetMaterial("titanium_grade1");
  materialCollimatorCoating = G4Material::GetMaterial("Aluminium");
  materialSample = G4Material::GetMaterial("Iron");
  materialBeWindow = G4Material::GetMaterial("G4_Be");
  materialMetalTube = G4Material::GetMaterial("G4_STAINLESS-STEEL");
  materialSupportPins = G4Material::GetMaterial("PEEK");
  materialContacts = G4Material::GetMaterial("Aluminium");
  materialPolyethylene = G4Material::GetMaterial("G4_POLYETHYLENE");

  G4cout<<" materials imported succesfully "<<G4endl;

}

void DetectorConstruction::SetVolName(G4ThreeVector thePoint){
  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* myVolume= theNavigator->LocateGlobalPointAndSetup(thePoint);
  fVolName =  myVolume->GetName();
}


void DetectorConstruction::UpdateGeometry(){

  //define new one
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::Clean();
  G4LogicalVolumeStore::Clean();
  G4SolidStore::Clean();

  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

/*
Clears stored geometry, then constructs all volumes that can be used in the simulation.

Builds and places volumes in world.

Defines detector sensitivities and properties.
*/
G4VPhysicalVolume* DetectorConstruction::Construct()
{
// ============================================================= Define Volumes =============================================================
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();
  //The experimental Dark Box
  fWorldBox = new G4Box("World",halfSizeDarkBoxX,halfSizeDarkBoxY,halfSizeDarkBoxZ);
  logicWorldBox = new G4LogicalVolume(fWorldBox,materialAir,"World",0,0,0);
  physicWorldBox = new G4PVPlacement(0,G4ThreeVector(),logicWorldBox,"World",0,false,0);

  G4double detectorExternalRadius = detectorInnerRadius + detectorThickness;
  geDetectorTube = new G4Tubs("target", detectorInnerRadius, detectorExternalRadius, detectorHeight, 0.*deg, 360.*deg);
  geLogicVolume = new G4LogicalVolume(geDetectorTube,fTargetMaterial, "target",0,0,0);

  //camera behind sample 
  cameraBox = new G4Box("camera",halfSizeCameraX,halfSizeCameraY,halfSizeCameraZ);
  logicCameraBox = new G4LogicalVolume(cameraBox,fTargetMaterial,"target",0,0,0);

  //solidGeContainer = new G4Tubs("ge_container", geContainerInnerRadius, geContainerOuterRadius, geContainerHalfThickness, 0.*deg,360.*deg);

  G4double sampleExternalRadius = sampleInnerRadius + sampleThickness;
  G4double sampleContainerExternalRadius = (8.39/2)*mm;
  G4double secondContainerExternalRadius = (12.09/2)*mm;
  G4double heightSampleContainer = 37.*mm;
  G4double heightSecondContainer = 93.*mm;

  G4double insertSampleContainer = 8.7*mm;
  G4double insertSampleContainerContainer = 15.*mm;

  sampleTube = new G4Tubs("sample", sampleInnerRadius, sampleExternalRadius, sampleHeight, 0.*deg, 360.*deg);
  G4Tubs* sampleTubeContainer = new G4Tubs("containerSample", sampleExternalRadius, sampleContainerExternalRadius, heightSampleContainer, 0.*deg, 360.*deg);
  G4Tubs* sampleTubeSecondContainer = new G4Tubs("containerOfContainer", sampleContainerExternalRadius, secondContainerExternalRadius, heightSecondContainer, 0.*deg, 360.*deg);

  logicSample = new G4LogicalVolume(sampleTube,fSampleMaterial, "sample",0,0,0);
  G4LogicalVolume* logicSampleContainer = new G4LogicalVolume(sampleTubeContainer,fSampleMaterial, "sample",0,0,0);
  G4LogicalVolume* logicSecondContainer = new G4LogicalVolume(sampleTubeSecondContainer,fSampleMaterial, "sample",0,0,0);

  G4double zPositionContainer = - sampleHeight - insertSampleContainer - heightSampleContainer/2;
  G4double zPositionContainerContainer =  zPositionContainer - insertSampleContainerContainer - heightSecondContainer/2;
  G4double zPositionCamera = - sampleHeight - sampleHeight/2.;

  //Detector
  G4VisAttributes* visualAttributesGeDetector = new G4VisAttributes(G4Colour::Blue());
  visualAttributesGeDetector->SetVisibility(true);
  visualAttributesGeDetector->SetForceWireframe(true);
  visualAttributesGeDetector->SetForceAuxEdgeVisible(true);
  visualAttributesGeDetector->SetForceSolid(true);
  geLogicVolume->SetVisAttributes(visualAttributesGeDetector);


  G4VisAttributes* visualAttributesCamera = new G4VisAttributes(G4Colour::Brown());
  visualAttributesCamera->SetVisibility(true);
  visualAttributesCamera->SetForceWireframe(true);
  visualAttributesCamera->SetForceAuxEdgeVisible(true);
  visualAttributesCamera->SetForceSolid(true);
  logicCameraBox->SetVisAttributes(visualAttributesCamera);



  G4VisAttributes* visualAttributesSample = new G4VisAttributes(G4Colour::Black());
  visualAttributesSample->SetVisibility(true);
  visualAttributesSample->SetForceSolid(true);
  logicSample->SetVisAttributes(visualAttributesSample);

  G4VisAttributes* visualAttributesSampleContainer = new G4VisAttributes(G4Colour::Blue());
  visualAttributesSampleContainer->SetVisibility(true);
  visualAttributesSampleContainer->SetForceSolid(true);
  logicSampleContainer->SetVisAttributes(visualAttributesSampleContainer);

  G4VisAttributes* visualAttributesSampleContainer2 = new G4VisAttributes(G4Colour::Red());
  visualAttributesSampleContainer2->SetVisibility(true);
  visualAttributesSampleContainer2->SetForceSolid(true);
  logicSecondContainer->SetVisAttributes(visualAttributesSampleContainer2);

  //  ============================================================= Place volumes =============================================================
  
  // Place main detector always at center of world volume
  switch(fDetectorType){

  //Detector alone for "direct" beam radiation	  
  case 0:
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		geLogicVolume,
		"target_"+std::to_string(1),
		logicWorldBox,false,1,false);
 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,- sampleHeight/2.),
		logicSample,
		"sample"+std::to_string(1),
		logicWorldBox,false,1,false);
 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,zPositionContainer),
		logicSampleContainer,
		"sampleContainer"+std::to_string(1),
		logicWorldBox,false,1,false);
 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,zPositionContainerContainer),
		logicSecondContainer,
		"sampleSecondContainer"+std::to_string(1),
		logicWorldBox,false,1,false);
 
    
     break;
  //with camera behind sample
  case 1:
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		geLogicVolume,
		"target_"+std::to_string(1),
		logicWorldBox,false,1,false);
 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,- sampleHeight/2.),
		logicSample,
		"sample"+std::to_string(1),
		logicWorldBox,false,1,false);
 
	 /*
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,zPositionContainer),
		logicSampleContainer,
		"sampleContainer"+std::to_string(1),
		logicWorldBox,false,1,false);
          */
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,zPositionCamera),
		logicCameraBox,
		"target_"+std::to_string(2),
		logicWorldBox,false,1,false);
 
    
     break;

  //Detector + collimator for "direct" beam radiation
  
  }

  //// Definition of simulation steps.
  //logicWorldBox->SetUserLimits(new G4UserLimits(0.1*mm));
  //geLogicVolume->SetUserLimits(new G4UserLimits(1.0*um));
  //logicBeWindow->SetUserLimits(new G4UserLimits(50.0*um));
  //logicContactVolume->SetUserLimits(new G4UserLimits(5.0*nm));

      
  return physicWorldBox;
}
