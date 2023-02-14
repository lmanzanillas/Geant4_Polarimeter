//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "SoleilMaterials.hh"
#include "G4GDMLParser.hh"
class DetectorMessenger;
class G4LogicalVolume;
class G4Material;
class SoleilMaterials;
class G4Box;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    static G4VPhysicalVolume *
    GetPhysicalVolumeByName(const G4String &name);

  public:
    virtual G4VPhysicalVolume* Construct();
    void UpdateGeometry();
    //static G4VPhysicalVolume* GetPhysicalVolumeByName(const G4String &name);
    void SetSize  (G4double);
    void SetCollimatorMaterial(G4String);
    void SetTargetMaterial(G4String);
    void SetGeContainerMaterial(G4String);
    void SetGeContainerMaterialCoating(G4String);
    void SetSampleMaterial(G4String);
    void SetOutputDirectory(G4String);
    void SetWorldMaterial(G4String);
    void SetDetectorType(G4int);
    void SetGeDetectorLength(G4double);
    void SetGeDetectorThickness(G4double);
    void SetGeDetectorWidth(G4double);
    void SetContactThickness(G4double);
    void SetSampleLength(G4double);
    void SetSampleThickness(G4double);
    void SetSampleWidth(G4double);
    void SetBeWindowRadius(G4double);
    void SetDetectorCollimatorX(G4double);
    void SetCollimatorThickness(G4double);
    void SetDistanceCollimatorDetector(G4double);
    void SetNumberOfTargetSamples(G4int);
    void SetVolName(G4ThreeVector);
    void SetDetectorName(G4String);
    void SetSetupName(G4String);
    void SetDataType(G4String);



  public:
    //const G4VPhysicalVolume* GetWorld() {return physicPenSampleBox;};
    const G4VPhysicalVolume* GetWorld() {return physicWorldBox;};
    G4int GetDetectorType(){return fDetectorType;};
    G4double GetDetectorCollimatorX(){return fDetectorCollimatorX;};
    G4double GetDetectorCollimatorY(){return fDetectorCollimatorY;};
    G4double GetSourceContainerY(){return fSourceContainerY;};
    G4String GetDetectorName(){return fDetectorName;};
    G4String GetDetectorOutputDataDirectory(){return data_output_directory;};
    G4String GetVolName(){return fVolName;};

    G4double GetCollimatorThickness()  {return CollimatorThickness;};
    G4double GetContactThickness()  {return ContactThickness;};
    G4double GetGeDetectorLength()  {return detectorInnerRadius*2.0;};
    G4double GetGeDetectorThickness()  {return detectorThickness*2.0;};
    G4double GetGeDetectorWidth()  {return detectorHeight*2.0;};
    G4double GetSampleLength()  {return sampleInnerRadius*2.0;};
    G4double GetSampleThickness()  {return sampleThickness*2.0;};
    G4double GetSampleWidth()  {return sampleHeight*2.0;};
    G4double GetBeWindowRadius()  {return beWindowRadius;};
    G4double GetSamplePositionZ()  {return zPositionSample;};
    G4double GetDistanceCollimatorDetector()  {return distanceCollimatorDetector;};
    G4int GetNumberOfTargetSamples()  {return nSamples;};
    G4String GetSetupName(){return fSetupName;};
    G4String GetDataType(){return fDataType;};
    G4Material*        GetWorldMaterial()   {return fWorldMaterial;};
    G4Material*        GetTargetMaterial()   {return fTargetMaterial;};
    G4Material*        GetCollimatorMaterial()   {return fCollimatorMaterial;};
    G4ThreeVector* fSourceVector;

    void               DefineMaterials();

  private:

    G4double halfSizeDarkBoxX;
    G4double halfSizeDarkBoxY;
    G4double halfSizeDarkBoxZ;

    G4double detectorInnerRadius;
    G4double detectorThickness;
    G4double detectorHeight;
    G4double CollimatorThickness;
    G4double ContactThickness;
    G4int nSamples;
    G4double sampleInnerRadius;
    G4double sampleThickness;
    G4double sampleHeight;
    G4double beWindowRadius;
    G4double zPositionSample;


    G4double fSiliconPlate_h;
    G4double fHolderWidth;

    G4Material* fWorldMaterial;
    G4Material* fTargetMaterial;
    G4Material* fCollimatorMaterial;
    G4Material* fSampleMaterial;
    G4String fSetupName;
    G4double halfSourceContainerThickness;
    G4double halfCollimatorThickness;
    G4double distanceCollimatorDetector;
    G4Box* fWorldBox;
    G4Tubs* geDetectorTube;
    G4Box* AlContactBox;
    G4Tubs* sampleTube;

    G4LogicalVolume* logicCollimatorLEAPSCoating;
    G4LogicalVolume* logicCollimatorLEAPS;
    G4LogicalVolume* logicCollimatorLEAPSSmall;
    G4LogicalVolume* logicCollimatorSAMBA;
    G4LogicalVolume* logicSample;
    G4LogicalVolume* geLogicVolume;
    G4LogicalVolume* logicContactVolume;
    G4LogicalVolume* logicBeWindow;
    G4LogicalVolume* logicMetalWindow;
    G4LogicalVolume* logicMetalWindowBack;
    G4LogicalVolume* logicMetalTube;
    G4LogicalVolume* logicWorldBox;
    G4LogicalVolume* logicGeContainer;
    G4LogicalVolume* logicGeContainerCoating;
    G4LogicalVolume* logicSupportPins;
    G4LogicalVolume* logicGeCapot;

    G4VSolid* solidBeWindow;
    G4VSolid* solidMetalWindow;
    G4VSolid* solidMetalWindowBack;
    G4VSolid* solidMetalTube;
    G4VSolid* solidGeContainer;
    G4VSolid* solidGeContainerCoating;
    G4VSolid* solidSupportPins;
    G4VSolid* solidGeCapotPrimitive;
    G4VSolid* solidGeCapot;
    G4VSolid* solidBoxCapotInner;
    

    G4VPhysicalVolume* physicWorldBox;
    G4VPhysicalVolume* physicPenSampleBox;
    G4VPhysicalVolume* physicSourceContainer;
    G4VPhysicalVolume* physicCollimator;



    G4Material* PVTMaterial;
    G4Material* PenMaterial;
    G4Material* materialBialkali;
    G4Material* materialSi;
    G4Material* materialBeWindow;
    G4Material* materialGe;
    G4Material* materialAir;
    G4Material* fVacuum;
    G4Material* materialTriggerFoilEJ212;
    G4Material* Pstyrene;
    G4Material* fGlass;
    G4Material* materialPMMA;
    G4Material* materialGreaseEJ550;
    G4Material* materialTeflon;
    G4Material* materialVikuiti;
    G4Material* materialPolyethylene;
    G4Material* materialTitanium;
    G4Material* materialCollimatorCoating;
    G4Material* materialContacts;
    G4Material* materialSample;
    G4Material* materialMetalTube;
    G4Material* materialSupportPins;
    G4Material* materialGeContainer;
    G4Material* materialGeContainerCoating;

    G4int fDetectorType;
    G4double fDetectorCollimatorX;
    G4double fDetectorCollimatorY;
    G4double fSourceContainerY;
    G4String fDetectorName;
    G4String fVolName;
    G4String data_output_directory;
    G4String fDataType;

    SoleilMaterials* materialConstruction;
    DetectorMessenger* fDetectorMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
