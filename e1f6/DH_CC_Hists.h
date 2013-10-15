#ifndef DH_CC_HISTS_H_
#define DH_CC_HISTS_H_
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

class DH_CC_Hists : public DataHandler
{
    public:
        vector<TH2*> hnphe_V_pmtS;
        const static int NPMTS = 36;
        DH_CC_Hists(std::string name = "CC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hnphe_V_pmtS = MakeHists(NPMTS, "hnphe_V_pmt_s%d", "sector %d", NPMTS, 0.5, NPMTS+0.5, 401, -0.5, 400.5);
        }
        virtual ~DH_CC_Hists()
        {
            fDir->cd();
            for_each(hnphe_V_pmtS.begin(), hnphe_V_pmtS.end(), DeleteObj);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hnphe_V_pmtS.begin(), hnphe_V_pmtS.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            int ccidx = d->cc[0]-1;
            if (d->id[0]==11 && ccidx>=0) {
                int segment = (d->cc_segm[ccidx]%1000)/10;
                int pmt_hit = d->cc_segm[ccidx]/1000-1;
                int sector = d->cc_sect[ccidx];
                int pmt_num = (2*segment-1);
                if (pmt_hit>0) pmt_num++;
                if (pmt_hit != 0) hnphe_V_pmtS[sector-1]->Fill(pmt_num, d->nphe[ccidx]);
            }
            
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_CC_HISTS_H_
