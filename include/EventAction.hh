#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class DetectorConstruction;
class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(DetectorConstruction* det, RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    virtual void AddInfo(G4double xF, G4double yF, G4double zF, G4double depE);


  private:

    DetectorConstruction*   fDetector;
    RunAction* 	fRunAction;

    int64_t eventNumber;
    G4double xFirstPen;
    G4double yFirstPen;
    G4double zFirstPen;
    G4double depositedEnergyPENStackedSample1;
     

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
