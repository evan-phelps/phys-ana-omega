#ifndef DH3PITOPHYSFRM_H
#define DH3PITOPHYSFRM_H
#include <stdexcept>
#include "data-handler.h" // Base class: DataHandler
#include "evt.h"
#include "h10looper.h"
#include "TTree.h"
class Dh3piToPhysFrm : public DataHandler
{
public:
	h10looper *_looper;
	TTree *t3pi;
	TDirectory *fDir;
	Evt *e3pi;
	Dh3piToPhysFrm(h10looper *h10l, std::string name = "3pi-tree", TDirectory *pDir = NULL) : DataHandler(name) {
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
					name += nexisting;
				}
			}
		}
		fDir->cd();
		_looper = h10l;
		e3pi = dynamic_cast<Evt*>(h10l->data);
		t3pi = e3pi->CreateTree("t3pi");
	}
	virtual ~Dh3piToPhysFrm() {
		if (t3pi) delete t3pi;
		if (fDir) delete fDir;
	}
public:
	virtual void Finalize(Data* d) {
		fDir->cd();
		t3pi->Write();
	}
	virtual bool Handle(Data* d) {
		_looper->GetEntry(_looper->jentry);
		Evt *e = dynamic_cast<Evt*>(d);
		e->Transform();
		t3pi->Fill();
		return true;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DH3PITOPHYSFRM_H
