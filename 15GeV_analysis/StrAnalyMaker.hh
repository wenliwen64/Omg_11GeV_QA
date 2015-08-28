#ifndef StrAnalyMaker_H
#define StrAnalyMaker_H
#include "TH1F.h"
#include "TFile.h"
#include "TObject.h"
#include <string>
class StrAnalyMaker: public TObject{
    TFile* mOverviewFile;
    TFile* mDatFile;
    TFile* mRotBgFile;
    Double_t pdgmass_xi;
    Int_t mKCentBin;
    Int_t mKPtBin;
    Double_t mNEventsUnweighted[2];
    Double_t mNEventsWeighted[2];
    Double_t mBr;
    Double_t mSigRangeLeft;
    Double_t mSigRangeRight;

    Double_t mRotNormLeftLowB;
    Double_t mRotNormLeftHighB;
    Double_t mRotNormRightLowB;
    Double_t mRotNormRightHighB;

    Double_t mRotScale_ratio[2][6];

    Double_t mRawSigCounts[2][6];
    Double_t mXRawSpectra[6];
    Double_t mXRawSpectraError[6];
    Double_t mYRawSpectra[2][6];
    Double_t mYRawSpectraError[2][6];
    Double_t mDptSpectra[6];

    void rotBgAnalysisInit();
    Double_t compRotNormFactor(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot);
public:
    StrAnalyMaker();
    ~StrAnalyMaker();
    void Init(std::string file_name, std::string, std::string);
    void plotRotInvMassWithData(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot, Double_t scale); 
    void compRawSigCounts(Int_t centbin, Int_t ptbin, TH1F* hdat, TH1F* hrot, Double_t scale);
    void compRawSpectra();
    void plotRawSpectra();
    void Analyze();

    ClassDef(StrAnalyMaker, 1)
};
#endif
