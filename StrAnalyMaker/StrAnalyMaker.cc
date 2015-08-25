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
StrAnalyMaker::StrAnalyMaker():pdgmass_xi(1.67245){
    mXRawSpectra[0] = 0.95;
    mXRawSpectra[1] = 1.40;
    mXRawSpectra[2] = 1.80;
    mXRawSpectra[3] = 2.20;
    mXRawSpectra[4] = 2.60;
    mXRawSpectra[5] = 3.20;

    mDptSpectra[0] = 0.5;
    mDptSpectra[1] = 0.4;
    mDptSpectra[2] = 0.4;
    mDptSpectra[3] = 0.4;
    mDptSpectra[4] = 0.4;
    mDptSpectra[5] = 0.6;
    std::cout << "StrAnalyMaker Constructor v0.01 2015-08-24 " << std::endl;
}

//StrAnalyMaker::~StrAnalyMaker(){}

void StrAnalyMaker::Init(std::string overview_file_name){
    // Initialize events number for each centrality 
    TFile* infile_overview = new TFile(overview_file_name.c_str(), "read"); 
    TH1F* h_centbin9_after0 = (TH1F*)infile_overview->Get("h_centbin9_after0");
    TH1F* h_centbin9_after1 = (TH1F*)infile_overview->Get("h_centbin9_after1");
    for(int i = 0; i < mKCentBin; i++){
	mNEventsUnweighted[i] = h_centbin9_after0->GetBinContent(i+2); 
	mNEventsWeighted[i] = h_centbin9_after1->GetBinContent(i+2); 
    }

    // Initialize branching ratio
    mBr = 0.678*0.639;

    // Initialize signal counting range
    mSigRangeLeft = 1.66;
    mSigRangeRight = 1.685;
}

void StrAnalyMaker::rotBgAnalysisInit(){
    mRotNormLeftLowB = pdgmass_xi - 0.05;
    mRotNormLeftHighB = pdgmass_xi - 0.01;

    mRotNormRightLowB = pdgmass_xi + 0.01;
    mRotNormRightHighB = pdgmass_xi + 0.05;
}

Double_t StrAnalyMaker::compRotNormFactor(Int_t centbin, Int_t ptbin,  TH1F* hdat, TH1F* hrot){
    Int_t ratio_l1 = hrot->FindBin(mRotNormLeftLowB);
    Int_t ratio_l2 = hrot->FindBin(mRotNormLeftHighB);
    Int_t ratio_u1 = hrot->FindBin(mRotNormRightLowB);
    Int_t ratio_u2 = hrot->FindBin(mRotNormRightHighB); 
 
    mRotScale_ratio[centbin][ptbin] = (hrot->Integral(ratio_l1, ratio_u1) + hrot->Integral(ratio_l2, ratio_u2)) / (hdat->Integral(ratio_l1, ratio_u1) + hdat->Integral(ratio_l2, ratio_u2));    
    return mRotScale_ratio[centbin][ptbin];
}

void StrAnalyMaker::plotRotInvMassWithData(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot, Double_t scale){
    
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
    gPad->Draw("Hist same");

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
    mRawSigCounts[centbin][ptbin] = hdat->Integral(mSigRangeLeft, mSigRangeRight) - hrot_copy->Integral(mSigRangeLeft, mSigRangeRight)/scale;
}

void StrAnalyMaker::compRawSpectra(){
    double PI = 3.1415926; 

    for(int i = 0; i < mKCentBin; i++){
        for(int j = 0; j < mKPtBin; j++){
            mYRawSpectra[i][j] = 1/(2*PI) * mRawSigCounts[i][j] / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i];
            mYRawSpectraError[i][j] = 1/(2*PI) * sqrt(mRawSigCounts[i][j]) / mXRawSpectra[j] / mDptSpectra[j] / mNEventsWeighted[i];
	}
    }
}

