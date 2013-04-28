#include "acc.h"
#include "particle-constants.h"
using namespace ParticleConstants;
using namespace std;
Acc::Acc(string infile)
{
	fIn = new TFile(infile.c_str());
	fHacc = (THnSparse*)fIn->Get("hacc");
	fHeff[E] = (TH3*)fIn->Get("ha_e");
	fHeff[P] = (TH3*)fIn->Get("ha_p");
	fHeff[PIP] = (TH3*)fIn->Get("ha_pip");
	fHeff[PIM] = (TH3*)fIn->Get("ha_pim");
	fHf = (THnSparse*)fIn->Get("heff");
	fHo = (THnSparse*)fIn->Get("hholes");
}

Acc::~Acc()
{
	delete fHacc;
	delete fHeff[E];
	delete fHeff[P];
	delete fHeff[PIP];
	delete fHeff[PIM];
	delete fHf;
	delete fHo;
	delete fIn;
}

float Acc::GetAcc(float w, float q2, float ct, float phi, float &err)
{
	double coord[] = {w,q2,ct,phi};
	long int bn = fHacc->GetBin(coord);
	float bc = fHacc->GetBinContent(bn);
	err = fHacc->GetBinError(bn);
	return bc;
}

float Acc::GetEff(float p, float theta, float phi, int pid, float &err)
{
	theta = 1/D2R*theta;
	phi = 1/D2R*phi;
	if (phi<-30) phi+=360;
	int ipart = IdxPart(pid);
	int bn = fHeff[ipart]->FindBin(phi,theta,p);
	float bc = fHeff[ipart]->GetBinContent(bn);
	err = fHeff[ipart]->GetBinError(bn);
	return bc;
}

float Acc::GetFiller(float w, float q2, float ct, float phi, float &err)
{
	double coord[] = {w,q2,ct,phi};
	long int bn = fHacc->GetBin(coord);
	float nf = fHf->GetBinContent(bn);
	float nh = fHo->GetBinContent(bn);
	float factor = nf/(nf+nh);
	float ef = fHf->GetBinError(bn)/nf;
	float eh = fHo->GetBinError(bn)/nh;
	err = sqrt(ef*ef+eh*eh)*factor;
	return factor;
}

int Acc::IdxPart(int pid)
{
	if (pid == 11) return 0;
	else if(pid == 2212) return 1;
	else if(pid == 211) return 2;
	else if(pid == -211) return 3;
	else return -1;
}
