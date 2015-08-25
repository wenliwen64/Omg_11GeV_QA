# =============================== 
# Makefile for StrAnalyMaker 
# =============================== 

ROOTGLIBS     = $(root-config --glibs)
ROOTCFLAGS    = $(root-config --cflags) 

CXX           = g++ 
CXXFLAGS      = -g -Wall -fPIC -pthread -m64 -I/home/lwen/Downloads/root/include
SOFLAGS       = -shared 

#CXXFLAGS     += $(ROOTCFLAGS) 

MYCLASSES = StrAnalyMaker.o StrAnalyMakerDict.o 

# =============================== 
StrAnalyMaker: StrAnalyMaker.cc 
# ------------------------------- 
	$(CXX) $(CXXFLAGS) -c StrAnalyMaker.cc -o StrAnalyMaker.o 
	rootcint  -f StrAnalyMakerDict.cc -c StrAnalyMaker.hh
	$(CXX) $(CXXFLAGS) -c StrAnalyMakerDict.cc -o StrAnalyMakerDict.o 	
	$(CXX) $(SOFLAGS) $(MYCLASSES) -o StrAnalyMaker.so $(ROOTGLIBS) 

# =============================== 
clean: 
	rm -f StrAnalyMaker.o StrAnalyMakerDict.o StrAnalyMakerDict.cc StrAnalyMakerDict.h libStrAnalyMaker.so 
# =============================== 
