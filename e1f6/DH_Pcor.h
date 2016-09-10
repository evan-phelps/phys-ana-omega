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

        DH_Pcor(std::string name = "pcor", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            _pcorr = new MomCorr_e1f("MomCorr");
            fDir->cd();
            h = new TH2F("h2", "h2", 50, 0, 5.5, 200, -10, 10);
        }
        virtual ~DH_Pcor()
        {
            delete _pcorr;
            fDir->cd();
            delete h;
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            h->Write();
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            //TLorentzVector MomCorr_e1f::PcorN(TLorentzVector Pin, Int_t charge, Int_t ipart)
            TLorentzVector lvE1_cor = _pcorr->PcorN(d->lvE1, -1, 0);
            d->p[0] = lvE1_cor.M();
            d->cz[0] = lvE1_cor.CosTheta();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_PCOR_H_
