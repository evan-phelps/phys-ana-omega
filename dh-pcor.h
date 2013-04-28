#ifndef DhPcor_H
#define DhPcor_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "particle-constants.h"
#include "mom_corr.h"
using namespace ParticleConstants;
class DhPcor : public DataHandler
{
public:
	class MomCorr_e1f *_pcorr;
	TLorentzVector pin;
	TH2 *hpcor;
	TDirectory *fDir;
	DhPcor(std::string name = "pcor", TDirectory *pDir = NULL, string infile = "/home/ephelps/analysis/omega/input/MomCorr") : DataHandler(name) {
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
				if (++nexisting>5) throw new std::runtime_error(emsg.c_str());
			}
		}
		_pcorr = new MomCorr_e1f((char*)infile.c_str());
				//printf("***** created _pcorr *****\n");
		fDir->cd();
				//printf("***** changed dir *****\n");
		hpcor = new TH2D("hpcor","e^{-} momentum corrections",550,0,5.5,160,-0.08,0.08);
				//printf("***** created hpcor *****\n");
	}
	virtual ~DhPcor() {
		delete hpcor;
		delete fDir;
	}
public:
	virtual void Finalize(Data* data) {
		fDir->cd();
		hpcor->SetDirectory(fDir);
		hpcor->Write();
	}
	virtual bool Handle(Data* data) {
				//printf("***** handling *****\n");
		bool passed = true;
		h10exp *d = dynamic_cast<h10exp*>(data);
				//printf("***** cast'd *****\n");
		int i = 0;
		if (d->id[i]==ELECTRON && d->p[i]>0) {
				//printf("***** electron with p>0 *****\n");
			Float_t cx = d->cx[i];
			Float_t cy = d->cy[i];
			Float_t cz = d->cz[i];
			Float_t p = d->p[i];
			Int_t q = d->q[i];
			Int_t id = d->id[i];
			Float_t px = p*cx;
			Float_t py = p*cy;
			Float_t pz = p*cz;
			Float_t mass = GetPdgMass(id);
			pin.SetXYZM(px,py,pz,mass);
				//printf("***** try to correct *****\n");
			TLorentzVector cp = _pcorr->PcorN(pin,q,id);
			d->p[i] = cp.P();
			d->cx[i] = cp.Px()/cp.P();
			d->cy[i] = cp.Py()/cp.P();
			d->cz[i] = cp.Pz()/cp.P();
				//printf("***** filling *****\n");
			hpcor->Fill(p,d->p[i]-p);
				//printf("***** filled *****\n");
		}
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DhPcor_H
