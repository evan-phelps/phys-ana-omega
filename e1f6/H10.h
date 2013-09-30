//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Sep 22 22:28:42 2013 by ROOT version 5.30/00
// from TChain H10/
//////////////////////////////////////////////////////////

#ifndef H10_h
#define H10_h

#include <stdexcept>
#include <string>
#include <math.h>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TRegexp.h>
#include <TStopwatch.h>
#include <TObjString.h>
#include <TEntryList.h>
#include "HandlerChain.h"
#include "DataHandler.h"
#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif
#include "Config.h"
using namespace H10Constants;

/*
TODO: Separate run conditions into separate class
rather than testing for run number (see, e.g., beamEnergy).
*/
class H10
{

    protected :
        Long64_t _ntoprocess;
        TRegexp *fRegExp_run, *fRegExp_Anum;
        TStopwatch _swmain;
        TStopwatch _swgroup;
        static const Int_t blocksize = 100000;
        Long64_t eventnum;
        void PrintProgress(Long64_t entry)
        {
            if ( ++eventnum % blocksize == 0 )
            {
                Float_t gtime = _swgroup.RealTime();
                Float_t ttime = _swmain.RealTime();
                Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
                Float_t remaining = (100/percentProcessed*ttime-ttime)/60;
                printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(blocksize/gtime),remaining);
                _swgroup.Start();
                _swmain.Start(kFALSE);
            }
        }

    public :
        HandlerChain *fHandlerChain;
        virtual void Add(DataHandler *dhandler)
        {
            fHandlerChain->Add(dhandler);
            dhandler->SetH10Looper(this);
        }

        Int_t fTreeNumber;
        Long64_t jentry;

        TTree          *fChain;  //!pointer to the analyzed TTree or TChain
        Int_t           fCurrent;//!current Tree number in a TChain

        bool is_sim;
        int run;
        int file_anum;
        std::string filename;
        Float_t beamEnergy;
        Config *cfg;

        // Declaration of leaf types
        UChar_t   npart;
        UChar_t   evstat;
        UInt_t    evntid;
        Char_t    evtype;
        Char_t    evntclas;
        Char_t    evthel;
        Int_t     evntclas2;
        Float_t   q_l;
        Float_t   t_l;
        Float_t   tr_time;
        Float_t   rf_time1;
        Float_t   rf_time2;
        Int_t     gpart;
        Short_t   id[40];
        Char_t    stat[40];
        UChar_t   dc[40];
        UChar_t   cc[40];
        UChar_t   sc[40];
        UChar_t   ec[40];
        UChar_t   lec[40];
        Float_t   p[40];
        Float_t   m[40];
        Char_t    q[40];
        Float_t   beta[40];
        Float_t   cx[40];
        Float_t   cy[40];
        Float_t   cz[40];
        Float_t   vx[40];
        Float_t   vy[40];
        Float_t   vz[40];
        Int_t     dc_part;
        UChar_t   dc_sect[40];
        UChar_t   dc_trk[40];
        Char_t    dc_stat[40];
        Float_t   dc_xsc[40];
        Float_t   dc_ysc[40];
        Float_t   dc_zsc[40];
        Float_t   dc_cxsc[40];
        Float_t   dc_cysc[40];
        Float_t   dc_czsc[40];
        Float_t   dc_xec[40];
        Float_t   dc_yec[40];
        Float_t   dc_zec[40];
        Float_t   dc_thcc[40];
        Float_t   dc_c2[40];
        Int_t     ec_part;
        UShort_t  ec_stat[40];
        UChar_t   ec_sect[40];
        Int_t     ec_whol[40];
        Int_t     ec_inst[40];
        Int_t     ec_oust[40];
        Float_t   etot[40];
        Float_t   ec_ei[40];
        Float_t   ec_eo[40];
        Float_t   ec_t[40];
        Float_t   ec_r[40];
        Float_t   ech_x[40];
        Float_t   ech_y[40];
        Float_t   ech_z[40];
        Float_t   ec_m2[40];
        Float_t   ec_m3[40];
        Float_t   ec_m4[40];
        Float_t   ec_c2[40];
        Int_t     sc_part;
        UChar_t   sc_sect[40];
        UChar_t   sc_hit[40];
        UChar_t   sc_pd[40];
        UChar_t   sc_stat[40];
        Float_t   edep[40];
        Float_t   sc_t[40];
        Float_t   sc_r[40];
        Float_t   sc_c2[40];
        Int_t     cc_part;
        UChar_t   cc_sect[40];
        UChar_t   cc_hit[40];
        Int_t     cc_segm[40];
        UShort_t  nphe[40];
        Float_t   cc_t[40];
        Float_t   cc_r[40];
        Float_t   cc_c2[40];
        Int_t     lac_part;
        Int_t     lec_sect[50];
        Int_t     lec_hit[50];
        Int_t     lec_stat[50];
        Float_t   lec_etot[50];
        Float_t   lec_t[50];
        Float_t   lec_r[50];
        Float_t   lec_x[50];
        Float_t   lec_y[50];
        Float_t   lec_z[50];
        Float_t   lec_c2[50];
        //MC banks
        Int_t     mcnentr;
        UChar_t   mcnpart;
        Int_t     mcst[20];
        Int_t     mcid[20];
        Int_t     mcpid[20];
        Float_t   mctheta[20];
        Float_t   mcphi[20];
        Float_t   mcp[20];
        Float_t   mcm[20];

        // List of branches
        TBranch   *b_npart;      //!
        TBranch   *b_evstat;     //!
        TBranch   *b_evntid;     //!
        TBranch   *b_evtype;     //!
        TBranch   *b_evntclas;   //!
        TBranch   *b_evthel;     //!
        TBranch   *b_evntclas2;  //!
        TBranch   *b_q_l;        //!
        TBranch   *b_t_l;        //!
        TBranch   *b_tr_time;    //!
        TBranch   *b_rf_time1;   //!
        TBranch   *b_rf_time2;   //!
        TBranch   *b_gpart;      //!
        TBranch   *b_id;         //!
        TBranch   *b_stat;       //!
        TBranch   *b_dc;         //!
        TBranch   *b_cc;         //!
        TBranch   *b_sc;         //!
        TBranch   *b_ec;         //!
        TBranch   *b_lec;        //!
        TBranch   *b_p;          //!
        TBranch   *b_m;          //!
        TBranch   *b_q;          //!
        TBranch   *b_beta;       //!
        TBranch   *b_cx;         //!
        TBranch   *b_cy;         //!
        TBranch   *b_cz;         //!
        TBranch   *b_vx;         //!
        TBranch   *b_vy;         //!
        TBranch   *b_vz;         //!
        TBranch   *b_dc_part;    //!
        TBranch   *b_dc_sect;    //!
        TBranch   *b_dc_trk;     //!
        TBranch   *b_dc_stat;    //!
        TBranch   *b_dc_xsc;     //!
        TBranch   *b_dc_ysc;     //!
        TBranch   *b_dc_zsc;     //!
        TBranch   *b_dc_cxsc;    //!
        TBranch   *b_dc_cysc;    //!
        TBranch   *b_dc_czsc;    //!
        TBranch   *b_dc_xec;     //!
        TBranch   *b_dc_yec;     //!
        TBranch   *b_dc_zec;     //!
        TBranch   *b_dc_thcc;    //!
        TBranch   *b_dc_c2;      //!
        TBranch   *b_ec_part;    //!
        TBranch   *b_ec_stat;    //!
        TBranch   *b_ec_sect;    //!
        TBranch   *b_ec_whol;    //!
        TBranch   *b_ec_inst;    //!
        TBranch   *b_ec_oust;    //!
        TBranch   *b_etot;       //!
        TBranch   *b_ec_ei;      //!
        TBranch   *b_ec_eo;      //!
        TBranch   *b_ec_t;       //!
        TBranch   *b_ec_r;       //!
        TBranch   *b_ech_x;      //!
        TBranch   *b_ech_y;      //!
        TBranch   *b_ech_z;      //!
        TBranch   *b_ec_m2;      //!
        TBranch   *b_ec_m3;      //!
        TBranch   *b_ec_m4;      //!
        TBranch   *b_ec_c2;      //!
        TBranch   *b_sc_part;    //!
        TBranch   *b_sc_sect;    //!
        TBranch   *b_sc_hit;     //!
        TBranch   *b_sc_pd;      //!
        TBranch   *b_sc_stat;    //!
        TBranch   *b_edep;       //!
        TBranch   *b_sc_t;       //!
        TBranch   *b_sc_r;       //!
        TBranch   *b_sc_c2;      //!
        TBranch   *b_cc_part;    //!
        TBranch   *b_cc_sect;    //!
        TBranch   *b_cc_hit;     //!
        TBranch   *b_cc_segm;    //!
        TBranch   *b_nphe;       //!
        TBranch   *b_cc_t;       //!
        TBranch   *b_cc_r;       //!
        TBranch   *b_cc_c2;      //!
        TBranch   *b_lac_part;   //!
        TBranch   *b_lec_sect;   //!
        TBranch   *b_lec_hit;    //!
        TBranch   *b_lec_stat;   //!
        TBranch   *b_lec_etot;   //!
        TBranch   *b_lec_t;      //!
        TBranch   *b_lec_r;      //!
        TBranch   *b_lec_x;      //!
        TBranch   *b_lec_y;      //!
        TBranch   *b_lec_z;      //!
        TBranch   *b_lec_c2;     //!
        TBranch   *b_mcnentr;    //!
        TBranch   *b_mcnpart;    //!
        TBranch   *b_mcst;       //!
        TBranch   *b_mcid;       //!
        TBranch   *b_mcpid;      //!
        TBranch   *b_mctheta;    //!
        TBranch   *b_mcphi;      //!
        TBranch   *b_mcp;        //!
        TBranch   *b_mcm;        //!

        H10(TTree *tree, std::string experiment);
        virtual ~H10();
        virtual Int_t    Cut(Long64_t entry);
        virtual Int_t    GetEntry(Long64_t entry);
        virtual Long64_t LoadTree(Long64_t entry);
        virtual void     Init(TTree *tree);
        virtual void     Loop(Long64_t ntoproc = -1, Bool_t fastcount = kTRUE, TEntryList *elist = 0);
        virtual Bool_t   Notify();
        virtual void     Show(Long64_t entry = -1);
        Float_t          E0() { return beamEnergy; };
        Float_t          nu() { return E0()-p[0]; };
        Float_t          Q2() { return -(nu()*nu()-p[0]*p[0]-E0()*E0()+2*E0()*p[0]*cz[0]); };
        Float_t          s()  { return -Q2()+2*MASS_P*nu()+MASS_P*MASS_P; };
        Float_t          W()  { return s() >= 0 ? sqrt(s()) : -sqrt(-s()); };
};
#endif

