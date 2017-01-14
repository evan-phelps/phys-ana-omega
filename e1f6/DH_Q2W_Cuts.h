#ifndef DH_Q2W_Cuts_H_
#define DH_Q2W_Cuts_H_
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

class DH_Q2W_Cuts : public DataHandler
{
    public:
        vector< vector<float> > cutparms;

        DH_Q2W_Cuts(std::string name = "DH_Q2W_Cuts", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_Q2W_Cuts()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
            printf("Getting Q2W cut parameters.\n");
            cutparms = d->cfg->GetSectorParms("wq2cut");
            printf("Number of sectors with Q2W cut parameters = %zu.\n", cutparms.size());
            for (int i=0; i<cutparms.size(); i++) {
                printf("Number of of cut parameters for sector %d = %zu.\n", i+1, cutparms[i].size());
                for (int j=0; j<cutparms[i].size(); j+=6) {
                    float p0l = cutparms[i][j+0];
                    float p1l = cutparms[i][j+1];
                    float p2l = cutparms[i][j+2];
                    float p0h = cutparms[i][j+3];
                    float p1h = cutparms[i][j+4];
                    float p2h = cutparms[i][j+5];
                    printf("%.3f\t%.3f\t%.3f\t:\t%.3f\t%.3f\t%.3f\n",
                        p0l,p1l,p2l,p0h,p1h,p2h);
                }
            }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            int sect = d->esector;
            for (int iparms=0; iparms<cutparms[sect-1].size(); iparms+=6) {
                float p0l = cutparms[sect-1][iparms+0];
                float p1l = cutparms[sect-1][iparms+1];
                float p2l = cutparms[sect-1][iparms+2];
                float p0h = cutparms[sect-1][iparms+3];
                float p1h = cutparms[sect-1][iparms+4];
                float p2h = cutparms[sect-1][iparms+5];
                float w = d->W;
                float q2 = d->Q2;
                float q2lo = p0l + p1l*w + p2l*w*w;
                float q2hi = p0h + p1h*w + p2h*w*w;
                if (q2 > q2lo && q2 < q2hi) return false;
            }
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_Q2W_Cuts_H_
