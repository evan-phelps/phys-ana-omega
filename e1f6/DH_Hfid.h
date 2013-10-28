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
        vector< vector<float> > parms_hfid_t0;
        vector< vector<float> > parms_hfid_F;
        vector< vector<float> > parms_hfid_b;

        double Dphi(float t, int sect)
        {
            float f = parms_hfid_F[sect-1][0];
            float b = parms_hfid_b[sect-1][0];
            float t0 = parms_hfid_t0[sect-1][0];
            return f*(1-Exp(b*(t-t0)));
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
            parms_hfid_t0 = d->cfg->GetSectorParms("hposfid_t0");
            parms_hfid_F = d->cfg->GetSectorParms("hposfid_F");
            parms_hfid_b = d->cfg->GetSectorParms("hposfid_b");
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            //hides positive hadrons not in fiducial volume
            bool passed = true;
            bool changed = false;
            for (int i = 0; i < d->gpart; i++) {
                if (d->q[i]>0) {
                    int isect = d->sector-1;
                    float theta = RadToDeg()*ACos(d->cz[0]);
                    float phi = RadToDeg()*ATan2(d->cy[0],d->cx[0]);
                    if (phi < -30) phi+=360;
                    phi -= (isect)*60;
                    float dphi = Dphi(theta, isect+1);
                    if (phi < -dphi || phi > dphi) {
                        d->id[i] = 0;
                        changed = true;
                    }
                }
            }
            if (changed) d->CalcLVs();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_HFID_H_
