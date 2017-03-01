#ifndef DH_HFID_H_
#define DH_HFID_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

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

#include <TMath.h>
#include <TF1.h>

using namespace H10Constants;
using namespace std;
using namespace TMath;

class DH_Hfid : public DataHandler
{
    public:
        float tightness;
        vector< vector<float> > parms_hfid_t0;
        vector< vector<float> > parms_hfid_F;
        vector< vector<float> > parms_hfid_b;
        long npos = 0, npos_passed = 0, npos_changed = 0;

        double Dphi(float t, int sect)
        {
            float f = parms_hfid_F[sect-1][0];
            float b = parms_hfid_b[sect-1][0];
            float t0 = parms_hfid_t0[sect-1][0];
            return f*(1-Exp(b*(t-t0)))-tightness;
        }
        DH_Hfid(std::string name = "DH_Hfid", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_Hfid()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
            tightness = d->cfg->GetFloat("hposfid_tightness");
            parms_hfid_t0 = d->cfg->GetSectorParms("hposfid_t0");
            parms_hfid_F = d->cfg->GetSectorParms("hposfid_F");
            parms_hfid_b = d->cfg->GetSectorParms("hposfid_b");
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            printf("DH_Hfid: %ld/%ld passed. %ld changed to bad status.\n", npos_passed, npos, npos_changed);
        }
        virtual bool Handle(H10* d)
        {
            //hides positive hadrons not in fiducial volume
            bool passed = true;
            bool changed = false;
            for (int i = 1; i < d->npart; i++) {
                if (d->q[i]>0) {
                    npos++;
                    float theta = RadToDeg()*ACos(d->cz[i]);
                    float phi = RadToDeg()*ATan2(d->cy[i],d->cx[i]);
                    if (phi < -30) phi+=360;
                    int isect = int((phi+30)/60);
                    if (isect==6) isect = 0;
                    phi -= (isect)*60;
                    float dphi = Dphi(theta, isect+1);
                    if (phi < -dphi || phi > dphi) {
                        if (d->stat[i]>0) {
                            d->stat[i] = -1*d->stat[i];
                            npos_changed++;
                        } else if (d->stat[i]==0) {
                            d->stat[i] = -128;
                            npos_changed++;
                        }
                        changed = true;
                    } else npos_passed++;
                }
            }
            if (changed) d->CalcLVs(true);
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_HFID_H_
