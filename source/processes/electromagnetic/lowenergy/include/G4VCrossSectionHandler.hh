//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4VCrossSectionHandler.hh,v 1.13 2003/06/16 16:59:51 gunter Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 16 Sep 2001   MGP           Created
// 26 Sep 2001   V.Ivanchenko  Hide copy constructor and assignement operator
// 18 Apr 2002   V.Ivanchenko  Move member function ValueForMaterial to public
// 21 Jan 2003   V.Ivanchenko  Cut per region
//
// -------------------------------------------------------------------

// Class description:
// Low Energy Electromagnetic Physics
// Base class for cross section manager for an electromagnetic physics process
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef G4VCROSSSECTIONHANDLER_HH
#define G4VCROSSSECTIONHANDLER_HH 1

#include "globals.hh"
#include "G4DataVector.hh"
#include <map>
#include <vector>
#include "G4MaterialCutsCouple.hh"

class G4VDataSetAlgorithm;
class G4VEMDataSet;
class G4Material;
class G4Element;

class G4VCrossSectionHandler {

public:

  G4VCrossSectionHandler();

  G4VCrossSectionHandler(G4VDataSetAlgorithm* interpolation,
			 G4double minE = 250*eV, G4double maxE = 100*GeV,
			 G4int nBins = 200,
			 G4double unitE = MeV, G4double unitData = barn,
			 G4int minZ = 1, G4int maxZ = 99);

  virtual ~G4VCrossSectionHandler();

  void Initialise(G4VDataSetAlgorithm* interpolation = 0,
		  G4double minE = 250*eV, G4double maxE = 100*GeV,
		  G4int numberOfBins = 200,
		  G4double unitE = MeV, G4double unitData = barn,
		  G4int minZ = 1, G4int maxZ = 99);

  G4int SelectRandomAtom(const G4MaterialCutsCouple* couple, G4double e) const;

  const G4Element* SelectRandomElement(const G4MaterialCutsCouple* material,
				             G4double e) const;

  G4int SelectRandomShell(G4int Z, G4double e) const;

  G4VEMDataSet* BuildMeanFreePathForMaterials(const G4DataVector* energyCuts = 0);

  G4double FindValue(G4int Z, G4double e) const;

  G4double FindValue(G4int Z, G4double e, G4int shellIndex) const;

  G4double ValueForMaterial(const G4Material* material, G4double e) const;

  void LoadData(const G4String& dataFile);

  void LoadShellData(const G4String& dataFile);

  void PrintData() const;

  void Clear();

protected:

  G4int NumberOfComponents(G4int Z) const;

  void ActiveElements();

  // Factory method
  virtual std::vector<G4VEMDataSet*>* BuildCrossSectionsForMaterials(const G4DataVector& energyVector,
								       const G4DataVector* energyCuts = 0) = 0;

  // Factory method
  virtual G4VDataSetAlgorithm* CreateInterpolation();

  const G4VDataSetAlgorithm* GetInterpolation() const { return interpolation; }


private:

  // Hide copy constructor and assignment operator
  G4VCrossSectionHandler(const G4VCrossSectionHandler&);
  G4VCrossSectionHandler & operator=(const G4VCrossSectionHandler &right);

  G4VDataSetAlgorithm* interpolation;

  G4double eMin;
  G4double eMax;
  G4int nBins;

  G4double unit1;
  G4double unit2;

  G4int zMin;
  G4int zMax;

  G4DataVector activeZ;

  std::map<G4int,G4VEMDataSet*,std::less<G4int> > dataMap;

  std::vector<G4VEMDataSet*>* crossSections;
};

#endif










