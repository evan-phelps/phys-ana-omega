#ifndef FID_H
#define FID_H

#include "TF1.h"
#include <stdio.h>
#include <string>
#include "acc.h"

using namespace std;

class Fid
{

	static Fid* ms_instance;

public:
	Acc *acc;
	static Fid* Instance(string infile = "input/fid.parms");
	static void Release();

	void Print() { };

	Bool_t InFid(Double_t p, Double_t theta, Double_t phi, Int_t sector,
	             Int_t pid, Double_t tightness = 1, Double_t t0tightness = 0);
	Bool_t InFid(Double_t p, Double_t theta, Double_t phi,
	             Int_t pid, Double_t tightness = 1, Double_t t0tightness = 0);
	void GetPhiPim(Double_t p, Double_t theta, Int_t sector, Double_t &phiH, Double_t &phiL,
					Int_t tightness = 0, Int_t t0tightness = 0);
	void GetPhiPos(Double_t theta, Int_t sector, Double_t &phiH, Double_t &phiL,
                     Int_t tightness = 0, Int_t t0tightness = 0);
	Double_t GetPhi(Double_t p, Double_t theta, Int_t sector,
	                Int_t tightness = 1, Int_t t0tightness = 0);
	static Double_t dPhiFid(Double_t *x, Double_t *parms);
	static TF1* fPhiFid(Double_t p, Int_t sector, Int_t weight = 1,
	                    Double_t tightness = 0, Double_t t0tightness = 0);
	TF1* fThetaMin(Int_t sector, Int_t pnum = 0);
	bool mPw[6][57];
private:
	Fid(string infile = "input/fid.parms");
	virtual ~Fid();
	static const double _rad2deg;
	string mFName;
	FILE* mF;

	Float_t mIbRatio;
	Float_t mDeltaTheta01[6];
	Float_t mMa[6];
	Float_t mMb[6];
	Float_t mTheta2[6];
	Float_t mTheta0_parms[6][3];
	Float_t mPhi0_parms[6][4];
	
	Float_t mTheta0pim_parms[6][3];
	Float_t mPhi0pim_parms[6][8];
	Float_t mTheta0pip_parms[6];
	Float_t mPhi0pip_parms[6][6];
};

#endif // FID_H
