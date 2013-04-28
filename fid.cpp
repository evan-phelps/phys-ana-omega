#include "fid.h"
#include "TF1.h"
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>

const double Fid::_rad2deg = 180/3.14159265359;
Fid* Fid::ms_instance = 0;
using namespace std;

Fid* Fid::Instance(string infile /* = "input/fid.parms" */)
{
	if (ms_instance == 0) {
		ms_instance = new Fid(infile);
	}
	return ms_instance;
}

void Fid::Release()
{
	if(ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

Fid::Fid(string infile /* = "input/fid.parms" */)
{
	//initialize SC paddle weights
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 57; j++) mPw[i][j] = 1;
	}
	mPw[0][3] = mPw[0][23] = mPw[0][41] = 0;
	mPw[1][16] = mPw[1][41] = mPw[1][45] = 0;
	mPw[2][2] = mPw[2][11] = mPw[2][29] = mPw[2][40] = mPw[2][42] = mPw[2][43] = mPw[2][46] = mPw[2][48] = 0;
	mPw[3][2] = mPw[3][5] = mPw[3][44] = 0;
	mPw[4][2] = mPw[4][3] = mPw[4][5] = mPw[4][18] = mPw[4][23] = mPw[4][36] = mPw[4][48] = 0;
	mPw[5][1] = mPw[5][3] = mPw[5][18] = mPw[5][41] = mPw[5][42] = mPw[5][43] = mPw[5][44]= 0;
	
	//initialize sector-independent values inline
	for (int i = 0; i < 6; i++ ) {
		mDeltaTheta01[i] = 8;
		mMa[i] = 1.5;
		mMb[i] = 0.1;
		mTheta2[i] = 150;
	}

	//initialize sector-dependent values from data file
	mFName = infile;
	mF = fopen(mFName.c_str(),"r");
	if (!mF)  {
		string emsg = infile;
		emsg += " not found!";
		throw new std::runtime_error(emsg.c_str());
	}
	char line[256];
	while ( fscanf(mF, "%s", &line[0])!=EOF ) {
		if (strcmp(line,"t0parms") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f, %f, %f\n", &mTheta0_parms[i][0], &mTheta0_parms[i][1], &mTheta0_parms[i][2]);
				//printf("t0: Sector %d: %.3f, %.3f, %.3f\n",i+1,mTheta0_parms[i][0], mTheta0_parms[i][1], mTheta0_parms[i][2]);
			}
		} else if (strcmp(line,"phi0parms") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f, %f, %f, %f\n", &mPhi0_parms[i][0], &mPhi0_parms[i][1], &mPhi0_parms[i][2], &mPhi0_parms[i][3]);
			}
		} else if (strcmp(line,"t0parmspim") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f, %f, %f\n", &mTheta0pim_parms[i][0], &mTheta0pim_parms[i][1], &mTheta0pim_parms[i][2]);
				//printf("t0: Sector %d: %.3f, %.3f, %.3f\n",i+1,mTheta0_parms[i][0], mTheta0_parms[i][1], mTheta0_parms[i][2]);
			}
		} else if (strcmp(line,"t0parmspip") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f\n", &mTheta0pip_parms[i]);
				//printf("t0: Sector %d: %.3f, %.3f, %.3f\n",i+1,mTheta0_parms[i][0], mTheta0_parms[i][1], mTheta0_parms[i][2]);
			}
		} else if (strcmp(line,"phiparmspim") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f, %f, %f, %f, %f, %f, %f, %f\n", &mPhi0pim_parms[i][0], &mPhi0pim_parms[i][1], &mPhi0pim_parms[i][2], &mPhi0pim_parms[i][3], &mPhi0pim_parms[i][4], &mPhi0pim_parms[i][5], &mPhi0pim_parms[i][6], &mPhi0pim_parms[i][7]);
			}
		} else if (strcmp(line,"phiparmspip") == 0) {
			for (int i = 0; i < 6; i++ ) {
				fscanf(mF, "%f, %f, %f, %f, %f, %f\n", &mPhi0pip_parms[i][0], &mPhi0pip_parms[i][1], &mPhi0pip_parms[i][2], &mPhi0pip_parms[i][3], &mPhi0pip_parms[i][4], &mPhi0pip_parms[i][5]);
			}
		}
	}
	//max. torus current to experimental torus current (for theta boundaries)
	mIbRatio = 3375.0/2250.0;
	fclose(mF);
	acc = new Acc();
}

