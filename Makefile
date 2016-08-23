#path var + dir commands

path=$(PWD)
MKDIR_BIN = mkdir -p $(path)/bin
MKDIR_LIB = mkdir -p $(path)/lib
MKDIR_OUTTXT = mkdir -p $(path)/out/outTxt
MKDIR_OUTROOT = mkdir -p $(path)/out/outROOT

#fortran var
FC := gfortran
FFLAGS := -g -static

#c var
CXX = g++  
CXXFLAGS = -Wall -O2 -Werror -Wextra -Wno-unused-local-typedefs
GCCVERSION := $(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 6)
ifeq "$(GCCVERSION)" "1"
  CXXFLAGS += -Wno-error=misleading-indentation
endif

#root
ROOTFLAGS = `root-config --cflags --libs`

#fastjet
FASTJETFLAGS = `/Users/cfmcginn/Projects/Fastjet/fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`

CURLBOOL = 0
ifeq ("$(wildcard $(path)/src/main-q-pythia-pp.1.0.2.f)","")
	CURLBOOL := 1
else ifeq ("$(wildcard $(path)/src/main-q-pythia.1.0.2.f)","")
	CURLBOOL := 1
else ifeq ("$(wildcard $(path)/src/main-q-pythiahadrons.1.0.2.f)","")
	CURLBOOL := 1
else ifeq ("$(wildcard $(path)/src/q-pythia.1.0.2.f)","")
	CURLBOOL := 1
else ifeq ("$(wildcard $(path)/src/q-pyshow.1.0.2.f)","")
	CURLBOOL := 1
endif

#programs to make
all: mkdirBin mkdirLib mkdirOutTxt mkdirOutRoot getFiles q-pythia.1.0.2.o main-q-pythia-pp.o main-q-pythia-pp.exe main-q-pythia.o main-q-pythia.exe main-q-pythiahadrons.o main-q-pythiahadrons.exe qPytToROOT.exe

mkdirBin: 
	$(MKDIR_BIN)

mkdirLib: 
	$(MKDIR_LIB)

mkdirOutTxt: 
	$(MKDIR_OUTTXT)

mkdirOutRoot: 
	$(MKDIR_OUTROOT)

getFiles:
ifeq ($(CURLBOOL), 1)
	curl -O http://igfae.usc.es/qatmc/q-pythia/v1.0/v1.0.2/q-pythia.1.0.2.tgz
	tar -xzvf q-pythia.1.0.2.tgz
ifneq ("$(wildcard $(path)/src/main-q-pythia-pp.1.0.2.f)","")
	rm main-q-pythia-pp.1.0.2.f
else
	mv main-q-pythia-pp.1.0.2.f src/
endif
ifneq ("$(wildcard $(path)/src/main-q-pythia.1.0.2.f)","")
	rm main-q-pythia.1.0.2.f
else
	mv main-q-pythia.1.0.2.f src/
endif
ifneq ("$(wildcard $(path)/src/main-q-pythiahadrons.1.0.2.f)","")
	rm main-q-pythiahadrons.1.0.2.f
else
	mv main-q-pythiahadrons.1.0.2.f src/
endif
ifneq ("$(wildcard $(path)/src/q-pythia.1.0.2.f)","")
	rm q-pythia.1.0.2.f
else
	mv q-pythia.1.0.2.f src/
endif
ifneq ("$(wildcard $(path)/src/q-pyshow.1.0.2.f)","")
	rm q-pyshow.1.0.2.f
else
	mv q-pyshow.1.0.2.f src/
endif	

	rm q-pythia.1.0.2.tgz
endif


q-pythia.1.0.2.o: 
	$(FC) -c -o $(path)/lib/q-pythia.1.0.2.o $(path)/src/q-pythia.1.0.2.f

main-q-pythia-pp.o: $(path)/lib/q-pythia.1.0.2.o
	$(FC) -c -o $(path)/lib/main-q-pythia-pp.1.0.2.o $(path)/src/main-q-pythia-pp.1.0.2.f

main-q-pythia-pp.exe: $(path)/lib/q-pythia.1.0.2.o $(path)/lib/main-q-pythia-pp.1.0.2.o
	$(FC) -o $(path)/bin/$@ $^ 

main-q-pythia.o: $(path)/lib/q-pythia.1.0.2.o
	$(FC) -c -o $(path)/lib/main-q-pythia.1.0.2.o $(path)/src/main-q-pythia.1.0.2.f

main-q-pythia.exe: $(path)/lib/q-pythia.1.0.2.o $(path)/lib/main-q-pythia.1.0.2.o
	$(FC) -o $(path)/bin/$@ $^ 

main-q-pythiahadrons.o: $(path)/lib/q-pythia.1.0.2.o
	$(FC) -c -o $(path)/lib/main-q-pythiahadrons.1.0.2.o $(path)/src/main-q-pythiahadrons.1.0.2.f

main-q-pythiahadrons.exe: $(path)/lib/q-pythia.1.0.2.o $(path)/lib/main-q-pythiahadrons.1.0.2.o
	$(FC) -o $(path)/bin/$@ $^ 

qPytToROOT.exe: 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(FASTJETFLAGS) -I $(path) -o bin/qPytToROOT.exe src/qPytToROOT.C

clean:
	rm -f *~
	rm -f \#*.*#
	rm -f $(path)/src/#*.*#
	rm -f $(path)/src/*~
	rm -f $(path)/include/*~
	rm -f $(path)/lib/q-py*.o 
	rm -f $(path)/lib/main*.o 
	rm -f $(path)/bin/*.exe

ifneq ("$(wildcard $(path)/bin)","")
	rmdir bin
endif

ifneq ("$(wildcard $(path)/lib)","")
	rmdir lib
endif

.PHONY: all
