#ifndef DH_EC_HISTS_H_
#define DH_EC_HISTS_H_
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

class DH_EC_Hists : public DataHandler
{
    public:
        vector<TH2*> hsf_V_p;
        vector<TH2*> heo_V_ei;

        DH_EC_Hists(std::string name = "EC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hsf_V_p = MakeHists(NSECTS, "hsf_V_p_%d", "sector %d",
                250, 0.0, 5.0, 100, 0.0, 0.5);
            heo_V_ei = MakeHists(NSECTS, "heo_V_ei_%d", "sector %d",
                300, 0.0, 0.3, 300, 0.0, 0.3);
        }
        virtual ~DH_EC_Hists()
        {
            fDir->cd();
            for_each(hsf_V_p.begin(), hsf_V_p.end(), DeleteObj);
            for_each(heo_V_ei.begin(), heo_V_ei.end(), DeleteObj);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hsf_V_p.begin(), hsf_V_p.end(), WriteObj);
            for_each(heo_V_ei.begin(), heo_V_ei.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            bool critbase = d->stat[0]>0 && d->sc[0]>0 && d->dc[0]>0 && d->ec[0]>0;
            if ( critbase )
            {
                int ecidx = d->ec[0]-1;
                int sectidx = d->ec_sect[ecidx]-1;
                hsf_V_p[sectidx]->Fill(d->p[0], d->etot[ecidx]/d->p[0]);
                heo_V_ei[sectidx]->Fill(d->ec_ei[ecidx], d->ec_eo[ecidx]);
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EC_HISTS_H_