Fid::~Fid()
{
	if (mF) delete mF;
	if (acc) delete acc;
}

void Fid::GetPhiPim(Double_t p, Double_t theta, Int_t sector, Double_t &phiH, Double_t &phiL,
                     Int_t tightness /* = 0 */, Int_t t0tightness /* = 0 */)
{
	Int_t isector = sector-1;
	//calculate min theta, parameter t0
	Float_t irat = mIbRatio;
	Double_t t0, t0a, t0b, t0c;
	t0a = mTheta0pim_parms[isector][0];
	t0b = mTheta0pim_parms[isector][1];
	t0c = mTheta0pim_parms[isector][2];
	t0 = t0a + t0b/((p+t0c)*irat);
	//calculate phi boundary
	if ( theta < t0+t0tightness ) phiH = phiL = 0;
	else {
		double aH = mPhi0pim_parms[isector][0];
		double bH = mPhi0pim_parms[isector][1];
		double cH = mPhi0pim_parms[isector][2] + p*mPhi0pim_parms[isector][3];
		double aL = mPhi0pim_parms[isector][4];
		double bL = mPhi0pim_parms[isector][5];
		double cL = mPhi0pim_parms[isector][6] + p*mPhi0pim_parms[isector][7];
		phiH = aH * (1 - exp(-bH*(theta-cH)));
		phiL = aL * (1 - exp(-bL*(theta-cL)));
	}
}

void Fid::GetPhiPos(Double_t theta, Int_t sector, Double_t &phiH, Double_t &phiL,
                     Int_t tightness /* = 0 */, Int_t t0tightness /* = 0 */)
{
	Int_t isector = sector-1;
	Double_t t0 = mTheta0pip_parms[isector];
	//calculate phi boundary
	if ( theta < t0+t0tightness ) phiH = phiL = 0;
	else {
		double aH = mPhi0pip_parms[isector][0];
		double bH = mPhi0pip_parms[isector][1];
		double cH = mPhi0pip_parms[isector][2];
		double aL = mPhi0pip_parms[isector][3];
		double bL = mPhi0pip_parms[isector][4];
		double cL = mPhi0pip_parms[isector][5];
		phiH = aH * (1 - exp(-bH*(theta-cH)));
		phiL = aL * (1 - exp(-bL*(theta-cL)));
	}
}

Double_t Fid::GetPhi(Double_t p, Double_t theta, Int_t sector,
						Int_t tightness /* = 1 */, Int_t t0tightness /* = 0 */)
{
	Int_t isector = sector-1;
	Float_t *t1mt0 = mDeltaTheta01;
	Float_t *ma = mMa;
	Float_t *mb = mMb;
	Float_t *t2 = mTheta2;
	//calculate min theta, parameter t0
	Float_t irat = mIbRatio;
	Double_t t0, t0a, t0b, t0c;
	t0a = mTheta0_parms[isector][0];
	t0b = mTheta0_parms[isector][1];
	t0c = mTheta0_parms[isector][2];
	t0 = t0a + t0b/((p+t0c)*irat);
	//calculate phi value at min theta, parameter phi0
	Double_t phi0, xl0l1, y0l0, ml0, ml1;
	xl0l1 = mPhi0_parms[isector][0];
	y0l0 = mPhi0_parms[isector][1];
	ml0 = mPhi0_parms[isector][2];
	ml1 = mPhi0_parms[isector][3];
	if (p <= xl0l1) phi0 = y0l0 + ml0*p;
	else phi0 = (y0l0 + ml0*p) + ml1*(p-xl0l1);
	//calculate phi boundary
	Double_t phib = 0;
	Double_t t1 = t0 + t1mt0[isector];
	if ( theta <= t0+t0tightness || theta >= t2[isector] ) phib = 0;
	else {
		Double_t phi1 = phi0 + ma[isector]*(t1-t0);
		if ( theta > t0 && theta < t1 ) phib = phi0 + ma[isector]*(theta-t0);
		else if ( theta >= t1 && theta < t2[isector] ) phib = phi1 + mb[isector]*(theta-t1);
	}
	phib -= tightness;
	phib = phib <= 0 ? 0 : phib;
	return phib;
}