void StrAnalyMaker::plotRawSpectra(){ // TODO:
    TCanvas* rawspectra_can = new TCanvas("rawspectra_can", "rawspectra_can");
    rawspectra_can->SetLogy();
    rawspectra_can->SetTicks(1, 1);

    TGraphErrors* GRawSpectra_1060 = new TGraphErrors(6, mXRawSpectra, 0, mYRawSpectra[0], mYRawSpectraError[0]); 
    GRawSpectra_1060->SetMarkerSize(2.0);
    GRawSpectra_1060->SetMarkerStyle(34);
    GRawSpectra_1060->SetMarkerColor(2);
    GRawSpectra_1060->SetMaximum(10E-2);
    GRawSpectra_1060->SetMinimum(10E-12);
    GRawSpectra_1060->GetXaxis()->SetLimits(0.50, 3.60);
    GRawSpectra_1060->SetTitle("#Omega^{-} Spectra, Au+Au 14.6GeV");
    GRawSpectra_1060->GetYaxis()->SetTitle("#frac{d^{2}N}{2#piNP_{T}dP_{T}dy}(GeV/c)^{-2}");
    GRawSpectra_1060->GetXaxis()->SetTitle("P_{T}(GeV/c)");
    GRawSpectra_1060->GetYaxis()->SetTitleOffset(1.3);
    GRawSpectra_1060->Draw("AP");

    TGraphErrors* GRawSpectra_010 = new TGraphErrors(6, mXRawSpectra, 0, mYRawSpectra[1], mYRawSpectraError[1]); 
    GRawSpectra_010->SetMarkerSize(2.0);
    GRawSpectra_010->SetMarkerStyle(34);
    GRawSpectra_010->SetMarkerColor(1);
    GRawSpectra_010->SetMaximum(10E-2);
    GRawSpectra_010->SetMinimum(10E-12);
    GRawSpectra_010->Draw("P sames");

    TLegend* leg = new TLegend(0.25, 0.25, 0.45, 0.45);
    leg->SetBorderSize(0);
    leg->AddEntry(GRawSpectra_1060, "10-60%", "p");
    leg->AddEntry(GRawSpectra_010, "0-10%", "p");
    leg->Draw("sames");

    rawspectra_can->SaveAs("../omg_plots/omg_rawspectra.pdf");  
}

void StrAnalyMaker::Analyze(std::string filename_dat, std::string filename_rot){
    TFile* infile_dat = new TFile(filename_dat.c_str(), "read");
    TFile* infile_rot = new TFile(filename_rot.c_str(), "read"); 
    for(int i = 0; i < mKPtBin; i++){
        char hist_name_rot_010[200]; 
        char hist_name_rot_1060[200];
        char hist_name_dat_010[200];
        char hist_name_dat_1060[200];
	sprintf(hist_name_rot_010, "sig_xipt%dcent_010", i+1);
	sprintf(hist_name_rot_1060, "sig_xipt%dcent_1060", i+1); 
	sprintf(hist_name_dat_010, "sig_xipt%dcent_010", i+1); 
	sprintf(hist_name_dat_1060, "sig_xipt%dcent_1060", i+1); 

        TH1F* hrot_010 = (TH1F*)infile_rot->Get(hist_name_rot_010);
        TH1F* hdat_010 = (TH1F*)infile_dat->Get(hist_name_dat_010);
        TH1F* hrot_1060 = (TH1F*)infile_rot->Get(hist_name_rot_1060);
        TH1F* hdat_1060 = (TH1F*)infile_dat->Get(hist_name_dat_1060);
       
        hrot_010->Sumw2();
        hdat_010->Sumw2();
        hrot_1060->Sumw2();
        hdat_1060->Sumw2();

        double rot_scale_1060 = compRotNormFactor(0, i, hdat_1060, hrot_1060);
        double rot_scale_010 = compRotNormFactor(1, i, hdat_010, hrot_010);

        plotRotInvMassWithData(0, i, hdat_1060, hrot_1060, rot_scale_1060);
        plotRotInvMassWithData(1, i, hdat_010, hrot_010, rot_scale_010);
       
        compRawSigCounts(0, i, hdat_1060, hrot_1060, rot_scale_1060); 
        compRawSigCounts(1, i, hdat_010, hrot_010, rot_scale_010); 

        compRawSpectra(); 
        compRawSpectra(); 
      
        plotRawSpectra();
    }
}
