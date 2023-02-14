#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
  : G4UImessenger(),
    fAction(Gun),fGunDir(0)
{

  fGunDir = new G4UIdirectory("/Ge2SOS/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fSourceType = new G4UIcmdWithAnInteger("/Ge2SOS/gun/sourceType",this);
  fSourceType->SetGuidance("Choose the type of source");
  fSourceType->SetParameterName("sourceType",true);
  fSourceType->SetDefaultValue(0);
  fSourceType->AvailableForStates(G4State_Idle);

  fSourceDirectionType = new G4UIcmdWithAnInteger("/Ge2SOS/gun/sourceDirectionType",this);
  fSourceDirectionType->SetGuidance("Choose the direction type");
  fSourceDirectionType->SetParameterName("sourceDirectionType",true);
  fSourceDirectionType->SetDefaultValue(0);
  fSourceDirectionType->AvailableForStates(G4State_Idle);

  fSourceGeometry = new G4UIcmdWithAnInteger("/Ge2SOS/gun/sourceGeometry",this);
  fSourceGeometry->SetGuidance("Choose the type of geometry for the source");
  fSourceGeometry->SetParameterName("sourceGeometry",true);
  fSourceGeometry->SetDefaultValue(0);
  fSourceGeometry->AvailableForStates(G4State_Idle);

  fSourceEnergy = new G4UIcmdWithADoubleAndUnit("/Ge2SOS/gun/sourceEnergy", this);
  fSourceEnergy->SetGuidance("Choose source energy");
  fSourceEnergy->SetParameterName("sourceEnergy",true);
  fSourceEnergy->SetDefaultValue(20.*keV);
  fSourceEnergy->AvailableForStates(G4State_Idle);

  fSourcePolarizationAngle = new G4UIcmdWithADouble("/Ge2SOS/gun/sourceGammaPolarizationAngle", this);
  fSourcePolarizationAngle->SetGuidance("Choose angle of polarization in degrees");
  fSourcePolarizationAngle->SetParameterName("sourcePolarAngle",true);
  fSourcePolarizationAngle->SetDefaultValue(0.);
  fSourcePolarizationAngle->AvailableForStates(G4State_Idle);

  fSourcePolarizationDegree = new G4UIcmdWithADouble("/Ge2SOS/gun/sourceGammaPolarizationDegree", this);
  fSourcePolarizationDegree->SetGuidance("Choose degree of polarization o to 1");
  fSourcePolarizationDegree->SetParameterName("sourcePolarDegree",true);
  fSourcePolarizationDegree->SetDefaultValue(1.);
  fSourcePolarizationDegree->AvailableForStates(G4State_Idle);

  fSourcePositionX = new G4UIcmdWithADoubleAndUnit("/Ge2SOS/gun/sourcePositionX",this);
  fSourcePositionX->SetGuidance("Set Source x position");
  fSourcePositionX->SetParameterName("fPositionX",true);
  fSourcePositionX->SetDefaultValue(0.*mm);
  fSourcePositionX->AvailableForStates(G4State_Idle);

  fSourcePositionY = new G4UIcmdWithADoubleAndUnit("/Ge2SOS/gun/sourcePositionY",this);
  fSourcePositionY->SetGuidance("Set Source y position");
  fSourcePositionY->SetParameterName("fPositionY",true);
  fSourcePositionY->SetDefaultValue(0.*mm);
  fSourcePositionY->AvailableForStates(G4State_Idle);

  fSourcePositionZ = new G4UIcmdWithADoubleAndUnit("/Ge2SOS/gun/sourcePositionZ",this);
  fSourcePositionZ->SetGuidance("Set Source z position");
  fSourcePositionZ->SetParameterName("fPositionZ",true);
  fSourcePositionZ->SetDefaultValue(30.*mm);
  fSourcePositionZ->AvailableForStates(G4State_Idle);

  fSourceDiameter = new G4UIcmdWithADoubleAndUnit("/Ge2SOS/gun/sourceDiameter",this);
  fSourceDiameter->SetGuidance("Set Source diameter or side");
  fSourceDiameter->SetParameterName("fDiameter",true);
  fSourceDiameter->SetDefaultValue(5.*mm);
  fSourceDiameter->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fSourceType;
  delete fSourceDirectionType;
  delete fSourceGeometry;
  delete fSourceEnergy;
  delete fSourcePolarizationAngle;
  delete fSourcePolarizationDegree;
  delete fGunDir;
  delete fSourcePositionX;
  delete fSourcePositionY;
  delete fSourcePositionZ;
  delete fSourceDiameter;
  //delete fAction;//debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fSourceType) {
  	fAction->SetSourceType(fSourceType->GetNewIntValue(newValue));
  }
  if(command == fSourceDirectionType) {
  	fAction->SetSourceDirectionType(fSourceDirectionType->GetNewIntValue(newValue));
  }
  if(command == fSourceGeometry) {
  	fAction->SetSourceGeometry(fSourceGeometry->GetNewIntValue(newValue));
  }
  if(command == fSourceEnergy){
  	fAction->SetSourceEnergy(fSourceEnergy->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePolarizationAngle){
  	fAction->SetSourcePolarizationAngle(fSourcePolarizationAngle->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePolarizationDegree){
  	fAction->SetSourcePolarizationDegree(fSourcePolarizationDegree->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePositionX){
  	fAction->SetSourcePositionX(fSourcePositionX->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePositionY){
  	fAction->SetSourcePositionY(fSourcePositionY->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePositionZ){
  	fAction->SetSourcePositionZ(fSourcePositionZ->GetNewDoubleValue(newValue));
  }
  if(command == fSourceDiameter){
  	fAction->SetSourceDiameter(fSourceDiameter->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
