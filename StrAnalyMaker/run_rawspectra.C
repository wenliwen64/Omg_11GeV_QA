void run_rawspectra(){
    gSystem->Load("./StrAnalyMaker.so");
    StrAnalyMaker aMaker;
    aMaker->Init("./overview.hist.root");
    aMaker->Analyze();
}
