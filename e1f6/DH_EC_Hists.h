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
        TObjArray *hsf_V_p_1stIsNegCcS;
        TObjArray *heo_V_ei_1stIsnegCcS;
        TObjArray *hsf_V_p_1stIsNegCcEiS;
        TObjArray *hsf_V_p_1stIsNegCcEiAntiS;
        DH_EC_Hists(std::string name = "EC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hsf_V_p_negS = MakeHists(NSECTS, "hsf_V_p_neg_%d", "Sampling Fraction, Negative, Sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            hsf_V_p_posS = MakeHists(NSECTS, "hsf_V_p_pos_%d", "Sampling Fraction, Positive, Sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            heo_V_ei_negS = MakeHists(NSECTS, "heo_V_ei_neg_%d", "EC Energy Outer vs. Inner, Negative, Sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
            heo_V_ei_posS = MakeHists(NSECTS, "heo_V_ei_pos_%d", "EC Energy Outer vs. Inner, Negative, Sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
            hsf_V_p_1stIsNegCcS = MakeHists(NSECTS, "hsf_V_p_1stIsNegCc_%d", "Sampling Fraction, 1^{st}, Negative, CC, Sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            heo_V_ei_1stIsnegCcS = MakeHists(NSECTS, "heo_V_ei_1stIsNegCc_%d", "EC Energy Outer vs. Inner, 1^{st}, Negative, CC, Sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
            hsf_V_p_1stIsNegCcEiS = MakeHists(NSECTS, "hsf_V_p_1stIsNegCcEi_%d", "Sampling Fraction, 1^{st}, Negative, CC, E_{inner} < Sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            hsf_V_p_1stIsNegCcEiAntiS = MakeHists(NSECTS, "hsf_V_p_1stIsNegCcEi_anti_%d", "Sampling Fraction, ANTI 1^{st}, Negative, CC, E_{inner} < Sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
        }
        virtual ~DH_EC_Hists()
        {
            fDir->cd();
            delete hsf_V_p_negS;
            delete hsf_V_p_posS;
            delete heo_V_ei_negS;
            delete heo_V_ei_posS;
            delete hsf_V_p_1stIsNegCcS;
            delete heo_V_ei_1stIsnegCcS;
            delete hsf_V_p_1stIsNegCcEiS;
            delete hsf_V_p_1stIsNegCcEiAntiS;
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            hsf_V_p_negS->Write();
            hsf_V_p_posS->Write();
            heo_V_ei_negS->Write();
            heo_V_ei_posS->Write();
            hsf_V_p_1stIsNegCcS->Write();
            heo_V_ei_1stIsnegCcS->Write();
            hsf_V_p_1stIsNegCcEiS->Write();
            hsf_V_p_1stIsNegCcEiAntiS->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            for (int i = 0; i < d->npart; i++) {
                if (d->sc[i] > 0 && d->dc[i] > 0 && d->ec[i] > 0) {
                    int ecidx = d->ec[i]-1;
                    int scidx = d->sc[i]-1;
                    int isector = d->sc_sect[scidx]-1;

                    TH2 *hsf, *he;
                    if (d->q[i] < 0) {
                        hsf = (TH2*)(*hsf_V_p_negS)[isector];
                        he = (TH2*)(*heo_V_ei_negS)[isector];
                    } else if (d->q[i] > 0) {    
                        hsf = (TH2*)(*hsf_V_p_posS)[isector];
                        he = (TH2*)(*heo_V_ei_posS)[isector];
                    } else continue;
                    hsf->Fill(d->p[i], d->etot[ecidx]/d->p[i]);
                    he->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
                }
            }
            TH2 *hsf0, *he0, *hsf0ei;
            if (d->sc[0] > 0 && d->dc[0] > 0 && d->ec[0] > 0 && d->cc[0] > 0) {
                int ecidx = d->ec[0]-1;
                int scidx = d->sc[0]-1;
                int isector = d->sc_sect[scidx]-1;
                int ccidx = d->cc[0]-1;
                if (d->id[0] == 11
                    && d->cc_sect[ccidx] == d->ec_sect[ecidx]
                    && d->cc_sect[ccidx] == d->sc_sect[scidx]) {
                        hsf0 = (TH2*)(*hsf_V_p_1stIsNegCcS)[isector];
                        he0 = (TH2*)(*heo_V_ei_1stIsnegCcS)[isector];
                        hsf0->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                        he0->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
                        if (d->ec_ei[ecidx] > 0.06 && d->nphe[ccidx] > 35) {
                            int isect = d->ec_sect[0]-1; //debug isector problem
                            hsf0ei = (TH2*)(*hsf_V_p_1stIsNegCcEiS)[isect];
                            hsf0ei->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                        } else {
                            int isect = d->ec_sect[0]-1; //debug isector problem
                            hsf0ei = (TH2*)(*hsf_V_p_1stIsNegCcEiAntiS)[isect];
                            hsf0ei->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                        }
                }
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EC_HISTS_H_
