#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fOneStepPrimaries(false), fEventAction(eventAction)
{

	fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
	//auto analysisManager = G4AnalysisManager::Instance();
	//G4Track* theTrack = theStep->GetTrack();
  	//G4ParticleDefinition* particleType = theTrack->GetDefinition();
	//G4cout << "Begin Stepping" << G4endl;
	fExpectedNextStatus = Undefined;

	G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
	//G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePrePoint->GetTouchable());
	//
	//G4StepStatus PreStepStatus = thePrePoint->GetStepStatus();
        

        G4double edepStep = theStep->GetTotalEnergyDeposit()/eV; 

        //double PosDirz = step->GetTrack()->GetPosition().x();
        //step->GetPreStepPoint()->GetPosition()
	//
	if ( thePrePV->GetName()=="target_1"){
                fEventAction->AddInfo(thePrePoint->GetPosition().x(),thePrePoint->GetPosition().y(),thePrePoint->GetPosition().z(),edepStep);
  	}

	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();


  	if(!thePostPV)
  	{//out of world
  		fExpectedNextStatus=Undefined;
  		return;
  	}

}
