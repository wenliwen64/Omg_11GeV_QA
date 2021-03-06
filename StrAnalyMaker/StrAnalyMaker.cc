#include "StrAnalyMaker.hh"
#include <TLegend.h>
#include <TH1F.h>
#include <TLine.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <fstream>

ClassImp(StrAnalyMaker)
StrAnalyMaker::StrAnalyMaker():pdgmass_xi(1.67245), mKCentBin(2), mKPtBin(6){
    mXRawSpectra[0] = 0.95;
    mXRawSpectra[1] = 1.40;
    mXRawSpectra[2] = 1.80;
    mXRawSpectra[3] = 2.20;
    mXRawSpectra[4] = 2.60;
    mXRawSpectra[5] = 3.20;

    mXRawSpectraError[0] = 0;
    mXRawSpectraError[1] = 0;
    mXRawSpectraError[2] = 0;
    mXRawSpectraError[3] = 0;
    mXRawSpectraError[4] = 0;
    mXRawSpectraError[5] = 0;

    mDptSpectra[0] = 0.5;
    mDptSpectra[1] = 0.4;
    mDptSpectra[2] = 0.4;
    mDptSpectra[3] = 0.4;
    mDptSpectra[4] = 0.4;
    mDptSpectra[5] = 0.6;
    std::cout << "StrAnalyMaker Constructor v0.01 2015-08-24 " << std::endl;
}

StrAnalyMaker::~StrAnalyMaker(){}

void StrAnalyMaker::Init(std::string overview_filename, std::string dat_filename, std::string rotbg_filename){
    std::cout << "!!! InitializationII for 11GeV Analysis" << std::endl;

    // Initialize TFile pointers 
    mOverviewFile = new TFile(overview_filename.c_str(), "read");
    mDatFile = new TFile(dat_filename.c_str(), "read");
    mRotBgFile = new TFile(rotbg_filename.c_str(), "read");

    TH1F* h_centbin9_unweighted = (TH1F*)mOverviewFile->Get("h_centbin9_after0");
    TH1F* h_centbin9_weighted = (TH1F*)mOverviewFile->Get("h_centbin9_after0");
    //TH1F* h_centbin9_weighted = (TH1F*)mOverviewFile->Get("h_centbin9_after");
     
    mNEventsUnweighted[0] = h_centbin9_unweighted->GetBinContent(8) + h_centbin9_unweighted->GetBinContent(7) + h_centbin9_unweighted->GetBinContent(6) + h_centbin9_unweighted->GetBinContent(5) + h_centbin9_unweighted->GetBinContent(4);
    mNEventsWeighted[0] = h_centbin9_weighted->GetBinContent(8) + h_centbin9_weighted->GetBinContent(7) + h_centbin9_weighted->GetBinContent(6) + h_centbin9_weighted->GetBinContent(5) + h_centbin9_weighted->GetBinContent(4);
    mNEventsUnweighted[1] = h_centbin9_unweighted->GetBinContent(10) + h_centbin9_unweighted->GetBinContent(9);
    mNEventsWeighted[1] = h_centbin9_weighted->GetBinContent(10) + h_centbin9_weighted->GetBinContent(9);

    for(int i = 0; i < mKCentBin; i++){
        std::cout << mNEventsUnweighted[i] << "cent" << i << " nevents unweighted!" << std::endl;
        std::cout << mNEventsWeighted[i] << "cent" << i << " nevents weighted!" << std::endl;
    }

    // Initialize branching ratio
    mBr = 0.678*0.639;

    // Initialize signal counting range
    mSigRangeLeft = 1.66;
    mSigRangeRight = 1.685;

    rotBgAnalysisInit();
}

void StrAnalyMaker::rotBgAnalysisInit(){
    std::cout << "!!! Analyze Rotational Background Initialization" << std::endl;
    // Initialize normalization range
    mRotNormLeftLowB = pdgmass_xi - 0.05;
    mRotNormLeftHighB = pdgmass_xi - 0.015;

    mRotNormRightLowB = pdgmass_xi + 0.015;
    mRotNormRightHighB = pdgmass_xi + 0.05;
}

