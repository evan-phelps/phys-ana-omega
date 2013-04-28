#ifndef DHSKIMMASS_H
#define DHSKIMMASS_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TH1.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhSkimMass : public DataHandler
{
public:
	TH1 *hW[2];
	TDirectory *fDir;
	DhSkimMass(std::string name = "unnamed", TDirectory *pDir = NULL) : DataHandler(name) {
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
		hW[0] = new TH1D("hW","hW",600,0,6);
		hW[1] = new TH1D("hW_cut","hW_cut",600,0,6);
	}
	virtual ~DhSkimMass() {
		for (int i = 0; i < 2; i++) {
			delete hW[i];
		}
		delete fDir;
	}
public:
	virtual void Finalize(Data* d) {
		fDir->cd();
		for (int i = 0; i < 2; i++) {
			hW[i]->SetDirectory(fDir);
			hW[i]->Write();
		}
	}
	virtual bool Handle(Data* data) {
		T_ *d = dynamic_cast<T_*>(data);
		bool passed = false;
		if (d->id[0]==ELECTRON) {
			float p = d->p[0];
			float px = p*d->cx[0];
			float py = p*d->cy[0];
			float pz = p*d->cz[0];
			float s = pow(E1F_E0-p+MASS_P,2)-(pow(E1F_E0-pz,2)+pow(-py,2)+pow(-px,2));
			float w = sqrt(s);
			hW[0]->Fill(w);
			if (sqrt(s)>1.5) {
				hW[1]->Fill(w);
				passed = true;
			}
		}
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DHSKIMMASS_H
