#ifndef DH_MMP_EXCLUSIVE_H_
#define DH_MMP_EXCLUSIVE_H_
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

class DH_MMp_Exclusive : public DataHandler
{
    public:
        int _top;
        TLorentzVector *lvE0, *lvE1, *lvP0, *lvP1, *lvPip, *lvPim;
        DH_MMp_Exclusive(std::string name = "DH_MMp_Exclusive", TDirectory *pDir = NULL, H10 *h10looper = NULL, int top=1) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            _top = top;
        }
        virtual ~DH_MMp_Exclusive()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            if (d->id[0] != 11) return false;
            //recalculate kinematics of event assuming only particles through npart,
            // i.e., status>0, etc.
            d->CalcLVs(true);

            //restrict to exclusive pid combinations, but allow neutral background
            if ( !(d->np==1 && (d->npip==1 || d->npim==1)) ) return false;
            if ( !(   (d->npos==2 && d->nneg==2)
                   || (d->npos==2 && d->nneg==1)
                   || (d->npos==1 && d->nneg==2)
                  )
                ) return false;

            // if ( !(d->MMp >= 0.600 && d->MMp <= 0.966) ) return false;
            if ( !(d->MMp >= 0.4 && d->MMp <= 1.2) ) return false;
            if ( (_top == 1 || _top == 2) && d->npip == 1 && d->MMppip < 0.3 ) return false;
            if ( (_top == 0 || _top == 2) && d->npim == 1 && d->MMppim < 0.3 ) return false;

            return true;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_MMP_EXCLUSIVE_H_
