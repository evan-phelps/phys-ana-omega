#ifndef DHTOP1_H
#define DHTOP1_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TLorentzVector.h"
#include "TFile.h"
#include "THnSparse.h"
#include "particle-constants.h"
using namespace ParticleConstants;
using namespace std;
class DhTop1 : public DataHandler
{
public:
	bool IsThrown;
	Double_t mmlow, mmhigh;
	Double_t mmppiplow;
	int nhists;
	THnSparse *hN[3];
	static const Int_t ndims = 6;
	//Double_t hNvals[ndims];
	TDirectory *fDir;
	DhTop1(std::string name = "tops", TDirectory *pDir = NULL, bool isthrown = false) : DataHandler(name) {
		IsThrown = isthrown;
		if (IsThrown) nhists = 1;
		else nhists = 3;
		if (pDir == NULL) {
			std::string emsg = name;
			emsg += ": no parent directory, and couldn't create file!";
			std::string newfilename = name += ".root";
			fDir = new TFile(newfilename.c_str(),"create");
			if (fDir == NULL) throw new std::runtime_error(emsg.c_str());
		} else {
			int nexisting = 0;
			std::string emsg = name;
			emsg += ": more than five folders!";
			while ( (fDir = pDir->mkdir(name.c_str())) == 0 ) {
				if (++nexisting>5) {
					throw new std::runtime_error(emsg.c_str());
				}
			}
		}
		fDir->cd();
		mmppiplow = MASS_PIM + MASS_PI0;
		mmlow = MASS_PIP + MASS_PIM + MASS_PI0;
		mmhigh = 1.5; //MASS_OMEGA + (MASS_OMEGA-mmlow);
		Double_t mmbs = 0.005;
		Int_t mmbins = (mmhigh-mmlow)/mmbs;
		mmhigh = mmlow + mmbins*mmbs;
		Double_t xmins[] = {1.6, 0.5, 0.1, -1, -PI, mmlow};
		Double_t xmaxs[] = {3.2, 5.5, 10.1, 1, PI, mmhigh};
		Int_t nbins[] = {80,20,20,20,18,mmbins};
		for (int i = 0; i < nhists; i++) {
			TString hn = TString::Format("top%d_yield",i+1);
			TString ht = TString::Format("Top %d: W, Q^{2}, t', cos(#theta), #phi, MM_{p}",i+1);
			hN[i] = new THnSparseD(hn.Data(), ht.Data(),ndims,nbins,xmins,xmaxs);
		}
	}
	virtual ~DhTop1() {
		for (int i = 0; i < nhists; i++) delete hN[i];
		delete fDir;
	}
public:
	virtual void Finalize(Data* d) {
		fDir->cd();
		//hN->SetDirectory(fDir);
		for (int i = 0; i < nhists; i++) hN[i]->Write();
	}
	virtual bool Handle(Data* data) {
		Evt *d = dynamic_cast<Evt*>(data);
		bool passed = false;
		if (IsThrown) {
			Double_t mmp = d->_omega->M();
			Double_t w = d->_w->M();
			Double_t q2 = d->_q->M2();
			Double_t tmin = pow(d->_p0->E()-d->_p1->E(),2)-pow(d->_p0->P()-d->_p1->P(),2);
			Double_t t = (*d->_p0 - *d->_p1).Mag2();
			Double_t t1 = t - tmin;
			Double_t cT = d->_omega->CosTheta();
			Double_t phi = d->_omega->Phi();
			Double_t hNvals[] = {w,-q2,-t1,cT,phi,mmp};
			hN[0]->Fill(hNvals);
		} else if (d->h10idx_p>0 && d->h10idx_e==0 && (d->h10idx_pip>0 || d->h10idx_pim>0)) {
			Double_t mmp = d->_omega->M();
			Double_t mmppip = d->_mmppip->M();
			Double_t mmppim = d->_mmppim->M();
			Double_t mpi0 = d->_pi0->M();
			if (mmp>mmlow && mmp<mmhigh) {
				Double_t w = d->_w->M();
				Double_t q2 = d->_q->M2();
				Double_t tmin = pow(d->_p0->E()-d->_p1->E(),2)-pow(d->_p0->P()-d->_p1->P(),2);
				Double_t t = (*d->_p0 - *d->_p1).Mag2();
				Double_t t1 = t - tmin;
				printf("t1 = %.2f = %.2f - %.2f\n",t1,t,tmin);
				Double_t cT = d->_omega->CosTheta();
				Double_t phi = d->_omega->Phi();
				Double_t hNvals[] = {w,-q2,-t1,cT,phi,mmp};
				if (d->h10idx_pip>0 && mmppip>mmppiplow) passed = hN[0]->Fill(hNvals);
				if (d->h10idx_pim>0 && mmppim>mmppiplow) passed = hN[1]->Fill(hNvals);
				if (d->h10idx_pip>0 && d->h10idx_pim>0
					&& mmppip>mmppiplow && mmppim>mmppiplow
					&& mpi0>0.05 && mpi0<0.075 && mpi0<0.195)
						passed = hN[2]->Fill(hNvals);
			}
		}
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DHSKIMMASS_H
