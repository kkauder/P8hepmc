
ifeq ($(os),Linux)
CXX          = g++ 
else
CXX          = c++
endif

CXXFLAGS     = -std=c++11
CXXFLAGS     += -I$(PYTHIA8)/include
CXXFLAGS     += -g

LDFLAGS      = 
LDFLAGS     += -L$(PYTHIA8)/lib -lpythia8

all : PythiaHepMC

PythiaHepMC : PythiaHepMC.o
	$(CXX) PythiaHepMC.o -o PythiaHepMC $(LDFLAGS) 

PythiaHepMC.o : PythiaHepMC.cxx
	$(CXX) $(CXXFLAGS) -c PythiaHepMC.cxx -o PythiaHepMC.o -I.

clean :
	rm -vf PythiaHepMC PythiaHepMC.o 
