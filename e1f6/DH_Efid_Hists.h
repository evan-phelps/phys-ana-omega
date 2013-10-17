#ifndef DH_EFID_HISTS_H_
#define DH_EFID_HISTS_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <TMath.h>

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
using namespace std;
using namespace TMath;

class DH_Efid_Hists : public DataHandler
{
    public:
        vector<TH2*> hsf_V_u;
        vector<TH2*> hsf_V_v;
        vector<TH2*> hsf_V_w;
        vector< vector<TH2*> > hangpS;
        static const int NPBINS = 43;   //683-4983, 100 MeV bins
        static const int PLO = 683;
        static const int PHI = 4983;
        int GetPbin(float p)
        {
            int pbin = (p-683)/100;
            return pbin;
        }
        DH_Efid_Hists(std::string name = "DH_Efid_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hsf_V_u = MakeHists(NSECTS, "hsf_V_u_%d", "SF vs. EC U, sector %d",
                450, 0.0, 450.0, 100, 0.0, 0.5);
            hsf_V_v = MakeHists(NSECTS, "hsf_V_v_%d", "SF vs. EC V, sector %d",
                450, 0.0, 450.0, 100, 0.0, 0.5);
            hsf_V_w = MakeHists(NSECTS, "hsf_V_w_%d", "SF vs. EC W, sector %d",
                450, 0.0, 450.0, 100, 0.0, 0.5);
            hangpS.resize(NPBINS);
            printf("resized!");
            for (int ipbin = 0; ipbin < NPBINS; ipbin++) {
                TString title = TString::Format("electron angles, p = [%d,%d] MeV, sector %s", PLO+100*ipbin, PLO+100*(ipbin+1), "%d");
                TString name = TString::Format("heang_%d_%d_s%s", PLO+100*ipbin, PLO+100*(ipbin+1), "%d");
                hangpS[ipbin] = MakeHists(NSECTS, name.Data(), title.Data(), 60, 0, 60, 240, -30, 30);
                printf("%d\t", ipbin);
            }
            printf("\n");
        }
        virtual ~DH_Efid_Hists()
        {
            fDir->cd();
            for_each(hsf_V_u.begin(), hsf_V_u.end(), DeleteObj);
            for_each(hsf_V_v.begin(), hsf_V_v.end(), DeleteObj);
            for_each(hsf_V_w.begin(), hsf_V_w.end(), DeleteObj);
            for (int ipbin = 0; ipbin < NPBINS; ipbin++) {
                for_each(hangpS[ipbin].begin(), hangpS[ipbin].end(), DeleteObj);
            }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hsf_V_u.begin(), hsf_V_u.end(), WriteObj);
            for_each(hsf_V_v.begin(), hsf_V_v.end(), WriteObj);
            for_each(hsf_V_w.begin(), hsf_V_w.end(), WriteObj);
            for (int ipbin = 0; ipbin < NPBINS; ipbin++) {
                for_each(hangpS[ipbin].begin(), hangpS[ipbin].end(), WriteObj);
            }
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            int ecidx = d->ec[0]-1;
            if (ecidx>=0) {
                double uvw[3];
                double xyz[3] = { d->ech_x[ecidx], d->ech_y[ecidx], d->ech_z[ecidx] };
                double sf = d->etot[ecidx]/d->p[0];
                int isec = d->ec_sect[ecidx]-1;
                d->GetUVW(xyz, uvw);
                hsf_V_u[isec]->Fill(uvw[0], sf);
                hsf_V_v[isec]->Fill(uvw[1], sf);
                hsf_V_w[isec]->Fill(uvw[2], sf);
                float theta = RadToDeg()*ACos(d->cz[0]);
                float phi = RadToDeg()*ATan2(d->cy[0],d->cx[0]);
                if (phi < -30) phi+=360;
                phi -= (d->sector-1)*60;
                float p = 1000*d->p[0];
                //printf("%.1f\t%d\n", p, GetPbin(p));
                //printf("%.3f, %.3f\n", theta, phi);
                if (p < PHI && p > PLO) hangpS[GetPbin(p)][d->sector-1]->Fill(theta, phi);
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EFID_HISTS_H_
