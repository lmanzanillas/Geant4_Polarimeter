#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4MTRunManager.hh"

#include "Randomize.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4DecayTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalConstants.hh"

#include "G4Navigator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
: G4VUserPrimaryGeneratorAction()
{
        fDetector = det;
	fPrimaryMessenger = new PrimaryGeneratorMessenger(this);
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	fPositionX = 0.0 *mm;
        fPositionY = 0.0 *mm;
	fPositionZ = 30.*mm;
	fDiameter = 0.1*mm;
	fsource_size_X = 0.1*mm;
	fsource_size_Y = 0.03*mm;
	fSourceType = 0;
	fSourceDirectionType = 0;
	fSourceGeometry = 1;
	fSourceEnergy = 10*keV;
	fTheta_polar = 0; //in degrees, so the default is horizontal polarization
	fOmega_polar = 0; //in degrees, so the default is linear polarization
	fPolarization_degree = 0.95; // values from 0 to 1, so the default is 95% polarization 
	fPhotonWavelength = 0;
	fParticleName = "void";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Randomises placement and momentum vectors for required sources.

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4double ionCharge   = 0.*eplus;
	G4double excitEnergy = 0.*eV;
	G4int Z=0, A=0;
	G4ParticleDefinition* ion;
        //Define a more realistic source geometry
        G4double r = fDiameter * std::sqrt(G4UniformRand()) / 2.;
        G4double theta = 2. * pi * G4UniformRand()*rad;
        G4double x_sourceframe = fPositionX + r*cos(theta)*CLHEP::mm;
        G4double y_sourceframe = fPositionY + r*sin(theta)*CLHEP::mm;
        G4double z_sourceframe = r*sin(theta)*CLHEP::mm;
	//G4cout<<" r "<<r<<" theta: "<<theta<<" x "<<x_sourceframe<<" y "<<y_sourceframe<<G4endl;
        G4ThreeVector position = G4ThreeVector(x_sourceframe, y_sourceframe, fPositionZ);
        
	if (fSourceGeometry == 1){
		x_sourceframe = fsource_size_X * (G4UniformRand() - 0.5);
		y_sourceframe = fsource_size_Y * (G4UniformRand() - 0.5);
        	position = G4ThreeVector(x_sourceframe, y_sourceframe, fPositionZ);
	}


	//For random direction in order to optimize we generate random point in a sphere, but we are interested only in the values close to phi = 180 degrees, which is why we use random()/20 -1
        theta = 2. * pi * G4UniformRand()*rad;
        G4double phi = acos(G4UniformRand()/20. -1.);
	G4double x_direction = sin(phi)*cos(theta);
	G4double y_direction = sin(phi)*sin(theta);
	G4double z_direction = cos(phi);
  	G4ThreeVector direction = G4ThreeVector(0,0,-1);

	//Polarazation of gammas
	G4double theta_polar_radians = fTheta_polar * pi * rad / 180; 
	G4double omega_polar_radians = fOmega_polar * pi * rad / 180; 
	G4double stoke_vector_e1 =  cos(theta_polar_radians)*cos(omega_polar_radians); 
	G4double stoke_vector_e2 =  sin(theta_polar_radians)*cos(omega_polar_radians); 
	G4double stoke_vector_e3 =  sin(omega_polar_radians); 
	G4ThreeVector stokes_vector = G4ThreeVector(stoke_vector_e1,stoke_vector_e2,stoke_vector_e3);

        //G4cout<<"stokes: "<<stokes_vector<<G4endl; 

	if( fPolarization_degree < G4UniformRand()){
		//G4cout<<" degree "<<fPolarization_degree<<G4endl;
		stokes_vector = G4ThreeVector(0,0,1);
	}

	if(fSourceDirectionType == 1){
		direction = G4ThreeVector(x_direction,y_direction,z_direction);
	}
       
        if(fDetector->GetDetectorType() == 2 || fDetector->GetDetectorType() == 3){
        	position = G4ThreeVector(fPositionZ, y_sourceframe/4., fDetector->GetSamplePositionZ() + z_sourceframe/4.);
	  	direction = G4ThreeVector(-1,0,0);
		G4cout<<" sample position "<<fDetector->GetSamplePositionZ()<<G4endl;
        }
        

	switch (fSourceType) {
		case 0:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->SetParticlePolarization(stokes_vector);
 			break;
		case 1:
			Z = 26;
			A = 55;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 2:
			Z = 55;
			A = 137;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 3:
			Z = 83;
			A = 207;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 4:
			Z = 38;
			A = 90;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 5:
			//Z = 95;
			//A = 241;
			//ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(59.5*keV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_direction,y_direction,z_direction));
			//fParticleGun->SetParticleDefinition(ion);
			//fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 6:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
 			break;
		case 7:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("opticalphoton"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(G4ThreeVector(fPositionX,fPositionY,fPositionZ));
			fParticleGun->SetParticleMomentumDirection(direction);
			break;

	}
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetParticleName(G4int Z, G4int A, G4double excitEnergy)
{
	fParticleName = G4IonTable::GetIonTable()->GetIonName(Z,A,excitEnergy);
}

void PrimaryGeneratorAction::SetSourcePositionX(G4double newPosition){
	fPositionX = newPosition;
}

void PrimaryGeneratorAction::SetSourcePositionY(G4double newPosition){
	fPositionY = newPosition;
}

void PrimaryGeneratorAction::SetSourcePositionZ(G4double newPosition){
	fPositionZ = newPosition;
}

void PrimaryGeneratorAction::SetSourceDiameter(G4double newDiameter){
	fDiameter = newDiameter;
}

void PrimaryGeneratorAction::SetSourceSizeX(G4double newSizeX){
	fsource_size_X = newSizeX;
}

void PrimaryGeneratorAction::SetSourceSizeY(G4double newSizeY){
	fsource_size_Y = newSizeY;
}

void PrimaryGeneratorAction::SetSourcePolarizationAngle(G4double newAngle){
	fTheta_polar = newAngle;
}

void PrimaryGeneratorAction::SetSourceCircularPolarizationAngle(G4double newAngle){
	fOmega_polar = newAngle;
}

void PrimaryGeneratorAction::SetSourcePolarizationDegree(G4double newPolarDegree){
	fPolarization_degree = newPolarDegree;
}

void PrimaryGeneratorAction::SetSourceType(G4int newType)
{
	if (newType <= 5 && newType >= 0)
	{
		fSourceType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-6)!" << G4endl;
		G4cerr << "The default option (0) is set!" << G4endl;
		fSourceType = 0;
	}
}

void PrimaryGeneratorAction::SetSourceDirectionType(G4int newType)
{

	if (newType <= 1 && newType >= 0)
	{
		fSourceDirectionType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-1)!" << G4endl;
		G4cerr << "The default option (0) is set!" << G4endl;
		fSourceType = 0;
	}
}
void PrimaryGeneratorAction::SetSourceGeometry(G4int newType)
{
	if (newType <= 1 && newType >= 0){ 
		fSourceGeometry = newType;}
	else{ 
		G4cerr<<"Possible values are 1 for rectangular and 0 for circunference"<<G4endl;
		G4cerr<<"Setting the default value to rectangular "<<G4endl;
  		fSourceGeometry = 1;
	}
}
void PrimaryGeneratorAction::SetSourceEnergy(G4double newEnergy)
{
	if (newEnergy>0)
	{
		fSourceEnergy = newEnergy;
	}
	else{
		G4cerr << "New energy is < 0." << G4endl;
		G4cerr << "The default option 10 keV is set!" << G4endl;
		fSourceEnergy = 10.*keV;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

