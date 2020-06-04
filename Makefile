
ifeq ($(os),Linux)
CXX          = g++ 
else
CXX          = c++
endif

CXXFLAGS     =
CXXFLAGS     += -I$(PYTHIA8)/include
CXXFLAGS     += -I$(HEPMC2_DIR)/include

LDFLAGS      = 
LDFLAGS     += -L$(PYTHIA8)/lib -lpythia8
LDFLAGS     += -L$(HEPMC2_DIR)/lib -lHepMC

all : PythiaHepMC

PythiaHepMC : PythiaHepMC.o
	$(CXX) PythiaHepMC.o -o PythiaHepMC $(LDFLAGS)

PythiaHepMC.o : PythiaHepMC.cxx
	$(CXX) $(CXXFLAGS) -c PythiaHepMC.cxx -o PythiaHepMC.o

clean :
	rm -vf PythiaHepMC PythiaHepMC.o
