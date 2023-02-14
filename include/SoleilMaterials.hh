/**
 * @file PenMaterials.hh
 * @author: (modified by) Luis Manzanillas
 * @date 2020, Max Planck Institute for Physics
 */


#ifndef SoleilMaterials_H
#define SoleilMaterials_H

#include "globals.hh"


class SoleilMaterials {
public:
  SoleilMaterials();
  ~SoleilMaterials();
  
  void Construct();
  
private:
  G4double lightYieldAntracene;
    
};

#endif
