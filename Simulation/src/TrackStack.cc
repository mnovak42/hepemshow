
#include "TrackStack.hh"

#include "G4HepEmTrack.hh"

TrackStack::TrackStack()
: fSize(16),
  fCurIndx(-1),
  fCurrentTrackID(0) {
  fTrackVect.resize(fSize);
}


int TrackStack::PopInto(G4HepEmTrack& track) {
  // return -1 if the secondary stack is empty
  if (fCurIndx<0) {
    return -1;
  }
  // compy the next avaiable seconday track to the primary
  Copy(fTrackVect[fCurIndx], track);
  // return with the currently used secondary index and decrease
  return fCurIndx--;
}


int TrackStack::GetTypeOfNextTrack() {
  if (fCurIndx<0) {
    return -999;
  }
  return fTrackVect[fCurIndx].GetCharge();
}


G4HepEmTrack& TrackStack::Insert() {
  // make sure that the size if fine
  ++fCurIndx;
  if (fCurIndx==fSize) {
    fSize *= 2;
    fTrackVect.resize(fSize);
  }
  // retrun a eference to the next avaiable secondary track
  fTrackVect[fCurIndx].ReSet();
  return fTrackVect[fCurIndx];
}


void TrackStack::Copy(G4HepEmTrack& from, G4HepEmTrack& to) {
  to.ReSet();
  to.SetPosition(from.GetPosition());
  to.SetDirection(from.GetDirection());
  to.SetEKin(from.GetEKin(), from.GetLogEKin());
  to.SetCharge(from.GetCharge());
  to.SetSafety(from.GetSafety());
  to.SetID(from.GetID());
  to.SetParentID(from.GetParentID());
  to.SetMCIndex(from.GetMCIndex());
  to.SetOnBoundary(from.GetOnBoundary());
}
