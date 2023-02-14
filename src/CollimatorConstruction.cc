#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"

#include "CollimatorConstruction.hh"

// Class to define logical virtual volume for the light collimator

G4VSolid* CollimatorConstruction::ConstructSAMBACollimator(G4double c_thick){

  //Initial block dimensions
  halfCollimatorSizeX = 18.*mm;
  halfCollimatorSizeY = 18.*mm;
  halfCollimatorSizeZ = c_thick/2.0;

  G4double hole_half_x_size = 2.1*mm;
  G4double hole_half_y_size = 2.1*mm;
  G4double hole_half_z_size = c_thick;


  //Subtraction block 
  G4VSolid* initialBlock = new G4Box("ti_block", halfCollimatorSizeX, halfCollimatorSizeY, halfCollimatorSizeZ);

  //G4VSolid* solidSAMBAHoles = new G4SubtractionSolid("samba_holes", initialBlock, solidBoxHole, 0,  G4ThreeVector(0.,0.,0.));

  G4double pixel_size_x = 4.2*mm;
  G4double pixel_size_y = 4.2*mm;
  G4double gap = 0.8*mm;
  G4double origin_active = -(29.2/2.0)*mm;
  
  G4double pos_x, pos_y;
  G4VSolid* solidSAMBAHoles;
  G4MultiUnion* munion_solid = new G4MultiUnion("UnitedBoxes");

  //remove or make  holes in collimator
  int hole_number = 1;
  for(int i = 1; i <= 6; i++ ){
	  pos_x = origin_active + pixel_size_x/2 + (i-1)*gap + (i-1)*pixel_size_x;
	  for(int j = 1; j <= 6; j++){
  		  G4VSolid* solidBoxHole = new G4Box("hole_inner", hole_half_x_size, hole_half_y_size, hole_half_z_size);
		  pos_y = origin_active + pixel_size_y/2 + (j-1)*gap + (j-1)*pixel_size_y;
		  G4ThreeVector pos = G4ThreeVector(pos_x,pos_y,0);
  		  G4RotationMatrix rot = G4RotationMatrix(0, 0, 0);
		  G4Transform3D tr = G4Transform3D(rot, pos);
		  munion_solid-> AddNode( *solidBoxHole, tr );
		  hole_number ++;
	  }
  }
  munion_solid-> Voxelize();
  solidSAMBAHoles = new G4SubtractionSolid("solidSAMBAHoles", initialBlock, munion_solid, 0,  G4ThreeVector(0,0,0.));
  return solidSAMBAHoles;

}

