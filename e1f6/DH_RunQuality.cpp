#ifndef _MON_LUMINOSITY_H_
#include "DH_RunQuality.h"
#endif
#include <iostream>
#include <vector>
#include <algorithm>
#include "TObject.h"
#include "TMath.h"

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

using namespace H10Constants;
//using namespace std;

DH_RunQuality::DH_RunQuality(std::string name, TDirectory *pDir) : DataHandler(name, pDir)
{
    fLb.lumblock_num = 0;
    fQl_last = fAnum_last = 0;
    fRun_last = -1;
    Clear();
    fDir->cd();

    lumblocks = new TTree("lumblocks", "luminosity-block information");
    lumblocks->Branch("file_name", fLb.file_name);

    lumblocks->Branch("run_num", &fLb.run_num);
    lumblocks->Branch("file_num", &fLb.file_num);
    lumblocks->Branch("lumblock_num", &fLb.lumblock_num);
    lumblocks->Branch("q_l_first", &fLb.q_l_first);
    lumblocks->Branch("q_l_last", &fLb.q_l_last);
    lumblocks->Branch("evt_num_first", &fLb.evt_num_first);
    lumblocks->Branch("evt_num_last", &fLb.evt_num_last);
    lumblocks->Branch("fc_charge_ltcorr", &fLb.fc_charge_ltcorr);
    lumblocks->Branch("livetime", &fLb.livetime);
    lumblocks->Branch("ntrigs", &fLb.ntrigs);
    lumblocks->Branch("nevts_procd", &fLb.nevts_procd);
    lumblocks->Branch("ne", &fLb.ne);
    lumblocks->Branch("ne_ctrl", &fLb.ne_w_1950_2050_q2_19_21);
    lumblocks->Branch("np", &fLb.np);
    lumblocks->Branch("npip", &fLb.npip);
    lumblocks->Branch("npim", &fLb.npim);
    lumblocks->Branch("nevts_Neg1st", &fLb.nevts_Neg1st);
    lumblocks->Branch("nevts_Neg1st_2Pos_exc", &fLb.nevts_Neg1st_2Pos_exc);
    lumblocks->Branch("nevts_Neg1st_1Pos1Neg_exc", &fLb.nevts_Neg1st_1Pos1Neg_exc);
    lumblocks->Branch("nevts_Neg1st_2Pos1Neg_exc", &fLb.nevts_Neg1st_2Pos1Neg_exc);
}


DH_RunQuality::~DH_RunQuality()
{
    delete lumblocks;
}


bool DH_RunQuality::Handle(H10 *d)
{
    bool passed = true;
    PrepBlock(d);
    CountAll(d);
    return passed;
}


void DH_RunQuality::Wrapup(H10 *d)
{

}


void DH_RunQuality::Finalize(H10 *d)
{
    FillPreviousBlock(d);
    fDir->cd();
    lumblocks->Write("", TObject::kOverwrite);
}


void DH_RunQuality::Clear()
{
    memset(fLb.file_name, 0, 256);
    fLb.run_num = fLb.file_num = fLb.evt_num_first
        = fLb.evt_num_last = fLb.fc_charge_ltcorr = fLb.livetime = -1;
    fLb.ntrigs = fLb.nevts_procd = fLb.ne = fLb.np = fLb.npip = fLb.npim
        = fLb.ne_w_1950_2050_q2_19_21 = fLb.nevts_Neg1st = fLb.nevts_Neg1st_2Pos_exc
        = fLb.nevts_Neg1st_1Pos1Neg_exc
        = fLb.nevts_Neg1st_2Pos1Neg_exc = 0;
}


bool DH_RunQuality::PrepBlock(H10 *d)
{
    /* CURRENTLY, the passed status of this block does not inform the
     * passed status of the Handler
     */
    bool passed = true;
    bool newrun = (fLb.run_num != d->run);
    bool newfile = (newrun || (fLb.file_num != d->file_anum));
    bool newblock = (newrun || (fQl_last < d->q_l));
    bool somethingswrong = (!newfile && (fQl_last > d->q_l));
    /* Assume:
     * 	+ events are ordered in time and, therefore, by file Anumber and run;
     *	+ q_l always changes if file changes (verified for e1f data, where
     * 		leading events of each file have q_l = 0). */

    if (newblock)
    {
        //if this is a new run, there is nothing to fill yet,
        // OTHERWISE fill and clear...
        if (!newrun) {
            FillPreviousBlock(d);
            Clear();
        }
        /* ***************** HANDLE CURRENT BLOCK *********************** */
        std::copy(d->filename.begin(), d->filename.end(), fLb.file_name);
        fLb.file_name[d->filename.size()] = '\0';
        fLb.run_num = d->run;
        fLb.file_num = d->file_anum;
        fLb.lumblock_num++;
        fLb.q_l_first = d->q_l;
        fLb.q_l_last = -1;
        fLb.evt_num_first = newrun ? 0 : d->evntid;
        fLb.evt_num_last = -1;
        fLb.fc_charge_ltcorr = -1;
        fLb.livetime = d->t_l;
        fQl_last = d->q_l;
        fAnum_last = d->file_anum;
        fRun_last = d->run;
        if (somethingswrong)
        {
            std::cerr << "WARNING! Drop in accumulated charge." << std::endl;
        }
    }
    return passed;
}

bool DH_RunQuality::CountAll(H10 *d)
{
    fLb.nevts_procd++;
    bool negfirst = d->q[0] < 0 ? true : false;
    negfirst = negfirst && d->sc[0]>0 && d->cc[0]>0 && d->dc[0]>0 && d->ec[0]>0;
    int npos = 0, nneg = 0;
    for (int ipart = 0; ipart < d->npart; ipart++)
    {
        switch(d->id[ipart])
        {
            case ELECTRON:
                fLb.ne++;
                if (d->W > 1.95 && d->W <= 2.05 && d->Q2 > 1.9 && d->Q2 <= 2.1) {
                    fLb.ne_w_1950_2050_q2_19_21++;
                }
                break;
            case PROTON:
                fLb.np++;
                break;
            case PIP:
                fLb.npip++;
                break;
            case PIM:
                fLb.npim++;
                break;
        }
        if (ipart > 0)
        {
            npos += (d->q[ipart] > 0);
            nneg += (d->q[ipart] < 0);
        }
    }
    fLb.nevts_Neg1st += negfirst;
    fLb.nevts_Neg1st_2Pos_exc += (negfirst && npos==2 && nneg==0);
    fLb.nevts_Neg1st_1Pos1Neg_exc += (negfirst && npos==1 && nneg==1);
    fLb.nevts_Neg1st_2Pos1Neg_exc += (negfirst && npos==2 && nneg==1);
    // bool passed = negfirst && ((npos==2 && nneg==0)
    //     || (npos==1 && nneg==1)
    //     || (npos==2 && nneg==1));
    bool passed = true;
    return passed;
}


void DH_RunQuality::FillPreviousBlock(H10 *d)
{
    fLb.q_l_last = d->q_l;
    fLb.evt_num_last = d->evntid - 1;
    fLb.fc_charge_ltcorr = fLb.q_l_last - fLb.q_l_first;
    fLb.ntrigs = fLb.evt_num_last - fLb.evt_num_first;
    lumblocks->Fill();
}
