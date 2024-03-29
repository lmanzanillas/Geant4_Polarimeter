#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:

    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    DetectorConstruction*      fDetector;


    G4UIdirectory*             fPoSOSDir;
    G4UIdirectory*             fDetDir;

    G4UIcmdWithAString*         commandSetWorldMaterial;
    G4UIcmdWithAnInteger*       commandSetDetectorType;
    G4UIcmdWithAnInteger*       commandSetNumberOfTargetSamples;
    G4UIcmdWithADoubleAndUnit*  commandSetGeDetectorLength;
    G4UIcmdWithADoubleAndUnit*  commandSetGeDetectorThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetGeDetectorWidth;
    G4UIcmdWithADoubleAndUnit*  commandSetContactThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetBeWindowRadius;
    G4UIcmdWithADoubleAndUnit*  commandSetCollimatorThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetDistanceCollimatorDetector;
    G4UIcmdWithAString*         commandSetCollimatorMaterial;
    G4UIcmdWithAString*         commandSetTargetMaterial;
    G4UIcmdWithAString*         commandSetGeContainerMaterial;
    G4UIcmdWithAString*         commandSetGeContainerMaterialCoating;
    G4UIcmdWithAString*         commandSetDetectorName;
    G4UIcmdWithAString*         commandSetSetupName;
    G4UIcmdWithAString*         commandSetDataType;
    G4UIcmdWithAString*         commandSetOutputDirectory;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
