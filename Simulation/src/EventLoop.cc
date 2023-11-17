

#include "EventLoop.hh"

#include "G4HepEmTLData.hh"
#include "G4HepEmState.hh"

#include "PrimaryGenerator.hh"
#include "Geometry.hh"
#include "Results.hh"

#include "TrackStack.hh"
#include "SteppingLoop.hh"


#include "sys/time.h"
#include <ctime>
#include <iostream>


void EventLoop::ProcessEvents(G4HepEmTLData& theTLData, G4HepEmState& theState, PrimaryGenerator& thePrimaryGenerator, Geometry& theGeometry, Results& theResult, int numEventToSimulate, int verbosity) {
  //
  // first create the container for the tracks, i.e. the track-stack:
  // - before and at the end of a given event processing: empty
  // - at the start of a new event: all tracks of a new event are inserted
  //   (using the `PrimaryGenerator`) that is a single primary track in our case
  //   (however, should be no problem adding more than one primary tracks)
  // - during the processing of a given event:
  //     - one track is popped and tracked till the end of its history
  //     - while all generated secondary tracks (if any) are pushed to the stack
  TrackStack theTrackStack;
  //
  // report progress
  if (verbosity > 0) {
    std::cout << " --- EventLoop::ProcessEvents: starts simulation of N = " << numEventToSimulate << " events..." << std::endl;
  }
  //
  // init the event ID (counter) to zero
  int eventID = 0;
  // set the initial time stamp to meaure the event processing time
  struct timeval start;
  gettimeofday(&start, NULL);
  //
  int reportProgress = -1;
  if (verbosity > 0) {
    reportProgress = std::max(1, numEventToSimulate/10);
  }
  //
  // enter to the event loop: generate and simulate as many events as required
  while (eventID < numEventToSimulate) {
    // report progress if it was rquested
    if ( verbosity > 0 && (eventID+1) % reportProgress == 0) {
      std::cout << "      - starts processing #event = " << (eventID+1) << std::endl;
    }
    //
    // 0. Reset the track ID before each new event such that it starts from zero again.
    theTrackStack.ReSetTrackID();
    //
    // 1. Generate the primary track of this event:
    // NOTE: each event is assumed to have one primary now just for simplicity
    //       (no problem though with inserting more than one primary into the stack)
    // - the primary track is the very first track in the stack, so obtain one
    //   track reference from the stack and generate one primary into that
    G4HepEmTrack& primaryTrack = theTrackStack.Insert();
    thePrimaryGenerator.GenerateOne(primaryTrack);
    primaryTrack.SetID(theTrackStack.GetNextTrackID());
    //
    // 2. Invoke the beginning of event action (by passing the current primary track)
    BeginOfEventAction(theResult, eventID, primaryTrack);
    //
    //
    // 3. While the track-stack becomes empty:
    //   - pop-up one track (into the `HepEmTLData` primary electron/gamma track)
    //   - track this particle till the end of its history in a step-by-step way
    //     NOTE: secondaries are insterted into the track-stack after each step
    //   Processing/simulation of this event is completed when the track-stack
    //   becomes empty again
    //   NOTE: `GetTypeOfNextTrack` returns -1, 0, +1 if the next track in the
    //          stack is an e-, gamma or e+, while -999 in case of empty stack.
    int trackType = -1;
    while ( (trackType = theTrackStack.GetTypeOfNextTrack()) > -2 ) {
      G4HepEmTrack* nextTrack = nullptr;
      // depending if the next track is a gamma or e-/e+ track:
      if (trackType == 0) { // the next track is a gamma
        // - obtain the primary gamma track from the TL-data which the next track
        //   from the stack will be popped into
        G4HepEmGammaTrack* gTrack = theTLData.GetPrimaryGammaTrack();
        // - perform the before "start-tracking" procedure: reset the track
        //   properties and the random engine (throw away cached rnd number)
        gTrack->ReSet();
        theTLData.GetRNGEngine()->DiscardGauss();
        // - get the common track part of this primary track
        nextTrack = gTrack->GetTrack();
      } else { // the next track is an e- or e+
        // - obtain the primary electron track from the TL-data which the next track
        //   from the stack will be popped into
        G4HepEmElectronTrack* eTrack = theTLData.GetPrimaryElectronTrack();
        // - perform the before "start-tracking" procedure: reset the track
        //   properties and the random engine (throw away cached rnd number)
        eTrack->ReSet();
        theTLData.GetRNGEngine()->DiscardGauss();
        // - get the common track part of this primary track
        nextTrack = eTrack->GetTrack();
      }
      // - pop the next track from the stack into this
      theTrackStack.PopInto(*nextTrack);
      // - the simplified "navigation" assumes, that tracks start from inside
      //   the `calorimeter` volume. This is true for secondary (ParentID > -1)
      //   tracks by default as they are generated inside the calorimeter but
      //   not for primary tracks (ParentID = -1) generated outside of the
      //   calorimeter volume (in the vacuum, pointing to the calorimeter).
      //   Therefore, primaries need to be moved to the calorimeter boundary
      //   (as they point into the calorimeter they will be inside then).
      if (nextTrack->GetParentID() < 0) {
        double* pos = nextTrack->GetPosition();
        pos[0] = theGeometry.GetCaloStartXposition();
      }
      // - invoke the beginning of tracking action before start tracking this track
      BeginOfTrackingAction(theResult, *nextTrack);
      // - call the gamma/electron stepper to simulate the entire history of this
      //   next-track (provided now in the primary gamma/electron track member of
      //   the TL-data)
      //   NOTE: the secondaries, generated during the simulation of the history
      //         of this track, are all inserted into the track stack.
      if (trackType == 0) { // the next track is a gamma
        SteppingLoop::GammaStepper(theTLData, theState, theTrackStack, theGeometry, theResult, eventID);
      } else {              // the next track is an e- or e+
        SteppingLoop::ElectronStepper(theTLData, theState, theTrackStack, theGeometry, theResult, eventID);
      }
      // - invoke the end of tracking action when the end of its simulation history is reached
      EndOfTrackingAction(theResult, *nextTrack);
    };
    //
    // 4. Call the end of event action
    EndOfEventAction(theResult, eventID);
    //
    // increase the event ID (i.e. counter of simulated events)
    ++eventID;;
  };
  //
  // calculate and report the event processing time
  struct timeval finish;
  gettimeofday(&finish, NULL);
  const double theTime = ((double)(finish.tv_sec-start.tv_sec)*1000000 + (double)(finish.tv_usec-start.tv_usec)) / 1000000;
  if (verbosity > 0) {
    std::cout << " --- EventLoop::ProcessEvents: completed simulation within t = " << theTime << " [s]" << std::endl;
  }
}


