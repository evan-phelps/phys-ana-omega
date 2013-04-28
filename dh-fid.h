#ifndef DHFID_H
#define DHFID_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TH2.h"
#include "fid.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhFid : public DataHandler
{
protected:
	long long _nEtotal;
	long long _nEpassed;
public:
	Fid *fid;
	TDirectory *fDir;
	static const char* PCUT[2];
	static const char* PCUTTITLE[2];
	static const int NPART = 4;
	enum { _E, _P, _PIP, _PIM };
	static const char* PNAME[NPART];
	static const char* PTITLE[NPART];
	TH2 *hmon[NPART][2];
	DhFid(std::string name = "fid", TDirectory *pDir = NULL, string infile = "input/fid.parms") : DataHandler(name) {
		_nEtotal = _nEpassed = 0;
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
		fid = Fid::Instance(infile);
		fDir->cd();
		for (int ipart = 0; ipart < NPART; ipart++) {
			for (int i = 0; i < 2; i++ ) {
				TString hn = TString::Format("hmon_%s_%s",PNAME[ipart],PCUT[i]);
				TString ht = TString::Format("%s %s",PNAME[ipart],PCUTTITLE[i]);
				hmon[ipart][i] = new TH2I(hn.Data(),ht.Data(),360,-3.14159,3.14159,120,0,2*3.14159/3);
			}
		}
	}
	virtual ~DhFid() {
		for (int ipart = 0; ipart < NPART; ipart++) {
			for (int i = 0; i < 2; i++) {
				delete hmon[ipart][i];
			}
		}
		delete fDir;
	}
	virtual void Finalize(Data* data) {
		T_ *d = dynamic_cast<T_*>(data);
		float fideacc = (float)_nEpassed/_nEtotal;
		printf("electron fiducial acceptance: %lli / %lli = %.2f\n",_nEpassed,_nEtotal,fideacc);
		fDir->cd();
		for (int ipart = 0; ipart < NPART; ipart++) {
			for (int i = 0; i < 2; i++) {
				hmon[ipart][i]->SetDirectory(fDir);
				hmon[ipart][i]->Write();
			}
		}
	}
	virtual bool Handle(Data* data) {
		T_ *d = dynamic_cast<T_*>(data);
		for (int pnum = 0; pnum < d->gpart; pnum++) {
			int id = d->id[pnum];
			int scidx = d->sc[pnum]-1;
			int sector = scidx > -1 ? d->sc_sect[scidx] : 0;
			float p = d->p[pnum];
			float theta = acos(d->cz[pnum]);
			float phi = atan2(d->cy[pnum],d->cx[pnum]);
			bool passed = fid->InFid(p,theta,phi,sector,id,1,0);
			switch(id) {
				case ELECTRON:
					_nEtotal++ && hmon[_E][0]->Fill(phi,theta,p>1.0 && p<1.1);
					if (passed) _nEpassed++ && hmon[_E][1]->Fill(phi,theta,p>1.0 && p<1.1);
			}
		}
		return true;
	}
	virtual void Wrapup(Data* d) {
	}
};
#endif // DHFID_H

template <class T_>
const char* DhFid<T_>::PNAME[DhFid::NPART] = {"e", "p", "pip", "pim"};
template <class T_>
const char* DhFid<T_>::PTITLE[DhFid::NPART] = {"e^{-}", "p", "#pi^{+}", "#pi{-}"};
template <class T_>
const char* DhFid<T_>::PCUT[2] = {"all", "cut"};
template <class T_>
const char* DhFid<T_>::PCUTTITLE[2] = {"All", "Fiducial"};