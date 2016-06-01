// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4StepFileReader.hh,v 2.2 1998/10/20 16:31:34 broglia Exp $
// GEANT4 tag $Name: geant4-00 $
//
#ifndef G4STEPFILEREADER_HH
#define G4STEPFILEREADER_HH

#include "globals.hh"
#include "instmgr.h"

class G4StepFileReader
{
public:
  virtual void ReadSTEPFile(G4String)=0;
  virtual void SaveSTEPFile()=0;
  virtual void UpdateSTEPFile()=0;
  virtual InstMgr GetInstanceManager()=0;
};

#endif