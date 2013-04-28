#ifndef EVT_H
#define EVT_H

#include "TSystem.h"
#include "TLorentzRotation.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "data.h"
#include "particle-constants.h"
using namespace ParticleConstants;
using namespace TMath;
class Evt : public Data
{
protected:
	TRotation *_3rot;
	TVector3 *_boost;
	TLorentzRotation *_4rot;
public:
	TLorentzVector *_e0, *_p0, *_e1, *_p1, *_pip, *_pim, *_pi0;
	TLorentzVector *_w, *_q, *_omega, *_ppip, *_ppim, *_pippim, *_pippi0, *_pimpi0;
	TLorentzVector *_mmppip, *_mmppim;
	int h10idx_e, h10idx_p, h10idx_pip, h10idx_pim;
	TBranch *b_e0, *b_p0, *b_e1, *b_p1, *b_pip, *b_pim, *b_pi0, *b_w, *b_q, *b_omega,
	        *b_ppip, *b_ppim, *b_pippim, *b_pippi0, *b_pimpi0, *b_mmppip, *b_mmppim,
	        *b_h10idx_e, *b_h10idx_p, *b_h10idx_pip, *b_h10idx_pim;
	Evt() {
		gSystem->Load("libPhysics");
		TLorentzVector::Class()->IgnoreTObjectStreamer();
		TVector3::Class()->IgnoreTObjectStreamer();
		_3rot = 0;
		_4rot = 0;
		_boost = 0;
		_e0 = 0; //new TLorentzVector();
		_p0 = 0; //new TLorentzVector();
		_e1 = 0; //new TLorentzVector();
		_p1 = 0; //new TLorentzVector();
		_pip = 0; //new TLorentzVector();
		_pim = 0; //new TLorentzVector();
		_pi0 = 0; //new TLorentzVector();
		_w = 0; //new TLorentzVector();
		_q = 0; //new TLorentzVector();
		_omega = 0; //new TLorentzVector();
		_ppip = 0; //new TLorentzVector();
		_ppim = 0; //new TLorentzVector();
		_pippim = 0; //new TLorentzVector();
		_pippi0 = 0; //new TLorentzVector();
		_pimpi0 = 0; //new TLorentzVector();
		_mmppip = 0; //new TLorentzVector();
		_mmppim = 0; //new TLorentzVector();
	}
	virtual ~Evt() {
		if (_3rot) delete _3rot;
		if (_4rot) delete _4rot;
		if (_boost) delete _boost;
		if (_e0) delete _e0;
		if (_p0) delete _p0;
		if (_e1) delete _e1;
		if (_p1) delete _p1;
		if (_pip) delete _pip;
		if (_pim) delete _pim;
		if (_pi0) delete _pi0;
		if (_w) delete _w;
		if (_q) delete _q;
		if (_omega) delete _omega;
		if (_ppip) delete _ppip;
		if (_ppim) delete _ppim;
		if (_pippim) delete _pippim;
		if (_pippi0) delete _pippi0;
		if (_pimpi0) delete _pimpi0;
		if (_mmppip) delete _mmppip;
		if (_mmppim) delete _mmppim;
	}
	void CalcRot() {
		if (_3rot) delete _3rot;
		if (_4rot) delete _4rot;
		if (_boost) delete _boost;
		TVector3 uz = _q->Vect().Unit();
		TVector3 ux = (_e0->Vect().Cross(_e1->Vect())).Unit();
		ux.Rotate(-PiOver2(),uz);
		_3rot = new TRotation();
		_3rot->SetZAxis(uz,ux).Invert();
		//_w and _q are in z-direction
		_boost = new TVector3(-1*_w->BoostVector());
		_4rot = new TLorentzRotation(*_3rot); //*_boost);
		*_4rot *= *_boost; //*_3rot;
	}
	void Transform() {
		CalcRot();
		_e0->Transform(*_4rot);
		_p0->Transform(*_4rot);
		_e1->Transform(*_4rot);
		_p1->Transform(*_4rot);
		_pip->Transform(*_4rot);
		_pim->Transform(*_4rot);
		_pi0->Transform(*_4rot);
		_w->Transform(*_4rot);
		_q->Transform(*_4rot);
		_omega->Transform(*_4rot);
		_ppip->Transform(*_4rot);
		_ppim->Transform(*_4rot);
		_pippim->Transform(*_4rot);
		_pippi0->Transform(*_4rot);
		_pimpi0->Transform(*_4rot);
		_mmppip->Transform(*_4rot);
		_mmppim->Transform(*_4rot);
		//_e0->SetXYZT(0,0,E1F_E0,E1F_E0);
		//_p0->SetXYZT(0,0,0,MASS_P);
	}
	TTree *CreateTree(std::string treename) {
		TTree *t3pi = new TTree(treename.c_str(),"4-vectors for pip, pim, pi0 channel");
		_e0 = new TLorentzVector();
		_p0 = new TLorentzVector();
		_e0->SetXYZT(0,0,E1F_E0,E1F_E0);
		_p0->SetXYZT(0,0,0,MASS_P);
		_e1 = new TLorentzVector();
		_p1 = new TLorentzVector();
		_pip = new TLorentzVector();
		_pim = new TLorentzVector();
		_pi0 = new TLorentzVector();
		_w = new TLorentzVector();
		_q = new TLorentzVector();
		_omega = new TLorentzVector();
		_ppip = new TLorentzVector();
		_ppim = new TLorentzVector();
		_pippim = new TLorentzVector();
		_pippi0 = new TLorentzVector();
		_pimpi0 = new TLorentzVector();
		_mmppip = new TLorentzVector();
		_mmppim = new TLorentzVector();
		int splitlevel = 0;
		int bufsize = 16000;
		t3pi->Branch("w4","TLorentzVector",&_w, bufsize, splitlevel);
		t3pi->Branch("q4","TLorentzVector",&_q, bufsize, splitlevel);
		t3pi->Branch("e0","TLorentzVector",&_e0, bufsize, splitlevel);
		t3pi->Branch("e1","TLorentzVector",&_e1, bufsize, splitlevel);
		t3pi->Branch("p0","TLorentzVector",&_p0, bufsize, splitlevel);
		t3pi->Branch("p1","TLorentzVector",&_p1, bufsize, splitlevel);
		t3pi->Branch("pip","TLorentzVector",&_pip, bufsize, splitlevel);
		t3pi->Branch("pim","TLorentzVector",&_pim, bufsize, splitlevel);
		t3pi->Branch("pi0","TLorentzVector",&_pi0, bufsize, splitlevel);
		t3pi->Branch("omega","TLorentzVector",&_omega, bufsize, splitlevel);
		t3pi->Branch("ppip","TLorentzVector",&_ppip, bufsize, splitlevel);
		t3pi->Branch("ppim","TLorentzVector",&_ppim, bufsize, splitlevel);
		t3pi->Branch("pippim","TLorentzVector",&_pippim, bufsize, splitlevel);
		t3pi->Branch("pippi0","TLorentzVector",&_pippi0, bufsize, splitlevel);
		t3pi->Branch("pimpi0","TLorentzVector",&_pimpi0, bufsize, splitlevel);
		t3pi->Branch("mmppip","TLorentzVector",&_mmppip, bufsize, splitlevel);
		t3pi->Branch("mmppim","TLorentzVector",&_mmppim, bufsize, splitlevel);
		t3pi->Branch("h10idx_e",&h10idx_e, bufsize, splitlevel);
		t3pi->Branch("h10idx_p",&h10idx_p, bufsize, splitlevel);
		t3pi->Branch("h10idx_pip",&h10idx_pip, bufsize, splitlevel);
		t3pi->Branch("h10idx_pim",&h10idx_pim, bufsize, splitlevel);
		return t3pi;
	}
	virtual bool Bind(TTree *fChain, int cachesize = -1) {
		fChain->SetBranchAddress("w4",&_w, &b_w);
		fChain->SetBranchAddress("q4",&_q, &b_q);
		fChain->SetBranchAddress("e0",&_e0, &b_e0);
		fChain->SetBranchAddress("p0",&_p0, &b_p0);
		fChain->SetBranchAddress("p1",&_p1, &b_p1);
		fChain->SetBranchAddress("e1",&_e1, &b_e1);
		fChain->SetBranchAddress("pip",&_pip, &b_pip);
		fChain->SetBranchAddress("pim",&_pim, &b_pim);
		fChain->SetBranchAddress("pi0",&_pi0, &b_pi0);
		fChain->SetBranchAddress("omega",&_omega, &b_omega);
		fChain->SetBranchAddress("ppip",&_ppip, &b_ppip);
		fChain->SetBranchAddress("ppim",&_ppim, &b_ppim);
		fChain->SetBranchAddress("pippim",&_pippim, &b_pippim);
		fChain->SetBranchAddress("pippi0",&_pippi0, &b_pippi0);
		fChain->SetBranchAddress("pimpi0",&_pimpi0, &b_pimpi0);
		fChain->SetBranchAddress("mmppip",&_mmppip, &b_mmppip);
		fChain->SetBranchAddress("mmppim",&_mmppim, &b_mmppim);
		fChain->SetBranchAddress("h10idx_e",&h10idx_e, &b_h10idx_e);
		fChain->SetBranchAddress("h10idx_p",&h10idx_p, &b_h10idx_p);
		fChain->SetBranchAddress("h10idx_pip",&h10idx_pip, &b_h10idx_pip);
		fChain->SetBranchAddress("h10idx_pim",&h10idx_pim, &b_h10idx_pim);
		fChain->SetBranchStatus("*",1);
		return true;
	}
	virtual bool CheapPop(Long64_t ientry) const {
		b_e0->GetEntry(ientry);
		b_p0->GetEntry(ientry);
		b_e1->GetEntry(ientry);
		b_p1->GetEntry(ientry);
		b_pip->GetEntry(ientry);
		b_pim->GetEntry(ientry);
		b_pi0->GetEntry(ientry);
		b_w->GetEntry(ientry);
		b_q->GetEntry(ientry);
		b_omega->GetEntry(ientry);
		b_ppip->GetEntry(ientry);
		b_ppim->GetEntry(ientry);
		b_pippim->GetEntry(ientry);
		b_pippi0->GetEntry(ientry);
		b_pimpi0->GetEntry(ientry);
		b_mmppip->GetEntry(ientry);
		b_mmppim->GetEntry(ientry);
		b_h10idx_e->GetEntry(ientry);
		b_h10idx_p->GetEntry(ientry);
		b_h10idx_pip->GetEntry(ientry);
		b_h10idx_pim->GetEntry(ientry);
		return true;
	}
	virtual bool BigPop(Long64_t ientry) const {
		return true;
	}
};

#endif // EVT_H
