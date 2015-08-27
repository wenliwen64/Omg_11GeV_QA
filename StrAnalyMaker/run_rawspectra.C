void run_rawspectra(){
    gSystem->Load("./StrAnalyMaker.so");
    StrAnalyMaker* aMaker = new StrAnalyMaker();
    //aMaker->Init("./0818_overview.reweight.histo.root");
//    aMaker->Init("./0818_overview_11GeV.reweight.histo.root", "./0826_11GeV_omg.local_analysis.root", "./0826_11GeV_omgrot.local_analysis.root");// feng's upstream files
    aMaker->Init("./0818_overview_11GeV.reweight.histo.root", "./0820_11GeV_omg.local_analysis.root", "./0825_11GeV_omgrot.local_analysis.root");// feng's upstream files
    aMaker->Analyze();
}
