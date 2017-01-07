#ifndef DH_PCOR_H_
#define DH_PCOR_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "TString.h"

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#include "mom_corr.h"
#include "MomCorr_E16.h"
#ifndef H10_h
#include "H10.h"
#endif
class H10;

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

#define PI 3.14159265
#define R2D 57.29578
#define D2R 0.01745329

using namespace H10Constants;
using namespace std;

class DH_Pcor : public DataHandler
{
    protected:
        TString mom_corr_type;
        float bfield_current;
        class MomCorr_e1f *_pcorr;

    public:
        vector<TH2*> hpcorS;
        vector<TH2*> hpzcorS;
        vector<TH2*> hpphicorS;

        DH_Pcor(std::string name = "pcor", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_Pcor()
        {
            if (_pcorr) delete _pcorr;
            fDir->cd();
            for_each(hpcorS.begin(), hpcorS.end(), DeleteObj);
            for_each(hpzcorS.begin(), hpzcorS.end(), DeleteObj);
            for_each(hpphicorS.begin(), hpphicorS.end(), DeleteObj);
        }
        virtual void Setup(H10* d)
        {
            mom_corr_type = d->cfg->GetString("mom_corr");
            printf("mom_corr_type = %s\n", mom_corr_type.Data());
            Float_t brat = d->cfg->GetFloat("bfieldratio");
            bfield_current = brat==0 ? 0 : 3375.0/brat; //3375 is max current
            if (mom_corr_type.EqualTo("MomCorr_e1f")) _pcorr = new MomCorr_e1f((char*)"MomCorr");
            hpcorS = MakeHists(NSECTS, "hpcor_%d", "momentum change, S%d", 55, 0, 5.5, 200, -0.10, 0.10);
            hpzcorS = MakeHists(NSECTS, "hpzcor_%d", "z-momentum change, S%d", 55, 0, 5.5, 200, -.1, .1);
            hpphicorS = MakeHists(NSECTS, "hpphicor_%d", "phi-momentum change, S%d", 55, 0, 5.5, 360, -PI, PI);
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hpcorS.begin(), hpcorS.end(), WriteObj);
            for_each(hpzcorS.begin(), hpzcorS.end(), WriteObj);
            for_each(hpphicorS.begin(), hpphicorS.end(), WriteObj);
        }
        virtual bool Handle(H10* d)
        {
            bool passed = true;
            if ( mom_corr_type.EqualTo("No") ) return passed;
            if ( (d->id[0] != 11) || (d->sc[0] <= 0) || (d->dc[0] <= 0)) return passed;

            Float_t pb4 = d->p[0];
            Float_t czb4 = d->cz[0];
            Float_t phib4 = atan2(d->cy[0], d->cx[0]);
            Float_t pnew = pb4;
            Float_t cznew = czb4;
            Float_t phinew = phib4;

            if (mom_corr_type.EqualTo("MomCorr_e1f")) {
                TLorentzVector lvE1_cor = _pcorr->PcorN(d->lvE1, -1, 11);
                pnew = lvE1_cor.P();
                cznew = lvE1_cor.CosTheta();
                phinew = lvE1_cor.Phi();
            } else if(mom_corr_type.EqualTo("MomCorr_e16")) {
                using namespace MomCorr_E16;
                float thetaeld = R2D*acos(czb4);
                float phield = R2D*phib4;
                if (phield < 0) phield += 360;
                float pel = pb4;
                float torcur = bfield_current;
                // float secte = int(phield/60)+1;
                float secte = d->sc_sect[d->sc[0]-1];
                float thetaeldnew = thetaeld;
                float newpel = pel;
                e_corr_sub(thetaeld, phield, pel, torcur, secte, thetaeldnew, newpel);
                pnew = newpel;
                cznew = cos(D2R*thetaeldnew);
                phinew = D2R*phield; //will be same as phib4
            } else return passed;
            d->p[0] = pnew;
            d->cz[0] = cznew;
            hpcorS[d->esector-1]->Fill(pb4, pnew-pb4);
            hpzcorS[d->esector-1]->Fill(pb4, cznew-czb4);
            hpphicorS[d->esector-1]->Fill(pb4, phinew-phib4);
            d->CalcLVs();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_PCOR_H_
