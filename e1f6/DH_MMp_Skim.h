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
        bool m_require_dcsc;
        TLorentzVector *lvE0, *lvE1, *lvP0, *lvP1, *lvPip, *lvPim;
        double mmp, mmppi;
        TH1 *h_nPerE_any, *h_nPerE_stat, *h_nPerE_stat_id,
            *h_nPerE_stat_dcscstat, *h_nPerE_stat_dcscstat_id;
        TH1 *h_nevts;
        vector<TH2*> hmmppip_V_mmpS;
        DH_MMp_Skim(std::string name = "DH_MMp_Skim", TDirectory *pDir = NULL, H10 *h10looper = NULL, bool require_dcsc = true) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            mmp = mmppi = 0;
            m_require_dcsc = require_dcsc;
            lvE0 = new TLorentzVector();
            lvP0 = new TLorentzVector(0, 0, 0, MASS_P);
            lvE1 = new TLorentzVector();
            lvP1 = new TLorentzVector();
            lvPip = new TLorentzVector();
            lvPim = new TLorentzVector();
            h_nPerE_any = new TH1D("h_nPerE_any", "missing mass matches per event",
                               11, -0.5, 10.5);
            h_nPerE_stat = new TH1D("h_nPerE_stat", "missing mass matches per event, stat",
                                     11, -0.5, 10.5);
            h_nPerE_stat_id = new TH1D("h_nPerE_stat_id", "missing mass matches per event, stat, seb id",
                                        11, -0.5, 10.5);
            h_nPerE_stat_dcscstat = new TH1D("h_nPerE_stat_dcscstat", "missing mass matches per event, stat, dcsc",
                                             11, -0.5, 10.5);
            h_nPerE_stat_dcscstat_id = new TH1D("h_nPerE_stat_dcscstat_id", "missing mass matches per event, stat, dcsc, id",
                                                11, -0.5, 10.5);
            h_nevts = new TH1D("h_nevts", "number of events survived",
                                6, -0.5, 5.5);
            hmmppip_V_mmpS = MakeHists(5, "hmmppip_V_mmp_%d", "MMppip vs. MMp, crit #%d", 80, 0.4, 1.2, 120, 0, 1.2);
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
            delete h_nPerE_any;
            delete h_nPerE_stat;
            delete h_nPerE_stat_id;
            delete h_nPerE_stat_dcscstat;
            delete h_nPerE_stat_dcscstat_id;
            delete h_nevts;
            for_each(hmmppip_V_mmpS.begin(), hmmppip_V_mmpS.end(), DeleteObj);
        }
        virtual void Setup(H10 *d)
        {
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            h_nPerE_any->Write();
            h_nPerE_stat->Write();
            h_nPerE_stat_id->Write();
            h_nPerE_stat_dcscstat->Write();
            h_nPerE_stat_dcscstat_id->Write();
            h_nevts->Write();
            for_each(hmmppip_V_mmpS.begin(), hmmppip_V_mmpS.end(), WriteObj);
        }
        bool CutMM(H10* d, int iproton, int ipi)
        {
            lvP1->SetXYZM(d->p[iproton]*d->cx[iproton],
                          d->p[iproton]*d->cy[iproton],
                          d->p[iproton]*d->cz[iproton],
                          MASS_P);
            //doesn't matter if it's pi+ or pi-, so using lvPip and MASS_PIP
            lvPip->SetXYZM(d->p[ipi]*d->cx[ipi],
                           d->p[ipi]*d->cy[ipi],
                           d->p[ipi]*d->cz[ipi],
                           MASS_PIP);
            mmp = (*lvE0 - *lvE1 + *lvP0 - *lvP1).M();
            mmppi = (*lvE0 - *lvE1 + *lvP0 - *lvP1 - *lvPip).M();
            return (mmp >= 0.4 && mmp <= 1.2 && mmppi > 0);
        }
        virtual bool Handle(H10* d)
        {
            int nparts = d->gpart; //m_use_npart ? d->npart : d->gpart;
            lvE0->SetXYZT(0, 0, d->E0, d->E0);
            lvE1->SetXYZT(d->p[0]*d->cx[0], d->p[0]*d->cy[0], d->p[0]*d->cz[0], d->p[0]);
            int npos = 0, nneg = 0;
            const int NPOS_MAX = 8, NNEG_MAX = 8;
            int ipos[NPOS_MAX], ineg[NNEG_MAX];
            for (int ipart = 1; ipart < nparts; ipart++) {
                if (d->q[ipart]>0 && npos < NPOS_MAX) ipos[npos++] = ipart;
                else if (d->q[ipart]<0 && nneg < NNEG_MAX) ineg[nneg++] = ipart;
            }

            bool passed = false;

            //outer loop: assume each positive particle is the proton
            int nPerE_any = 0;
            int nPerE_stat = 0;
            int nPerE_stat_id = 0;
            int nPerE_stat_dcscstat = 0;
            int nPerE_stat_dcscstat_id = 0;
            for (int iproton = 0; iproton < npos; iproton++) {
                //assume each other positive particle is a pion
                for (int ipip = 0; ipip < npos; ipip++) {
                    if (iproton != ipip) {
                        if (CutMM(d, ipos[iproton], ipos[ipip])) {
                            nPerE_any++;
                            hmmppip_V_mmpS[0]->Fill(mmp, mmppi);
                            if (d->stat[ipos[iproton]] > 0 && d->stat[ipos[ipip]] > 0) {
                                nPerE_stat++;
                                hmmppip_V_mmpS[1]->Fill(mmp, mmppi);
                                if (d->id[ipos[iproton]]==PROTON && d->id[ipos[ipip]]==PIP) {
                                    nPerE_stat_id++;
                                    hmmppip_V_mmpS[3]->Fill(mmp, mmppi);
                                }
                                int dcidx_p = d->dc[ipos[iproton]]-1;
                                int scidx_p = d->sc[ipos[iproton]]-1;
                                int dcidx_pip = d->dc[ipos[ipip]]-1;
                                int scidx_pip = d->sc[ipos[ipip]]-1;
                                if (   dcidx_p>0 && scidx_p>0 && dcidx_pip>0 && scidx_pip>0
                                     && d->dc_stat[dcidx_p]>0 && d->sc_stat[scidx_p]>0
                                     && d->dc_stat[dcidx_pip]>0 && d->sc_stat[scidx_pip]>0) {
                                    nPerE_stat_dcscstat++;
                                    hmmppip_V_mmpS[2]->Fill(mmp, mmppi);
                                    //when we require dcscstat, let's also require that the
                                    //particle assumed to be proton has a higher mass than
                                    //the one assumed to be the pion...
                                    if (d->m[ipos[iproton]]>d->m[ipos[ipip]])
                                        passed = true;
                                    if (d->id[ipos[iproton]]==PROTON && d->id[ipos[ipip]]==PIP) {
                                        nPerE_stat_dcscstat_id++;
                                        hmmppip_V_mmpS[4]->Fill(mmp, mmppi);
                                    }
                                }
                            }
                            if (!m_require_dcsc) passed = true;
                        }
                    }
                }
                //assume each negative particle is a pion
                for (int ipim = 0; ipim < nneg; ipim++) {
                    if (CutMM(d, ipos[iproton], ineg[ipim])) {
                        // nPerE_any++;
                        if (d->stat[ipos[iproton]] > 0 && d->stat[ineg[ipim]] > 0) {
                            // nPerE_stat++;
                            // if (d->id[ipos[iproton]]==PROTON && d->id[ineg[ipim]]==PIM) {
                            //     nPerE_stat_id++;
                            // }
                            int dcidx_p = d->dc[ipos[iproton]]-1;
                            int scidx_p = d->sc[ipos[iproton]]-1;
                            int dcidx_pim = d->dc[ineg[ipim]]-1;
                            int scidx_pim = d->sc[ineg[ipim]]-1;
                            if (   dcidx_p>0 && scidx_p>0 && dcidx_pim>0 && scidx_pim>0
                                 && d->dc_stat[dcidx_p]>0 && d->sc_stat[scidx_p]>0
                                 && d->dc_stat[dcidx_pim]>0 && d->sc_stat[scidx_pim]>0) {
                                // nPerE_stat_dcscstat++;
                                passed = true;
                                // if (d->id[ipos[iproton]]==PROTON && d->id[ineg[ipim]]==PIM) {
                                //     nPerE_stat_dcscstat_id++;
                                // }
                            }
                        }
                        if (!m_require_dcsc) passed = true;
                    }
                }
            }
            h_nPerE_any->Fill(nPerE_any);
            h_nPerE_stat->Fill(nPerE_stat);
            h_nPerE_stat_id->Fill(nPerE_stat_id);
            h_nPerE_stat_dcscstat->Fill(nPerE_stat_dcscstat);
            h_nPerE_stat_dcscstat_id->Fill(nPerE_stat_dcscstat_id);
            h_nevts->SetBinContent(1,h_nevts->GetBinContent(1)+1);
            if (nPerE_any>0) h_nevts->SetBinContent(2,h_nevts->GetBinContent(2)+1);
            if (nPerE_stat>0) h_nevts->SetBinContent(3,h_nevts->GetBinContent(3)+1);
            if (nPerE_stat_dcscstat>0) h_nevts->SetBinContent(4,h_nevts->GetBinContent(4)+1);
            if (nPerE_stat_id>0) h_nevts->SetBinContent(5,h_nevts->GetBinContent(5)+1);
            if (nPerE_stat_dcscstat_id>0) h_nevts->SetBinContent(6,h_nevts->GetBinContent(6)+1);
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_MMP_SKIM_H_