Double_t Fid::dPhiFid(Double_t *x, Double_t *parms)
{
	Fid *_fid = Fid::Instance();
	Double_t theta = x[0];
	Double_t p = parms[0];
	Int_t sector = parms[1];
	Double_t tightness = parms[2];
	Double_t t0tightness = parms[3];
	Int_t w = parms[4];
	Double_t phib = _fid->GetPhi(p, theta, sector, tightness, t0tightness);
	return w*phib;
}

TF1* Fid::fPhiFid(Double_t p, Int_t sector, Int_t weight /* = 1 */,
                  Double_t tightness /* = 0 */, Double_t t0tightness /* = 0 */)
{
	TF1 *retFunc = new TF1((const char*)"fphifid",&dPhiFid,0.0,70.0,5);
	retFunc->SetParameters(p,sector,tightness,t0tightness,weight);
	return retFunc;
}

TF1* Fid::fThetaMin(Int_t sector, Int_t pnum /* = 0 */)
{
	//  Fid *_fid = Fid::Instance();
	Double_t t0a, t0b, t0c;
	Int_t isector = sector-1;
	t0a = mTheta0_parms[isector][0];
	t0b = mTheta0_parms[isector][1];
	t0c = mTheta0_parms[isector][2];
	TF1 *retFunc = new TF1((const char*)"fthetamin","[0]+[1]/((x+[2])*3375.0/2250.0)",0,5.5);
	retFunc->SetParameters(t0a,t0b,t0c);
	return retFunc;
}

Bool_t Fid::InFid(Double_t p, Double_t theta, Double_t phi, Int_t sector, Int_t pid,
                  Double_t tightness /* = 1 */, Double_t t0tightness /* = 0 */)
{
	if (sector==0) throw "sector = 0 !!!";
	theta *= _rad2deg;
	phi *= _rad2deg;
	Bool_t isInIrreg = kTRUE;
	Int_t isector = sector-1;
	//make sure phi is between -30 and 330 degrees
	phi = phi >= 330 ? phi-360 : (phi<-30 ? phi+360 : phi);
	//rotate to sector center = 0 degrees
	phi -= 60*isector;
	Double_t phiH = 0;
	Double_t phiL = 0;
	if ( pid == 11 ) {
		phiH = GetPhi(p, theta, sector, tightness, t0tightness);
		phiL = -phiH;
	}
	else if ( pid == -211 ) GetPhiPim(p, theta, sector, phiH, phiL, tightness, t0tightness);
	else if ( pid == 2212 || 211) GetPhiPos(theta, sector, phiH, phiL, tightness, t0tightness);

	//if ( sector == 6 && pnum == 0 && p < 1 ) isInIrreg = kFALSE;

	return (isInIrreg && phi < phiH && phi > phiL);
}

Bool_t Fid::InFid(Double_t p, Double_t theta, Double_t phi, Int_t pid,
                  Double_t tightness /* = 1 */, Double_t t0tightness /* = 0 */)
{
  Double_t _phi = _rad2deg*phi;
  //Double_t _theta = _rad2deg*theta;
  if (_phi<-30) _phi+=360;
  //determine sector
  Double_t sectphi = _phi + 30;
  Int_t iSector = sectphi/60; //relies on truncation
  iSector = iSector < 6 ? iSector : 0;
  //printf("%.3f, %.2f, %.2f, %d\n",p,theta,phi,iSector);
  return InFid(p,theta,phi,iSector+1,pid,tightness,t0tightness);
}