#ifdef H10_cxx
Int_t H10::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}


Long64_t H10::LoadTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent)
    {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}


void H10::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    TBranch *simTestBranch = fChain->GetBranch("mcnentr");
    TBranch *expTestBranch = fChain->GetBranch("gpart");
    if (simTestBranch)
    {
        is_sim = kTRUE;
        fChain->SetBranchAddress("mcnentr", &mcnentr, &b_mcnentr);
        fChain->SetBranchAddress("mcnpart", &mcnpart, &b_mcnpart);
        fChain->SetBranchAddress("mcst", mcst, &b_mcst);
        fChain->SetBranchAddress("mcid", mcid, &b_mcid);
        fChain->SetBranchAddress("mcpid", mcpid, &b_mcpid);
        fChain->SetBranchAddress("mctheta", mctheta, &b_mctheta);
        fChain->SetBranchAddress("mcphi", mcphi, &b_mcphi);
        fChain->SetBranchAddress("mcp", mcp, &b_mcp);
        fChain->SetBranchAddress("mcm", mcm, &b_mcm);
    }
    if (expTestBranch)
    {
        fChain->SetBranchAddress("npart", &npart, &b_npart);
        fChain->SetBranchAddress("evstat", &evstat, &b_evstat);
        fChain->SetBranchAddress("evntid", &evntid, &b_evntid);
        fChain->SetBranchAddress("evtype", &evtype, &b_evtype);
        fChain->SetBranchAddress("evntclas", &evntclas, &b_evntclas);
        fChain->SetBranchAddress("evthel", &evthel, &b_evthel);
        fChain->SetBranchAddress("evntclas2", &evntclas2, &b_evntclas2);
        fChain->SetBranchAddress("q_l", &q_l, &b_q_l);
        fChain->SetBranchAddress("t_l", &t_l, &b_t_l);
        fChain->SetBranchAddress("tr_time", &tr_time, &b_tr_time);
        fChain->SetBranchAddress("rf_time1", &rf_time1, &b_rf_time1);
        fChain->SetBranchAddress("rf_time2", &rf_time2, &b_rf_time2);
        fChain->SetBranchAddress("gpart", &gpart, &b_gpart);
        fChain->SetBranchAddress("id", id, &b_id);
        fChain->SetBranchAddress("stat", stat, &b_stat);
        fChain->SetBranchAddress("dc", dc, &b_dc);
        fChain->SetBranchAddress("cc", cc, &b_cc);
        fChain->SetBranchAddress("sc", sc, &b_sc);
        fChain->SetBranchAddress("ec", ec, &b_ec);
        fChain->SetBranchAddress("lec", lec, &b_lec);
        fChain->SetBranchAddress("p", p, &b_p);
        fChain->SetBranchAddress("m", m, &b_m);
        fChain->SetBranchAddress("q", q, &b_q);
        fChain->SetBranchAddress("b", beta, &b_beta);
        fChain->SetBranchAddress("cx", cx, &b_cx);
        fChain->SetBranchAddress("cy", cy, &b_cy);
        fChain->SetBranchAddress("cz", cz, &b_cz);
        fChain->SetBranchAddress("vx", vx, &b_vx);
        fChain->SetBranchAddress("vy", vy, &b_vy);
        fChain->SetBranchAddress("vz", vz, &b_vz);
        fChain->SetBranchAddress("dc_part", &dc_part, &b_dc_part);
        fChain->SetBranchAddress("dc_sect", dc_sect, &b_dc_sect);
        fChain->SetBranchAddress("dc_trk", dc_trk, &b_dc_trk);
        fChain->SetBranchAddress("dc_stat", dc_stat, &b_dc_stat);
        fChain->SetBranchAddress("dc_xsc", dc_xsc, &b_dc_xsc);
        fChain->SetBranchAddress("dc_ysc", dc_ysc, &b_dc_ysc);
        fChain->SetBranchAddress("dc_zsc", dc_zsc, &b_dc_zsc);
        fChain->SetBranchAddress("dc_cxsc", dc_cxsc, &b_dc_cxsc);
        fChain->SetBranchAddress("dc_cysc", dc_cysc, &b_dc_cysc);
        fChain->SetBranchAddress("dc_czsc", dc_czsc, &b_dc_czsc);
        fChain->SetBranchAddress("dc_xec", dc_xec, &b_dc_xec);
        fChain->SetBranchAddress("dc_yec", dc_yec, &b_dc_yec);
        fChain->SetBranchAddress("dc_zec", dc_zec, &b_dc_zec);
        fChain->SetBranchAddress("dc_thcc", dc_thcc, &b_dc_thcc);
        fChain->SetBranchAddress("dc_c2", dc_c2, &b_dc_c2);
        fChain->SetBranchAddress("ec_part", &ec_part, &b_ec_part);
        fChain->SetBranchAddress("ec_stat", ec_stat, &b_ec_stat);
        fChain->SetBranchAddress("ec_sect", ec_sect, &b_ec_sect);
        fChain->SetBranchAddress("ec_whol", ec_whol, &b_ec_whol);
        fChain->SetBranchAddress("ec_inst", ec_inst, &b_ec_inst);
        fChain->SetBranchAddress("ec_oust", ec_oust, &b_ec_oust);
        fChain->SetBranchAddress("etot", etot, &b_etot);
        fChain->SetBranchAddress("ec_ei", ec_ei, &b_ec_ei);
        fChain->SetBranchAddress("ec_eo", ec_eo, &b_ec_eo);
        fChain->SetBranchAddress("ec_t", ec_t, &b_ec_t);
        fChain->SetBranchAddress("ec_r", ec_r, &b_ec_r);
        fChain->SetBranchAddress("ech_x", ech_x, &b_ech_x);
        fChain->SetBranchAddress("ech_y", ech_y, &b_ech_y);
        fChain->SetBranchAddress("ech_z", ech_z, &b_ech_z);
        fChain->SetBranchAddress("ec_m2", ec_m2, &b_ec_m2);
        fChain->SetBranchAddress("ec_m3", ec_m3, &b_ec_m3);
        fChain->SetBranchAddress("ec_m4", ec_m4, &b_ec_m4);
        fChain->SetBranchAddress("ec_c2", ec_c2, &b_ec_c2);
        fChain->SetBranchAddress("sc_part", &sc_part, &b_sc_part);
        fChain->SetBranchAddress("sc_sect", sc_sect, &b_sc_sect);
        fChain->SetBranchAddress("sc_hit", sc_hit, &b_sc_hit);
        fChain->SetBranchAddress("sc_pd", sc_pd, &b_sc_pd);
        fChain->SetBranchAddress("sc_stat", sc_stat, &b_sc_stat);
        fChain->SetBranchAddress("edep", edep, &b_edep);
        fChain->SetBranchAddress("sc_t", sc_t, &b_sc_t);
        fChain->SetBranchAddress("sc_r", sc_r, &b_sc_r);
        fChain->SetBranchAddress("sc_c2", sc_c2, &b_sc_c2);
        fChain->SetBranchAddress("cc_part", &cc_part, &b_cc_part);
        fChain->SetBranchAddress("cc_sect", cc_sect, &b_cc_sect);
        fChain->SetBranchAddress("cc_hit", cc_hit, &b_cc_hit);
        fChain->SetBranchAddress("cc_segm", cc_segm, &b_cc_segm);
        fChain->SetBranchAddress("nphe", nphe, &b_nphe);
        fChain->SetBranchAddress("cc_t", cc_t, &b_cc_t);
        fChain->SetBranchAddress("cc_r", cc_r, &b_cc_r);
        fChain->SetBranchAddress("cc_c2", cc_c2, &b_cc_c2);
        fChain->SetBranchAddress("lac_part", &lac_part, &b_lac_part);
        fChain->SetBranchAddress("lec_sect", lec_sect, &b_lec_sect);
        fChain->SetBranchAddress("lec_hit", lec_hit, &b_lec_hit);
        fChain->SetBranchAddress("lec_stat", lec_stat, &b_lec_stat);
        fChain->SetBranchAddress("lec_etot", lec_etot, &b_lec_etot);
        fChain->SetBranchAddress("lec_t", lec_t, &b_lec_t);
        fChain->SetBranchAddress("lec_r", lec_r, &b_lec_r);
        fChain->SetBranchAddress("lec_x", lec_x, &b_lec_x);
        fChain->SetBranchAddress("lec_y", lec_y, &b_lec_y);
        fChain->SetBranchAddress("lec_z", lec_z, &b_lec_z);
        fChain->SetBranchAddress("lec_c2", lec_c2, &b_lec_c2);
    }

    Notify();
}


Bool_t H10::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.
    int tn = fChain->GetTreeNumber();
    if (tn != fTreeNumber)
    {
        TString fullfn = ((TChain*)fChain)->GetFile()->GetName();
        TObjArray *tokens = fullfn.Tokenize("/");
        TObjString *tok = (TObjString*)tokens->At(tokens->GetLast());
        TString fn = tok->GetString();
        run = ((TString)fn(*fRegExp_run)).Atoi();
        //printf("opening run %d\n",run);
        file_anum = ((TString)((TString)fn(*fRegExp_Anum))(1,2)).Atoi();
        filename = fn.Data();
        delete tokens;
    }
    return kTRUE;
}


void H10::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}


Int_t H10::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
#endif                           // #ifdef H10_cxx
