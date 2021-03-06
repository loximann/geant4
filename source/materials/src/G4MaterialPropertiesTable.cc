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
//
//
//
////////////////////////////////////////////////////////////////////////
// G4MaterialPropertiesTable Implementation
////////////////////////////////////////////////////////////////////////
//
// File: G4MaterialPropertiesTable.cc
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     2005-05-12 add SetGROUPVEL(), courtesy of
//              Horton-Smith (bug report #741), by P. Gumplinger
//              2002-11-05 add named material constants by P. Gumplinger
//              1999-11-05 Migration from G4RWTPtrHashDictionary to STL
//                         by John Allison
//              1997-03-26 by Peter Gumplinger
//              > cosmetics (only)
//
////////////////////////////////////////////////////////////////////////

#include "G4MaterialPropertiesTable.hh"

#include "globals.hh"
#include "G4Log.hh"
#include "G4OpticalMaterialProperties.hh"
#include "G4PhysicalConstants.hh"

#include <algorithm>
#include <cassert>

G4MaterialPropertiesTable::G4MaterialPropertiesTable()
{
  // elements of these 2 vectors must be in same order as
  // the corresponding enums in G4MaterialPropertiesIndex.hh
  G4MaterialPropertyName.push_back(G4String("RINDEX"));
  G4MaterialPropertyName.push_back(G4String("REFLECTIVITY"));
  G4MaterialPropertyName.push_back(G4String("REALRINDEX"));
  G4MaterialPropertyName.push_back(G4String("IMAGINARYRINDEX"));
  G4MaterialPropertyName.push_back(G4String("EFFICIENCY"));
  G4MaterialPropertyName.push_back(G4String("TRANSMITTANCE"));
  G4MaterialPropertyName.push_back(G4String("SPECULARLOBECONSTANT"));
  G4MaterialPropertyName.push_back(G4String("SPECULARSPIKECONSTANT"));
  G4MaterialPropertyName.push_back(G4String("BACKSCATTERCONSTANT"));
  G4MaterialPropertyName.push_back(G4String("GROUPVEL"));
  G4MaterialPropertyName.push_back(G4String("MIEHG"));
  G4MaterialPropertyName.push_back(G4String("RAYLEIGH"));
  G4MaterialPropertyName.push_back(G4String("WLSCOMPONENT"));
  G4MaterialPropertyName.push_back(G4String("WLSABSLENGTH"));
  G4MaterialPropertyName.push_back(G4String("WLSCOMPONENT2"));
  G4MaterialPropertyName.push_back(G4String("WLSABSLENGTH2"));
  G4MaterialPropertyName.push_back(G4String("ABSLENGTH"));
  G4MaterialPropertyName.push_back(G4String("PROTONSCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("DEUTERONSCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("TRITONSCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("ALPHASCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("IONSCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("ELECTRONSCINTILLATIONYIELD"));
  G4MaterialPropertyName.push_back(G4String("SCINTILLATIONCOMPONENT1"));
  G4MaterialPropertyName.push_back(G4String("SCINTILLATIONCOMPONENT2"));
  G4MaterialPropertyName.push_back(G4String("SCINTILLATIONCOMPONENT3"));

  assert(G4MaterialPropertyName.size() == kNumberOfPropertyIndex);

  G4MaterialConstPropertyName.push_back(G4String("SURFACEROUGHNESS"));
  G4MaterialConstPropertyName.push_back(G4String("ISOTHERMAL_COMPRESSIBILITY"));
  G4MaterialConstPropertyName.push_back(G4String("RS_SCALE_FACTOR"));
  G4MaterialConstPropertyName.push_back(G4String("WLSMEANNUMBERPHOTONS"));
  G4MaterialConstPropertyName.push_back(G4String("WLSTIMECONSTANT"));
  G4MaterialConstPropertyName.push_back(G4String("WLSMEANNUMBERPHOTONS2"));
  G4MaterialConstPropertyName.push_back(G4String("WLSTIMECONSTANT2"));
  G4MaterialConstPropertyName.push_back(G4String("MIEHG_FORWARD"));
  G4MaterialConstPropertyName.push_back(G4String("MIEHG_BACKWARD"));
  G4MaterialConstPropertyName.push_back(G4String("MIEHG_FORWARD_RATIO"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONYIELD"));
  G4MaterialConstPropertyName.push_back(G4String("RESOLUTIONSCALE"));
  G4MaterialConstPropertyName.push_back(G4String("FERMIPOT"));
  G4MaterialConstPropertyName.push_back(G4String("DIFFUSION"));
  G4MaterialConstPropertyName.push_back(G4String("SPINFLIP"));
  G4MaterialConstPropertyName.push_back(G4String("LOSS"));
  G4MaterialConstPropertyName.push_back(G4String("LOSSCS"));
  G4MaterialConstPropertyName.push_back(G4String("ABSCS"));
  G4MaterialConstPropertyName.push_back(G4String("SCATCS"));
  G4MaterialConstPropertyName.push_back(G4String("MR_NBTHETA"));
  G4MaterialConstPropertyName.push_back(G4String("MR_NBE"));
  G4MaterialConstPropertyName.push_back(G4String("MR_RRMS"));
  G4MaterialConstPropertyName.push_back(G4String("MR_CORRLEN"));
  G4MaterialConstPropertyName.push_back(G4String("MR_THETAMIN"));
  G4MaterialConstPropertyName.push_back(G4String("MR_THETAMAX"));
  G4MaterialConstPropertyName.push_back(G4String("MR_EMIN"));
  G4MaterialConstPropertyName.push_back(G4String("MR_EMAX"));
  G4MaterialConstPropertyName.push_back(G4String("MR_ANGNOTHETA"));
  G4MaterialConstPropertyName.push_back(G4String("MR_ANGNOPHI"));
  G4MaterialConstPropertyName.push_back(G4String("MR_ANGCUT"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONTIMECONSTANT1"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONTIMECONSTANT2"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONTIMECONSTANT3"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONRISETIME1"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONRISETIME2"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONRISETIME3"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(G4String("SCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(G4String("PROTONSCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(G4String("PROTONSCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(G4String("PROTONSCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(
    G4String("DEUTERONSCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(
    G4String("DEUTERONSCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(
    G4String("DEUTERONSCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(G4String("TRITONSCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(G4String("TRITONSCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(G4String("TRITONSCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(G4String("ALPHASCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(G4String("ALPHASCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(G4String("ALPHASCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(G4String("IONSCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(G4String("IONSCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(G4String("IONSCINTILLATIONYIELD3"));
  G4MaterialConstPropertyName.push_back(
    G4String("ELECTRONSCINTILLATIONYIELD1"));
  G4MaterialConstPropertyName.push_back(
    G4String("ELECTRONSCINTILLATIONYIELD2"));
  G4MaterialConstPropertyName.push_back(
    G4String("ELECTRONSCINTILLATIONYIELD3"));

  assert(G4MaterialConstPropertyName.size() == kNumberOfConstPropertyIndex);
}

G4MaterialPropertiesTable::~G4MaterialPropertiesTable()
{
  MPiterator it;
  for(it = MP.begin(); it != MP.end(); ++it)
  {
    delete(*it).second;
  }
  MP.clear();
  MCP.clear();

  G4MaterialPropertyName.clear();
  G4MaterialConstPropertyName.clear();
}

G4int G4MaterialPropertiesTable::GetConstPropertyIndex(const G4String& key,
                                                       G4bool warning) const
{
  // Returns the constant material property index corresponding to a key

  size_t index =
    std::distance(G4MaterialConstPropertyName.begin(),
                  std::find(G4MaterialConstPropertyName.begin(),
                            G4MaterialConstPropertyName.end(), key));
  if(index < G4MaterialConstPropertyName.size())
    return index;
  if(warning)
  {
    G4ExceptionDescription ed;
    ed << "Constant Material Property Index for key " << key << " not found.";
    G4Exception("G4MaterialPropertiesTable::GetConstPropertyIndex()", "mat206",
                JustWarning, ed);
  }
  return -1;
}

G4int G4MaterialPropertiesTable::GetPropertyIndex(const G4String& key,
                                                  G4bool warning) const
{
  // Returns the material property index corresponding to a key
  size_t index = std::distance(G4MaterialPropertyName.begin(),
                               std::find(G4MaterialPropertyName.begin(),
                                         G4MaterialPropertyName.end(), key));
  if(index < G4MaterialPropertyName.size())
    return index;
  if(warning)
  {
    G4ExceptionDescription ed;
    ed << "Material Property Index for key " << key << " not found.";
    G4Exception("G4MaterialPropertiesTable::GetPropertyIndex()", "mat207",
                JustWarning, ed);
  }
  return -1;
}

G4double G4MaterialPropertiesTable::GetConstProperty(const G4int index) const
{
  // Returns the constant material property corresponding to an index
  // fatal exception if property not found

  MCPiterator j;
  j = MCP.find(index);
  if(j != MCP.end())
    return j->second;
  G4ExceptionDescription ed;
  ed << "Constant Material Property Index " << index << " not found.";
  G4Exception("G4MaterialPropertiesTable::GetConstProperty()", "mat202",
              FatalException, ed);
  return 0.;
}

G4double G4MaterialPropertiesTable::GetConstProperty(const G4String& key) const
{
  // Returns the constant material property corresponding to a key
  // fatal exception if property not found

  return GetConstProperty(GetConstPropertyIndex(key));
}

G4double G4MaterialPropertiesTable::GetConstProperty(const char* key) const
{
  return GetConstProperty(G4String(key));
}

G4bool G4MaterialPropertiesTable::ConstPropertyExists(const G4int index) const
{
  // Returns true if a const property corresponding to 'index' exists

  MCPiterator j;
  j = MCP.find(index);
  if(j != MCP.end())
    return true;
  return false;
}

G4bool G4MaterialPropertiesTable::ConstPropertyExists(const G4String& key) const
{
  // Returns true if a const property 'key' exists
  return ConstPropertyExists(GetConstPropertyIndex(key));
}

G4bool G4MaterialPropertiesTable::ConstPropertyExists(const char* key) const
{
  return ConstPropertyExists(G4String(key));
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::GetProperty(
  const G4String& key, G4bool warning)
{
  // Returns a Material Property Vector corresponding to a key
  const G4int index = GetPropertyIndex(key, warning);
  return GetProperty(index);
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::GetProperty(
  const char* key, G4bool warning)
{
  return GetProperty(G4String(key), warning);
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::GetProperty(
  const G4int index, G4bool warning)
{
  // Returns a Material Property Vector corresponding to an index
  MPiterator i;
  i = MP.find(index);
  if(i != MP.end())
    return i->second;
  if(warning)
  {
    G4ExceptionDescription ed;
    ed << "Material Property for index " << index << " not found.";
    G4Exception("G4MaterialPropertiesTable::GetPropertyIndex()", "mat208",
                JustWarning, ed);
  }
  return nullptr;
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::AddProperty(
  const G4String& key, const std::vector<G4double>& photonEnergies,
  const std::vector<G4double>& propertyValues, G4bool createNewKey)
{
  if(photonEnergies.size() != propertyValues.size())
  {
    G4ExceptionDescription ed;
    ed << "AddProperty error!";
    G4Exception("G4MaterialPropertiesTable::AddProperty()", "mat210",
                FatalException, ed);
  }

  // if the key doesn't exist, add it if requested
  if(std::find(G4MaterialPropertyName.begin(), G4MaterialPropertyName.end(),
               key) == G4MaterialPropertyName.end())
  {
    if(createNewKey)
    {
      G4MaterialPropertyName.push_back(key);
    }
    else
    {
      G4ExceptionDescription ed;
      ed << "Attempting to create a new material property key " << key
         << " without setting\n"
         << "createNewKey parameter of AddProperty to true.";
      G4Exception("G4MaterialPropertiesTable::AddProperty()", "mat220",
                  FatalException, ed);
    }
  }

  G4MaterialPropertyVector* mpv =
    new G4MaterialPropertyVector(photonEnergies, propertyValues);
  G4int index = GetPropertyIndex(key);
  MP[index]   = mpv;

  // if key is RINDEX, we calculate GROUPVEL -
  // contribution from Tao Lin (IHEP, the JUNO experiment)
  if(key == "RINDEX")
  {
    CalculateGROUPVEL();
  }

  return mpv;
}

G4MaterialPropertyVector* G4MaterialPropertiesTable::AddProperty(
  const char* key, G4double* PhotonEnergies, G4double* PropertyValues,
  G4int NumEntries, G4bool createNewKey)
{
  // Provides a way of adding a property to the Material Properties
  // Table given a pair of numbers and a key
  G4String k(key);
  std::vector<G4double> energies;
  std::vector<G4double> values;
  for(G4int i = 0; i < NumEntries; ++i)
  {
    energies.push_back(PhotonEnergies[i]);
    values.push_back(PropertyValues[i]);
  }
  return AddProperty(k, energies, values, createNewKey);
}

void G4MaterialPropertiesTable::AddProperty(const G4String& key,
                                            G4MaterialPropertyVector* mpv,
                                            G4bool createNewKey)
{
  //  Provides a way of adding a property to the Material Properties
  //  Table given an G4MaterialPropertyVector Reference and a key
  // if the key doesn't exist, add it
  if(std::find(G4MaterialPropertyName.begin(), G4MaterialPropertyName.end(),
               key) == G4MaterialPropertyName.end())
  {
    if(createNewKey)
    {
      G4MaterialPropertyName.push_back(key);
    }
    else
    {
      G4ExceptionDescription ed;
      ed << "Attempting to create a new material property key " << key
         << " without setting\n"
         << "createNewKey parameter of AddProperty to true.";
      G4Exception("G4MaterialPropertiesTable::AddProperty()", "mat221",
                  FatalException, ed);
    }
  }
  G4int index = GetPropertyIndex(key);
  MP[index]   = mpv;

  // if key is RINDEX, we calculate GROUPVEL -
  // contribution from Tao Lin (IHEP, the JUNO experiment)
  if(key == "RINDEX")
  {
    CalculateGROUPVEL();
  }
}

void G4MaterialPropertiesTable::AddProperty(const char* key,
                                            G4MaterialPropertyVector* mpv,
                                            G4bool createNewKey)
{
  AddProperty(G4String(key), mpv, createNewKey);
}

void G4MaterialPropertiesTable::AddProperty(const G4String& key,
                                            const G4String& mat)
{
  G4MaterialPropertyVector* v =
    G4OpticalMaterialProperties::GetProperty(key, mat);
  AddProperty(key, v);
}

void G4MaterialPropertiesTable::AddConstProperty(const G4String& key,
                                                 G4double PropertyValue,
                                                 G4bool createNewKey)
{
  // Provides a way of adding a constant property to the Material Properties
  // Table given a key
  if(std::find(G4MaterialConstPropertyName.begin(),
               G4MaterialConstPropertyName.end(),
               key) == G4MaterialConstPropertyName.end())
  {
    if(createNewKey)
    {
      G4MaterialPropertyName.push_back(key);
    }
    else
    {
      G4ExceptionDescription ed;
      ed << "Attempting to create a new material constant property key " << key
         << " without setting"
         << " createNewKey parameter of AddProperty to true.";
      G4Exception("G4MaterialPropertiesTable::AddProperty()", "mat222",
                  FatalException, ed);
    }
  }
  G4int index = GetConstPropertyIndex(key);

  MCP[index] = PropertyValue;
}

void G4MaterialPropertiesTable::AddConstProperty(const char* key,
                                                 G4double PropertyValue,
                                                 G4bool createNewKey)
{
  // Provides a way of adding a constant property to the Material Properties
  // Table given a key
  AddConstProperty(G4String(key), PropertyValue, createNewKey);
}

void G4MaterialPropertiesTable::RemoveConstProperty(const G4String& key)
{
  G4int index = GetConstPropertyIndex(key);

  MCP.erase(index);
}

void G4MaterialPropertiesTable::RemoveConstProperty(const char* key)
{
  GetConstPropertyIndex(G4String(key));
}

void G4MaterialPropertiesTable::RemoveProperty(const G4String& key)
{
  G4int index = GetPropertyIndex(key);
  MP.erase(index);
}

void G4MaterialPropertiesTable::RemoveProperty(const char* key)
{
  RemoveProperty(G4String(key));
}

void G4MaterialPropertiesTable::AddEntry(const G4String& key,
                                         G4double aPhotonEnergy,
                                         G4double aPropertyValue)
{
  // Allows to add an entry pair directly to the Material Property Vector
  // given a key
  if(std::find(G4MaterialPropertyName.begin(), G4MaterialPropertyName.end(),
               key) == G4MaterialPropertyName.end())
  {
    G4MaterialPropertyName.push_back(key);
  }
  G4int index = GetPropertyIndex(key);

  G4MaterialPropertyVector* targetVector = MP[index];
  if(targetVector != nullptr)
  {
    targetVector->InsertValues(aPhotonEnergy, aPropertyValue);
  }
  else
  {
    G4Exception("G4MaterialPropertiesTable::AddEntry()", "mat203",
                FatalException, "Material Property Vector not found.");
  }
  if(key == "RINDEX")
  {
    CalculateGROUPVEL();
  }
}

void G4MaterialPropertiesTable::AddEntry(const char* key,
                                         G4double aPhotonEnergy,
                                         G4double aPropertyValue)
{
  AddEntry(G4String(key), aPhotonEnergy, aPropertyValue);
}

void G4MaterialPropertiesTable::DumpTable()
{
  // material properties
  MPiterator i;
  for(i = MP.begin(); i != MP.end(); ++i)
  {
    G4cout << (*i).first << ": " << G4MaterialPropertyName[(*i).first]
           << G4endl;
    if((*i).second != nullptr)
    {
      (*i).second->DumpValues();
    }
    else
    {
      G4Exception("G4MaterialPropertiesTable::DumpTable()", "mat204",
                  JustWarning, "NULL Material Property Vector Pointer.");
    }
  }
  // material constant properties
  MCPiterator j;
  for(j = MCP.begin(); j != MCP.end(); ++j)
  {
    G4cout << j->first << ": " << G4MaterialConstPropertyName[j->first]
           << G4endl;
    if(j->second != 0.)
    {
      G4cout << j->second << G4endl;
    }
    else
    {
      G4Exception("G4MaterialPropertiesTable::DumpTable()", "mat202",
                  JustWarning, "No Material Constant Property.");
    }
  }
}

#ifdef G4MULTITHREADED
#  include "G4AutoLock.hh"
namespace
{
  G4Mutex materialPropertyTableMutex = G4MUTEX_INITIALIZER;
}
#endif

G4MaterialPropertyVector* G4MaterialPropertiesTable::CalculateGROUPVEL()
{
#ifdef G4MULTITHREADED
  G4AutoLock mptm(&materialPropertyTableMutex);
#endif

  // check if "GROUPVEL" already exists. If so, remove it.
  MPiterator itr;
  itr = MP.find(kGROUPVEL);
  if(itr != MP.end())
    this->RemoveProperty("GROUPVEL");

  // fetch RINDEX data, give up if unavailable
  G4MaterialPropertyVector* rindex = this->GetProperty(kRINDEX);
  if(rindex == nullptr)
  {
    return nullptr;
  }

  // RINDEX exists but has no entries, give up
  if(rindex->GetVectorLength() == 0)
  {
    return nullptr;
  }

  // add GROUPVEL vector
  G4MaterialPropertyVector* groupvel = new G4MaterialPropertyVector();

  // fill GROUPVEL vector using RINDEX values
  // rindex built-in "iterator" was advanced to first entry above
  G4double E0 = rindex->Energy(0);
  G4double n0 = (*rindex)[0];

  if(E0 <= 0.)
  {
    G4Exception("G4MaterialPropertiesTable::CalculateGROUPVEL()", "mat205",
                FatalException, "Optical Photon Energy <= 0");
  }

  if(rindex->GetVectorLength() >= 2)
  {
    // good, we have at least two entries in RINDEX
    // get next energy/value pair

    G4double E1 = rindex->Energy(1);
    G4double n1 = (*rindex)[1];

    if(E1 <= 0.)
    {
      G4Exception("G4MaterialPropertiesTable::CalculateGROUPVEL()", "mat205",
                  FatalException, "Optical Photon Energy <= 0");
    }

    G4double vg;

    // add entry at first photon energy
    vg = c_light / (n0 + (n1 - n0) / G4Log(E1 / E0));

    // allow only for 'normal dispersion' -> dn/d(logE) > 0
    if((vg < 0) || (vg > c_light / n0))
    {
      vg = c_light / n0;
    }

    groupvel->InsertValues(E0, vg);

    // add entries at midpoints between remaining photon energies
    for(size_t i = 2; i < rindex->GetVectorLength(); ++i)
    {
      vg = c_light / (0.5 * (n0 + n1) + (n1 - n0) / G4Log(E1 / E0));

      // allow only for 'normal dispersion' -> dn/d(logE) > 0
      if((vg < 0) || (vg > c_light / (0.5 * (n0 + n1))))
      {
        vg = c_light / (0.5 * (n0 + n1));
      }
      groupvel->InsertValues(0.5 * (E0 + E1), vg);

      // get next energy/value pair, or exit loop
      E0 = E1;
      n0 = n1;
      E1 = rindex->Energy(i);
      n1 = (*rindex)[i];

      if(E1 <= 0.)
      {
        G4Exception("G4MaterialPropertiesTable::CalculateGROUPVEL()", "mat205",
                    FatalException, "Optical Photon Energy <= 0");
      }
    }

    // add entry at last photon energy
    vg = c_light / (n1 + (n1 - n0) / G4Log(E1 / E0));

    // allow only for 'normal dispersion' -> dn/d(logE) > 0
    if((vg < 0) || (vg > c_light / n1))
    {
      vg = c_light / n1;
    }
    groupvel->InsertValues(E1, vg);
  }
  else  // only one entry in RINDEX -- weird!
  {
    groupvel->InsertValues(E0, c_light / n0);
  }

  this->AddProperty("GROUPVEL", groupvel);

  return groupvel;
}

std::vector<G4String> G4MaterialPropertiesTable::GetMaterialPropertyNames()
  const
{
  return G4MaterialPropertyName;
}

std::vector<G4String> G4MaterialPropertiesTable::GetMaterialConstPropertyNames()
  const
{
  return G4MaterialConstPropertyName;
}
