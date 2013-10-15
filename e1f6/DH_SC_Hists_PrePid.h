#ifndef DH_SC_HISTS_PREPID_H_
#define DH_SC_HISTS_PREPID_H_
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

class DH_SC_Hists_PrePid : public DataHandler
{
    public:
        vector< vector<TH2*> > hdt_V_p_pos_pS;
        vector< vector<TH2*> > hdt_V_p_pos_pipS;
        vector< vector<TH2*> > hdt_V_p_neg_pimS;
        vector< vector<TH2*> > hdt_V_p_neg_eS;
        vector< vector<TH2*> > hdt_V_p_neg_e_part1S;
        vector<TH2*> hm_V_pdlS;
        vector<TH2*> hetime_V_pdlS;
        static const int NPDLS = 48;
        TLorentzVector lv;
        DH_SC_Hists_PrePid(std::string name = "SC_Hists_PrePid", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hdt_V_p_pos_pS.resize(NSECTS);
            hdt_V_p_pos_pipS.resize(NSECTS);
            hdt_V_p_neg_pimS.resize(NSECTS);
            hdt_V_p_neg_eS.resize(NSECTS);
            hdt_V_p_neg_e_part1S.resize(NSECTS);
            hm_V_pdlS = MakeHists(NSECTS, "hm_V_pdl_s%d", "hadron mass spectrum per paddle, sector %d",
                48, 0.5, 48.5, 450, -0.4, 1.4);
            hetime_V_pdlS = MakeHists(NSECTS, "hetime_V_pdl_s%d", "electron time per paddle, sector %d",
                24, 0.5, 24.5, 40, -1, 1);
            for (int isect = 0; isect < NSECTS; isect++) {
                TString title = TString::Format("particle id, positive, assume p, sector %d, paddle %s",isect+1, "%d");
                TString name = TString::Format("hdt_V_p_pos_p_s%d_p%s",isect+1, "%d");
                hdt_V_p_pos_pS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
                title = TString::Format("particle id, positive, assume pip, sector %d, paddle %s",isect+1, "%d");
                name = TString::Format("hdt_V_p_pos_pip_s%d_p%s",isect+1, "%d");
                hdt_V_p_pos_pipS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
                title = TString::Format("particle id, negitive, assume pip, sector %d, paddle %s",isect+1, "%d");
                name = TString::Format("hdt_V_p_neg_pim_s%d_p%s",isect+1, "%d");
                hdt_V_p_neg_pimS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
                title = TString::Format("particle id, negitive, assume e, sector %d, paddle %s",isect+1, "%d");
                name = TString::Format("hdt_V_p_neg_e_s%d_p%s",isect+1, "%d");
                hdt_V_p_neg_eS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
                title = TString::Format("particle id, negitive, assume e, 1st part, sector %d, paddle %s",isect+1, "%d");
                name = TString::Format("hdt_V_p_neg_e_part1_s%d_p%s",isect+1, "%d");
                hdt_V_p_neg_e_part1S[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 40, -1, 1);
            }
        }
        virtual ~DH_SC_Hists_PrePid()
        {
            fDir->cd();
            for (int isect = 0; isect < NSECTS; isect++) {
                for_each(hdt_V_p_pos_pS[isect].begin(), hdt_V_p_pos_pS[isect].end(), DeleteObj);
                for_each(hdt_V_p_pos_pipS[isect].begin(), hdt_V_p_pos_pipS[isect].end(), DeleteObj);
                for_each(hdt_V_p_neg_pimS[isect].begin(), hdt_V_p_neg_pimS[isect].end(), DeleteObj);
                for_each(hdt_V_p_neg_eS[isect].begin(), hdt_V_p_neg_eS[isect].end(), DeleteObj);
                for_each(hdt_V_p_neg_e_part1S[isect].begin(), hdt_V_p_neg_e_part1S[isect].end(), DeleteObj);
            }
            for_each(hm_V_pdlS.begin(), hm_V_pdlS.end(), DeleteObj);
            for_each(hetime_V_pdlS.begin(), hetime_V_pdlS.end(), DeleteObj);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for (int isect = 0; isect < NSECTS; isect++) {
                for_each(hdt_V_p_pos_pS[isect].begin(), hdt_V_p_pos_pS[isect].end(), WriteObj);
                for_each(hdt_V_p_pos_pipS[isect].begin(), hdt_V_p_pos_pipS[isect].end(), WriteObj);
                for_each(hdt_V_p_neg_pimS[isect].begin(), hdt_V_p_neg_pimS[isect].end(), WriteObj);
                for_each(hdt_V_p_neg_eS[isect].begin(), hdt_V_p_neg_eS[isect].end(), WriteObj);
                for_each(hdt_V_p_neg_e_part1S[isect].begin(), hdt_V_p_neg_e_part1S[isect].end(), WriteObj);
            }
            for_each(hm_V_pdlS.begin(), hm_V_pdlS.end(), WriteObj);
            for_each(hetime_V_pdlS.begin(), hetime_V_pdlS.end(), WriteObj);
        }
        float DCt(float px, float py, float pz, float mass, float scr)
        {
            lv.SetXYZM(px,py,pz,mass);
            float beta = lv.Beta();
            float dct = scr/(beta*SOL);
            return dct;
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            for (int ipart = 0; ipart < d->npart; ipart++) {
                int scidx = d->sc[ipart]-1, dcidx = d->dc[ipart]-1;
                int ipdl = d->sc_pd[scidx]-1, isec = d->sector-1;
                if (scidx > 0 && dcidx >= 0) {
                    float mom = d->p[ipart];
                    float px = mom*d->cx[ipart], py = mom*d->cy[ipart], pz = mom*d->cz[ipart];
                    float tof = d->sc_t[scidx]-d->tr_time;
                    if (d->q[ipart] > 0) {
                        hdt_V_p_pos_pS[isec][ipdl]->Fill(mom, tof-DCt(px,py,pz,MASS_P,d->sc_r[scidx]));
                        hdt_V_p_pos_pipS[isec][ipdl]->Fill(mom, tof-DCt(px,py,pz,MASS_PIP,d->sc_r[scidx]));
                    } else if (d->q[ipart]<0) {
                        hdt_V_p_neg_eS[isec][ipdl]->Fill(mom, tof-DCt(px,py,pz,MASS_E,d->sc_r[scidx]));
                        hdt_V_p_neg_pimS[isec][ipdl]->Fill(mom, tof-DCt(px,py,pz,MASS_PIM,d->sc_r[scidx]));
                        if (ipart == 0) {
                            hdt_V_p_neg_e_part1S[isec][ipdl]->Fill(mom, tof-DCt(px,py,pz,MASS_E,d->sc_r[scidx]));
                            hetime_V_pdlS[isec]->Fill(ipdl+1, tof-DCt(px,py,pz,MASS_E,d->sc_r[scidx]));
                        }
                    }
                    if (ipart>0) {
                        hm_V_pdlS[isec]->Fill(ipdl+1, d->m[ipart]);
                    }
                }
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_SC_HISTS_PREPID_H_