Double_t StrAnalyMaker::compRotNormFactor(Int_t centbin, Int_t ptbin,  TH1F* hdat, TH1F* hrot){
    std::cout << "!!! Compute Rot Norm Factor!" << std::endl;

    Int_t ratio_l1 = hrot->FindBin(mRotNormLeftLowB);
    Int_t ratio_l2 = hrot->FindBin(mRotNormRightLowB);
    Int_t ratio_u1 = hrot->FindBin(mRotNormLeftHighB);
    Int_t ratio_u2 = hrot->FindBin(mRotNormRightHighB);
 
    //mRotScale_ratio[centbin][ptbin] = (hrot->Integral(ratio_l1, ratio_u1) + hrot->Integral(ratio_l2, ratio_u2)) / (hdat->Integral(ratio_l1, ratio_u1) + hdat->Integral(ratio_l2, ratio_u2));    
    mRotScale_ratio[centbin][ptbin] = 1.;
    std::cout << "------Norm Factor For cent" << centbin << "pt" << ptbin << "is " << mRotScale_ratio[centbin][ptbin] << std::endl;
    return mRotScale_ratio[centbin][ptbin];
}

void StrAnalyMaker::plotRotInvMassWithData(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot, Double_t scale){
    
    std::cout << "!!! Plot Inv Mass" << std::endl;
    hdat->SetMarkerStyle(8);
    hdat->Draw("Hist");
    hdat->GetXaxis()->SetTitle("InvMass(GeV)");
    hdat->GetYaxis()->SetTitle("Counts");

    TH1F* hrot_copy = (TH1F*)hrot->Clone();
    hrot_copy->Sumw2();
    hrot_copy->Scale(1./scale);
    hrot_copy->SetLineColor(2);
    hrot_copy->SetFillColor(2);
    hrot_copy->SetFillStyle(3354);
    gPad->SetTicks(1, 1);
    hrot_copy->Draw("Hist sames");

    TLine* lline = new TLine(mSigRangeLeft, 0, mSigRangeLeft, hdat->GetMaximum());
    TLine* uline  = new TLine(mSigRangeRight, 0, mSigRangeRight, hdat->GetMaximum());
    lline->SetLineColor(4);
    lline->SetLineWidth(2);
    lline->SetLineStyle(10);
    uline->SetLineColor(4);
    uline->SetLineWidth(2);
    uline->SetLineStyle(10);
    lline->Draw("sames");
    uline->Draw("sames");

    TLine* l1line = new TLine(mRotNormLeftLowB, 0, mRotNormLeftLowB, hdat->GetMaximum());
    TLine* u1line = new TLine(mRotNormLeftHighB, 0, mRotNormLeftHighB, hdat->GetMaximum());
    l1line->SetLineColor(6);
    l1line->SetLineWidth(2);
    l1line->SetLineStyle(10);
    u1line->SetLineColor(6);
    u1line->SetLineWidth(2);
    u1line->SetLineStyle(10);
    l1line->Draw("sames");
    u1line->Draw("sames");

    TLine* l2line = new TLine(mRotNormRightLowB, 0, mRotNormRightLowB, hdat->GetMaximum());
    TLine* u2line = new TLine(mRotNormRightHighB, 0, mRotNormRightHighB, hdat->GetMaximum());
    l2line->SetLineColor(6);
    l2line->SetLineWidth(2);
    l2line->SetLineStyle(10);
    u2line->SetLineColor(6);
    u2line->SetLineWidth(2);
    u2line->SetLineStyle(10);
    l2line->Draw("sames");
    u2line->Draw("sames"); 
    
    char plot_name[50];
    sprintf(plot_name, "../omg_plots/rot_xipt%dcent%d.pdf", ptbin+1, centbin);
    gPad->SaveAs(plot_name);
}

