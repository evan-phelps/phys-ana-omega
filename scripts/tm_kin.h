#include "h10t3pi_sel.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include "particle-constants.h"
#include "scripts/xsect-utils.C"

using namespace TMath;
using namespace ParticleConstants;

class TmKin : public h10t3pi_sel
{
public:
    TTree *tkin;
    TFile *fout;
    struct _kin {
        Float_t s, t, u,
                W, Q2, ct, phi,
                mmp, mmppip, mmppim, mmppippim,
                t0, t1, xb, nu, eps, vgflux;
    } kin;
    /* lab-frame particle kinematics */
    struct _partkin {
        Float_t p, theta, phi;
    };
    _partkin parts[4];
    TmKin() : h10t3pi_sel() {
        printf("in TmKin::TmKin()\n");
    }
    ~TmKin() {
        printf("in TmKin::TmKin()\n");
        delete tkin;
        delete fout;
    }
    virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
        //Int_t retval = fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
        //if (t3pi) t3pi->GetEntry(entry, getall);
        Int_t retval = 0;
        retval += b_w4->GetEntry(entry);
        retval += b_q4->GetEntry(entry);
        retval += b_omega->GetEntry(entry);
        retval += b_p1->GetEntry(entry);
        retval += b_p0->GetEntry(entry);
        retval += b_mmppip->GetEntry(entry);
        retval += b_mmppim->GetEntry(entry);
        retval += b_pi0->GetEntry(entry);
        retval += b_le1->GetEntry(entry);
        retval += b_lp1->GetEntry(entry);
        retval += b_lpip->GetEntry(entry);
        retval += b_lpim->GetEntry(entry);
        retval += b_h10idx_e->GetEntry(entry);
        retval += b_h10idx_p->GetEntry(entry);
        retval += b_h10idx_pip->GetEntry(entry);
        retval += b_h10idx_pim->GetEntry(entry);
        return retval;
    }
    void Setup() {
        printf("in TmKin::Setup()\n");
        fout = new TFile("TmKin.root","recreate");
        tkin = new TTree("tkin","physics frame kinematics");
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
    void Process() {
        ClearVars();
        kin.s = w4->M2();
        kin.t = (*q4 - *omega).M2();
        kin.u = (*q4 - *p1).M2();
        kin.W = Sqrt(kin.s);
        kin.Q2 = -q4->M2();
        kin.ct = omega->CosTheta();
        kin.phi = omega->Phi();
        kin.mmp = omega->M();
        kin.mmppip = mmppip->M();
        kin.mmppim = mmppim->M();
        kin.mmppippim = pi0->M();
        kin.t0 = 2*(MASS_P*MASS_P - p0->E()*p1->E() + p0->P()*p1->P());
        kin.t1 = kin.t-kin.t0;
        kin.nu = epXsect::nu(kin.W,kin.Q2); //(*p0)*(*q4)/MASS_P;
        kin.xb = epXsect::xb(kin.W,kin.Q2); //kin.Q2/(2*MASS_P*kin.nu);
        kin.eps = epXsect::epsilon(kin.W,kin.Q2);
        kin.vgflux = epXsect::vgflux(kin.W,kin.Q2);
        TLorentzVector *lv[4] = { le1, lp1, lpip, lpim };
        int hidxs[4] = { h10idx_e, h10idx_p, h10idx_pip, h10idx_pim };
        //printf("%d\t%d\t%d\t%d\n",hidxs[0],hidxs[1],hidxs[2],hidxs[3]);
        for (int ilv = 0; ilv < 4; ilv++) {
            if (hidxs[ilv]>=0) {
                parts[ilv].p = lv[ilv]->P();
                parts[ilv].theta = lv[ilv]->Theta();
                parts[ilv].phi = lv[ilv]->Phi();
            }
        }
        tkin->Fill();
    }
    void Finalize() {
        printf("in TmKin::Finalize()\n");
        fout->Write();
    }
    void ClearVars() {
        kin.s = kin.t = kin.u = kin.W = kin.Q2 = kin.ct = kin.phi = kin.mmp = kin.mmppip = kin.mmppim = kin.mmppippim = kin.t0 = kin.t1 = kin.nu = kin.xb = kin.eps = kin.vgflux = 0.0;
        for (int i = 0; i < 4; i++) {
            parts[i].p = parts[i].theta = parts[i].phi = 0.0;
        }
    }
};
