
ifeq ($(os),Linux)
CXX          = g++ 
else
CXX          = c++
endif

CXXFLAGS     = -std=c++11
CXXFLAGS     += -I$(PYTHIA8)/include

LDFLAGS      = 
LDFLAGS     += -L$(PYTHIA8)/lib -lpythia8

all : PythiaHepMC

PythiaHepMC : PythiaHepMC.o
	$(CXX) PythiaHepMC.o -o PythiaHepMC $(LDFLAGS) -L$(HEPMC2_DIR)/lib -lHepMC -L$(HEPMC_DIR)/lib $(LDFLAGS) -lHepMC3

PythiaHepMC.o : PythiaHepMC.cxx CustomPythia8ToHepMC3.h
	$(CXX) $(CXXFLAGS) -c PythiaHepMC.cxx -o PythiaHepMC.o -I$(HEPMC2_DIR)/include -I$(HEPMC_DIR)/include -I.

clean :
	rm -vf PythiaHepMC PythiaHepMC.o PythiaHepMC3 PythiaHepMC3.o
