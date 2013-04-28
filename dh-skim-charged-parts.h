#ifndef SKIMCHARGEDPARTS_H
#define SKIMCHARGEDPARTS_H
#include "data-handler.h"
#include <stdexcept>
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhSkimChargedParts : public DataHandler
{
public:
	TDirectory *fDir;
	TH1I *hevtsum;
	int np, npip, npim, nphoton, nother;
	static const Int_t NUM_EVTCUTS = 6;
	enum {
	    EVT_NULL, EVT, EVT_EX, EVT_P, EVT_PPIP, EVT_PPIM, EVT_PPIPPIM
	};
	DhSkimChargedParts(std::string name = "1or2pions", TDirectory *pDir = NULL) : DataHandler(name) {
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
		hevtsum = new TH1I("hevtsum","Event Statistics",NUM_EVTCUTS,0.5,NUM_EVTCUTS+0.5);
		hevtsum->GetXaxis()->SetBinLabel(EVT,"Total");
		hevtsum->GetXaxis()->SetBinLabel(EVT_EX,"excl");
		hevtsum->GetXaxis()->SetBinLabel(EVT_P,"p");
		hevtsum->GetXaxis()->SetBinLabel(EVT_PPIP,"p#pi^{+}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_PPIM,"p#pi^{-}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_PPIPPIM,"p#pi^{+}#pi^{-}");
	}
	virtual ~DhSkimChargedParts() {
		delete hevtsum;
		delete fDir;
	}
public:
	virtual void Finalize(Data *d) {
		fDir->cd();
		hevtsum->SetDirectory(fDir);
		hevtsum->Write();
	}
	virtual bool Handle(Data *data) {
		T_ *d = dynamic_cast<T_*>(data);
		np = npip = npim = nphoton = nother = 0;
		bool passed = false;
		hevtsum->Fill(EVT);
		for (int i = 1; i < d->gpart; i++) {
			switch(d->id[i]) {
				case PROTON: np++; break;
				case PIP: npip++; break;
				case PIM: npim++; break;
				case PHOTON: nphoton++; break;
				default: nother++; break;
			}
		}
		if (nother>0) {
			passed = false;
			hevtsum->Fill(EVT_EX);
		} else if (np==1) {
			hevtsum->Fill(EVT_P);
			passed = true;
			if (npip==1 && npim==0) hevtsum->Fill(EVT_PPIP);
			else if (npip==0 && npim==1) hevtsum->Fill(EVT_PPIM);
			else if (npip==1 && npim==1) hevtsum->Fill(EVT_PPIPPIM);
			else passed = false;
		}
		return passed;
	}
	virtual void Wrapup(Data *d) {
	}
};
#endif // SKIMCHARGEDPARTS_H
