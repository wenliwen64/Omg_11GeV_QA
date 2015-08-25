# =============================== 
# Makefile for StrAnalyMaker 
# =============================== 

ROOTGLIBS     = $(root-config --glibs)
ROOTCFLAGS    = $(root-config --cflags) 

CXX           = g++ 
CXXFLAGS      = -g -Wall -fPIC -pthread -m64 -I/usr/local/include/root
SOFLAGS       = -shared 

#CXXFLAGS     += $(ROOTCFLAGS) 

MYCLASSES = StrAnalyMaker.o StrAnalyMakerDict.o 

# =============================== 
StrAnalyMaker: StrAnalyMaker.cc 
# ------------------------------- 
	$(CXX) $(CXXFLAGS) -c StrAnalyMaker.cc -o StrAnalyMaker.o 
	rootcint -f StrAnalyMakerDict.cc -c StrAnalyMaker.hh
	$(CXX) $(CXXFLAGS) -c StrAnalyMakerDict.cc -o StrAnalyMakerDict.o 	
	$(CXX) $(SOFLAGS) $(MYCLASSES) -o StrAnalyMaker.so $(ROOTGLIBS) 
	#cp StrAnalyMaker.so /usr/local/lib/root

# =============================== 
install:
	cp StrAnalyMaker.so /usr/local/lib/root
clean: 
	rm -f StrAnalyMaker.o StrAnalyMakerDict.o StrAnalyMakerDict.cc StrAnalyMakerDict.h StrAnalyMaker.so 
# =============================== 
