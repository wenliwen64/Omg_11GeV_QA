void run_rawspectra(){
    gSystem->Load("./StrAnalyMaker.so");
    StrAnalyMaker* aMaker = new StrAnalyMaker();
    //aMaker->Init("./0818_overview.reweight.histo.root");
    aMaker->Init("./0818_overview_11GeV.reweight.histo.root", "/home/lwen/Documents/Omg_Phi_15GeV/Omega/plot_scripts/0628_2015_omg.local_analysis.root", "/home/lwen/Documents/Omg_Phi_15GeV/Omega/plot_scripts/0715_2015_omgrot.local_analysis.root");
    aMaker->Analyze();
}
