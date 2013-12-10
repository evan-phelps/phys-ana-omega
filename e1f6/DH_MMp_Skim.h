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
        virtual bool Handle(H10* d)
        {
            lvE0->SetXYZT(0, 0, d->E0, d->E0);
            lvE1->SetXYZT(d->p[0]*d->cx[0], d->p[0]*d->cy[0], d->p[0]*d->cz[0], d->p[0]);
            /* ****
            TODO: require electron.
                  additionally, for one of combination of pid assignments according to charge,
                  mmp, mmppip, and mmppim must be within missing mass range.
            */
            // int ipart = partsidx[1];
            // lvP1->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_P);
            // ipart = partsidx[2];
            // lvPip->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_PIP);
            // ipart = partsidx[3];
            // lvPim->SetXYZM(d->p[ipart]*d->cx[ipart], d->p[ipart]*d->cy[ipart], d->p[ipart]*d->cz[ipart], MASS_PIM);
            // TLorentzVector lvMMP = (*lvE0 - *lvE1 + *lvP0 - *lvP1);
            // TLorentzVector lvMMPPIP = lvMMP - *lvPip;
            // TLorentzVector lvMMPPIM = lvMMP - *lvPim;
            return d->MMp > 0.4 && d->MMp < 1.2;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_MMP_SKIM_H_
