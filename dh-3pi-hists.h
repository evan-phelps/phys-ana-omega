#ifndef DH3PIHISTS_H
#define DH3PIHISTS_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH2.h"
#include "particle-constants.h"
using namespace ParticleConstants;
class Dh3piHists : public DataHandler
{
public:
	TH2 *hmmppipVmmp;
	TDirectory *fDir;
	Dh3piHists(std::string name = "unnamed", TDirectory *pDir = NULL) : DataHandler(name) {
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
		hmmppipVmmp = new TH2F("hmmppipVmmp","MM_{p#pi^{+} vs. MM_{p}",250,0,2.5,200,0,2);
	}
	virtual ~Dh3piHists() {
		delete hmmppipVmmp;
		delete fDir;
	}
public:
	virtual void Finalize(Data* d) {
		fDir->cd();
		hmmppipVmmp->SetDirectory(fDir);
		hmmppipVmmp->Write();
		hmmppipVmmp->Draw("colz");
	}
	virtual bool Handle(Data* data) {
		Evt *d = dynamic_cast<Evt*>(data);
		bool passed = true;
		TLorentzVector lvmmp = *d->_w - *d->_p1;
		TLorentzVector lvmmppip = lvmmp - *d->_pip;
		//lvmmp.Print();
		//cout << "******" << endl;
		if (d->h10idx_p>0 && d->h10idx_pip>0) hmmppipVmmp->Fill(lvmmp.M(),lvmmppip.M());
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DHSKIMMASS_H
