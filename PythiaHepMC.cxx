// Create pythia eP collisions and save to HepMC

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/Print.h"

#include "CustomPythia8ToHepMC3.h"
// #include "Pythia8ToHepMC3.h"


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
  p8.readString("Beams:idB = 11");    // electron
  p8.readString("Beams:eA  = 250");   // proton energy in GeV
  p8.readString("Beams:eB  = 20");    // electron energy in GeV
  p8.readString("Beams:frameType = 2");

  string basename = "nc";
  // string basename = "dis";
  
  p8.readString("Main:timesAllowErrors = 10000"); // allow more errors, eP is brittle
  // p8.readString("WeakBosonExchange:ff2ff(t:W) = on"); //  charged-current DIS
  if ( basename=="nc" ){
    p8.readString("WeakBosonExchange:ff2ff(t:gmZ) = on"); // neutral-current DIS
  }
  if ( basename=="dis" ){
    p8.readString("PDF:lepton2gamma = on"); // DIS
    // // requires additional libraries
    // PDFPtr photonFlux = make_shared<Lepton2gamma2>(-11);
    // p8.setPhotonFluxPtr(photonFlux, 0);
  }
  p8.readString("PhaseSpace:Q2Min = 25");
  p8.readString("SpaceShower:dipoleRecoil=on");
  p8.readString("SpaceShower:pTmaxMatch=2");
  p8.readString("PDF:lepton = off");
  p8.readString("TimeShower:QEDshowerByL=off");
    
  // More Process details
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
  p8.readString("Next:numberShowEvent = 5"); 

  // Random seed
  p8.readString("Random:setSeed = on");
  p8.readString("Random:seed = 42");
  
  // initialize
  p8.init();

  // HepMC 2 
  HepMC::Pythia8ToHepMC ToHepMC2;
  HepMC::IO_GenEvent ascii_io( basename + "_hepmc2.dat", std::ios::out);

    // HepMC 3
  HepMC3::Pythia8ToHepMC3 ToHepMC3;

  std::shared_ptr<HepMC3::GenRunInfo> run = std::make_shared<HepMC3::GenRunInfo>();
  struct HepMC3::GenRunInfo::ToolInfo generator={
    std::string("Pythia8"),
    std::to_string(PYTHIA_VERSION).substr(0,5),
    std::string("Used generator")
  };
  run->tools().push_back(generator);
  // Can be used to save the name of the control card, if
  // pythia.readFile( ...) is used
  // struct HepMC3::GenRunInfo::ToolInfo config={std::string(argv[1]),"1.0",std::string("Control cards")};
  // run->tools().push_back(config);
  std::vector<std::string> names;
  for (int iWeight=0; iWeight < p8.info.nWeights(); ++iWeight) {
    std::string s=p8.info.weightLabel(iWeight);
    if (!s.length()) s=std::to_string((long long int)iWeight);
    names.push_back(s);
  }
  if (!names.size()) names.push_back("default");
  run->set_weight_names(names);
  HepMC3::WriterAscii file(basename + "_hepmc3.dat",run);

  // Run
  int nevents = 100;
  for (int ev = 0; ev < nevents; ++ ev ){
    p8.next();
    // cout << "--------------------------------------------------------" << endl;
    // cout << "-- Scale = " << p8.event.scale() << endl;
    // cout << "-- ScaleSecond = " << p8.event.scaleSecond() << endl;
    // cout << "--------------------------------------------------------" << endl;
    // p8.event.list();
    
    // Construct new empty HepMC2 event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    HepMC::GenEvent* hepmc2evt = new HepMC::GenEvent();
    ToHepMC2.fill_next_event( p8, hepmc2evt );
    // write it out
    ascii_io << hepmc2evt;
    delete hepmc2evt;    

    // Construct new empty HepMC3 event and fill it.
    HepMC3::GenEvent hepmc3evt( HepMC3::Units::GEV, HepMC3::Units::MM );
    ToHepMC3.fill_next_event(p8.event, &hepmc3evt, -1, &p8.info);
    // write it out
    file.write_event(hepmc3evt);
    
    if( ev==0 ) {
      std::cout << "First event: " << std::endl;
      HepMC3::Print::listing(hepmc3evt);
    }
	
  }

  file.close();

  p8.stat();
  // Done.
  return 0;

					  
}