G4VSolid* CollimatorConstruction::ConstructCollimator(G4double c_thick, G4double c_coating_thick){


  //Initial block dimensions
  halfCollimatorSizeX = 14.*mm;
  halfCollimatorSizeY = 14.*mm;
  halfCollimatorSizeZ = c_thick/2.0;

  G4double blinded_area = 0.9*mm;
  G4double short_side_trap = 2.95*mm;
  G4double long_side_trap = 7.10*mm;

  G4double long_side_inner_trap = 5.66*mm;
  G4double short_side_inner_trap = 2.83*mm;

  G4double collimatorHoleThickness = halfCollimatorSizeZ*2.0 + 1.0*mm; 

  G4double collimatorHoleWidth = 0.5*3.5*mm; 
  G4double collimatorHoleLongSide = 0.5*(long_side_trap - blinded_area/2.); 
  G4double collimatorHoleShortSide = 0.5*(short_side_trap - blinded_area/2.);

  G4double collimatorHoleWidthCenter = 0.5*(2.5*mm - blinded_area/2.0); 
  G4double collimatorHoleLongSideCenter = 0.5*(long_side_inner_trap - blinded_area/2.); 
  G4double collimatorHoleShortSideCenter = 0.5*(short_side_inner_trap - blinded_area/2.); 


  G4double halfCollimatorSizeXCoating = halfCollimatorSizeX - c_coating_thick;
  G4double halfCollimatorSizeYCoating = halfCollimatorSizeY - c_coating_thick;
  G4double halfCollimatorSizeZCoating = halfCollimatorSizeZ - c_coating_thick;
  G4double collimatorHoleWidthCoating = collimatorHoleWidth + c_coating_thick; 
  G4double collimatorHoleWidthCenterCoating = collimatorHoleWidthCenter + c_coating_thick; 
  G4double collimatorHoleLongSideCoating = collimatorHoleLongSide + c_coating_thick; 
  G4double collimatorHoleShortSideCoating = collimatorHoleShortSide + c_coating_thick;
  G4double collimatorHoleLongSideCenterCoating = collimatorHoleLongSideCenter + c_coating_thick; 
  G4double collimatorHoleShortSideCenterCoating = collimatorHoleShortSideCenter + c_coating_thick; 




  //Hole 1
  // Rotation matrix
  G4double phiHoles = 0.0*pi/180.0;
  G4double alphaHoles = 90.0*pi/180.0;
  // u, v, w are the daughter axes, projected on the mother frame
  G4ThreeVector u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  G4ThreeVector v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  G4ThreeVector w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole1(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform1(rotationHole1, positionHole1);

  //Hole 2
  phiHoles = 180.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole2 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole2(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform2(rotationHole2, positionHole2);


  //Hole 3
  phiHoles = 60.0*pi/180.0;
  alphaHoles = 150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole3  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole3(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform3 = G4Transform3D(rotationHole3,positionHole3);


  //Hole 4
  phiHoles = -120.0*pi/180.0;
  alphaHoles = -30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole4 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole4(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform4 = G4Transform3D(rotationHole4,positionHole4);

  //Hole 5
  phiHoles = 120.0*pi/180.0;
  alphaHoles = -150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole5  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole5(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles),0.0*mm);
  G4Transform3D transform5 = G4Transform3D(rotationHole5,positionHole5);

  //Hole 6
  phiHoles = -60.0*pi/180.0;
  alphaHoles = 30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole6 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole6(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform6 = G4Transform3D(rotationHole6,positionHole6);

  //central hexagon hole top
  phiHoles = -180.0*pi/180.0;
  alphaHoles = 90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC1(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC1 = G4Transform3D(rotationHoleC1,positionHoleC1);

  
  //central hexagon hole bottom
  phiHoles = 0.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC2  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC2(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC2 = G4Transform3D(rotationHoleC2,positionHoleC2);
 
  //Subtraction block 
  G4VSolid* initialBlockCoating = new G4Box("lg_block_coating", halfCollimatorSizeXCoating, halfCollimatorSizeYCoating, halfCollimatorSizeZCoating);

  // Solid 
  G4VSolid* solidCollimatorHoleCoating = new G4Trap("colhole_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg);

  // Solid center holes 
  G4VSolid* solidCollimatorHoleCenterCoating = new G4Trap("colholeCenter_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCenterCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCenterCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg);

  G4VSolid* tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", initialBlockCoating, solidCollimatorHoleCoating, transform1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform2);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform3);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform4);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform5);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform6);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC2);


  G4VSolid* finalCollimator = tiBlockCoating;
  return finalCollimator;

}

G4VSolid* CollimatorConstruction::ConstructCollimatorCoating(G4double c_thick, G4double c_coating_thick){


  //Initial block dimensions
  halfCollimatorSizeX = 14.*mm;
  halfCollimatorSizeY = 14.*mm;
  halfCollimatorSizeZ = c_thick/2.0;

  G4double blinded_area = 0.9*mm;
  G4double short_side_trap = 2.95*mm;
  G4double long_side_trap = 7.10*mm;

  G4double long_side_inner_trap = 5.66*mm;
  G4double short_side_inner_trap = 2.83*mm;

  G4double collimatorHoleThickness = halfCollimatorSizeZ*2.0 + 1.0*mm; 

  G4double collimatorHoleWidth = 0.5*3.5*mm; 
  G4double collimatorHoleLongSide = 0.5*(long_side_trap - blinded_area/2.); 
  G4double collimatorHoleShortSide = 0.5*(short_side_trap - blinded_area/2.);

  G4double collimatorHoleWidthCenter = 0.5*(2.5*mm - blinded_area/2.0); 
  G4double collimatorHoleLongSideCenter = 0.5*(long_side_inner_trap - blinded_area/2.); 
  G4double collimatorHoleShortSideCenter = 0.5*(short_side_inner_trap - blinded_area/2.); 


  G4double halfCollimatorSizeXCoating = halfCollimatorSizeX - c_coating_thick;
  G4double halfCollimatorSizeYCoating = halfCollimatorSizeY - c_coating_thick;
  G4double halfCollimatorSizeZCoating = halfCollimatorSizeZ - c_coating_thick;
  G4double collimatorHoleWidthCoating = collimatorHoleWidth + c_coating_thick; 
  G4double collimatorHoleWidthCenterCoating = collimatorHoleWidthCenter + c_coating_thick; 
  G4double collimatorHoleLongSideCoating = collimatorHoleLongSide + c_coating_thick; 
  G4double collimatorHoleShortSideCoating = collimatorHoleShortSide + c_coating_thick;
  G4double collimatorHoleLongSideCenterCoating = collimatorHoleLongSideCenter + c_coating_thick; 
  G4double collimatorHoleShortSideCenterCoating = collimatorHoleShortSideCenter + c_coating_thick; 


  //Hole 1
  // Rotation matrix
  G4double phiHoles = 0.0*pi/180.0;
  G4double alphaHoles = 90.0*pi/180.0;
  // u, v, w are the daughter axes, projected on the mother frame
  G4ThreeVector u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  G4ThreeVector v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  G4ThreeVector w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole1(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform1(rotationHole1, positionHole1);

  //Hole 2
  phiHoles = 180.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole2 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole2(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform2(rotationHole2, positionHole2);


  //Hole 3
  phiHoles = 60.0*pi/180.0;
  alphaHoles = 150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole3  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole3(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform3 = G4Transform3D(rotationHole3,positionHole3);


  //Hole 4
  phiHoles = -120.0*pi/180.0;
  alphaHoles = -30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole4 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole4(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform4 = G4Transform3D(rotationHole4,positionHole4);

  //Hole 5
  phiHoles = 120.0*pi/180.0;
  alphaHoles = -150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole5  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole5(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles),0.0*mm);
  G4Transform3D transform5 = G4Transform3D(rotationHole5,positionHole5);

  //Hole 6
  phiHoles = -60.0*pi/180.0;
  alphaHoles = 30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole6 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole6(4.5*mm*std::cos(alphaHoles),4.5*mm*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform6 = G4Transform3D(rotationHole6,positionHole6);

  //central hexagon hole top
  phiHoles = -180.0*pi/180.0;
  alphaHoles = 90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC1(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC1 = G4Transform3D(rotationHoleC1,positionHoleC1);

  
  //central hexagon hole bottom
  phiHoles = 0.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC2  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC2(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC2 = G4Transform3D(rotationHoleC2,positionHoleC2);
 



  // Collimator
  G4VSolid* initialBlock = new G4Box("lg_block", halfCollimatorSizeX, halfCollimatorSizeY, halfCollimatorSizeZ);

  // Solid 
  G4VSolid* solidCollimatorHole = new G4Trap("colhole",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidth,
                              collimatorHoleShortSide, collimatorHoleLongSide,
                              0.0*deg, collimatorHoleWidth,
                              collimatorHoleShortSide, collimatorHoleLongSide,
                              0.0*deg);

  // Solid center holes 
  G4VSolid* solidCollimatorHoleCenter = new G4Trap("colholeCenter",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCenter,
                              collimatorHoleShortSideCenter, collimatorHoleLongSideCenter,
                              0.0*deg, collimatorHoleWidthCenter,
                              collimatorHoleShortSideCenter, collimatorHoleLongSideCenter,
                              0.0*deg);

  G4VSolid* tiBlock = new G4SubtractionSolid("tiBlock", initialBlock, solidCollimatorHole, transform1);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHole, transform2);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHole, transform3);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHole, transform4);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHole, transform5);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHole, transform6);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHoleCenter, transformC1);
  tiBlock = new G4SubtractionSolid("tiBlock", tiBlock, solidCollimatorHoleCenter, transformC2);

   
  //Subtraction block 
  G4VSolid* initialBlockCoating = new G4Box("lg_block_coating", halfCollimatorSizeXCoating, halfCollimatorSizeYCoating, halfCollimatorSizeZCoating);

  // Solid 
  G4VSolid* solidCollimatorHoleCoating = new G4Trap("colhole_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg);

  // Solid center holes 
  G4VSolid* solidCollimatorHoleCenterCoating = new G4Trap("colholeCenter_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCenterCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCenterCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg);

  G4VSolid* tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", initialBlockCoating, solidCollimatorHoleCoating, transform1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform2);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform3);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform4);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform5);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform6);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC2);

  G4VSolid* coatingShape = new G4SubtractionSolid("collimator_coating",tiBlock,tiBlockCoating); 
  
  G4VSolid* finalCoating = coatingShape;
  return finalCoating;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////                 Small collimator   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




G4VSolid* CollimatorConstruction::ConstructSmallCollimator(G4double c_thick, G4double c_coating_thick){


  //Initial block dimensions
  halfCollimatorSizeX = 14.*mm;
  halfCollimatorSizeY = 14.*mm;
  halfCollimatorSizeZ = c_thick/2.0;


  G4double collimatorHoleThickness = halfCollimatorSizeZ*2.0 + 1.0*mm; 
  G4double collimatorHoleWidth = 0.5*1.6*mm; 
  G4double collimatorHoleLongSide = 0.5*3.1*mm; 
  G4double collimatorHoleShortSide = 0.5*1.1*mm;

  G4double collimatorHoleWidthCenter = 0.5*1.0*mm; 
  G4double collimatorHoleLongSideCenter = 0.5*2.5*mm; 

  G4double position_hole = 2.25*mm;

  G4double halfCollimatorSizeXCoating = halfCollimatorSizeX - c_coating_thick;
  G4double halfCollimatorSizeYCoating = halfCollimatorSizeY - c_coating_thick;
  G4double halfCollimatorSizeZCoating = halfCollimatorSizeZ - c_coating_thick;
  G4double collimatorHoleWidthCoating = collimatorHoleWidth + c_coating_thick; 
  G4double collimatorHoleWidthCenterCoating = collimatorHoleWidthCenter + c_coating_thick; 
  G4double collimatorHoleLongSideCoating = collimatorHoleLongSide + c_coating_thick; 
  G4double collimatorHoleShortSideCoating = collimatorHoleShortSide + c_coating_thick;
  G4double collimatorHoleLongSideCenterCoating = collimatorHoleLongSideCenter + c_coating_thick; 



  //Hole 1
  // Rotation matrix
  G4double phiHoles = 0.0*pi/180.0;
  G4double alphaHoles = 90.0*pi/180.0;
  // u, v, w are the daughter axes, projected on the mother frame
  G4ThreeVector u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  G4ThreeVector v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  G4ThreeVector w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole1(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform1(rotationHole1, positionHole1);

  //Hole 2
  phiHoles = 180.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole2 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole2(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform2(rotationHole2, positionHole2);


  //Hole 3
  phiHoles = 60.0*pi/180.0;
  alphaHoles = 150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole3  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole3(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform3 = G4Transform3D(rotationHole3,positionHole3);


  //Hole 4
  phiHoles = -120.0*pi/180.0;
  alphaHoles = -30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole4 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole4(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform4 = G4Transform3D(rotationHole4,positionHole4);

  //Hole 5
  phiHoles = 120.0*pi/180.0;
  alphaHoles = -150.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole5  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHole5(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles),0.0*mm);
  G4Transform3D transform5 = G4Transform3D(rotationHole5,positionHole5);

  //Hole 6
  phiHoles = -60.0*pi/180.0;
  alphaHoles = 30.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHole6 = G4RotationMatrix(u1,v1,w1);
  G4ThreeVector positionHole6(position_hole*std::cos(alphaHoles),position_hole*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transform6 = G4Transform3D(rotationHole6,positionHole6);

  //central hexagon hole top
  phiHoles = -180.0*pi/180.0;
  alphaHoles = 90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC1  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC1(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC1 = G4Transform3D(rotationHoleC1,positionHoleC1);

  
  //central hexagon hole bottom
  phiHoles = 0.0*pi/180.0;
  alphaHoles = -90.0*pi/180.0;
  u1 = G4ThreeVector(std::cos(phiHoles), std::sin(phiHoles), 0);
  v1 = G4ThreeVector(-std::sin(phiHoles), std::cos(phiHoles), 0);
  w1 = G4ThreeVector(0, 0, 1);
  G4RotationMatrix rotationHoleC2  = G4RotationMatrix(u1, v1, w1);
  G4ThreeVector positionHoleC2(collimatorHoleWidthCenter*std::cos(alphaHoles), collimatorHoleWidthCenter*std::sin(alphaHoles), 0.0*mm);
  G4Transform3D transformC2 = G4Transform3D(rotationHoleC2,positionHoleC2);
 
  //Subtraction block 
  G4VSolid* initialBlockCoating = new G4Box("lg_block_coating", halfCollimatorSizeXCoating, halfCollimatorSizeYCoating, halfCollimatorSizeZCoating);

  // Solid 
  G4VSolid* solidCollimatorHoleCoating = new G4Trap("colhole_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg, collimatorHoleWidthCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCoating,
                              0.0*deg);

  // Solid center holes 
  G4VSolid* solidCollimatorHoleCenterCoating = new G4Trap("colholeCenter_coating",
                              collimatorHoleThickness, 0.0*deg,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg, collimatorHoleWidthCenterCoating,
                              collimatorHoleShortSideCoating, collimatorHoleLongSideCenterCoating,
                              0.0*deg);

  G4VSolid* tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", initialBlockCoating, solidCollimatorHoleCoating, transform1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform2);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform3);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform4);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform5);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCoating, transform6);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC1);
  tiBlockCoating = new G4SubtractionSolid("tiBlockCoating", tiBlockCoating, solidCollimatorHoleCenterCoating, transformC2);

  G4VSolid* finalSmallCollimator = tiBlockCoating;
  return finalSmallCollimator;

}


