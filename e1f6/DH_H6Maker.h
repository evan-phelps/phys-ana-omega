#ifndef DHH6MAKER_H
#define DHH6MAKER_H
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <math.h>

#include "THnSparse.h"
#include "TMath.h"
#include "TLorentzRotation.h"
#include "TRotation.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TString.h"
#include "TGraph.h"

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
        THnSparseF *hbd_nphe_eff;
        static const int NPHE_MIN = 30;
        vector< vector<float> > parms_cc_nphe;
        TString fn_shape_sim;
        TH1 *shape_sim_norm_Q2;
        THnSparseF *hbd_shape_norm_Q2;
        TGraph *h_nbins;
        static const Long64_t BSIZE = 1000000;
        static const int NPOINTS = 100;
        Long64_t last_processed;
        int last_point_num;
        TGraph *g_relerr_mean;

        DH_H6Maker(std::string name = "H6Maker", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            hbd = GetH6();
            hbd_nphe_eff = GetH6("hbd_nphe_eff", "nphe cutoff efficiency");
            hbd_shape_norm_Q2 = GetH6("hbd_shape_norm_Q2", "first-order simulation shaping, Q2 only");
            h_nbins = new TGraph(100);
            last_processed = 0;
            last_point_num = 0;
            g_relerr_mean = new TGraph(100);
        }
        virtual ~DH_H6Maker()
        {
            delete hbd;
            if (hbd_nphe_eff) delete hbd_nphe_eff;
            if (shape_sim_norm_Q2) delete shape_sim_norm_Q2;
            if (hbd_shape_norm_Q2) delete hbd_shape_norm_Q2;
            delete h_nbins;
        }
        virtual void Setup(H10 *d)
        {
            parms_cc_nphe = d->cfg->GetSectorParms("cc_nphe");
            fn_shape_sim = d->cfg->GetString("shape_thrown_Q2");
            shape_sim_norm_Q2
            if ( !fn_shape_sim.IsNull() ) {
                TFile fin(fn_shape_sim.Data());
                shape_sim_norm_Q2 = (TH1*)fin.Get("hQ2_norm");
                shape_sim_norm_Q2->SetDirectory(fDir);
            }
        }
        static THnSparseF* GetH6(std::string hname = "hbd_yield", std::string htitle = "W, Q^{2}, t', cos(#theta), #phi, mmp")
        {
            /* NOTE that Q2 needs to be binned so that the following values fall on edges:
                    1.75, 2.25, 2.75, 3.25, 3.75, 4.25, and 4.75
             */
            Int_t bins2[] = { 80, 110, 9, 10, 18, 35 };
            Double_t xmin2[] = { 1.6, 0.75, 0.1, -1, -Pi(), 0.6 };
            Double_t xmax2[] = { 3.2, 6.25, 8,  1,  Pi(), 0.95 };
            THnSparseF *hbd = new THnSparseF(hname.c_str(), htitle.c_str(), 6, bins2, xmin2, xmax2);
            Double_t tbins[] = { 0.1, 0.25, 0.45, 0.65, 0.85, 1.15, 1.5, 2.06, 3, 8 };
            hbd->SetBinEdges(2,tbins);
            hbd->Sumw2();
            return hbd;
            // Int_t bins2[] = { 80, 6, 9, 12, 18, 35 };
            // Double_t xmin2[] = { 1.6, 1.35, 0.1, -1, -Pi(), 0.6 };
            // Double_t xmax2[] = { 3.2, 5.5, 8,  1,  Pi(), 0.95 };
            // THnSparseF *hbd = new THnSparseF(hname.c_str(), htitle.c_str(), 6, bins2, xmin2, xmax2);
            // Double_t qbins[] = { 1.35, 1.5, 1.75, 2.1, 2.89, 4, 5.5 };
            // Double_t tbins[] = { 0.1, 0.25, 0.45, 0.65, 0.85, 1.15, 1.5, 2.06, 3, 8 };
            // Double_t cbins[] = { -1,-0.9,-0.8,-0.6,-0.4,-0.2,-0.0,0.2,0.4,0.6,0.8,0.9,1 };
            // hbd->SetBinEdges(1,qbins);
            // hbd->SetBinEdges(2,tbins);
            // hbd->SetBinEdges(3,cbins);
            // hbd->Sumw2();
            // return hbd;
        }
        virtual void Fill(H10 *d, Double_t weight=1, bool do_cc_eff=false)
        {
            double bincoords[] = {d->W, d->Q2, -d->t, d->cosTheta, d->phi, d->MMp};
            hbd->Fill(bincoords, weight);

            if (do_cc_eff) {
                double cc_eff = 1;
                int ccidx = d->cc[0]-1;
                int pmt_hit = d->cc_segm[ccidx]/1000-1;
                if (pmt_hit == 0) cc_eff = 1;
                else {
                    int segment = (d->cc_segm[ccidx]%1000)/10;
                    int sector = d->cc_sect[ccidx];
                    int pmt_num = (2*segment-1);
                    if (pmt_hit>0) pmt_num++;
                    cc_eff = parms_cc_nphe[sector-1][pmt_num-1];
                }
                hbd_nphe_eff->Fill(bincoords, cc_eff);
            }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            hbd->Write();
            if (hbd_nphe_eff) {
                hbd_nphe_eff->Divide(hbd);
                hbd_nphe_eff->Write();
            }
            for (int ipoint=last_point_num; ipoint < h_nbins->GetMaxSize(); ipoint++) {
                g_relerr_mean->RemovePoint(ipoint);
                h_nbins->RemovePoint(ipoint);
            }
            fDir->WriteObject(h_nbins, "h_nbins");
            fDir->WriteObject(g_relerr_mean, "g_relerr_mean");
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
        virtual void Finalize(H10* d)
        {
            DH_H6Maker::Finalize(d);
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            Fill(d, 1, true);
            if (d->eventnum/(BSIZE+last_processed)>0) {
                last_processed += BSIZE;
                if (last_point_num > h_nbins->GetMaxSize())
                    h_nbins->Expand(last_point_num+NPOINTS);
                    g_relerr_mean->Expand(last_point_num+NPOINTS);
                Int_t dims[] = {0, 1, 3, 4};
                THnSparse *h4 = hbd->Projection(4, dims);
                h_nbins->SetPoint(last_point_num, d->eventnum, h4->GetNbins());
                TH1 *hrelerr_tmp = new TH1D("htemp", "htemp", 1000, 0, 1);
                for (int ibin=0; ibin<h4->GetNbins(); ibin++) {
                    hrelerr_tmp->Fill(h4->GetBinError(ibin)/h4->GetBinContent(ibin));
                }
                g_relerr_mean->SetPoint(last_point_num++, d->eventnum, hrelerr_tmp->GetMean());
                delete hrelerr_tmp;
                delete h4;
            }
            return passed;
        }
};