void StrAnalyMaker::compRawSigCounts(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot, Double_t scale){
    TH1F* hrot_copy = (TH1F*)hrot->Clone();
    Int_t sigRangeLeftBin = hdat->FindBin(mSigRangeLeft);
    Int_t sigRangeRightBin = hdat->FindBin(mSigRangeRight);
    Double_t datCounts = hdat->Integral(sigRangeLeftBin, sigRangeRightBin);
    Double_t rotCounts = hrot_copy->Integral(sigRangeLeftBin, sigRangeRightBin)/scale;
    mRawSigCounts[centbin][ptbin] = datCounts - rotCounts;
    mRawSigCountsError[centbin][ptbin] = sqrt(datCounts + rotCounts);
    std::cout << "mRawSigCounts for cent " << centbin << "pt" << ptbin << "is " << mRawSigCounts[centbin][ptbin] << std::endl;
}

void StrAnalyMaker::compRawSpectra(){
    double PI = 3.1415926; 

    for(int i = 0; i < mKCentBin; i++){
        for(int j = 0; j < mKPtBin; j++){
            mYRawSpectra[i][j] = 1/(2*PI) * mRawSigCounts[i][j] / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i] / mBr;
            mYRawSpectraError[i][j] = 1/(2*PI) * mRawSigCountsError[i][j] / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i] / mBr;
            std::cout << "mYRawSpectra for cent" << i << "pt" << j <<" is " << mYRawSpectra[i][j] << "mYRawSpectraError is " << mYRawSpectraError[i][j] << std::endl;
	}
    }
}

void StrAnalyMaker::plotRawSpectra(){ // TODO:
    TCanvas* rawspectra_can = new TCanvas("rawspectra_can", "rawspectra_can");
    rawspectra_can->SetLogy();
    rawspectra_can->SetTicks(1, 1);

    TGraphErrors* GRawSpectra_1060 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[0], mXRawSpectraError, mYRawSpectraError[0]); 
    GRawSpectra_1060->SetMarkerSize(1.0);
    GRawSpectra_1060->SetMarkerStyle(34);
    GRawSpectra_1060->SetMarkerColor(2);
    GRawSpectra_1060->SetMaximum(10E-1);
    GRawSpectra_1060->SetMinimum(10E-9);
    GRawSpectra_1060->GetXaxis()->SetLimits(0.50, 3.60);
    GRawSpectra_1060->SetTitle("#Omega^{-} Spectra, Au+Au 11GeV");
    GRawSpectra_1060->GetYaxis()->SetTitle("#frac{d^{2}N}{2#piNP_{T}dP_{T}dy}(GeV/c)^{-2}");
    GRawSpectra_1060->GetXaxis()->SetTitle("P_{T}(GeV/c)");
    GRawSpectra_1060->GetYaxis()->SetTitleOffset(1.3);
    GRawSpectra_1060->Draw("AP");

    TGraphErrors* GRawSpectra_010 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[1], mXRawSpectraError, mYRawSpectraError[1]); 
    GRawSpectra_010->SetMarkerSize(1.0);
    GRawSpectra_010->SetMarkerStyle(34);
    GRawSpectra_010->SetMarkerColor(1);
    GRawSpectra_010->Draw("P same");

    TLegend* leg = new TLegend(0.65, 0.65, 0.85, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(GRawSpectra_1060, "10-60%", "p");
    leg->AddEntry(GRawSpectra_010, "0-10%", "p");
    leg->Draw("sames");

    rawspectra_can->SaveAs("../omg_plots/omg_rawspectra.pdf");  
}

