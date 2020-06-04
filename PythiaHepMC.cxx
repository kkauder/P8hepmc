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

  
  
// void PythiaGun::Exec()
// {
//   do{
//     next();
//     p62.clear();
    
//     // pTarr[0]=0.0; pTarr[1]=0.0;
//     // pindexarr[0]=0; pindexarr[1]=0;

//     for(int parid=0; parid<event.size(); parid++){
//       if ( parid<3 )continue;      // 0, 1, 2: total event and beams      
//       Pythia8::Particle& particle = event[parid];

//       if(!FSR_on) {
//           // only accept particles after MPI
//           if ( particle.status()!=62 ) continue;
//           // only accept gluons and quarks
//           // Also accept Gammas to put into the hadron's list
//           if ( fabs( particle.id() ) > 5 && (particle.id() !=21 && particle.id() !=22) ) continue;
          
//           // reject rare cases of very soft particles that don't have enough e to get
//           // reasonable virtuality
//           if ( particle.pT() < 1.0/sqrt(vir_factor) ) continue;
            
//             //if(particle.id()==22) cout<<"########this is a photon!######" <<endl;
//           // accept
//       } else { // FSR_on true: use Pythia vacuum shower instead of MATTER
//           if ( !particle.isFinal()) continue;
//     	  // only accept gluons and quarks
//           // Also accept Gammas to put into the hadron's list
//           if ( fabs( particle.id() ) > 5 && (particle.id() !=21 && particle.id() !=22) ) continue;
//       }    
//       p62.push_back( particle );

//     }

//     // if you want at least 2
//     if ( p62.size() < 2 ) continue;
//     //if ( p62.size() < 1 ) continue;
    
//     // Now have all candidates, sort them
//     // sort by pt
//     std::sort( p62.begin(), p62.end(), greater_than_pt() );
//     // // check...
//     // for (auto& p : p62 ) cout << p.pT() << endl;
    
//     flag62=true;

//   }while(!flag62);


//   double p[4], xLoc[4];

//   // This location should come from an initial state
//   for (int i=0;i<=3; i++) {
//     xLoc[i] = 0.0;
//   };

//   // // Roll for a starting point
//   // // See: https://stackoverflow.com/questions/15039688/random-generator-from-vector-with-probability-distribution-in-c
//   // std::random_device device;
//   // std::mt19937 engine(device()); // Seed the random number engine

  
//   if (!ini) {
//       JSWARN << "No initial state module, setting the starting location to 0. Make sure to add e.g. trento before PythiaGun.";
//   } else {
//     auto num_bin_coll = ini->GetNumOfBinaryCollisions();
//     if ( num_bin_coll.size()==0 ){
//       JSWARN << "num_of_binary_collisions is empty, setting the starting location to 0. Make sure to add e.g. trento before PythiaGun.";
//     } else {	 
//       std::discrete_distribution<> dist( begin(num_bin_coll),end(num_bin_coll) ); // Create the distribution
    
//       // Now generate values
//       auto idx = dist( *GetMt19937Generator() );
//       auto coord = ini->CoordFromIdx( idx );
//       xLoc[1] = get<0>( coord );
//       xLoc[2] = get<1>( coord );
//     }
//   }
    

//   // Loop through particles

//   // Only top two
//   //for(int np = 0; np<2; ++np){

//   // Accept them all

//   int hCounter = 0 ;
//   for(int np = 0; np<p62.size(); ++np){
//     Pythia8::Particle& particle = p62.at( np );

//     VERBOSE(7)<<"Adding particle with pid = " << particle.id()
// 	      <<" at x=" << xLoc[1]
// 	      <<", y=" << xLoc[2]
// 	      <<", z=" << xLoc[3];
    
//     VERBOSE(7) <<"Adding particle with pid = " << particle.id()
// 	       << ", pT = " << particle.pT()
// 	       << ", y = " << particle.y()
// 	       << ", phi = " << particle.phi()
// 	       << ", e = " << particle.e();

//     VERBOSE(7) <<" at x=" << xLoc[1]
// 	       <<", y=" << xLoc[2]
// 	       <<", z=" << xLoc[3];

//    // if(particle.id() !=22)
//    // {
//         if(flag_useHybridHad != 1) {
//             AddParton(make_shared<Parton>(0, particle.id(),0,particle.pT(),particle.y(),particle.phi(),particle.e(),xLoc) );
//         } else {
//     	    auto ptn = make_shared<Parton>(0,particle.id(),0,particle.pT(),particle.y(),particle.phi(),particle.e(),xLoc);
//             ptn->set_color(particle.col()); ptn->set_anti_color(particle.acol()); ptn->set_max_color(1000*(np+1));
//             AddParton(ptn);
//         }
//     //}
//     //else
//     //{
//     //          AddHadron(make_shared<Hadron>(hCounter,particle.id(),particle.status(),particle.pT(),particle.eta(),particle.phi(),particle.e(),xLoc));
//     //          hCounter++;
//     //}

//   }
  

 
//   VERBOSE(8)<<GetNHardPartons();
// }

