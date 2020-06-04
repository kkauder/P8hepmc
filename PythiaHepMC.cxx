// Create pythia eP collisions and save to HepMC

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

int main ()
{
  Pythia8::Pythia p8;

  // Species 
  p8.readString("Beams:idA = 2212"); // proton
  p8.readString("Beams:idB = 11");   // electron
  p8.readString("Beams:eA  = 100");  // 100 GeV proton
  p8.readString("Beams:eB  = 10");   // 10 GeV electron
  p8.readString("Beams:frameType = 2");

  // Process details
  // p8.readString("WeakBosonExchange:ff2ff(t:W) = on"); //  charged-current DIS
  p8.readString("WeakBosonExchange:ff2ff(t:gmZ) = on"); // neutral-current DIS
  p8.readString("PhaseSpace:Q2Min = 100");
  p8.readString("PDF:lepton = off");
  p8.readString("TimeShower:QEDshowerByL=off");
  p8.readString("SpaceShower:pTmaxMatch=2");
  p8.readString("SpaceShower:dipoleRecoil=on");


  // ISR, hadronization, etc.
  p8.readString("HadronLevel:Decay = off");
  p8.readString("HadronLevel:all = on");
  p8.readString("PartonLevel:ISR = off");
  p8.readString("PartonLevel:MPI = off");
  p8.readString("PartonLevel:FSR = off");
  p8.readString("PromptPhoton:all=on");
  
  // Display
  p8.readString("Init:showProcesses = on");
  p8.readString("Init:showChangedSettings = on");
  p8.readString("Init:showMultipartonInteractions = on");
  p8.readString("Init:showChangedParticleData = on");

  p8.readString("Next:numberShowInfo = 1"); 
  p8.readString("Next:numberShowProcess = 1"); 
  p8.readString("Next:numberShowEvent = 1"); 

  // Random seed
  p8.readString("Random:setSeed = on");
  p8.readString("Random:seed = 42");
  

  // initialize
  p8.init();

  // HepMC
  HepMC::Pythia8ToHepMC ToHepMC;

  // Specify file where HepMC events will be stored.
  HepMC::IO_GenEvent ascii_io("hepmcout.dat", std::ios::out);


  // Run
  int nevents = 10;
  for (int ev = 0; ev < nevents; ++ ev ){
    p8.next();

    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( p8, hepmcevt );

    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;    
  }

  p8.stat();
  // Done.
  return 0;

					  
}

