#ifndef DH_MMP_SKIM_H_
#define DH_MMP_SKIM_H_
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

class DH_MMp_Skim : public DataHandler
{
    public:
        TLorentzVector *lvE0, *lvE1, *lvP0, *lvP1, *lvPip, *lvPim;
        DH_MMp_Skim(std::string name = "DH_MMp_Skim", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            lvE0 = new TLorentzVector();
            lvP0 = new TLorentzVector(0, 0, 0, MASS_P);
            lvE1 = new TLorentzVector();
            lvP1 = new TLorentzVector();
            lvPip = new TLorentzVector();
            lvPim = new TLorentzVector();
        }
        virtual ~DH_MMp_Skim()
        {
            fDir->cd();
            delete lvE0;
            delete lvE1;
            delete lvP0;
            delete lvP1;
            delete lvPip;
            delete lvPim;
        }
        virtual void Setup(H10 *d)
        {
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        // bool CutMM(H10* d, int iproton, int ipi)
        // {
        //     lvP1->SetXYZM(d->p[iproton]*d->cx[iproton], d->p[iproton]*d->cy[iproton], d->p[iproton]*d->cz[iproton], MASS_P);
        //     double mmp = (*lvE0 - *lvE1 + *lvP0 - *lvP1).M();
        //     //doesn't matter if it's pi+ or pi-, so using pip variables
        //     lvPip->SetXYZM(d->p[ipi]*d->cx[ipi], d->p[ipi]*d->cy[ipi], d->p[ipi]*d->cz[ipi], MASS_PIP);
        //     double mmppi = (*lvE0 - *lvE1 + *lvP0 - *lvP1 - *lvPip).M();
        //     return (mmp >= 0.4 && mmp <= 1.2 && mmppi > 0);
        // }
        // virtual bool Handle(H10* d)
        // {
        //     lvE0->SetXYZT(0, 0, d->E0, d->E0);
        //     lvE1->SetXYZT(d->p[0]*d->cx[0], d->p[0]*d->cy[0], d->p[0]*d->cz[0], d->p[0]);
        //     int npos = 0, nneg = 0;
        //     const int NPOS_MAX = 2, NNEG_MAX = 1;
        //     int ipos[NPOS_MAX], ineg[NNEG_MAX];
        //     ipos[0] = ipos[1] = ineg[0] = -1;
        //     for (int ipart = 1; ipart < d->gpart; ipart++) {
        //         //store h10 indexes of positives and negatives up to maximum of topologies;
        //         //H10 shouldn't know anything about topologies -- this is a cludge.
        //         if (d->q[ipart]>0 && npos < NPOS_MAX) ipos[npos++] = ipart;
        //         else if (d->q[ipart]<0 && nneg < NNEG_MAX) ineg[nneg++] = ipart;
        //     }

        //     if (npos==2 && nneg==1) {
        //         return (CutMM(d, ipos[0], ineg[0]) && CutMM(d, ipos[0], ipos[1])
        //             || (CutMM(d, ipos[1], ineg[0])) && CutMM(d, ipos[1], ipos[0]));
        //     } else if (npos==2 && nneg==0) {
        //         return CutMM(d, ipos[0], ipos[1]) || CutMM(d, ipos[1], ipos[0]);
        //     } else if (npos==1 && nneg==1) {
        //         return CutMM(d, ipos[0], ineg[0]);
        //     }
        //     return false;
        // }
        virtual bool Handle(H10* d)
        {
            return d->MMp > 0.4 && d->MMp < 1.2;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_MMP_SKIM_H_
