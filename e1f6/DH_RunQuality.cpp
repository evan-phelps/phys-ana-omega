#ifndef _MON_LUMINOSITY_H_
#include "DH_RunQuality.h"
#endif
#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

using namespace H10Constants;

DH_RunQuality::DH_RunQuality(std::string name, TDirectory *pDir) : DataHandler(name)
{
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
	/* CREATE TREES and HISTOGRAMS */
	hq2_V_w = new TH2D("hq2_V_w", "hq2_V_w", 600, 0, 6, 800, 0, 8);
	hq2_V_w_elast_exc = new TH2D("hq2_V_w_elast_exc", "hq2_V_w_elast_exc", 100, 0.5, 1.5, 600, 0, 6);
	hmmppippim_V_mmp = new TH2D("hmmppippim_V_mmp", "hmmppippim_V_mmp", 180, 0.2, 2.0, 250, -0.5, 2);
	lvE0 = new TLorentzVector();
	lvP0 = new TLorentzVector(0, 0, 0, MASS_P);
	lvE1 = new TLorentzVector();
	lvP1 = new TLorentzVector();
	lvPip = new TLorentzVector();
	lvPim = new TLorentzVector();
}

DH_RunQuality::~DH_RunQuality()
{
	delete hq2_V_w;
	delete hq2_V_w_elast_exc;
	delete hmmppippim_V_mmp;
	delete fDir;
	delete lvE0;
	delete lvE1;
	delete lvP0;
	delete lvP1;
	delete lvPip;
	delete lvPim;
}

bool DH_RunQuality::Handle(H10 *d)
{
	bool passed = true;
	if (d->id[0]==ELECTRON) {
		Double_t E0 = d->beamEnergy;
		Double_t M = MASS_P;
		Double_t p = d->p[0];
		Double_t nu = E0-p;
		Double_t pz = p*d->cz[0];
		Double_t Q2 = -(nu*nu - p*p - E0*E0 + 2*E0*pz);
		Double_t s = -Q2 + 2*M*nu + M*M;
		Double_t W = s >= 0 ? sqrt(s) : -sqrt(-s);
		hq2_V_w->Fill(W, Q2);
		if (d->npart==2 && d->id[1]==PROTON) {
			if (abs(-d->p[0]*d->cx[0]-d->p[1]*d->cx[1])<0.2
				&& abs(-d->p[0]*d->cy[0]-d->p[1]*d->cy[1])<0.2) {
				hq2_V_w_elast_exc->Fill(W, Q2);
			}
		}
		int npart = d->npart;
		int gpart = d->gpart;
		if (npart>2 && gpart<8) {
			int np, npip, npim, nothercharged;
			np = npip = npim = nothercharged = 0;
			int partsidx[] = {0, -1, -1, -1};
			for (int ipart = 1; ipart < npart; ipart++) {
				if (nothercharged>0) break;
				switch(d->id[ipart]) {
					case PROTON:
						np++;
						partsidx[1] = ipart;
						break;
					case PIP:
						npip++;
						partsidx[2] = ipart;
						break;
					case PIM:
						npim++;
						partsidx[3] = ipart;
						break;
					default:
						if (d->q[ipart] != 0) nothercharged++;
				}
			}
			if (np==1 && npip==1 && npim==1 && nothercharged==0) {
				lvE0->SetXYZT(0, 0, E0, E0);
				lvE1->SetXYZT(d->p[0]*d->cx[0], d->p[0]*d->cy[0], d->p[0]*d->cz[0], d->p[0]);
				int ipart = partsidx[1];
				lvP1->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_P);
				ipart = partsidx[2];
				lvPip->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_PIP);
				ipart = partsidx[3];
				lvPim->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_PIM);
				TLorentzVector lvMMP = (*lvE0 - *lvE1 + *lvP0 - *lvP1);
				TLorentzVector lvMMPPIPPIM = lvMMP - *lvPip - *lvPim;
				hmmppippim_V_mmp->Fill(lvMMP.M(), lvMMPPIPPIM.M());
			}
		}
	}
	return passed;
}

void DH_RunQuality::Wrapup(H10 *d)
{

}

void DH_RunQuality::Finalize(H10 *d)
{
	fDir->cd();
}
