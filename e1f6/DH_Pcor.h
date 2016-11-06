#ifndef DH_PCOR_H_
#define DH_PCOR_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#include "mom_corr.h"
#ifndef H10_h
#include "H10.h"
#endif
class H10;

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

using namespace H10Constants;
using namespace std;

class DH_Pcor : public DataHandler
{
    protected:
        class MomCorr_e1f *_pcorr;

    public:
        TH2 *h;
        TH2 *h2;

        DH_Pcor(std::string name = "pcor", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            _pcorr = new MomCorr_e1f("MomCorr");
            fDir->cd();
            h = new TH2F("hpcor", "momentum change", 50, 0, 5.5, 200, -0.10, 0.10);
            h2 = new TH2F("hczcor", "cz change", 50, 0, 5.5, 200, -1, 1);
        }
        virtual ~DH_Pcor()
        {
            delete _pcorr;
            fDir->cd();
            delete h;
            delete h2;
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            h->Write();
            h2->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            //TLorentzVector MomCorr_e1f::PcorN(TLorentzVector Pin, Int_t charge, Int_t ipart)
            TLorentzVector lvE1_cor = _pcorr->PcorN(d->lvE1, -1, 11);
            Float_t pb4 = d->p[0];
            Float_t czb4 = d->cz[0];
            d->p[0] = pb4+lvE1_cor.P();
            d->cz[0] = czb4+lvE1_cor.CosTheta();
            h->Fill(pb4, d->p[0]-pb4);
            h2->Fill(pb4, d->cz[0]-czb4);
            d->CalcLVs();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_PCOR_H_
