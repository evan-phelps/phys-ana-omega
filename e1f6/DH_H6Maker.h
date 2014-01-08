#ifndef DHH6MAKER_H
#define DHH6MAKER_H
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>

#include "THnSparse.h"
#include "TMath.h"
#include "TLorentzRotation.h"
#include "TRotation.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#ifndef H10_h
#include "H10.h"
#endif
class H10;

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

using namespace H10Constants;
using namespace TMath;

class DH_H6Maker : public DataHandler
{
    public:
        THnSparseF *hbd;
        DH_H6Maker(std::string name = "H6Maker", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            hbd = GetH6();
        }
        virtual ~DH_H6Maker()
        {
            delete hbd;
        }
        static THnSparseF* GetH6(std::string hname = "hbd_yield", std::string htitle = "W, Q^{2}, t', cos(#theta), #phi, mmp")
        {
            Int_t bins2[] = { 80, 6, 8, 12, 18, 35 };
            Double_t xmin2[] = { 1.6, 1.35, 0, -1, -Pi(), 0.6 };
            Double_t xmax2[] = { 3.2, 5.5, 8,  1,  Pi(), 0.95 };
            THnSparseF *hbd = new THnSparseF(hname.c_str(), htitle.c_str(), 6, bins2, xmin2, xmax2);
            Double_t qbins[] = { 1.35, 1.5, 1.75, 2.1, 2.89, 4, 5.5 };
            Double_t tbins[] = { 0.1, 0.25, 0.45, 0.65, 0.85, 1.15, 1.5, 2.06, 3, 8 };
            Double_t cbins[] = { -1,-0.9,-0.8,-0.6,-0.4,-0.2,-0.0,0.2,0.4,0.6,0.8,0.9,1 };
            hbd->SetBinEdges(1,qbins);
            hbd->SetBinEdges(2,tbins);
            hbd->SetBinEdges(3,cbins);
            hbd->Sumw2();
            return hbd;
        }
        virtual void Fill(H10 *d)
        {
            double bincoords[] = {d->W, d->Q2, -d->t1, d->cosTheta, d->phi, d->MMp};
            hbd->Fill(bincoords);
        }
        virtual void Setup(H10 *d)
        {
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            hbd->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};

class DH_H6Maker_Exp : public DH_H6Maker
{
    public:
        DH_H6Maker_Exp(std::string name = "H6Maker_Exp", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DH_H6Maker(name, pDir, h10looper)
        {
        }
        virtual ~DH_H6Maker_Exp()
        {
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            Fill(d);
            return passed;
        }
};

class DH_H6Maker_Recon : public DH_H6Maker
{
    public:
        DH_H6Maker_Recon(std::string name = "H6Maker_Recon", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DH_H6Maker(name, pDir, h10looper)
        {
        }
        virtual ~DH_H6Maker_Recon()
        {
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            Fill(d);
            return passed;
        }
};

class DH_H6Maker_Thrown : public DH_H6Maker
{
    public:
        TLorentzVector  lvE0, lvE1, lvP0, lvP1, lvPip, lvPim, lvW, lvq, lvMMp;
        DH_H6Maker_Thrown(std::string name = "H6Maker_Thrown", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DH_H6Maker(name, pDir, h10looper)
        {
        }
        virtual ~DH_H6Maker_Thrown()
        {
        }
        virtual void Setup(H10 *d)
        {
            lvE0 = d->lvE0;
            lvP0 = d->lvP0;
        }
        void Pop4P(H10* d, int pid, TLorentzVector &lv)
        {
            int ipart = 0;
            for ( ; ipart < d->mcnpart; ipart++) {
                if (pid == d->mcid[ipart]) break;
            }
            float theta = DegToRad()*d->mctheta[ipart];
            float mcphi = DegToRad()*d->mcphi[ipart];  //mcphi to avoid shadowing phi
            float p = d->mcp[ipart];
            float m = d->mcm[ipart];
            float px = p*sin(theta)*cos(mcphi);
            float py = p*sin(theta)*sin(mcphi);
            float pz = p*cos(theta);
            lv.SetXYZM(px,py,pz,m);
        }
        virtual bool Handle(H10* d)
        {
            Pop4P(d, ELECTRON, lvE1);
            Pop4P(d, PROTON, lvP1);
            Pop4P(d, PIP, lvPip);
            Pop4P(d, PIM, lvPim);

            lvq = lvE0-lvE1;
            lvW = lvq+lvP0;
            lvMMp = lvW-lvP1;

            double cosTheta, phi, t, t0, t1, W, Q2, MMp;
            W = lvW.M();
            Q2 = -lvq.M2();
            MMp = lvMMp.M();
            //4-rotate into boosted frame and get CM variables
            TVector3 uz = lvq.Vect().Unit();
            TVector3 ux = (lvE0.Vect().Cross(lvE1.Vect())).Unit();
            ux.Rotate(-PI/2,uz);
            TRotation _3rot;
            _3rot.SetZAxis(uz,ux).Invert();
            TLorentzRotation _4rot(_3rot);
            TVector3 _boost(-1*lvW.BoostVector());
            _4rot *= _boost;
            TLorentzVector _mmp = lvMMp;
            _mmp.Transform(_4rot);
            cosTheta = _mmp.CosTheta();
            phi = _mmp.Phi();
            //printf("(%.2f, %.2f)\n", phi, cosTheta);
            t = (lvP1-lvP0).M2();
            TLorentzVector _p0 = lvP0;
            TLorentzVector _p1 = lvP1;
            _p0.Transform(_4rot);
            _p1.Transform(_4rot);
            t0 = pow(_p0.E()-_p1.E(),2)-pow(_p0.P()-_p1.P(),2);
            t1 = t-t0;

            double bincoords[] = {W, Q2, -t1, cosTheta, phi, MMp};
            hbd->Fill(bincoords);

            return true;
        }
};
#endif                           // DHTM10CLONE_H
