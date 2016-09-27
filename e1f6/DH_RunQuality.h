#ifndef _MON_LUMINOSITY_H_
#define _MON_LUMINOSITY_H_

#include <iostream>
#include <stdexcept>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TEntryList.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TLorentzVector.h"

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#ifndef H10_h
#include "H10.h"
#endif
class H10;

using namespace std;

class DH_RunQuality: public DataHandler
{
    public:
        DH_RunQuality(std::string name = "RunQuality", TDirectory *pDir = NULL);
        virtual ~DH_RunQuality();
        virtual bool Handle(H10 *d);
        virtual void Wrapup(H10 *d);
        virtual void Finalize(H10 *d);
        /* luminosity block-level leaves */
        struct sLB
        {
            char file_name[256];
            UShort_t run_num;
            UChar_t file_num;
            UInt_t lumblock_num;
            Float_t q_l_first;
            Float_t q_l_last;
            ULong64_t evt_num_first;
            ULong64_t evt_num_last;
            Float_t fc_charge_ltcorr;
            Float_t livetime;
            ULong64_t ntrigs;
            ULong64_t nevts_procd;
            ULong64_t ne;
            ULong64_t ne_w_1950_2050_q2_19_21;
            ULong64_t np;
            ULong64_t npip;
            ULong64_t npim;
            ULong64_t nevts_Neg1st;
            ULong64_t nevts_Neg1st_2Pos_exc;
            ULong64_t nevts_Neg1st_1Pos1Neg_exc;
            ULong64_t nevts_Neg1st_2Pos1Neg_exc;
        } fLb;
        TTree *lumblocks;
    protected:
        float fQl_last, fAnum_last, fRun_last;
        bool firstfile, a00exists;
        TLorentzVector *lvE0, *lvE1, *lvP0, *lvP1, *lvPip, *lvPim;
        void Clear();
        bool PrepBlock(H10 *d);
        bool CountAll(H10 *d);
        void FillPreviousBlock(H10 *d);
};
#endif                           // _MON_LUMINOSITY_H_
