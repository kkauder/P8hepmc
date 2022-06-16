// Create pythia eP collisions and save to HepMC

#include "Pythia8/Pythia.h"

#include<iostream>
using std::cout;
using std::cerr;
using std::endl;

#include<string>
using std::string;

int main ()
{
  Pythia8::Pythia p8;

  // Species 
  p8.readString("Beams:idA = 2212");  // proton
  p8.readString("Beams:idB = 2212");  // proton
  p8.readString("Beams:eA  = 100");   // proton energy in GeV
  p8.readString("Beams:eB  = 100");   // proton energy in GeV
  p8.readString("Beams:frameType = 2"); // needed if eA != eB
  

  // Show initialization at INFO level
  p8.readString("Init:showProcesses = on");
  p8.readString("Init:showChangedSettings = on");
  p8.readString("Init:showMultipartonInteractions = on");
  p8.readString("Init:showChangedParticleData = on");

  p8.readString("Next:numberShowInfo = 1");
  p8.readString("Next:numberShowProcess = 1");
  p8.readString("Next:numberShowEvent = 1");

  // Standard settings
  p8.readString("HardQCD:all = on");
  p8.readString("HadronLevel:Decay = on");
  p8.readString("HadronLevel:all = on");
  p8.readString("PartonLevel:ISR = on");
  p8.readString("PartonLevel:MPI = on");

  p8.readString("PromptPhoton:all=on");
  p8.readString("WeakSingleBoson:all=off");
  p8.readString("WeakDoubleBoson:all=off");
  p8.readString("PartonLevel:FSR = on");

  p8.readString("PhaseSpace:pTHatMin = 2");
  p8.readString("PhaseSpace:pTHatMax = 50");

  // Random seed
  p8.readString("Random:setSeed = on");
  p8.readString("Random:seed = 42");
  
  // initialize
  p8.init();

  // Run
  int nevents = 10;
  for (int ev = 0; ev < nevents; ++ ev ){
    p8.next();
    // cout << "--------------------------------------------------------" << endl;
    // cout << "-- Scale = " << p8.event.scale() << endl;
    // cout << "-- ScaleSecond = " << p8.event.scaleSecond() << endl;
    // cout << "--------------------------------------------------------" << endl;
    p8.event.list();
    
  }

  p8.stat();
  // Done.
  return 0;

					  
}

