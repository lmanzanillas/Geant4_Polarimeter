#include "EventAction.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(DetectorConstruction* det, RunAction* runAction)
: G4UserEventAction(),fDetector(det),
fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* myEvent)
{
	depositedEnergyPENStackedSample1 = 0.;
        
        eventNumber = myEvent->GetEventID();
        xFirstPen = 0.;
        yFirstPen = 0.;
        zFirstPen = 0.;
	if (myEvent->GetEventID() % 10000 == 0)
		G4cout << "starting event no.: " << myEvent->GetEventID() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::AddInfo(G4double xF, G4double yF, G4double zF, G4double depE){
	if (depE > 1.0){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleIColumn(0, eventNumber);
		analysisManager->FillNtupleDColumn(1, xF);
		analysisManager->FillNtupleDColumn(2, yF);
		analysisManager->FillNtupleDColumn(3, zF);
		analysisManager->FillNtupleDColumn(4, depE);
		analysisManager->AddNtupleRow(0);
        }
}




void EventAction::EndOfEventAction(const G4Event* myEvent)
{
	if (myEvent->GetEventID() % 10000 == 0)
		G4cout << "finish event no.: " << myEvent->GetEventID() << G4endl;

}
