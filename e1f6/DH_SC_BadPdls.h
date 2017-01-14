#ifndef DH_SC_BADPDLS_H_
#define DH_SC_BADPDLS_H_
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

class DH_SC_BadPdls : public DataHandler
{
    public:
        vector< vector<float> > badpdls;
        vector< vector<float> > tr_effs_e;
        vector< vector<float> > tr_effs_p;
        vector< vector<float> > tr_effs_pip;
        TTree *t_tr_effs;
        float tr_eff_e;
        float tr_eff_p;
        float tr_eff_pip;
        float tr_eff;

        DH_SC_BadPdls(std::string name = "DH_SC_BadPdls", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            t_tr_effs = NULL;
        }
        virtual ~DH_SC_BadPdls()
        {
            fDir->cd();
            if (t_tr_effs) delete t_tr_effs;
        }
        virtual void Setup(H10 *d)
        {
            badpdls = d->cfg->GetSectorParms("scpdls_bad");
            tr_effs_e = d->cfg->GetSectorParms("tr_effs_e");
            tr_effs_p = d->cfg->GetSectorParms("tr_effs_p");
            tr_effs_pip = d->cfg->GetSectorParms("tr_effs_pip");
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            if (t_tr_effs) {
                t_tr_effs->BuildIndex("run", "evntid");
                t_tr_effs->Write();
            }
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true, changed = false;
            for (int i = 0; i < d->npart; i++) {
                int scidx = d->sc[i]-1;
                if (scidx >= 0) {
                    int isect = d->sc_sect[scidx]-1;
                    int pdl = d->sc_pd[scidx];
                    if (std::find(badpdls[isect].begin(), badpdls[isect].end(), pdl) != badpdls[isect].end()) {
                        d->id[i] = 0;
                        changed = true;
                    }
                }
            }
            if (changed) d->CalcLVs();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
            if (tr_effs_e[0].size()>0) {
                if (t_tr_effs == NULL)
                {
                    t_tr_effs = new TTree("t_tr_effs", "t_tr_effs");
                    t_tr_effs->Branch("run", &(d->run));
                    t_tr_effs->Branch("evntid", &(d->evntid));
                    t_tr_effs->Branch("tr_eff_e", &tr_eff_e);
                    t_tr_effs->Branch("tr_eff_p", &tr_eff_p);
                    t_tr_effs->Branch("tr_eff_pip", &tr_eff_pip);
                    t_tr_effs->Branch("tr_eff", &tr_eff);
                }
                for (int ipart=0; ipart<d->npart; ipart++) {
                    int scidx = d->sc[ipart]-1;
                    if (scidx >= 0) {
                        int isect = d->sc_sect[scidx]-1;
                        int ipdl = d->sc_pd[scidx]-1;
                        if (d->id[ipart]==11) {
                            if (ipdl >= 0 && ipdl < 24) {
                                tr_eff_e = tr_effs_e[isect][ipdl];
                            } else {
                                tr_eff_e = 0;
                            }
                        } else if (d->id[ipart]==2212) {
                            if (ipdl >= 0 && ipdl < 31) {
                                tr_eff_p = tr_effs_p[isect][ipdl];
                            } else {
                                tr_eff_p = 0;
                            }
                        } else if (d->id[ipart]==211) {
                            if (ipdl >= 0 && ipdl < 48) {
                                tr_eff_pip = tr_effs_pip[isect][ipdl];
                            } else {
                                tr_eff_pip = 0;
                            }
                        }
                    }
                }
                tr_eff = tr_eff_e*tr_eff_p*tr_eff_pip;
                t_tr_effs->Fill();
            }
        }
};
#endif                           // DH_SC_BADPDLS_H_
