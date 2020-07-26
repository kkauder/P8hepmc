// Create pythia eP collisions and save to HepMC

#include "Pythia8/Pythia.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/Print.h"

#include "Pythia8/Pythia.h"
#include "CustomPythia8ToHepMC3.h"
// #include "Pythia8ToHepMC3.h"

#include <iostream>
using namespace HepMC3;


#include<iostream>
using std::cout;
using std::cerr;
using std::endl;

int main ()
{
  Pythia8::Pythia p8;

  // Species 
  p8.readString("Beams:idA = 2212"); // proton
  p8.readString("Beams:idB = 11");   // electron
  p8.readString("Beams:eA  = 920");  // 100 GeV proton
  p8.readString("Beams:eB  = 27l5");   // 10 GeV electron
  p8.readString("Beams:frameType = 2");

  p8.readString("Main:timesAllowErrors = 10000"); // allow more errors, eP is brittle
  // p8.readString("WeakBosonExchange:ff2ff(t:gmZ) = on"); // neutral-current DIS
  p8.readString("PDF:lepton2gamma = on");
  p8.readString("PhaseSpace:Q2Min = 25");
  p8.readString("SpaceShower:dipoleRecoil=on");
  p8.readString("SpaceShower:pTmaxMatch=2");
  p8.readString("PDF:lepton = off");
  p8.readString("TimeShower:QEDshowerByL=off");
    
  // // Process details
  // // p8.readString("WeakBosonExchange:ff2ff(t:W) = on"); //  charged-current DIS
  // p8.readString("WeakBosonExchange:ff2ff(t:gmZ) = on"); // neutral-current DIS
  // p8.readString("PDF:lepton = off");
  // p8.readString("TimeShower:QEDshowerByL=off");
  // p8.readString("SpaceShower:pTmaxMatch=2");
  // p8.readString("SpaceShower:dipoleRecoil=on");

  // p8.readString("PDF:lepton2gammaSet = 2");
  // PDFPtr photonFlux = make_shared<Lepton2gamma2>(-11);
  // p8.setPhotonFluxPtr(photonFlux, 0);

  p8.readString("TimeShower:QEDshowerByL=off");
  // p8.readString("SpaceShower:pTmaxMatch=2");
  // p8.readString("SpaceShower:dipoleRecoil=on");

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
  Pythia8ToHepMC3 ToHepMC;

  std::shared_ptr<GenRunInfo> run = std::make_shared<GenRunInfo>();
  struct GenRunInfo::ToolInfo generator={std::string("Pythia8"),std::to_string(PYTHIA_VERSION).substr(0,5),std::string("Used generator")};
  run->tools().push_back(generator);
  // struct GenRunInfo::ToolInfo config={std::string(argv[1]),"1.0",std::string("Control cards")};
  // run->tools().push_back(config);
  std::vector<std::string> names;
  for (int iWeight=0; iWeight < p8.info.nWeights(); ++iWeight) {
    std::string s=p8.info.weightLabel(iWeight);
    if (!s.length()) s=std::to_string((long long int)iWeight);
    names.push_back(s);
  }
  if (!names.size()) names.push_back("default");
  run->set_weight_names(names);
  WriterAscii file("dis_hepmc3.out",run);
  
  // Run
  int nevents = 10;
  for (int ev = 0; ev < nevents; ++ ev ){
    p8.next();
    // cout << "--------------------------------------------------------" << endl;
    // cout << "-- Scale = " << p8.event.scale() << endl;
    // cout << "-- ScaleSecond = " << p8.event.scaleSecond() << endl;
    // cout << "--------------------------------------------------------" << endl;
    // p8.event.list();
    
    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    GenEvent hepmc( Units::GEV, Units::MM );
    
    ToHepMC.fill_next_event(p8.event, &hepmc, -1, &p8.info);
    
    if( ev==0 ) {
      std::cout << "First event: " << std::endl;
      Print::listing(hepmc);
    }
    
    file.write_event(hepmc);
  }
  
  file.close();

  p8.stat();
  // Done.
  return 0;

					  
}