void StrAnalyMaker::plotComparison(){
    Double_t fengXRawSpectra[6] = {1.0, 1.4, 1.8, 2.2, 2.6, 3.2}; 
    Double_t fengYRawSpectra[2][6] = {{1.52978e-05, 2.2308e-05, 1.64271e-05, 7.22805e-06, 2.24535e-06, 5.6901e-07}, {2.8587e-05, 5.06169e-05, 5.6667e-05, 2.77574e-05, 1.01175e-05, 1.38406e-06}};
    Double_t fengYRawSpectraError[2][6] = {{2.09142e-06, 2.12988e-06, 1.49347e-06, 8.39072e-07, 4.29054e-07, 1.21412e-07}, {1.17364e-05, 1.18276e-05, 8.56325e-06, 4.94291e-06, 2.50449e-06, 5.36288e-07}};
    TGraphErrors* GRawSpectra_1060 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[0], mXRawSpectraError, mYRawSpectraError[0]); 
    GRawSpectra_1060->SetMarkerSize(2.0);
    GRawSpectra_1060->SetMarkerStyle(34);
    GRawSpectra_1060->SetMarkerColor(2);
    GRawSpectra_1060->SetMaximum(10E-1);
    GRawSpectra_1060->SetMinimum(1.5E-10);
    GRawSpectra_1060->GetXaxis()->SetLimits(0.50, 3.60);
    GRawSpectra_1060->SetTitle("#Omega^{-} Spectra, Au+Au 11GeV");
    GRawSpectra_1060->GetYaxis()->SetTitle("#frac{d^{2}N}{2#piNP_{T}dP_{T}dy}(GeV/c)^{-2}");
    GRawSpectra_1060->GetXaxis()->SetTitle("P_{T}(GeV/c)");
    GRawSpectra_1060->GetYaxis()->SetTitleOffset(1.3);

    TGraphErrors* GRawSpectra_010 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[1], mXRawSpectraError, mYRawSpectraError[1]); 
    GRawSpectra_010->SetMarkerSize(2.0);
    GRawSpectra_010->SetMarkerStyle(34);
    GRawSpectra_010->SetMarkerColor(1);

    TGraphErrors* fengG_1060 = new TGraphErrors(6, fengXRawSpectra, fengYRawSpectra[0], 0, fengYRawSpectraError[0]);
    fengG_1060->SetMarkerSize(2.0);
    fengG_1060->SetMarkerStyle(28);
    fengG_1060->SetMarkerColor(2);

    TGraphErrors* fengG_010 = new TGraphErrors(6, fengXRawSpectra, fengYRawSpectra[1], 0, fengYRawSpectraError[1]);
    fengG_010->SetMarkerSize(2.0);
    fengG_010->SetMarkerStyle(28);
    fengG_010->SetMarkerColor(1);

    TCanvas* can_compare = new TCanvas("cc", "cc", 600, 700);
    //TCanvas* can_compare = new TCanvas();
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetBottomMargin(0);
    pad1->SetTicks(1, 1);
    pad1->SetLogy();
    pad1->Draw();
    pad1->cd();
    GRawSpectra_1060->Draw("AP same");
    GRawSpectra_010->Draw("P same");
    fengG_1060->Draw("P same");
    fengG_010->Draw("P same");

    TLegend* leg = new TLegend(0.65, 0.65, 0.85, 0.85);
    leg->SetBorderSize(0);
    leg->AddEntry(GRawSpectra_1060, "10-60%% Liwen", "p");
    leg->AddEntry(GRawSpectra_010, "0-10%% Liwen", "p");
    leg->AddEntry(fengG_1060, "10-10%% Feng", "p");
    leg->AddEntry(fengG_010, "0-10%% Feng", "p");
    leg->Draw("sames");


    can_compare->cd();
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTicks(1, 1);
    pad2->SetTopMargin(0);
    pad2->Draw();
    pad2->cd();
    // Calculate the ratio and errors
    Double_t ratioX[5] = {1.4, 1.8, 2.2, 2.6, 3.2};
    Double_t ratio[2][5];
    Double_t ratioError[2][5];
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 5; j++){
            ratio[i][j] = mYRawSpectra[i][j+1] / fengYRawSpectra[i][j+1];
            Double_t err1 = mYRawSpectraError[i][j+1] / mYRawSpectra[i][j+1];
            Double_t err2 = fengYRawSpectraError[i][j+1] / mYRawSpectra[i][j+1];
            ratioError[i][j] = sqrt(err1*err1 + err2*err2); 
	}
    }
    TGraphErrors* ratioG_1060 = new TGraphErrors(5, ratioX, ratio[0], 0, ratioError[0]);
    ratioG_1060->SetMarkerSize(2.0);
    ratioG_1060->SetMarkerStyle(20);
    ratioG_1060->SetMarkerColor(2);
    ratioG_1060->SetMaximum(2.1);
    ratioG_1060->SetMinimum(0.5);
    ratioG_1060->GetXaxis()->SetLimits(0.50, 3.60);
    ratioG_1060->SetTitle("");
    ratioG_1060->GetYaxis()->SetTitle("ratio");
    ratioG_1060->GetXaxis()->SetTitle("P_{T}(GeV/c)");
    ratioG_1060->GetYaxis()->SetTitleOffset(.34);
    ratioG_1060->GetXaxis()->SetTitleOffset(1.00);

    Double_t pixel_size = GRawSpectra_1060->GetYaxis()->GetLabelSize()*pad1->GetWh()*pad1->GetAbsHNDC();
    Double_t label_size = pixel_size / pad2->GetWh() / pad2->GetAbsHNDC(); 
    ratioG_1060->GetYaxis()->SetLabelSize(label_size);
    ratioG_1060->GetXaxis()->SetLabelSize(label_size);
    Double_t font = GRawSpectra_1060->GetYaxis()->GetTitleFont();
    ratioG_1060->GetXaxis()->SetTitleSize(0.11);
    ratioG_1060->GetYaxis()->SetTitleSize(0.11);

    TGraphErrors* ratioG_010 = new TGraphErrors(5, ratioX, ratio[1], 0, ratioError[1]);
    ratioG_010->SetMarkerSize(2.0); 
    ratioG_010->SetMarkerStyle(20);
    ratioG_010->SetMarkerColor(1);
 
    ratioG_1060->Draw("AP");
    ratioG_010->Draw("P same");

    TLine* lline = new TLine(0.5, 1, 3.6, 1);
    lline->SetLineColor(1);
    lline->SetLineWidth(2);
    lline->SetLineStyle(10);
    lline->Draw("sames");
    can_compare->SaveAs("compare_feng_notsamepico.pdf");
}

