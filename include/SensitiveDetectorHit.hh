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
/// \file SensitiveDetectorHit.hh
/// \brief Definition of the SensitiveDetectorHit class

#ifndef SensitiveDetectorHit_h
#define SensitiveDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4DynamicParticle.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class SensitiveDetectorHit : public G4VHit
{
  public:
    SensitiveDetectorHit();
    SensitiveDetectorHit(const SensitiveDetectorHit&);
    virtual ~SensitiveDetectorHit();

    // operators
    const SensitiveDetectorHit& operator=(const SensitiveDetectorHit&);
    G4int operator==(const SensitiveDetectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetStepLength (G4double l) {fTrackLength = l;};

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetStepLength() const {return fTrackLength;};

  private:

      G4int         fTrackID;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4double fTrackLength; ///< Track length in the  sensitive volume
      G4DynamicParticle fdynParticle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SensitiveDetectorHit> SensitiveDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<SensitiveDetectorHit>* SensitiveDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SensitiveDetectorHit::operator new(size_t)
{
  if(!SensitiveDetectorHitAllocator)
      SensitiveDetectorHitAllocator = new G4Allocator<SensitiveDetectorHit>;
  return (void *) SensitiveDetectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SensitiveDetectorHit::operator delete(void *hit)
{
  SensitiveDetectorHitAllocator->FreeSingle((SensitiveDetectorHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
