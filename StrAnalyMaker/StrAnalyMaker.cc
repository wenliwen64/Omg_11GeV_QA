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
    std::cout << "!!! InitializationII for 14.5GeV Analysis" << std::endl;

    // Initialize TFile pointers 
    mOverviewFile = new TFile(overview_filename.c_str(), "read");
    mDatFile = new TFile(dat_filename.c_str(), "read");
    mRotBgFile = new TFile(rotbg_filename.c_str(), "read");

    TH1F* h_centbin9_unweighted = (TH1F*)mOverviewFile->Get("h_centbin9_after0");
    TH1F* h_centbin9_weighted = (TH1F*)mOverviewFile->Get("h_centbin9_after1");
     
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
    mRawSigCounts[centbin][ptbin] = hdat->Integral(sigRangeLeftBin, sigRangeRightBin) - hrot_copy->Integral(sigRangeLeftBin, sigRangeRightBin)/scale;
    std::cout << "mRawSigCounts for cent " << centbin << "pt" << ptbin << "is " << mRawSigCounts[centbin][ptbin] << std::endl;
}

void StrAnalyMaker::compRawSpectra(){
    double PI = 3.1415926; 

    for(int i = 0; i < mKCentBin; i++){
        for(int j = 0; j < mKPtBin; j++){
            mYRawSpectra[i][j] = 1/(2*PI) * mRawSigCounts[i][j] / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i];
            mYRawSpectraError[i][j] = 1/(2*PI) * sqrt(mRawSigCounts[i][j]) / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i];
            std::cout << "mYRawSpectra for cent" << i << "pt" << j <<" is " << mYRawSpectra[i][j] << "mYRawSpectraError is " << mYRawSpectraError[i][j] << std::endl;
	}
    }
}

void StrAnalyMaker::plotRawSpectra(){ // TODO:
    TCanvas* rawspectra_can = new TCanvas("rawspectra_can", "rawspectra_can");
    rawspectra_can->SetLogy();
    rawspectra_can->SetTicks(1, 1);

    TGraphErrors* GRawSpectra_1060 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[0], mXRawSpectraError, mYRawSpectraError[0]); 
    GRawSpectra_1060->SetMarkerSize(2.0);
    GRawSpectra_1060->SetMarkerStyle(34);
    GRawSpectra_1060->SetMarkerColor(2);
    GRawSpectra_1060->SetMaximum(10E-1);
    GRawSpectra_1060->SetMinimum(10E-9);
    GRawSpectra_1060->GetXaxis()->SetLimits(0.50, 3.60);
    GRawSpectra_1060->SetTitle("#Omega^{-} Spectra, Au+Au 14.5GeV");
    GRawSpectra_1060->GetYaxis()->SetTitle("#frac{d^{2}N}{2#piNP_{T}dP_{T}dy}(GeV/c)^{-2}");
    GRawSpectra_1060->GetXaxis()->SetTitle("P_{T}(GeV/c)");
    GRawSpectra_1060->GetYaxis()->SetTitleOffset(1.3);
    GRawSpectra_1060->Draw("AP");


    TGraphErrors* GRawSpectra_010 = new TGraphErrors(6, mXRawSpectra, mYRawSpectra[1], mXRawSpectraError, mYRawSpectraError[1]); 
    GRawSpectra_010->SetMarkerSize(2.0);
    GRawSpectra_010->SetMarkerStyle(34);
    GRawSpectra_010->SetMarkerColor(1);
    GRawSpectra_010->Draw("P same");

    TLegend* leg = new TLegend(0.25, 0.25, 0.45, 0.45);
    leg->SetBorderSize(0);
    leg->AddEntry(GRawSpectra_1060, "10-60%", "p");
    leg->AddEntry(GRawSpectra_010, "0-10%", "p");
    leg->Draw("sames");

    rawspectra_can->SaveAs("../omg_plots/omg_rawspectra.pdf");  
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

        plotRotInvMassWithData(0, i, hdat_1060, hrot_1060, 1.);
        plotRotInvMassWithData(1, i, hdat_010, hrot_010, 1.);
       
        compRawSigCounts(0, i, hdat_1060, hrot_1060, 1.); 
        compRawSigCounts(1, i, hdat_010, hrot_010, 1.); 

    }
    compRawSpectra(); 
      
    plotRawSpectra();
}
