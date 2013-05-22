#include "h10t3pi_sel.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <THStack.h>
#include <TH2.h>
#include "particle-constants.h"
#include "scripts/xsect-utils.C"
#include "fid.h"
#include "scripts/simw.C"
#include "input/cc_eff_lazy_programmer.h"

using namespace TMath;
using namespace ParticleConstants;

class h3maker : public h10t3pi_sel
{
    public:
        Fid *fid;
        TFile *fout;
        TH3 *h3;
        TH2 *h2pfw;              //proton momentum versus W
        TH2 *h2eff_cc, *h2eff_acc, *h2eff_nentries;
        THStack *hs[7];
        bool w8ed;

        h3maker() : h10t3pi_sel()
        {
            printf("in h3maker::h3maker()\n");
            w8ed = true;
            fid = NULL;
            fout = NULL;
            h3 = NULL;
            h2pfw = NULL;
            h2eff_cc = NULL;
            h2eff_acc = NULL;
            h2eff_nentries = NULL;
            for (int i = 0; i < 7; i++) hs[i] = NULL;
        }
        ~h3maker()
        {
            printf("in h3maker::h3maker()\n");
            delete h3;
            delete h2pfw;
            delete h2eff_cc;
            delete h2eff_acc;
            delete h2eff_nentries;
            delete fout;
        }
        /*
        virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
          Int_t retval = 0;
          if (w8ed) retval = fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
          else {
            retval += b_k_W->GetEntry(entry);
        retval += b_k_Q2->GetEntry(entry);
        retval += b_k_mmp->GetEntry(entry);
        }
        //printf("*** entry = %lld\t",entry);
        return retval;
        }
        */
        virtual void Setup()
        {
            printf("in h3maker::Setup()\n");
            TString option = GetOption();
            w8ed = !option.Contains("now8");
            fid = Fid::Instance();
            fout = new TFile("h3maker-hn.root","recreate");
            h3 = new TH3F("hq2wmmp","Q^2:W:mmp",160,0.4,2.0,32,1.6,3.2,7,1.5,5.1);
            h2pfw = new TH2F("h2pfw","p_p vs W",160,1.6,3.2,400,0,4);
            Double_t qbins[] = { 1.5, 1.6, 1.8, 2.1, 2.4, 2.76, 3.3, 5.1 };
            h2eff_nentries = new TH2F("hq2w_eff_nentries","Q^2:W nentries",32,1.6,3.2,7,1.5,5.1);
            h2eff_nentries->GetYaxis()->Set(7,qbins);
            h2eff_cc = new TH2F("hq2w_eff_cc","Q^2:W CC cut efficiency",32,1.6,3.2,7,1.5,5.1);
            h2eff_cc->GetYaxis()->Set(7,qbins);
            h2eff_acc = new TH2F("hq2w_eff_acc","Q^2:W acceptance",32,1.6,3.2,7,1.5,5.1);
            h2eff_acc->GetYaxis()->Set(7,qbins);
            h3->GetZaxis()->Set(7,qbins);
            h3->Sumw2();
            //Double_t qbins[] = { 1.5, 2.0, 2.5, 3.1, 5.1 };
            //h3->GetZaxis()->Set(4,qbins);
        }
        virtual void Process()
        {
            int segment = (cc_segm[cc[h10idx_e]-1]%1000)/10;
            int pmt_hit = (cc_segm[cc[h10idx_e]-1]/1000)-1;
            int sector = cc_sect[cc[h10idx_e]-1];

            float err = 0;
            float wacc = fid->acc->GetAcc(kin.W,kin.Q2,kin.ct,kin.phi,err);
            float wcceff = ccw8(sector,segment,pmt_hit);
            double invw = kin.vgflux*wcceff*wacc;
            double weight = 0;
            if (invw > 0) weight = 1/invw;

            int ibinq2w = h2eff_acc->FindBin(kin.W, kin.Q2);
            h2eff_nentries->Fill(kin.W, kin.Q2);
            h2eff_acc->Fill(kin.W, kin.Q2, wacc);
            h2eff_cc->Fill(kin.W, kin.Q2, wcceff);
            h2pfw->Fill(kin.W,parts[1].p);
            float err2 = err*err + h2eff_acc->GetBinError(ibinq2w)*h2eff_acc->GetBinError(ibinq2w);
            h2eff_acc->SetBinError(ibinq2w, sqrt(err2));
            h2eff_cc->SetBinError(ibinq2w, 0);

            if (!w8ed) h3->Fill(kin.mmp,kin.W,kin.Q2);
            else
            {
                //printf("%f, %f, %f, %d, %d, %d\n",kin.vgflux,ccw8(sector,segment,pmt_hit),fid->acc->GetAcc(kin.W,kin.Q2,kin.ct,kin.phi,err),sector,segment,pmt_hit);
                if (invw > 0)
                {
                    int ibin = h3->FindBin(kin.mmp,kin.W,kin.Q2);

                    double binerror = h3->GetBinError(ibin);
                    // if (ibin==4161) {
                    // 	double bincontent = h3->GetBinContent(ibin);
                    // 	printf("before fill: %.2f +/- %.4f (%.2f\%)\n",bincontent,binerror,100*binerror/bincontent);
                    // }
                    h3->Fill(kin.mmp,kin.W,kin.Q2,weight);
                    // if (ibin==4161) {
                    // 	double bincontent = h3->GetBinContent(ibin);
                    // 	double berr = h3->GetBinError(ibin);
                    // 	printf(" after fill: %.2f +/- %.4f (%.2f\%)\n",bincontent,berr,100*berr/bincontent);
                    // 	printf("acc = %.3f +/- %.5f (%.2f\%)\n",wacc,err,100*err/wacc);
                    // }

                    //float err2 = binerror*binerror + 1.0/(wacc*wacc) + err*err/(wacc*wacc*wacc*wacc);
                    float err2 = binerror*binerror + weight*weight + err*err/(wacc*wacc*wacc*wacc);

                    //if (ibin==4161) printf("current: %.2f +/- %.3f (%.2f\%)\n*********************\n",weight,sqrt(err2),100*sqrt(err2)/weight);
                    h3->SetBinError(ibin,sqrt(err2));
                }
            }
        }
        virtual void Finalize()
        {
            printf("in h3maker::Finalize()\n");
            for (int ibinx = 1; ibinx < h2eff_nentries->GetNbinsX(); ibinx++) {
                for (int ibiny = 0; ibiny < h2eff_nentries->GetNbinsY(); ibiny++) {
                    h2eff_nentries->SetBinError(ibinx, ibiny, 0);
                    double err = h2eff_acc->GetBinError(ibinx, ibiny);
                    double scale = h2eff_nentries->GetBinContent(ibinx, ibiny);
                    h2eff_acc->SetBinError(ibinx, ibiny, err/scale);
                }
            }
            h2eff_cc->Divide(h2eff_nentries);
            h2eff_acc->Divide(h2eff_nentries);
            h2eff_cc->SetBit(TH1::kIsAverage);
            h2eff_acc->SetBit(TH1::kIsAverage);
            h3->Write();
            h2pfw->Write();
            h2eff_nentries->Write();
            h2eff_cc->Write();
            h2eff_acc->Write();
            TAxis *wax = h3->GetYaxis();
            TAxis *q2ax = h3->GetZaxis();
            int wbinof = wax->GetNbins()+1;
            int q2binof = q2ax->GetNbins()+1;
            for (int q2bin = 1; q2bin < q2binof; q2bin++)
            {
                hs[q2bin-1] = new THStack();
                for (int wbin = 1; wbin < wbinof; wbin++)
                {
                    float wvals[] = { wax->GetBinCenter(wbin), wax->GetBinLowEdge(wbin), wax->GetBinLowEdge(wbin+1) };
                    float q2vals[] = { q2ax->GetBinCenter(q2bin), q2ax->GetBinLowEdge(q2bin), q2ax->GetBinLowEdge(q2bin+1) };
                    TString hn = TString::Format("mmp_%.3f_%.3f",wvals[0],q2vals[0]);
                    TString ht = TString::Format("W = (%.3f,%.3f), Q^2 = (%.3f,%.3f)",wvals[1],wvals[2],q2vals[1],q2vals[2]);
                    TH1 *hmmp = h3->ProjectionX(hn.Data(),wbin,wbin,q2bin,q2bin);
                    //printf("%s\n",hn.Data());
                    if (hmmp->Integral()>0)
                    {
                        //printf("+++ %s\n",ht.Data());
                        hmmp->SetTitle(ht.Data());
                        hs[q2bin-1]->Add((TH1F*)hmmp->Clone());
                    }
                }
                TString hsn = TString::Format("hs%d",q2bin-1);
                fout->WriteObject(hs[q2bin-1],hsn.Data());
            }
        }
};
