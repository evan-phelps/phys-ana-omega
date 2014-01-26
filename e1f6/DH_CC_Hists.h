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

/**
* DataHandler to produce histograms with number of photo-electrons per CC PMT.
*/
class DH_CC_Hists : public DataHandler
{
    public:
        vector<TH2*> hnphe_V_pmt_eS;
        vector<TH2*> hnphe_V_pmt_pS;
        vector<TH2*> hnphe_V_pmt_pipS;
        vector<TH2*> hnphe_V_pmt_pimS;
        const static int NPMTS = 36;
        DH_CC_Hists(std::string name = "CC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hnphe_V_pmt_eS = MakeHists(NSECTS, "hnphe_V_pmt_e_s%d", "electrons, sector %d", NPMTS, 0.5, NPMTS+0.5, 350, -0.5, 349.5);
            hnphe_V_pmt_pS = MakeHists(NSECTS, "hnphe_V_pmt_p_s%d", "protons, sector %d", NPMTS, 0.5, NPMTS+0.5, 350, -0.5, 349.5);
            hnphe_V_pmt_pipS = MakeHists(NSECTS, "hnphe_V_pmt_pip_s%d", "pos. pions, sector %d", NPMTS, 0.5, NPMTS+0.5, 350, -0.5, 349.5);
            hnphe_V_pmt_pimS = MakeHists(NSECTS, "hnphe_V_pmt_pim_s%d", "neg. pions, sector %d", NPMTS, 0.5, NPMTS+0.5, 350, -0.5, 349.5);
        }
        virtual ~DH_CC_Hists()
        {
            fDir->cd();
            for_each(hnphe_V_pmt_eS.begin(), hnphe_V_pmt_eS.end(), DeleteObj);
            for_each(hnphe_V_pmt_pS.begin(), hnphe_V_pmt_pS.end(), DeleteObj);
            for_each(hnphe_V_pmt_pipS.begin(), hnphe_V_pmt_pipS.end(), DeleteObj);
            for_each(hnphe_V_pmt_pimS.begin(), hnphe_V_pmt_pimS.end(), DeleteObj);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hnphe_V_pmt_eS.begin(), hnphe_V_pmt_eS.end(), WriteObj);
            for_each(hnphe_V_pmt_pS.begin(), hnphe_V_pmt_pS.end(), WriteObj);
            for_each(hnphe_V_pmt_pipS.begin(), hnphe_V_pmt_pipS.end(), WriteObj);
            for_each(hnphe_V_pmt_pimS.begin(), hnphe_V_pmt_pimS.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            for (int i = 0; i < d->gpart; i++) {
                int ccidx = d->cc[i]-1;
                if (ccidx >= 0) {
                    int pmt_hit = d->cc_segm[ccidx]/1000-1;
                    if (pmt_hit != 0) {
                        int segment = (d->cc_segm[ccidx]%1000)/10;
                        int sector = d->cc_sect[ccidx];
                        int pmt_num = (2*segment-1);
                        if (pmt_hit>0) pmt_num++;
                        int pid = d->id[i];
                        if (pid==11) hnphe_V_pmt_eS[sector-1]->Fill(pmt_num, d->nphe[ccidx]);
                        else if (pid==2212) hnphe_V_pmt_pS[sector-1]->Fill(pmt_num, d->nphe[ccidx]);
                        else if (pid==211) hnphe_V_pmt_pipS[sector-1]->Fill(pmt_num, d->nphe[ccidx]);
                        else if (pid==-211) hnphe_V_pmt_pimS[sector-1]->Fill(pmt_num, d->nphe[ccidx]);
                    }
                }
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_CC_HISTS_H_
/*
.L Config.cpp+
.L DataHandler.h+
.L HandlerChain.cpp+
.L H10.C+
.L DH_CC_Hists.h+
TChain *c = new TChain("h10clone/h10")
c->Add("/data/jobs/skim.20140102/batch/e1f_skim/3????.root")
H10 *h10proc = new H10(c, "input.e1f.exp.parms")
h10proc->Add(new DH_CC_Hists())
h10proc->Loop(-1,kFALSE)
*/