#ifndef DHTMH10CLONE_H
#define DHTM10CLONE_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TTree.h"
#include "h10looper.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhTmH10clone : public DataHandler
{
public:
	TTree *h10;
	h10looper *_h10looper;
	int treenumber;
	TDirectory *fDir;
	DhTmH10clone(h10looper *h10l, std::string name = "h10clone", TDirectory *pDir = NULL) : DataHandler(name) {
		treenumber = -1;
		_h10looper = h10l;
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
				} else {
					//std::ostringstream ss(name);
					//ss << nexisting;
					//name = ss.str();
					name += nexisting;
				}
			}
		}
		fDir->cd();
	}
	virtual ~DhTmH10clone() {
		delete h10;
		delete fDir;
	}
public:
	virtual void Finalize(Data* d) {
		fDir->cd();
		h10->Write();
	}
	virtual bool Handle(Data* data) {
		//T_ *d = dynamic_cast<T_*>(data);
		bool passed = true;
		if (treenumber != _h10looper->fChain->GetTreeNumber()) {
			if (treenumber == -1) {
				fDir->cd();
				h10 = (TTree*) _h10looper->fChain->GetTree()->CloneTree(0);
				h10->SetDirectory(fDir);
			}
			_h10looper->GetEntry(_h10looper->jentry);
			_h10looper->fChain->CopyAddresses(h10); //,kTRUE);
			treenumber = _h10looper->fChain->GetTreeNumber();
		}
		h10->Fill();
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DHTM10CLONE_H
