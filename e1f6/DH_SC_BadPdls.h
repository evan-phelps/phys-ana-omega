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

        DH_SC_BadPdls(std::string name = "DH_SC_BadPdls", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_SC_BadPdls()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
            badpdls = d->cfg->GetSectorParms("scpdls_bad");
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
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
        }
};
#endif                           // DH_SC_BADPDLS_H_
