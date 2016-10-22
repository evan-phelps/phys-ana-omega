#ifndef DH_CC_Nphe_H_
#define DH_CC_Nphe_H_
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

class DH_CC_Nphe : public DataHandler
{
    public:
        static const int NPHE_MIN = 30;
        vector< vector<float> > parms_cc_nphe;
        TTree *t_cc_eff;
        float cc_eff;
        DH_CC_Nphe(std::string name = "DH_CC_Nphe", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            t_cc_eff = NULL;
        }
        virtual ~DH_CC_Nphe()
        {
            fDir->cd();
            if (t_cc_eff) delete t_cc_eff;
        }
        virtual void Setup(H10 *d)
        {
            parms_cc_nphe = d->cfg->GetSectorParms("cc_nphe");
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            if (t_cc_eff) {
                t_cc_eff->BuildIndex("run", "evntid");
                t_cc_eff->Write();
            }
        }
        virtual bool Handle(H10* d)
        {
            //For now, let's just make a global nphe cutoff.  This
            //  must be separately corrected with a per-PMT efficiency
            //  factor.
            //Keep in mind that the "electrons" with nphe<cutoff
            //  should actually be re-assessed as pion candidates
            //  and a new electron should be searched for in the
            //  event.  For now, this is ignored.
            int ccidx = d->cc[0]-1;

            if (ccidx < 0) return false;
            if (d->nphe[ccidx]<NPHE_MIN) return false;

            return true;
        }
        virtual void Wrapup(H10* d)
        {
            if (t_cc_eff == NULL)
            {
                t_cc_eff = new TTree("t_cc_eff", "t_cc_eff");

                t_cc_eff->Branch("run", &(d->run));
                t_cc_eff->Branch("evntid", &(d->evntid));
                t_cc_eff->Branch("nphe_cut_eff", &cc_eff);
            }
            //For now, let's just make a global nphe cutoff.  This
            //  must be separately corrected with a per-PMT efficiency
            //  factor.
            //Keep in mind that the "electrons" with nphe<cutoff
            //  should actually be re-assessed as pion candidates
            //  and a new electron should be searched for in the
            //  event.  For now, this is ignored.
            int ccidx = d->cc[0]-1;
            int pmt_hit = d->cc_segm[ccidx]/1000-1;
            if (pmt_hit == 0) cc_eff = 1.0;
            else {
                int segment = (d->cc_segm[ccidx]%1000)/10;
                int sector = d->cc_sect[ccidx];
                int pmt_num = (2*segment-1);
                if (pmt_hit>0) pmt_num++;
                cc_eff = parms_cc_nphe[sector-1][pmt_num-1];
            }
            t_cc_eff->Fill();
        }
};
#endif                           // DH_CC_Nphe_H_