class DH_H6Maker_Recon : public DH_H6Maker
{
    public:
        TLorentzVector  lvE0, lvE1, lvP0;
        DH_H6Maker_Recon(std::string name = "H6Maker_Recon", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DH_H6Maker(name, pDir, h10looper)
        {
        }
        virtual ~DH_H6Maker_Recon()
        {
        }
        virtual void Setup(H10 *d)
        {
            DH_H6Maker::Setup(d);
            lvE0 = d->lvE0;
            lvP0 = d->lvP0;
        }
        virtual void Finalize(H10* d)
        {
            DH_H6Maker::Finalize(d);
            fDir->cd();
            if (hbd_shape_norm_Q2) {
                hbd_shape_norm_Q2->Divide(hbd);
                hbd_shape_norm_Q2->Write();
            }
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
            bool passed = true;
            Fill(d);
            if (shape_sim_norm_Q2) {
                Pop4P(d, ELECTRON, lvE1);
                TLorentzVector lvq = lvE0-lvE1;
                Double_t Q2 = -lvq.M2();
                Int_t q2bin = shape_sim_norm_Q2->FindBin(Q2);
                Double_t invw = shape_sim_norm_Q2->GetBinContent(q2bin);
                // Double_t weight = invw==0 ? 0 : 1/invw;
                double bincoords[] = {d->W, d->Q2, -d->t, d->cosTheta, d->phi, d->MMp};
                hbd_shape_norm_Q2->Fill(bincoords, invw);
            }
            if (d->eventnum/(BSIZE+last_processed)>0) {
                last_processed += BSIZE;
                if (last_point_num > h_nbins->GetMaxSize())
                    h_nbins->Expand(last_point_num+NPOINTS);
                    g_relerr_mean->Expand(last_point_num+NPOINTS);
                Int_t dims[] = {0, 1, 3, 4};
                THnSparse *h4 = hbd->Projection(4, dims);
                h_nbins->SetPoint(last_point_num, d->eventnum, h4->GetNbins());
                TH1 *hrelerr_tmp = new TH1D("htemp", "htemp", 1000, 0, 1);
                for (int ibin=0; ibin<h4->GetNbins(); ibin++) {
                    hrelerr_tmp->Fill(h4->GetBinError(ibin)/h4->GetBinContent(ibin));
                }
                g_relerr_mean->SetPoint(last_point_num++, d->eventnum, hrelerr_tmp->GetMean());
                delete hrelerr_tmp;
                delete h4;
            }
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
            DH_H6Maker::Setup(d);
            lvE0 = d->lvE0;
            lvP0 = d->lvP0;
        }
        virtual void Finalize(H10* d)
        {
            DH_H6Maker::Finalize(d);
            fDir->cd();
            if (hbd_shape_norm_Q2) {
                hbd_shape_norm_Q2->Divide(hbd);
                hbd_shape_norm_Q2->Write();
            }
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

            if (shape_sim_norm_Q2) {
                Int_t q2bin = shape_sim_norm_Q2->FindBin(Q2);
                Double_t invw = shape_sim_norm_Q2->GetBinContent(q2bin);
                // Double_t weight = invw==0 ? 0 : 1/invw;
                hbd_shape_norm_Q2->Fill(bincoords, invw);
            }
            if (d->eventnum/(BSIZE+last_processed)>0) {
                last_processed += BSIZE;
                if (last_point_num > h_nbins->GetMaxSize())
                    h_nbins->Expand(last_point_num+NPOINTS);
                    g_relerr_mean->Expand(last_point_num+NPOINTS);
                Int_t dims[] = {0, 1, 3, 4};
                THnSparse *h4 = hbd->Projection(4, dims);
                h_nbins->SetPoint(last_point_num, d->eventnum, h4->GetNbins());
                TH1 *hrelerr_tmp = new TH1D("htemp", "htemp", 1000, 0, 1);
                for (int ibin=0; ibin<h4->GetNbins(); ibin++) {
                    hrelerr_tmp->Fill(h4->GetBinError(ibin)/h4->GetBinContent(ibin));
                }
                g_relerr_mean->SetPoint(last_point_num++, d->eventnum, hrelerr_tmp->GetMean());
                delete hrelerr_tmp;
                delete h4;
            }
            return true;
        }
};
#endif                           // DHTM10CLONE_H
