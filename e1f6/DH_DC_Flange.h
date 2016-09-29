#ifndef DH_DC_FLANGE_H_
#define DH_DC_FLANGE_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include "TString.h"
#include "TF1.h"
#include "TMath.h"
#include "TH3.h"

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
using namespace TMath;

class DH_DC_Flange : public DataHandler
{
    public:
        vector<TH3*> hangpS;
        vector< vector<float> > parms_sc_borders;
        TF1 *f_scx_lo[NSECTS], *f_scx_hi[NSECTS];
        Int_t xmin[NSECTS], xmax[NSECTS];

        DH_DC_Flange(std::string name = "DH_DC_Flange", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            hangpS = MakeHists(NSECTS, "hangp_s%d", "hangp_s%d", 120, 0, 60, 120, -30, 30, 60, 0, 6);
        }
        virtual ~DH_DC_Flange()
        {
            fDir->cd();
            for_each(hangpS.begin(), hangpS.end(), DeleteObj);
        }
        virtual void Setup(H10 *d)
        {
            parms_sc_borders = d->cfg->GetSectorParms("efid_sc_x_of_y");
            for (int isect = 0; isect < 6; isect++) {
                xmin[isect] = parms_sc_borders[isect][0];
                xmax[isect] = parms_sc_borders[isect][1];
                Float_t m_lo = parms_sc_borders[isect][2];
                Float_t b_lo = parms_sc_borders[isect][3];
                Float_t m_hi = parms_sc_borders[isect][4];
                Float_t b_hi = parms_sc_borders[isect][5];

                f_scx_lo[isect] = new TF1(TString::Format("f_scx_lo_%d",isect+1),
                                          "[0]*x+[1]", -250, 50);
                f_scx_hi[isect] = new TF1(TString::Format("f_scx_hi_%d",isect+1),
                                          "[0]*x+[1]", -250, 50);
                f_scx_lo[isect]->SetParameters(m_lo, b_lo);
                f_scx_hi[isect]->SetParameters(m_hi, b_hi);
            }
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            for_each(hangpS.begin(), hangpS.end(), WriteObj);
            for (int isect = 0; isect < NSECTS; isect++) {
                delete f_scx_lo[isect];
                delete f_scx_hi[isect];
            }
        }
        virtual bool Handle(H10* d)
        {
            Int_t dcidx = d->dc[0] - 1;
            if ( dcidx < 0) return false;

            Int_t isect = d->dc_sect[dcidx] - 1;
            Float_t scx = d->dc_xsc[dcidx];
            Float_t scy = d->dc_ysc[dcidx];

            if ( scx < xmin[isect] || scx > xmax[isect] ) return false;
            if (   scx < f_scx_lo[isect]->Eval(scy)
                || scx < f_scx_hi[isect]->Eval(scy) ) return false;

            Float_t theta = RadToDeg()*ACos(d->cz[0]);
            Float_t phi = RadToDeg()*ATan2(d->cy[0],d->cx[0]);
            if (phi < -30) phi+=360;
            phi -= (d->esector-1)*60;
            Float_t p = d->p[0];
            hangpS[isect]->Fill(theta, phi, p);

            return true;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_DC_FLANGE_H_
