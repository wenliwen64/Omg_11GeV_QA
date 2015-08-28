# =============================== 
# Makefile for StrAnalyMaker 
# =============================== 
ROOTGLIBS     = `root-config --glibs`
ROOTCFLAGS    = `root-config --cflags` 

CXX           = g++ 
CXXFLAGS      = -g -Wall -fPIC
#CXXFLAGS      = -g -Wall -fPIC -pthread -m64 -I/home/lwen/Downloads/root/include# ->home
#CXXFLAGS      = -I/usr/local/include/root ->office
SOFLAGS       = -shared 

CXXFLAGS     += $(ROOTCFLAGS)

MYCLASSES = StrAnalyMaker.o StrAnalyMakerDict.o 

# =============================== 
StrAnalyMaker: StrAnalyMaker.cc 
# ------------------------------- 
	echo $(ROOTGLIBS)
	echo $(ROOTCFLAGS)
	$(CXX) $(CXXFLAGS) -c StrAnalyMaker.cc -o StrAnalyMaker.o 
	rootcint -f StrAnalyMakerDict.cc -c StrAnalyMaker.hh Linkdef.h
	$(CXX) $(CXXFLAGS) -c StrAnalyMakerDict.cc -o StrAnalyMakerDict.o 	
	$(CXX) $(SOFLAGS) $(MYCLASSES) $(ROOTGLIBS) -o StrAnalyMaker.so  
	#cp StrAnalyMaker.so /usr/local/lib/root

# =============================== 
install:
	cp StrAnalyMaker.so /usr/local/lib/root
clean: 
	rm -f StrAnalyMaker.o StrAnalyMakerDict.o StrAnalyMakerDict.cc StrAnalyMakerDict.h StrAnalyMaker.so StrAnalyMaker_cc.so StrAnalyMaker_cc.d 
# =============================== 
