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
        vector<TH2*> htprime_V_wS;
        vector<TH2*> hcosTheta_V_phiS;
        vector<TH2*> hmmppip_V_mmpS;
        vector<TH2*> hmmppim_V_mmpS;
        vector<TH2*> hmmppippim_V_mmpS;

        DH_Hists_Monitor(std::string name = "DH_Hists_Monitor", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hq2_V_wS = MakeHists(NSECTS, "hq2_V_w_%d", "Q^{2} vs. W, Sector %d", 340, 0.0, 3.4, 800, 0.0, 8.0);
            htprime_V_wS = MakeHists(NSECTS, "htprime_V_w_%d", "-t^{#prime} vs. W, Sector %d", 340, 0.0, 3.4, 900, -1.0, 8.0);
            hcosTheta_V_phiS = MakeHists(NSECTS, "hcosTheta_V_phi_%d", "cos(#theta) vs. #phi, Sector %d", 720, -PI, PI, 200, -1, 1);
            hmmppip_V_mmpS = MakeHists(NSECTS, "hmmppip_V_mmp_%d", "MMppip vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
            hmmppim_V_mmpS = MakeHists(NSECTS, "hmmppim_V_mmp_%d", "MMppim vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
            hmmppippim_V_mmpS = MakeHists(NSECTS, "hmmppippim_V_mmp_%d", "MMppippim vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
        }
        virtual ~DH_Hists_Monitor()
        {
            fDir->cd();
            for_each(hq2_V_wS.begin(), hq2_V_wS.end(), DeleteObj);
            for_each(htprime_V_wS.begin(), htprime_V_wS.end(), DeleteObj);
            for_each(hcosTheta_V_phiS.begin(), hcosTheta_V_phiS.end(), DeleteObj);
            for_each(hmmppip_V_mmpS.begin(), hmmppip_V_mmpS.end(), DeleteObj);
            for_each(hmmppim_V_mmpS.begin(), hmmppim_V_mmpS.end(), DeleteObj);
            for_each(hmmppippim_V_mmpS.begin(), hmmppippim_V_mmpS.end(), DeleteObj);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hq2_V_wS.begin(), hq2_V_wS.end(), WriteObj);
            for_each(htprime_V_wS.begin(), htprime_V_wS.end(), WriteObj);
            for_each(hcosTheta_V_phiS.begin(), hcosTheta_V_phiS.end(), WriteObj);
            for_each(hmmppip_V_mmpS.begin(), hmmppip_V_mmpS.end(), WriteObj);
            for_each(hmmppim_V_mmpS.begin(), hmmppim_V_mmpS.end(), WriteObj);
            for_each(hmmppippim_V_mmpS.begin(), hmmppippim_V_mmpS.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            
            Int_t scidx = d->sc[0]-1;
            Int_t isect = d->sc_sect[scidx]-1;

            if (scidx>=0 && d->id[0]==ELECTRON) {
                hq2_V_wS[isect]->Fill(d->W, d->Q2);
                if (d->np==1) {
                    //printf("%.2f\n", -d->t1);
                    htprime_V_wS[isect]->Fill(d->W, -d->t1);
                    hcosTheta_V_phiS[isect]->Fill(d->phi, d->cosTheta);
                    if (d->npip==1) hmmppip_V_mmpS[isect]->Fill(d->MMp, d->MMppip);
                    if (d->npim==1) hmmppim_V_mmpS[isect]->Fill(d->MMp, d->MMppim);
                    if (d->npip==1 && d->npim==1) hmmppippim_V_mmpS[isect]->Fill(d->MMp, d->MMppippim);
                }
            }

            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_HISTS_MONITOR_H_
