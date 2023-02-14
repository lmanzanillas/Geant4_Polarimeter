#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),
 fDetector(Det),
 fPoSOSDir(0),
 fDetDir(0),
 commandSetWorldMaterial(0),
 commandSetDetectorType(0),
 commandSetNumberOfTargetSamples(0),
 commandSetGeDetectorLength(0),
 commandSetGeDetectorThickness(0),
 commandSetGeDetectorWidth(0),
 commandSetContactThickness(0),
 commandSetBeWindowRadius(0),
 commandSetCollimatorThickness(0),
 commandSetDistanceCollimatorDetector(0),
 commandSetCollimatorMaterial(0),
 commandSetTargetMaterial(0),
 commandSetGeContainerMaterial(0),
 commandSetGeContainerMaterialCoating(0),
 commandSetDetectorName(0),
 commandSetSetupName(0),
 commandSetDataType(0),
 commandSetOutputDirectory(0)
 {
  fDetDir = new G4UIdirectory("/PoSOS/det/");
  fDetDir->SetGuidance("detector construction commands");

  commandSetCollimatorMaterial = new G4UIcmdWithAString("/PoSOS/det/setCollimatorMat",this);
  commandSetCollimatorMaterial->SetGuidance("Select material of the collimator.");
  commandSetCollimatorMaterial->SetParameterName("choice",false);
  commandSetCollimatorMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetCollimatorMaterial->SetToBeBroadcasted(false);

  commandSetTargetMaterial = new G4UIcmdWithAString("/PoSOS/det/setGeDetectorMat",this);
  commandSetTargetMaterial->SetGuidance("Select material of the target.");
  commandSetTargetMaterial->SetParameterName("choice",false);
  commandSetTargetMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetTargetMaterial->SetToBeBroadcasted(false);

  commandSetGeContainerMaterial = new G4UIcmdWithAString("/PoSOS/det/setGeContainerMat",this);
  commandSetGeContainerMaterial->SetGuidance("Select material of the ge container.");
  commandSetGeContainerMaterial->SetParameterName("choice",false);
  commandSetGeContainerMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeContainerMaterial->SetToBeBroadcasted(false);

  commandSetGeContainerMaterialCoating = new G4UIcmdWithAString("/PoSOS/det/setGeContainerMatCoating",this);
  commandSetGeContainerMaterialCoating->SetGuidance("Select material of the ge container coating.");
  commandSetGeContainerMaterialCoating->SetParameterName("choice",false);
  commandSetGeContainerMaterialCoating->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeContainerMaterialCoating->SetToBeBroadcasted(false);

  commandSetDetectorName = new G4UIcmdWithAString("/PoSOS/det/setGeDetectorName",this);
  commandSetDetectorName->SetGuidance("Select name of detector.");
  commandSetDetectorName->SetParameterName("choice",false);
  commandSetDetectorName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorName->SetToBeBroadcasted(false);

  commandSetSetupName = new G4UIcmdWithAString("/PoSOS/det/setSetupName",this);
  commandSetSetupName->SetGuidance("Select name of setup.");
  commandSetSetupName->SetParameterName("choice",false);
  commandSetSetupName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetSetupName->SetToBeBroadcasted(false);

  commandSetDataType = new G4UIcmdWithAString("/PoSOS/det/setDataType",this);
  commandSetDataType->SetGuidance("Select format of data: csv, hdf5, root.");
  commandSetDataType->SetParameterName("choice",false);
  commandSetDataType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDataType->SetToBeBroadcasted(false);

  commandSetOutputDirectory = new G4UIcmdWithAString("/PoSOS/det/setOutputDirectory",this);
  commandSetOutputDirectory->SetGuidance("Set output directory");
  commandSetOutputDirectory->SetParameterName("choice",false);
  commandSetOutputDirectory->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetOutputDirectory->SetToBeBroadcasted(false);

  commandSetWorldMaterial = new G4UIcmdWithAString("/PoSOS/det/setWorldMat",this);
  commandSetWorldMaterial->SetGuidance("Select material of the world.");
  commandSetWorldMaterial->SetParameterName("choice",false);
  commandSetWorldMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetWorldMaterial->SetToBeBroadcasted(false);

  commandSetGeDetectorLength = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setGeDetectorLength",this);
  commandSetGeDetectorLength->SetGuidance("Set length of target samples");
  commandSetGeDetectorLength->SetParameterName("SampleLength",false);
  commandSetGeDetectorLength->SetRange("SampleLength>0.");
  commandSetGeDetectorLength->SetUnitCategory("Length");
  commandSetGeDetectorLength->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorLength->SetToBeBroadcasted(false);

  commandSetGeDetectorThickness = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setGeDetectorThickness",this);
  commandSetGeDetectorThickness->SetGuidance("Set thickness of target samples");
  commandSetGeDetectorThickness->SetParameterName("SampleThickness",false);
  commandSetGeDetectorThickness->SetRange("SampleThickness>0.");
  commandSetGeDetectorThickness->SetUnitCategory("Length");
  commandSetGeDetectorThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorThickness->SetToBeBroadcasted(false);

  commandSetGeDetectorWidth = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setGeDetectorWidth",this);
  commandSetGeDetectorWidth->SetGuidance("Set width of target samples");
  commandSetGeDetectorWidth->SetParameterName("SampleWidth",false);
  commandSetGeDetectorWidth->SetRange("SampleWidth>0.");
  commandSetGeDetectorWidth->SetUnitCategory("Length");
  commandSetGeDetectorWidth->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorWidth->SetToBeBroadcasted(false);

  commandSetContactThickness = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setContactThickness",this);
  commandSetContactThickness->SetGuidance("Set thickness of Al contact");
  commandSetContactThickness->SetParameterName("ContactThickness",false);
  commandSetContactThickness->SetRange("ContactThickness>0.");
  commandSetContactThickness->SetUnitCategory("Length");
  commandSetContactThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetContactThickness->SetToBeBroadcasted(false);

  commandSetBeWindowRadius = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setBeWindowRadius",this);
  commandSetBeWindowRadius->SetGuidance("Set radius of Be Window");
  commandSetBeWindowRadius->SetParameterName("WindowRadius",false);
  commandSetBeWindowRadius->SetRange("WindowRadius>0.");
  commandSetBeWindowRadius->SetUnitCategory("Length");
  commandSetBeWindowRadius->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetBeWindowRadius->SetToBeBroadcasted(false);


  commandSetCollimatorThickness = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setCollimatorThickness",this);
  commandSetCollimatorThickness->SetGuidance("Set thickness of target samples");
  commandSetCollimatorThickness->SetParameterName("CollimatorThickness",false);
  commandSetCollimatorThickness->SetRange("CollimatorThickness>0.");
  commandSetCollimatorThickness->SetUnitCategory("Length");
  commandSetCollimatorThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetCollimatorThickness->SetToBeBroadcasted(false);

  commandSetDistanceCollimatorDetector = new G4UIcmdWithADoubleAndUnit("/PoSOS/det/setDistanceCollimatorDetector",this);
  commandSetDistanceCollimatorDetector->SetGuidance("Set distance collimator detector");
  commandSetDistanceCollimatorDetector->SetParameterName("CollimatorDistance",false);
  commandSetDistanceCollimatorDetector->SetRange("CollimatorDistance>0.");
  commandSetDistanceCollimatorDetector->SetUnitCategory("Length");
  commandSetDistanceCollimatorDetector->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDistanceCollimatorDetector->SetToBeBroadcasted(false);


  commandSetDetectorType = new G4UIcmdWithAnInteger("/PoSOS/det/setDetectorType",this);
  commandSetDetectorType->SetGuidance("Set detector type");
  commandSetDetectorType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorType->SetToBeBroadcasted(false);

  commandSetNumberOfTargetSamples = new G4UIcmdWithAnInteger("/PoSOS/det/setNTargetSamples",this);
  commandSetNumberOfTargetSamples->SetGuidance("Set number of target Samples");
  commandSetNumberOfTargetSamples->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetNumberOfTargetSamples->SetToBeBroadcasted(false);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fPoSOSDir;
  delete commandSetWorldMaterial;
  delete commandSetDetectorType;
  delete commandSetNumberOfTargetSamples;
  delete commandSetGeDetectorLength;
  delete commandSetGeDetectorThickness;
  delete commandSetGeDetectorWidth;
  delete commandSetContactThickness;
  delete commandSetBeWindowRadius;
  delete commandSetCollimatorThickness;
  delete commandSetDistanceCollimatorDetector;
  delete commandSetCollimatorMaterial;
  delete commandSetTargetMaterial;
  delete commandSetGeContainerMaterial;
  delete commandSetGeContainerMaterialCoating;
  delete commandSetDetectorName;
  delete commandSetSetupName;
  delete commandSetDataType;
  delete commandSetOutputDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
   if( command == commandSetTargetMaterial ){
	fDetector->SetTargetMaterial(newValue);
   }

   if( command == commandSetGeContainerMaterial ){
	fDetector->SetGeContainerMaterial(newValue);
   }

   if( command == commandSetGeContainerMaterialCoating ){
	fDetector->SetGeContainerMaterialCoating(newValue);
   }

   if( command == commandSetDetectorName ){
	fDetector->SetDetectorName(newValue);
   }

   if( command == commandSetSetupName ){
	fDetector->SetSetupName(newValue);
   }

   if( command == commandSetDataType ){
	fDetector->SetDataType(newValue);
   }

   if( command == commandSetCollimatorMaterial ){
	fDetector->SetCollimatorMaterial(newValue);
   }

   if( command == commandSetOutputDirectory ){
	fDetector->SetOutputDirectory(newValue);
   }

   if( command == commandSetWorldMaterial ){
	fDetector->SetWorldMaterial(newValue);
   }

   if( command == commandSetGeDetectorLength ){
	fDetector->SetGeDetectorLength(commandSetGeDetectorLength->GetNewDoubleValue(newValue));
   }

   if( command == commandSetGeDetectorThickness ){
	fDetector->SetGeDetectorThickness(commandSetGeDetectorThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetContactThickness ){
	fDetector->SetContactThickness(commandSetContactThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetGeDetectorWidth ){
	fDetector->SetGeDetectorWidth(commandSetGeDetectorWidth->GetNewDoubleValue(newValue));
   }

   if( command == commandSetBeWindowRadius ){
	fDetector->SetBeWindowRadius(commandSetBeWindowRadius->GetNewDoubleValue(newValue));
   }

   if( command == commandSetCollimatorThickness ){
	fDetector->SetCollimatorThickness(commandSetCollimatorThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDistanceCollimatorDetector ){
	fDetector->SetDistanceCollimatorDetector(commandSetDistanceCollimatorDetector->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDetectorType ){
	fDetector->SetDetectorType(commandSetDetectorType->GetNewIntValue(newValue));
   }
  
   if( command == commandSetNumberOfTargetSamples ){
	fDetector->SetNumberOfTargetSamples(commandSetNumberOfTargetSamples->GetNewIntValue(newValue));
   }
   
}
