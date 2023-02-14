#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* );
    virtual ~PrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    PrimaryGeneratorAction* 		fAction;
    G4UIdirectory*                  fGunDir;
    G4UIcmdWithAnInteger*			fSourceType;
    G4UIcmdWithAnInteger*			fSourceDirectionType;
    G4UIcmdWithAnInteger*			fSourceGeometry;
    G4UIcmdWithADoubleAndUnit* fSourceEnergy;
    G4UIcmdWithADouble* fSourcePolarizationAngle;
    G4UIcmdWithADouble* fSourcePolarizationDegree;
    G4UIcmdWithADoubleAndUnit* fSourcePositionX;
    G4UIcmdWithADoubleAndUnit* fSourcePositionY;
    G4UIcmdWithADoubleAndUnit* fSourcePositionZ;
    G4UIcmdWithADoubleAndUnit* fSourceDiameter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