void StrAnalyMaker::Analyze(){
    std::cout << "Load infile_dat/rot successfully!" << std::endl;
    for(int i = 0; i < mKPtBin; i++){
        char hist_name_rot_010[200]; 
        char hist_name_rot_1060[200];
        char hist_name_dat_010[200];
        char hist_name_dat_1060[200];
	sprintf(hist_name_rot_010, "sig_xipt%dcent_010", i+1);
	sprintf(hist_name_rot_1060, "sig_xipt%dcent_1060", i+1); 
	sprintf(hist_name_dat_010, "sig_xipt%dcent_010", i+1); 
	sprintf(hist_name_dat_1060, "sig_xipt%dcent_1060", i+1); 

        TH1F* hrot_010 = (TH1F*)mRotBgFile->Get(hist_name_rot_010);
        TH1F* hdat_010 = (TH1F*)mDatFile->Get(hist_name_dat_010);
        TH1F* hrot_1060 = (TH1F*)mRotBgFile->Get(hist_name_rot_1060);
        TH1F* hdat_1060 = (TH1F*)mDatFile->Get(hist_name_dat_1060);
       
	hrot_010->Rebin(2);
        hdat_010->Rebin(2);
        hrot_1060->Rebin(2);
        hdat_1060->Rebin(2);

        hrot_010->Sumw2();
        hdat_010->Sumw2();
        hrot_1060->Sumw2();
        hdat_1060->Sumw2();

        double rot_scale_1060 = compRotNormFactor(0, i, hdat_1060, hrot_1060);
        double rot_scale_010 = compRotNormFactor(1, i, hdat_010, hrot_010);
        //rot_scale_1060 = 1.;
        //rot_scale_010 = 1.;

        plotRotInvMassWithData(0, i, hdat_1060, hrot_1060, rot_scale_1060);
        plotRotInvMassWithData(1, i, hdat_010, hrot_010, rot_scale_010);
       
        compRawSigCounts(0, i, hdat_1060, hrot_1060, rot_scale_1060); 
        compRawSigCounts(1, i, hdat_010, hrot_010, rot_scale_010); 

    }
    compRawSpectra(); 
      
    plotRawSpectra();

    plotComparison();
}
