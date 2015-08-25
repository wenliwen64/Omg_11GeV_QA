void run_rawspectra(){
    gSystem->Load("./StrAnalyMaker.cc++");
    StrAnalyMaker* aMaker = new StrAnalyMaker();
    //aMaker->Init("./0818_overview.reweight.histo.root");
    aMaker->Init("/home/lwen/Documents/Omg_Phi_15GeV/Omega/plot_scripts/0820_overview.histo.root");
    aMaker->Analyze("/home/lwen/Documents/Omg_Phi_15GeV/Omega/plot_scripts/0628_2015_omg.local_analysis.root", "/home/lwen/Documents/Omg_Phi_15GeV/Omega/plot_scripts/0715_2015_omgrot.local_analysis.root");
}
