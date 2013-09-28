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
        TObjArray *hsf_V_p_clean;
        TObjArray *heo_V_ei_clean;
        TObjArray *hsf_V_p_clean_anti;
        TObjArray *heo_V_ei_clean_anti;

        DH_EC_Hists(std::string name = "EC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hsf_V_p_clean = MakeHists(NSECTS, "hsf_V_p_clean_%d", "stat>0, nphe>40, etot>0.14, ei>0.07, sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            heo_V_ei_clean = MakeHists(NSECTS, "heo_V_ei_clean_%d", "stat>0, nphe>40, sf>0.26, p>0.64, sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
            hsf_V_p_clean_anti = MakeHists(NSECTS, "hsf_V_p_clean_anti_%d", "NOT stat>0, nphe>40, etot>0.14, ei>0.07, sector %d",
                                        250, 0.0, 5.0, 100, 0.0, 0.5);
            heo_V_ei_clean_anti = MakeHists(NSECTS, "heo_V_ei_clean_anti_%d", "NOT stat>0, nphe>40, sf>0.26, p>0.64, sector %d",
                                        300, 0.0, 0.3, 300, 0.0, 0.3);
        }
        virtual ~DH_EC_Hists()
        {
            fDir->cd();
            delete hsf_V_p_clean;
            delete heo_V_ei_clean;
            delete hsf_V_p_clean_anti;
            delete heo_V_ei_clean_anti;
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            hsf_V_p_clean->Write();
            heo_V_ei_clean->Write();
            hsf_V_p_clean_anti->Write();
            heo_V_ei_clean_anti->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            bool critbase = d->stat[0]>0 && d->sc[0]>0 && d->dc[0]>0 && d->ec[0]>0;
            if ( critbase ) {
                int ecidx = d->ec[0]-1;
                int sectidx = d->ec_sect[ecidx]-1;
                bool critnphe = d->nphe[d->cc[0]-1]>40;
                bool critsf = d->etot[ecidx]/d->p[0]>0.26 && d->p[0]>0.64;
                bool critetot = d->etot[ecidx] > 0.14 && d->ec_ei[ecidx] > 0.07;
                /* CORRECT EC TOTAL ENERGY !!!! */
                d->etot[ecidx] = d->etot[ecidx] > d->ec_ei[ecidx]+d->ec_eo[ecidx] ? d->etot[ecidx] : d->ec_ei[ecidx]+d->ec_eo[ecidx];
                if (critnphe && critetot){
                    TH2 *h = (TH2*)hsf_V_p_clean->At(sectidx);
                    h->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                } else {
                    TH2 *h = (TH2*)hsf_V_p_clean_anti->At(sectidx);
                    h->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                }
                if (critnphe && critsf) {
                    TH2 *h = (TH2*)heo_V_ei_clean->At(sectidx);
                    h->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
                } else {
                    TH2 *h = (TH2*)heo_V_ei_clean_anti->At(sectidx);
                    h->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
                }
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EC_HISTS_H_
