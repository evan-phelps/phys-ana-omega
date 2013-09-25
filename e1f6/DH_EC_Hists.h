#ifndef DH_EC_HISTS_H_
#define DH_EC_HISTS_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include "TObjArray.h"

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

class DH_EC_Hists : public DataHandler
{
    public:
        TObjArray *hsf_V_p_negS;
        TObjArray *hsf_V_p_posS;
        TObjArray *heo_V_ei_negS;
        TObjArray *heo_V_ei_posS;
        DH_EC_Hists(std::string name = "EC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hsf_V_p_negS = MakeHists(NSECTS, "hsf_V_p_neg_%d", "Sampling Fraction, Negative, Sector %d",
                                        550, 0.0, 5.5, 50, 0.0, 0.5);
            hsf_V_p_posS = MakeHists(NSECTS, "hsf_V_p_pos_%d", "Sampling Fraction, Positive, Sector %d",
                                        550, 0.0, 5.5, 50, 0.0, 0.5);
            heo_V_ei_negS = MakeHists(NSECTS, "heo_V_ei_neg_%d", "EC Energy Outer vs. Inner, Negative, Sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
            heo_V_ei_posS = MakeHists(NSECTS, "heo_V_ei_pos_%d", "EC Energy Outer vs. Inner, Negative, Sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
        }
        virtual ~DH_EC_Hists()
        {
            fDir->cd();
            delete hsf_V_p_negS;
            delete hsf_V_p_posS;
            delete heo_V_ei_negS;
            delete heo_V_ei_posS;
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            hsf_V_p_negS->Write();
            hsf_V_p_posS->Write();
            heo_V_ei_negS->Write();
            heo_V_ei_posS->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            for (int i = 0; i < d->npart; i++) {
                if (d->sc[i] > 0 && d->dc[i] > 0 && d->ec[i] > 0) {
                    int ecidx = d->ec[i]-1;
                    int scidx = d->sc[i]-1;
                    int isect = d->sc_sect[scidx]-1;

                    TH2 *hsf, *he;
                    if (d->q[i] < 0) {
                        hsf = (TH2*)(*hsf_V_p_negS)[isect];
                        he = (TH2*)(*heo_V_ei_negS)[isect];
                    } else if (d->q[i] > 0) {    
                        hsf = (TH2*)(*hsf_V_p_posS)[isect];
                        he = (TH2*)(*heo_V_ei_posS)[isect];
                    } else continue;
                    hsf->Fill(d->p[i], d->etot[ecidx]/d->p[i]);
                    he->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
                }
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EC_HISTS_H_
