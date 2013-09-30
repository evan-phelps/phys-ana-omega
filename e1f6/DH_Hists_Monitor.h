#ifndef DH_HISTS_MONITOR_H_
#define DH_HISTS_MONITOR_H_
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

class DH_Hists_Monitor : public DataHandler
{
    public:
        vector<TH2*> hq2_V_wS;
        vector<TH2*> hw_V_tprimeS;
        vector<TH2*> hcosTheta_V_phiS;
        vector<TH2*> hmmppip_V_mmp;
        vector<TH2*> hmmppim_V_mmp;
        vector<TH2*> hmmppippim_V_mmp;

        DH_Hists_Monitor(std::string name = "DH_Hists_Monitor", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hq2_V_wS = MakeHists(NSECTS, "hq2_V_w_%d", "Q^{2} vs. W, Sector %d", 340, 0.0, 3.4, 800, 0.0, 8.0);
        }
        virtual ~DH_Hists_Monitor()
        {
            fDir->cd();
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hq2_V_wS.begin(), hq2_V_wS.end(), WriteObj);
            for_each(hw_V_tprimeS.begin(), hw_V_tprimeS.end(), WriteObj);
            for_each(hcosTheta_V_phiS.begin(), hcosTheta_V_phiS.end(), WriteObj);
            for_each(hmmppip_V_mmp.begin(), hmmppip_V_mmp.end(), WriteObj);
            for_each(hmmppim_V_mmp.begin(), hmmppim_V_mmp.end(), WriteObj);
            for_each(hmmppippim_V_mmp.begin(), hmmppippim_V_mmp.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            
            Int_t scidx = d->sc[0]-1;
            Int_t isect = d->sc_sect[scidx]-1;

            if (scidx>=0) {
                hq2_V_wS[isect]->Fill(d->W, d->Q2);
            }

            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_HISTS_MONITOR_H_
