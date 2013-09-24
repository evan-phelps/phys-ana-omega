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

DH_RunQuality::DH_RunQuality(std::string name, TDirectory *pDir) : DataHandler(name, pDir)
{
	fLb.lumblock_num = 0;
	Clear();
	firstfile = true;
	a00exists = true;
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
	lumblocks = new TTree("lumblocks", "luminosity-block information");
	lumblocks->Branch("file_name", fLb.file_name);

	lumblocks->Branch("run_num", &fLb.run_num);
	lumblocks->Branch("file_num", &fLb.file_num);
	lumblocks->Branch("lumblock_num", &fLb.lumblock_num);
	lumblocks->Branch("q_l_first", &fLb.q_l_first);
	lumblocks->Branch("q_l_last", &fLb.q_l_last);
	lumblocks->Branch("evt_num_first", &fLb.evt_num_first);
	lumblocks->Branch("evt_num_last", &fLb.evt_num_last);
	lumblocks->Branch("fc_charge_ltcorr", &fLb.fc_charge_ltcorr);
	lumblocks->Branch("livetime", &fLb.livetime);
	lumblocks->Branch("ntrigs", &fLb.ntrigs);
	lumblocks->Branch("ne", &fLb.ne);
	lumblocks->Branch("np", &fLb.np);
	lumblocks->Branch("npip", &fLb.npip);
	lumblocks->Branch("npim", &fLb.npim);
	lumblocks->Branch("nevts_Neg1st", &fLb.nevts_Neg1st);
	lumblocks->Branch("nevts_Neg1st_2Pos_exc", &fLb.nevts_Neg1st_2Pos_exc);
	lumblocks->Branch("nevts_Neg1st_1Pos1Neg_exc", &fLb.nevts_Neg1st_1Pos1Neg_exc);
	lumblocks->Branch("nevts_Neg1st_2Pos1Neg_exc", &fLb.nevts_Neg1st_2Pos1Neg_exc);
	lumblocks->Branch("hq2_V_w", hq2_V_w);
	lumblocks->Branch("hq2_V_w_elast_exc", hq2_V_w_elast_exc);
	lumblocks->Branch("hmmppippim_V_mmp", hmmppippim_V_mmp);
}

DH_RunQuality::~DH_RunQuality()
{
	delete hq2_V_w;
	delete hq2_V_w_elast_exc;
	delete hmmppippim_V_mmp;
	delete lumblocks;
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
	PrepBlock(d);
	FillHists(d);
	passed = passed & CountAll(d);  //count regardless of block
	passed = false;
	return passed;
}

void DH_RunQuality::Wrapup(H10 *d)
{

}

void DH_RunQuality::Finalize(H10 *d)
{
	FillPreviousBlock(d);
	fDir->cd();
	lumblocks->Write();
}

void DH_RunQuality::Clear() {
	memset(fLb.file_name, 0, 256);
	fLb.run_num = fLb.file_num = fLb.evt_num_first
		= fLb.evt_num_last = fLb.fc_charge_ltcorr = fLb.livetime = -1;
	fLb.ntrigs = fLb.ne = fLb.np = fLb.npip = fLb.npim
		= fLb.nevts_Neg1st = fLb.nevts_Neg1st_2Pos_exc
		= fLb.nevts_Neg1st_1Pos1Neg_exc
		= fLb.nevts_Neg1st_2Pos1Neg_exc = 0;
	 //= fLb.lumblock_num
}

bool DH_RunQuality::PrepBlock(H10 *d)
{
	bool passed = true;
	bool newrun = (fLb.run_num != d->run);
	bool newfile = (newrun || (fLb.file_num != d->file_anum));
	bool newblock = (newrun || (fQl_last < d->q_l));
	bool somethingswrong = (!newfile && (fQl_last > d->q_l));
	/* Assume:
	 * 	+ events are ordered in time and, therefore, by file Anumber and run;
	 *	+ q_l always changes if file changes (verified for e1f data, where
	 * 		leading events of each file have q_l = 0). */
	/* Reject events of unkown luminosity. In e1-f case, these
		result from missing A00 files, making the first non-zero q_l
		in A01 the first known reference for accumulated charge. */
	if (newrun) {
		/* set flag for first file in run */
		firstfile = true;
		/* does first file, A00, exist */
		if(d->file_anum != 0) {
			a00exists = false;
		} else {
			a00exists = true;
		}
	} else if(newfile) firstfile = false;
	if (newblock) {
		/* reject unknown-luminosity event */
		if ( firstfile && !a00exists && (d->q_l == 0) ) passed = false;
		else {
			if (!newrun) {
				FillPreviousBlock(d);
				/* reset lum block data */
				Clear();
			}
			/* ***************** HANDLE CURRENT BLOCK *********************** */
			std::copy(d->filename.begin(), d->filename.end(), fLb.file_name);
			fLb.file_name[d->filename.size()] = '\0';
			fLb.run_num = d->run;
			fLb.file_num = d->file_anum;
			fLb.lumblock_num++;
			fLb.q_l_first = d->q_l;
			fLb.q_l_last = -1;
			fLb.evt_num_first = d->evntid;
			fLb.evt_num_last = -1;
			fLb.fc_charge_ltcorr = -1;
			fLb.livetime = d->t_l;
			fQl_last = d->q_l;
			fAnum_last = d->file_anum;
			fRun_last = d->run;
			if (somethingswrong) {
				std::cerr << "WARNING! Drop in accumulated charge." << std::endl;
			}
		}
	}
	return passed;
}

void DH_RunQuality::FillHists(H10 *d)
{
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
			if (abs(-d->p[0]*d->cx[0]-d->p[1]*d->cx[1])<0.05
				&& abs(-d->p[0]*d->cy[0]-d->p[1]*d->cy[1])<0.05) {
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
}

bool DH_RunQuality::CountAll(H10 *d)
{
	fLb.ntrigs++;
	bool negfirst = d->q[0] < 0 ? true : false;
	int npos = 0, nneg = 0;
	for (int ipart = 0; ipart < d->gpart; ipart++) {
		switch(d->id[ipart]) {
			case ELECTRON:
				fLb.ne++;
				break;
			case PROTON:
				fLb.np++;
				break;
			case PIP:
				fLb.npip++;
				break;
			case PIM:
				fLb.npim++;
				break;
		}
		if (ipart > 0) {
			 npos += (d->q[ipart] > 0);
			 nneg += (d->q[ipart] < 0);
		}
	}
	fLb.nevts_Neg1st += negfirst;
	fLb.nevts_Neg1st_2Pos_exc += (negfirst && npos==2);
	fLb.nevts_Neg1st_1Pos1Neg_exc += (negfirst && npos==1 && nneg==1);
	fLb.nevts_Neg1st_2Pos1Neg_exc += (negfirst && npos==2 && nneg==1);

	bool passed = fLb.nevts_Neg1st_2Pos_exc
					|| fLb.nevts_Neg1st_1Pos1Neg_exc
					|| fLb.nevts_Neg1st_2Pos1Neg_exc;
	return passed;
}

void DH_RunQuality::FillPreviousBlock(H10 *d) {
	fLb.q_l_last = d->q_l;
	fLb.evt_num_last = d->evntid - 1;
	fLb.fc_charge_ltcorr = fLb.q_l_last - fLb.q_l_first;
	lumblocks->Fill();
}
