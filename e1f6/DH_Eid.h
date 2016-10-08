#ifndef DH_EID_H_
#define DH_EID_H_
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

using namespace H10Constants;
using namespace std;

class DH_Eid : public DataHandler
{
    public:
        vector< vector<float> > parms_sf_hi_pol3;
        vector< vector<float> > parms_sf_lo_pol3;
        vector< vector<float> > parms_ei_lo_pol0;
        float ec_pmom_lo;
        vector<TH1*> heidS;
        vector<TH1*> heidindS;
        vector<TH1*> heidS_sample_2_2;
        vector<TH1*> heidindS_sample_2_2;
        int nconditions;

        DH_Eid(std::string name = "DH_EC_Eid", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            nconditions = 12;
            ec_pmom_lo = 0;
            fDir->cd();
            static const char* const binlabels[] = {"UNDERFLOW",
                                                    "CC",
                                                    "EC",
                                                    "DC",
                                                    "SC",
                                                    "same-sector",
                                                    "DC status > 0",
                                                    "status > 0",
                                                    "1^{st} is negative",
                                                    "p > p_{threshold}",
                                                    "CC LowP, SF HighP",
                                                    "E_{inner} > E^{min}_{inner}",
                                                    "#frac{E_{EC}}{p} #approx SF",
                                                    "OVERFLOW"};
            heidS = MakeHists(NSECTS, "heid_s%d", "electron id summary, sector %d", nconditions, -0.5, nconditions-0.5);
            heidindS = MakeHists(NSECTS, "heidind_s%d", "electron id summary, independent, sector %d", nconditions, -0.5, nconditions-0.5);
            heidS_sample_2_2 = MakeHists(NSECTS, "heid_sample_2_2_s%d", "electron id summary, sector %d", nconditions, -0.5, nconditions-0.5);
            heidindS_sample_2_2 = MakeHists(NSECTS, "heidind_sample_2_2_s%d", "electron id summary, independent, sector %d", nconditions, -0.5, nconditions-0.5);
            for (int isect = 0; isect < 6; isect++) {
                for (int ibin = 1; ibin <= nconditions; ibin++) {
                    heidS[isect]->GetXaxis()->SetBinLabel(ibin, binlabels[ibin]);
                    heidindS[isect]->GetXaxis()->SetBinLabel(ibin, binlabels[ibin]);
                    heidS_sample_2_2[isect]->GetXaxis()->SetBinLabel(ibin, binlabels[ibin]);
                    heidindS_sample_2_2[isect]->GetXaxis()->SetBinLabel(ibin, binlabels[ibin]);
                }
            }
        }
        virtual ~DH_Eid()
        {
            fDir->cd();
            for_each(heidS.begin(), heidS.end(), DeleteObj);
            for_each(heidindS.begin(), heidindS.end(), DeleteObj);
        }
        virtual void Setup(H10 *d)
        {
            parms_sf_lo_pol3 = d->cfg->GetSectorParms("ec_sf_lo_pol3");
            parms_sf_hi_pol3 = d->cfg->GetSectorParms("ec_sf_hi_pol3");
            parms_ei_lo_pol0 = d->cfg->GetSectorParms("ec_ei_lo");
            ec_pmom_lo = d->cfg->GetFloat("ec_pmom_lo");
            // printf("read parameters:\n");
            // for (int isect = 0; isect < NSECTS; isect++) {
            //     printf("%.3f, %.3f, %.3f, %.3f\n", parms_sf_lo_pol3[isect][0], parms_sf_lo_pol3[isect][1], parms_sf_lo_pol3[isect][2], parms_sf_lo_pol3[isect][3]);
            //     printf("%.3f, %.3f, %.3f, %.3f\n", parms_sf_hi_pol3[isect][0], parms_sf_hi_pol3[isect][1], parms_sf_hi_pol3[isect][2], parms_sf_hi_pol3[isect][3]);
            //     printf("%.3f\n", parms_ei_lo_pol0[isect][0]);
            // }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(heidS.begin(), heidS.end(), WriteObj);
            for_each(heidindS.begin(), heidindS.end(), WriteObj);
            for_each(heidS_sample_2_2.begin(), heidS_sample_2_2.end(), WriteObj);
            for_each(heidindS_sample_2_2.begin(), heidindS_sample_2_2.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            int ecidx = d->ec[0]-1, scidx = d->sc[0]-1, ccidx = d->cc[0]-1, dcidx = d->dc[0]-1;
            int ecsect = 0, scsect = 0, ccsect = 0, dcsect = 0;
            double sf = 0, sflo = 0, sfhi = 0;
            /* CORRECT EC TOTAL ENERGY !!!! */
            if (ecidx >= 0) {
                d->etot[ecidx] = d->etot[ecidx] > d->ec_ei[ecidx]+d->ec_eo[ecidx] ? d->etot[ecidx] : d->ec_ei[ecidx]+d->ec_eo[ecidx];
                ecsect = d->ec_sect[ecidx];
                sf = d->etot[ecidx]/d->p[0];
                double mom = d->p[0];
                sflo = parms_sf_lo_pol3[ecsect-1][0] + parms_sf_lo_pol3[ecsect-1][1]*mom + parms_sf_lo_pol3[ecsect-1][2]*mom*mom + parms_sf_lo_pol3[ecsect-1][3]*mom*mom*mom;
                sfhi = parms_sf_hi_pol3[ecsect-1][0] + parms_sf_hi_pol3[ecsect-1][1]*mom + parms_sf_hi_pol3[ecsect-1][2]*mom*mom + parms_sf_hi_pol3[ecsect-1][3]*mom*mom*mom;
            }
            if (scidx >= 0) scsect = d->sc_sect[scidx];
            if (ccidx >= 0) ccsect = d->cc_sect[ccidx];
            if (dcidx >= 0) dcsect = d->dc_sect[dcidx];
            
            bool isX0neg = d->q[0]<0;
            bool isSC = scidx>=0, isCC = ccidx>=0, isEC = ecidx>=0, isDC = dcidx>=0,
                 isStatGood = d->stat[0]>0, isDcStatGood = (dcidx>=0 && d->dc_stat[dcidx]>0), isPmom = d->p[0]>ec_pmom_lo;

            bool isSameSector = (isSC && isCC && isDC && isEC && d->esector==scsect && d->esector==ccsect && d->esector==dcsect && d->esector==ecsect);
            bool isEi = (isEC && d->ec_ei[ecidx] > parms_ei_lo_pol0[ecsect-1][0]);
            bool isSF = (isEC && sf > sflo && sf < sfhi);
            bool isCCorSF = (isCC && d->p[0]<2) || (isSF && isEi && d->p[0]>=2);
            bool barr[] = {isCC, isEC, isDC, isSC, isSameSector, isDcStatGood, isStatGood, isX0neg, isPmom, isCCorSF, isEi, isSF};
            for (int i = 0; i < nconditions; i++) {
                if (barr[i]) {
                    heidindS[d->esector-1]->Fill(i);
                    if (d->W > 1.95 && d->W <= 2.05 && d->Q2 > 1.9 && d->Q2 <= 2.1) {
                        heidindS_sample_2_2[d->esector-1]->Fill(i);
                    }
                }
                passed = passed ? barr[i] : false;          //if false, don't switch back to true
                if (passed && barr[i]) {
                    heidS[d->esector-1]->Fill(i);
                    if (d->W > 1.95 && d->W <= 2.05 && d->Q2 > 1.9 && d->Q2 <= 2.1) {
                        heidS_sample_2_2[d->esector-1]->Fill(i);
                    }
                }
            }

            if (passed) d->id[0] = 11;
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EID_H_