void EventLoop::BeginOfEventAction(Results& theResult, int eventID, const G4HepEmTrack& thePrimaryTrack) {
  // reset all per-event accumulators in results, i.e. that are used to accumulate data during one event
  theResult.fPerEventRes.fEdepAbs        = 0.0;
  theResult.fPerEventRes.fEdepGap        = 0.0;

  theResult.fPerEventRes.fNumSecGamma    = 0.0;
  theResult.fPerEventRes.fNumSecElectron = 0.0;
  theResult.fPerEventRes.fNumSecPositron = 0.0;

  theResult.fPerEventRes.fNumStepsGamma  = 0.0;
  theResult.fPerEventRes.fNumStepsElPos  = 0.0;
}

void EventLoop::EndOfEventAction(Results& theResult, int eventID) {
  // propagare the data accunulated during this event to the results
  double dum = theResult.fPerEventRes.fEdepAbs;
  theResult.fEdepAbs  += dum;
  theResult.fEdepAbs2 += dum*dum;

  dum = theResult.fPerEventRes.fEdepGap;
  theResult.fEdepGap  += dum;
  theResult.fEdepGap2 += dum*dum;


  dum = theResult.fPerEventRes.fNumSecGamma;
  theResult.fNumSecGamma  += dum;
  theResult.fNumSecGamma2 += dum*dum;

  dum = theResult.fPerEventRes.fNumSecElectron;
  theResult.fNumSecElectron  += dum;
  theResult.fNumSecElectron2 += dum*dum;

  dum = theResult.fPerEventRes.fNumSecPositron;
  theResult.fNumSecPositron  += dum;
  theResult.fNumSecPositron2 += dum*dum;


  dum = theResult.fPerEventRes.fNumStepsGamma;
  theResult.fNumStepsGamma  += dum;
  theResult.fNumStepsGamma2 += dum*dum;

  dum = theResult.fPerEventRes.fNumStepsElPos;
  theResult.fNumStepsElPos  += dum;
  theResult.fNumStepsElPos2 += dum*dum;
}


void EventLoop::BeginOfTrackingAction(Results& theResult, G4HepEmTrack& theTrack) {
  // check if this track is a secondary (parent ID > -1) then its type (based on the charge)
  if (theTrack.GetParentID() > -1) {
    const int ich = theTrack.GetCharge();
    switch (ich) {
      case  0: theResult.fPerEventRes.fNumSecGamma    += 1.0;
               break;
      case -1: theResult.fPerEventRes.fNumSecElectron += 1.0;
               break;
      default: theResult.fPerEventRes.fNumSecPositron += 1.0;
               break;
    }
  }
}


void EventLoop::EndOfTrackingAction(Results& /*theResult*/, G4HepEmTrack& /*theTrack*/) {
  // we do nothing here now (but we could of course)
}
