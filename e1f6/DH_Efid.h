#ifndef DH_EFID_H_
#define DH_EFID_H_
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

class DH_Efid : public DataHandler
{
    public:
        vector< vector<float> > parms_efid_theta_min;
        vector< vector<float> > parms_efid_phi_t0;
        vector< vector<float> > parms_efid_phi_pol3;
        vector< vector<float> > parms_efid_phi_bexp;
        vector< vector<float> > parms_efid_uvw;
        float bfieldrat;
        float thetamax;
        float pmin;
        float tightness;

        double F(float p, int sect)
        {
            float sum = 0;
            for (int i = 0; i < 4; i++) {
                sum += parms_efid_phi_pol3[sect-1][i]*Power(p,i);
            }
            return sum;
        }
        double B(double p, int sect)
        {
            return parms_efid_phi_bexp[sect-1][0];
        }
        double T0(float p, int sect)
        {
            float a = parms_efid_phi_t0[sect-1][0],
                  b = parms_efid_phi_t0[sect-1][1],
                  c = parms_efid_phi_t0[sect-1][2];
            return a + b/((p+c)*bfieldrat) + tightness;
        }
        double TMIN(float p, int sect)
        {
            float a = parms_efid_theta_min[sect-1][0],
                  b = parms_efid_theta_min[sect-1][1],
                  c = parms_efid_theta_min[sect-1][2];
            return a + b/((p+c)*bfieldrat);
        }
        double Dphi(float p, float t, int sect)
        {
            float f = F(p, sect);
            float b = B(p, sect);
            float t0 = T0(p, sect);
            return f*(1-Exp(b*(t-t0))) - tightness;
        }
        DH_Efid(std::string name = "DH_Efid", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            bfieldrat = 1.0;
            thetamax = 50;
            pmin = 0.600;
            tightness = 0;
            fDir->cd();
        }
        virtual ~DH_Efid()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
            parms_efid_theta_min = d->cfg->GetSectorParms("efid_theta_min");
            parms_efid_phi_t0 = d->cfg->GetSectorParms("efid_phi_t0");
            parms_efid_phi_pol3 = d->cfg->GetSectorParms("efid_phi_pol3");
            parms_efid_phi_bexp = d->cfg->GetSectorParms("efid_phi_bexp");
            parms_efid_uvw = d->cfg->GetSectorParms("efid_uvw");
            bfieldrat = d->cfg->GetFloat("bfieldratio");
            thetamax = d->cfg->GetFloat("efid_tmax");
            pmin = d->cfg->GetFloat("ec_pmom");
            tightness = d->cfg->GetFloat("efid_tightness");

            // printf("read parameters:\n");
            // for (int isect = 0; isect < NSECTS; isect++) {
            //     printf("     F: %.3f, %.3f, %.3f, %.3f\n", parms_efid_phi_pol3[isect][0], parms_efid_phi_pol3[isect][1], parms_efid_phi_pol3[isect][2], parms_efid_phi_pol3[isect][3]);
            //     printf("    t0: %.3f, %.3f, %.3f\n", parms_efid_theta_min[isect][0], parms_efid_theta_min[isect][1], parms_efid_theta_min[isect][2]);
            //     printf("t0_mod: %.3f\n", parms_efid_phi_t0[isect][0]);
            //     printf("     b: %.3f\n", parms_efid_phi_bexp[isect][0]);
            // }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = false;

            int isect = d->esector-1;
            float p = d->p[0];
            float theta = RadToDeg()*ACos(d->cz[0]);
            float phi = RadToDeg()*ATan2(d->cy[0],d->cx[0]);
            if (phi < -30) phi+=360;
            phi -= (isect)*60;
            float dphi = Dphi(p, theta, isect+1);
            passed = p > pmin && phi > -dphi && phi < dphi && theta > TMIN(p,isect+1);

            // int ecidx = d->ec[0]-1;
            // if (ecidx>=0) {
            //     double uvw[3];
            //     double xyz[3] = { d->ech_x[ecidx], d->ech_y[ecidx], d->ech_z[ecidx] };
            //     //double sf = d->etot[ecidx]/d->p[0];
            //     int isectec = d->ec_sect[ecidx]-1;
            //     d->GetUVW(xyz, uvw);
            //     double u = uvw[0], v = uvw[1], w = uvw[2];
            //     passed = u > parms_efid_uvw[isectec][0] && u < parms_efid_uvw[isectec][1]
            //              && v < parms_efid_uvw[isectec][2] && w < parms_efid_uvw[isectec][3];
            //     if (passed) {
            //         int isect = d->esector-1;
            //         float p = d->p[0];
            //         float theta = RadToDeg()*ACos(d->cz[0]);
            //         float phi = RadToDeg()*ATan2(d->cy[0],d->cx[0]);
            //         if (phi < -30) phi+=360;
            //         phi -= (isect)*60;
            //         float dphi = Dphi(p, theta, isect+1);
            //         passed = passed && p > pmin && phi > -dphi && phi < dphi;
            //     }
            // }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EFID_H_
