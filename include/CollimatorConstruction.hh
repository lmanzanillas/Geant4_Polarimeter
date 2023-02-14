#ifndef COLLIMATORCONSTRUCTION_H
#define COLLIMATORCONSTRUCTION_H

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"

class CollimatorConstruction
{
  public:
    G4VSolid* ConstructCollimator(G4double c_thick, G4double c_coating_thick);
    G4VSolid* ConstructCollimatorCoating(G4double c_thick, G4double c_coating_thick);
    G4VSolid* ConstructSmallCollimator(G4double c_thick, G4double c_coating_thick);
    G4VSolid* ConstructSAMBACollimator(G4double c_thick);

    G4double GetCollimatorSizeX()  {return halfCollimatorSizeX;};
    G4double GetCollimatorSizeY()  {return halfCollimatorSizeY;};
    G4double GetCollimatorSizeZ()  {return halfCollimatorSizeZ;};

  private:

    //Initial block dimensions
    G4double halfCollimatorSizeX ;
    G4double halfCollimatorSizeY ;
    G4double halfCollimatorSizeZ ;

};
#endif
