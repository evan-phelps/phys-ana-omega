#ifndef DHTM3PI_H
#define DHTM3PI_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TTree.h"
#include "TEntryList.h"
#include "TLorentzVector.h"
#include "particle-constants.h"
#include "evt.h"
#include "scripts/xsect-utils.C"
using namespace ParticleConstants;
template <class T_, bool MC_ = false>
class DhTm3pi : public DataHandler
{
public:
  TTree *t3pi, *t3pi_physframe, *tkin;
  TEntryList *_elist;
  Long64_t _evtnum;
  Evt *e3pi;
  struct _kin {
    Float_t s, t, u,
      W, Q2, ct, phi,
      mmp, mmppip, mmppim, mmppippim,
      t0, t1, xb, nu, eps, vgflux;
  } kin;
  struct _partkin {
    Float_t p, theta, phi;
  };
  _partkin parts[4];

  TDirectory *fDir;
  DhTm3pi(std::string name = "3pi-tree", TDirectory *pDir = NULL) : DataHandler(name) {
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
    _evtnum = -1;
    fDir->cd();
    e3pi = new Evt();
    t3pi = e3pi->CreateTree("t3pi");
    t3pi_physframe = e3pi->CreateTree("t3pi_physfrm");
    _elist = new TEntryList("elist","t3pi elist",t3pi);
    tkin = new TTree("tkin","scalar kinematics");
    tkin->Branch("s",&kin.s);
    tkin->Branch("t",&kin.t);
    tkin->Branch("u",&kin.u);
    tkin->Branch("W",&kin.W);
    tkin->Branch("Q2",&kin.Q2);
    tkin->Branch("ct",&kin.ct);
    tkin->Branch("phi",&kin.phi);
    tkin->Branch("mmp",&kin.mmp);
    tkin->Branch("mmppip",&kin.mmppip);
    tkin->Branch("mmppim",&kin.mmppim);
    tkin->Branch("mmppippim",&kin.mmppippim);
    tkin->Branch("t0",&kin.t0);
    tkin->Branch("t1",&kin.t1);
    tkin->Branch("xb",&kin.xb);
    tkin->Branch("nu",&kin.nu);
    tkin->Branch("eps",&kin.eps);
    tkin->Branch("vgflux",&kin.vgflux);
    tkin->Branch("e",&parts[0].p,"p:theta:phi");
    tkin->Branch("p",&parts[1].p,"p:theta:phi");
    tkin->Branch("pip",&parts[2].p,"p:theta:phi");
    tkin->Branch("pim",&parts[3].p,"p:theta:phi");
  }
  virtual ~DhTm3pi() {
    delete _elist;
    delete t3pi;
    delete t3pi_physframe;
    delete e3pi;
    delete tkin;
    delete fDir;
  }
public:
  virtual void Finalize(Data* d) {
    fDir->cd();
    t3pi->Write();
    _elist->Write();
    t3pi_physframe->Write();
    tkin->Write();
  }
  virtual bool Handle(Data* data) {
    T_ *d = dynamic_cast<T_*>(data);
    bool passed = false;
    e3pi->h10idx_e = -1, e3pi->h10idx_p = -1, e3pi->h10idx_pip = -1, e3pi->h10idx_pim = -1;
    e3pi->_e0->SetXYZT(0,0,E1F_E0,E1F_E0);
    e3pi->_p0->SetXYZT(0,0,0,MASS_P);
    TLorentzVector *_lvtmp;
    int gpart = (MC_ ? d->mcnpart : d->gpart);
    for (int i = 0; i < gpart; i++) {
      int pid = (MC_ ? d->mcid[i] : d->id[i]);
      _lvtmp = 0;
      switch(pid) {
      case ELECTRON: _lvtmp = e3pi->_e1; e3pi->h10idx_e = i; break;
      case PROTON: _lvtmp = e3pi->_p1; e3pi->h10idx_p = i; break;
      case PIP: _lvtmp = e3pi->_pip; e3pi->h10idx_pip = i; break;
      case PIM: _lvtmp = e3pi->_pim; e3pi->h10idx_pim = i; break;
      default: break;
      }
      if (_lvtmp > 0) {
	float p = (MC_ ? d->mcp[i] : d->p[i]);
	float px = p * (MC_ ? sin(D2R*d->mctheta[i])*cos(D2R*d->mcphi[i]) : d->cx[i]);
	float py = p * (MC_ ? sin(D2R*d->mctheta[i])*sin(D2R*d->mcphi[i]) : d->cy[i]);
	float pz = p * (MC_ ? cos(D2R*d->mctheta[i]) : d->cz[i]);
	float m = GetPdgMass(pid);
	_lvtmp->SetXYZM(px,py,pz,m);
      }			
    }
    if (e3pi->h10idx_e > -1 && e3pi->h10idx_p > -1) {
      *(e3pi->_q) = *(e3pi->_e0);
      *(e3pi->_q) -= *(e3pi->_e1);
      *(e3pi->_w) = *(e3pi->_q);
      *(e3pi->_w) += *(e3pi->_p0);
      *(e3pi->_omega) = *(e3pi->_w);
      *(e3pi->_omega) -= *(e3pi->_p1);
      *(e3pi->_ppip) = *(e3pi->_p1);
      *(e3pi->_ppip) += *(e3pi->_pip);
      *(e3pi->_ppim) = *(e3pi->_p1);
      *(e3pi->_ppim) += *(e3pi->_pim);
      *(e3pi->_pippim) = *(e3pi->_pip);
      *(e3pi->_pippim) += *(e3pi->_pim);
      *(e3pi->_mmppip) = *(e3pi->_w);
      *(e3pi->_mmppip) -= *(e3pi->_ppip);
      *(e3pi->_mmppim) = *(e3pi->_w);
      *(e3pi->_mmppim) -= *(e3pi->_ppim);
      if (e3pi->h10idx_pip < 0 || e3pi->h10idx_pim < 0) {
	if (e3pi->h10idx_pip < 0) {
	  e3pi->_pip->SetXYZT(0,0,0,0);
	  e3pi->_ppip->SetXYZT(0,0,0,0);
	  e3pi->_mmppip->SetXYZT(0,0,0,0);
	}
	if (e3pi->h10idx_pim < 0) {
	  e3pi->_pim->SetXYZT(0,0,0,0);
	  e3pi->_ppim->SetXYZT(0,0,0,0);
	  e3pi->_mmppim->SetXYZT(0,0,0,0);
	}
	e3pi->_pi0->SetXYZT(0,0,0,0);
	e3pi->_pippi0->SetXYZT(0,0,0,0);
	e3pi->_pimpi0->SetXYZT(0,0,0,0);
      } else {
	*(e3pi->_pi0) = *(e3pi->_omega);
	*(e3pi->_pi0) -= *(e3pi->_pip);
	*(e3pi->_pi0) -= *(e3pi->_pim);
	*(e3pi->_pippi0) = *(e3pi->_pi0);
	*(e3pi->_pippi0) += *(e3pi->_pip);
	*(e3pi->_pimpi0) = *(e3pi->_pi0);
	*(e3pi->_pimpi0) += *(e3pi->_pim);
      }
      t3pi->Fill();

      ClearVars();
      TLorentzVector *lv[4] = { e3pi->_e1, e3pi->_p1, e3pi->_pip, e3pi->_pim };
      int hidxs[4] = { e3pi->h10idx_e, e3pi->h10idx_p, e3pi->h10idx_pip, e3pi->h10idx_pim };
      //printf("%d\t%d\t%d\t%d\n",hidxs[0],hidxs[1],hidxs[2],hidxs[3]);
      for (int ilv = 0; ilv < 4; ilv++) {
	if (hidxs[ilv]>=0) {
	  parts[ilv].p = lv[ilv]->P();
	  parts[ilv].theta = lv[ilv]->Theta();
	  parts[ilv].phi = lv[ilv]->Phi();
	}
      }

      e3pi->Transform();

      kin.s = e3pi->_w->M2();
      kin.t = (*e3pi->_q - *e3pi->_omega).M2();
      kin.u = (*e3pi->_q - *e3pi->_p1).M2();
      kin.W = Sqrt(kin.s);
      kin.Q2 = -e3pi->_q->M2();
      kin.ct = e3pi->_omega->CosTheta();
      kin.phi = e3pi->_omega->Phi();
      kin.mmp = e3pi->_omega->M();
      kin.mmppip = e3pi->_mmppip->M();
      kin.mmppim = e3pi->_mmppim->M();
      kin.mmppippim = e3pi->_pi0->M();
      kin.t0 = 2*(MASS_P*MASS_P - e3pi->_p0->E()*e3pi->_p1->E() + e3pi->_p0->P()*e3pi->_p1->P());
      kin.t1 = kin.t-kin.t0;
      kin.nu = epXsect::nu(kin.W,kin.Q2); //(*p0)*(*q4)/MASS_P;
      kin.xb = epXsect::xb(kin.W,kin.Q2); //kin.Q2/(2*MASS_P*kin.nu);
      kin.eps = epXsect::epsilon(kin.W,kin.Q2);
      kin.vgflux = epXsect::vgflux(kin.W,kin.Q2);
      tkin->Fill();

      t3pi_physframe->Fill();
      ++_evtnum;
      passed = true;
    } else passed = false;
    return passed;
  }
  virtual void Wrapup(Data* d) {
    _elist->Enter(_evtnum);
  }
  void ClearVars() {
    kin.s = kin.t = kin.u = kin.W = kin.Q2 = kin.ct = kin.phi = kin.mmp = kin.mmppip = kin.mmppim = kin.mmppippim = kin.t0 = kin.t1 = kin.nu = kin.xb = kin.eps = kin.vgflux = 0.0;
    for (int i = 0; i < 4; i++) {
      parts[i].p = parts[i].theta = parts[i].phi = 0.0;
    }
  }
};
#endif // DHTM3PI_H
