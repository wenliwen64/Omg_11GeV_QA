# =============================== 
# Makefile for StrAnalyMaker 
# =============================== 

ROOTGLIBS     = $(root-config --glibs)
ROOTCFLAGS    = $(root-config --cflags) 

CXX           = g++ 
CXXFLAGS      = -g -Wall -fPIC -pthread -stdlib=libc++ -m64 -I/usr/local/Cellar/root/5.34.25/include/root
SOFLAGS       = -shared 

#CXXFLAGS     += $(ROOTCFLAGS) 

MYCLASSES = StrAnalyMaker.o StrAnalyMakerDict.o 

# =============================== 
StrAnalyMaker: StrAnalyMaker.cc 
# ------------------------------- 
	$(CXX) $(CXXFLAGS) -c StrAnalyMaker.cc -o StrAnalyMaker.o 
	rootcint  -f StrAnalyMakerDict.cc -c StrAnalyMaker.h 
	$(CXX) $(CXXFLAGS) -c StrAnalyMakerDict.cc -o StrAnalyMakerDict.o 	
	$(CXX) $(SOFLAGS) $(MYCLASSES) -o libStrAnalyMaker.so $(ROOTGLIBS) 

# =============================== 
clean: 
	rm -f StrAnalyMaker.o StrAnalyMakerDict.o StrAnalyMakerDict.cc StrAnalyMakerDict.h libStrAnalyMaker.so 
# =============================== 
