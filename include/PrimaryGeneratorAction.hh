#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class PrimaryGeneratorMessenger;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
	PrimaryGeneratorAction(DetectorConstruction*);
	~PrimaryGeneratorAction();

// public:
// 	PrimaryGeneratorAction();
// 	virtual ~PrimaryGeneratorAction();

public:
	virtual void GeneratePrimaries(G4Event*);
	void  SetSourceType(G4int newType);
	void  SetSourceDirectionType(G4int newType);
	void  SetSourceGeometry(G4int newType);
	void  SetSourceEnergy(G4double newEnergy);
	void  SetParticleName(G4int Z, G4int A, G4double excitEnergy);
	void  SetSourcePositionX(G4double newValue);
	void  SetSourcePositionY(G4double newValue);
	void  SetSourcePositionZ(G4double newValue);
	void  SetSourceDiameter(G4double newValue);
	void  SetSourceSizeX(G4double newValue);
	void  SetSourceSizeY(G4double newValue);
	void  SetSourcePolarizationDegree(G4double newValue);
	void  SetSourcePolarizationAngle(G4double newValue);
	void  SetSourceCircularPolarizationAngle(G4double newValue);
	G4int GetSourceType(void){return fSourceType;};
	G4int GetSourceGeometry(void){return fSourceGeometry;};
	G4double GetSourceEnergy(void){return fSourceEnergy;};
	G4String GetParticleName(void){return fParticleName;};
	G4double GetSourcePositionX(void){return fPositionX;};
	G4double GetSourcePositionY(void){return fPositionY;};
	G4double GetSourcePositionZ(void){return fPositionZ;};
	G4double GetSourceDiameter(void){return fDiameter;};
	G4double GetSourceSizeX(void){return fsource_size_X;};
	G4double GetSourceSizeY(void){return fsource_size_Y;};
	G4double GetSourcePolarizationAngle(void){return fTheta_polar;};
	G4double GetSourceCircularPolarizationAngle(void){return fOmega_polar;};
	G4double GetSourcePolarizationDegree(void){return fPolarization_degree;};
private:
	G4double fPositionX;
	G4double fPositionY;
	G4double fPositionZ;
	G4double fDiameter;
	G4double fsource_size_X;
	G4double fsource_size_Y;
	G4String fParticleName;
	PrimaryGeneratorMessenger* fPrimaryMessenger;
	G4ParticleGun*  fParticleGun;
	G4int           fSourceType;
	G4int           fSourceDirectionType;
	G4int           fSourceGeometry;
	G4double	fSourceEnergy;
	G4double	fTheta_polar;
	G4double	fOmega_polar;
	G4double	fPolarization_degree;
	DetectorConstruction*      fDetector;
	G4double		fPhotonWavelength;
	PrimaryGeneratorMessenger* fGunMessenger;
	G4ThreeVector fPoint;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*PrimaryGeneratorAction_h*/
