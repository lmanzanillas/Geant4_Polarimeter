// Make this appear first!
#include "G4Timer.hh"

#include "RunAction.hh"
#include "RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4Types.hh"
#include "G4Material.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "EventAction.hh"

#include <string>
#include <ctime>
#include <sys/stat.h>

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
 : G4UserRunAction(),fDetector(det),fPrimary(kin),
   fTimer(0)
{
  fTimer = new G4Timer;
  fMan = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
}

std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y_%H_%M",timeinfo);
    return std::string(buffer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  fMan = G4AnalysisManager::Instance();

  G4int sourceName = fPrimary->GetSourceType();
  G4String sourceString;

  switch (sourceName){
    case 0:
      sourceString = "gamma";
      break;
    case 1:
      sourceString = "Fe55";
      break;
    case 2:
      sourceString = "Cs-137";
      break;
    case 3:
      sourceString = "Bi-207";
      break;
    case 4:
      sourceString = "Sr-90";
      break;
    case 5:
      sourceString = "Am-241";
      break;
    case 6:
      sourceString = "optical photon";
      break;
  }

  G4cout << sourceString << G4endl;

  //G4String positionZ = G4BestUnit((fDetector->GetGeDetectorThickness()-fPrimary->GetSourcePositionZ()),"Length");
  G4String positionX ="_x_"+std::to_string((int)(1000 * fPrimary->GetSourcePositionX()))+"um";
  G4String positionY ="_y_"+std::to_string((int)(1000 *fPrimary->GetSourcePositionX()))+"um";
  G4String sourceEnergy =std::to_string((int)(1000. * fPrimary->GetSourceEnergy()))+"_keV";

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();
  //Target material
  G4String s_setup_Name =fDetector->GetSetupName();
  //n Samples
  //Thickness
  //std::stringstream stream_thickness;
  //stream_thickness << std::fixed << std::setprecision(2) <<fDetector->GetGeDetectorThickness();
  G4String s_target_thickness ="_DetSection_"+std::to_string((int)round(fDetector->GetGeDetectorWidth()))+"mm";
  G4String s_collimator_material =fDetector->GetCollimatorMaterial()->GetName();
  G4String s_collimatorThickness = "_collimator_"+s_collimator_material+"_"+std::to_string((int)fDetector->GetCollimatorThickness())+"mm";
  G4String s_setup_type = "_setup_"+std::to_string((int)fDetector->GetDetectorType());
  if(fDetector->GetDetectorType()==0 || fDetector->GetDetectorType()==2){
	s_collimatorThickness = "_no_collimator";
  }

  G4String directorName = fDetector->GetDetectorOutputDataDirectory()+"/"+sourceString+"_"+sourceEnergy
	  //+datetime()
	  +positionX 
	  +positionY 
          +s_target_thickness
	  +s_setup_type
	  +s_collimatorThickness
	  +"/";

  mkdir(directorName, 0777);
  
  fFileName = directorName+fDetector->GetDetectorName()
	  +"_"+s_setup_Name+"."
          +fDetector->GetDataType();

  fMan->SetVerboseLevel(0);
  fMan->OpenFile(fFileName);
  fMan->CreateNtuple("GeSOS","Detailed MC Info");
  fMan->CreateNtupleIColumn("EventID");
  fMan->CreateNtupleDColumn("x");
  fMan->CreateNtupleDColumn("y");
  fMan->CreateNtupleDColumn("z");
  fMan->CreateNtupleDColumn("E");
  fMan->FinishNtuple();

}

void RunAction::SetFileName(G4String fileName)
{
  fFileName = fileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
  G4cout << "End Run" << G4endl;

  fMan->Write();
  fMan->CloseFile();
  //delete fMan;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
