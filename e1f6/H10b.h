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
#include <TLorentzVector.h>
#include <TRotation.h>
#include <TLorentzRotation.h>
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
                printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",
		       percentProcessed, eventnum, ttime,
		       ((Int_t)(eventnum/ttime)), (Int_t)(blocksize/gtime), remaining);
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
        UChar_t         npart;
        UChar_t         evstat;
        UInt_t          evntid;
        Char_t          evntype;
        Short_t         evntclas;
        Float_t         q_l;
        Float_t         t_l;
        Float_t         tr_time;
        Float_t         rf_time;
        Int_t           l2bit;
        Int_t           l3bit;
        Int_t           hlsc;
        Int_t           intt;
        Int_t           gpart;
        Int_t           id[20];   //[gpart]
        Int_t           stat[20];   //[gpart]
        Int_t           dc[20];   //[gpart]
        Int_t           cc[20];   //[gpart]
        Int_t           sc[20];   //[gpart]
        Int_t           ec[20];   //[gpart]
        Int_t           lec[20];   //[gpart]
        Int_t           st[20];   //[gpart]
        Float_t         p[20];   //[gpart]
        Float_t         m[20];   //[gpart]
        Int_t           q[20];   //[gpart]
        Float_t         b[20];   //[gpart]
        Float_t         cx[20];   //[gpart]
        Float_t         cy[20];   //[gpart]
        Float_t         cz[20];   //[gpart]
        Float_t         vx[20];   //[gpart]
        Float_t         vy[20];   //[gpart]
        Float_t         vz[20];   //[gpart]
        Int_t           dc_part;
        Int_t           dc_sect[20];   //[dc_part]
        Int_t           dc_trk[20];   //[dc_part]
        Int_t           dc_stat[20];   //[dc_part]
        Int_t           tb_st[20];   //[dc_part]
        Float_t         dc_xsc[20];   //[dc_part]
        Float_t         dc_ysc[20];   //[dc_part]
        Float_t         dc_zsc[20];   //[dc_part]
        Float_t         dc_cxsc[20];   //[dc_part]
        Float_t         dc_cysc[20];   //[dc_part]
        Float_t         dc_czsc[20];   //[dc_part]
        Float_t         dc_vx[20];   //[dc_part]
        Float_t         dc_vy[20];   //[dc_part]
        Float_t         dc_vz[20];   //[dc_part]
        Float_t         dc_vr[20];   //[dc_part]
        Float_t         tl1_cx[20];   //[dc_part]
        Float_t         tl1_cy[20];   //[dc_part]
        Float_t         tl1_cz[20];   //[dc_part]
        Float_t         tl1_x[20];   //[dc_part]
        Float_t         tl1_y[20];   //[dc_part]
        Float_t         tl1_z[20];   //[dc_part]
        Float_t         tl1_r[20];   //[dc_part]
        Float_t         dc_c2[20];   //[dc_part]
        Int_t           ec_part;
        Int_t           ec_stat[20];   //[ec_part]
        Int_t           ec_sect[20];   //[ec_part]
        Int_t           ec_whol[20];   //[ec_part]
        Int_t           ec_inst[20];   //[ec_part]
        Int_t           ec_oust[20];   //[ec_part]
        Float_t         etot[20];   //[ec_part]
        Float_t         ec_ei[20];   //[ec_part]
        Float_t         ec_eo[20];   //[ec_part]
        Float_t         ec_t[20];   //[ec_part]
        Float_t         ec_r[20];   //[ec_part]
        Float_t         ech_x[20];   //[ec_part]
        Float_t         ech_y[20];   //[ec_part]
        Float_t         ech_z[20];   //[ec_part]
        Float_t         ec_m2[20];   //[ec_part]
        Float_t         ec_m3[20];   //[ec_part]
        Float_t         ec_m4[20];   //[ec_part]
        Float_t         ec_c2[20];   //[ec_part]
        Int_t           sc_part;   Int_t           sc_sect[20];   //[sc_part]
        Int_t           sc_hit[20];   //[sc_part]
        Int_t           sc_pd[20];   //[sc_part]
        Int_t           sc_stat[20];   //[sc_part]
        Float_t         edep[20];   //[sc_part]
        Float_t         sc_t[20];   //[sc_part]
        Float_t         sc_r[20];   //[sc_part]
        Float_t         sc_c2[20];   //[sc_part]
        Int_t           cc_part;
        Int_t           cc_sect[20];   //[cc_part]
        Int_t           cc_hit[20];   //[cc_part]
        Int_t           cc_segm[20];   //[cc_part]
        Int_t           nphe[20];   //[cc_part]
        Float_t         cc_t[20];   //[cc_part]
        Float_t         cc_r[20];   //[cc_part]
        Float_t         cc_c2[20];   //[cc_part]
        Int_t           lac_part;
        Int_t           lec_sect[20];   //[lac_part]
        Int_t           lec_hit[20];   //[lac_part]
        Int_t           lec_stat[20];   //[lac_part]
        Float_t         lec_etot[20];   //[lac_part]
        Float_t         lec_ein[20];   //[lac_part]
        Float_t         lec_t[20];   //[lac_part]
        Float_t         lec_r[20];   //[lac_part]
        Float_t         lec_x[20];   //[lac_part]
        Float_t         lec_y[20];   //[lac_part]
        Float_t         lec_z[20];   //[lac_part]
        Float_t         lec_c2[20];   //[lac_part]
        Int_t           nprt;
        Int_t           pidpart[12];   //[nprt]
        Float_t         xpart[12];   //[nprt]
        Float_t         ypart[12];   //[nprt]
        Float_t         zpart[12];   //[nprt]
        Float_t         epart[12];   //[nprt]
        Float_t         pxpart[12];   //[nprt]
        Float_t         pypart[12];   //[nprt]
        Float_t         pzpart[12];   //[nprt]
        Float_t         qpart[12];   //[nprt]
        Int_t           nsc1;
        Int_t           sc1sesctpd[40];   //[nsc1]
        Float_t         sctl[40];   //[nsc1]
        Float_t         scel[40];   //[nsc1]
        Float_t         sctr[40];   //[nsc1]
        Float_t         scer[40];   //[nsc1]

        // List of branches
        TBranch        *b_npart;   //!
        TBranch        *b_evstat;   //!
        TBranch        *b_evntid;   //!
        TBranch        *b_evntype;   //!
        TBranch        *b_evntclas;   //!
        TBranch        *b_q_l;   //!
        TBranch        *b_t_l;   //!
        TBranch        *b_tr_time;   //!
        TBranch        *b_rf_time;   //!
        TBranch        *b_l2bit;   //!
        TBranch        *b_l3bit;   //!
        TBranch        *b_hlsc;   //!
        TBranch        *b_intt;   //!
        TBranch        *b_gpart;   //!
        TBranch        *b_id;   //!
        TBranch        *b_stat;   //!
        TBranch        *b_dc;   //!
        TBranch        *b_cc;   //!
        TBranch        *b_sc;   //!
        TBranch        *b_ec;   //!
        TBranch        *b_lec;   //!
        TBranch        *b_st;   //!
        TBranch        *b_p;   //!
        TBranch        *b_m;   //!
        TBranch        *b_q;   //!
        TBranch        *b_b;   //!
        TBranch        *b_cx;   //!
        TBranch        *b_cy;   //!
        TBranch        *b_cz;   //!
        TBranch        *b_vx;   //!
        TBranch        *b_vy;   //!
        TBranch        *b_vz;   //!
        TBranch        *b_dc_part;   //!
        TBranch        *b_dc_sect;   //!
        TBranch        *b_dc_trk;   //!
        TBranch        *b_dc_stat;   //!
        TBranch        *b_tb_st;   //!
        TBranch        *b_dc_xsc;   //!
        TBranch        *b_dc_ysc;   //!
        TBranch        *b_dc_zsc;   //!
        TBranch        *b_dc_cxsc;   //!
        TBranch        *b_dc_cysc;   //!
        TBranch        *b_dc_czsc;   //!
        TBranch        *b_dc_vx;   //!
        TBranch        *b_dc_vy;   //!
        TBranch        *b_dc_vz;   //!
        TBranch        *b_dc_vr;   //!
        TBranch        *b_tl1_cx;   //!
        TBranch        *b_tl1_cy;   //!
        TBranch        *b_tl1_cz;   //!
        TBranch        *b_tl1_x;   //!
        TBranch        *b_tl1_y;   //!
        TBranch        *b_tl1_z;   //!
        TBranch        *b_tl1_r;   //!
        TBranch        *b_dc_c2;   //!
        TBranch        *b_ec_part;   //!
        TBranch        *b_ec_stat;   //!
        TBranch        *b_ec_sect;   //!
        TBranch        *b_ec_whol;   //!
        TBranch        *b_ec_inst;   //!
        TBranch        *b_ec_oust;   //!
        TBranch        *b_etot;   //!
        TBranch        *b_ec_ei;   //!
        TBranch        *b_ec_eo;   //!
        TBranch        *b_ec_t;   //!
        TBranch        *b_ec_r;   //!
        TBranch        *b_ech_x;   //!
        TBranch        *b_ech_y;   //!
        TBranch        *b_ech_z;   //!
        TBranch        *b_ec_m2;   //!
        TBranch        *b_ec_m3;   //!
        TBranch        *b_ec_m4;   //!
        TBranch        *b_ec_c2;   //!
        TBranch        *b_sc_part;   //!
        TBranch        *b_sc_sect;   //!
        TBranch        *b_sc_hit;   //!
        TBranch        *b_sc_pd;   //!
        TBranch        *b_sc_stat;   //!
        TBranch        *b_edep;   //!
        TBranch        *b_sc_t;   //!
        TBranch        *b_sc_r;   //!
        TBranch        *b_sc_c2;   //!
        TBranch        *b_cc_part;   //!
        TBranch        *b_cc_sect;   //!
        TBranch        *b_cc_hit;   //!
        TBranch        *b_cc_segm;   //!
        TBranch        *b_nphe;   //!
        TBranch        *b_cc_t;   //!
        TBranch        *b_cc_r;   //!
        TBranch        *b_cc_c2;   //!
        TBranch        *b_lac_part;   //!
        TBranch        *b_lec_sect;   //!
        TBranch        *b_lec_hit;   //!
        TBranch        *b_lec_stat;   //!
        TBranch        *b_lec_etot;   //!
        TBranch        *b_lec_ein;   //!
        TBranch        *b_lec_t;   //!
        TBranch        *b_lec_r;   //!
        TBranch        *b_lec_x;   //!
        TBranch        *b_lec_y;   //!
        TBranch        *b_lec_z;   //!
        TBranch        *b_lec_c2;   //!
        TBranch        *b_nprt;   //!
        TBranch        *b_pidpart;   //!
        TBranch        *b_xpart;   //!
        TBranch        *b_ypart;   //!
        TBranch        *b_zpart;   //!
        TBranch        *b_epart;   //!
        TBranch        *b_pxpart;   //!
        TBranch        *b_pypart;   //!
        TBranch        *b_pzpart;   //!
        TBranch        *b_qpart;   //!
        TBranch        *b_nsc1;   //!
        TBranch        *b_sc1sesctpd;   //!
        TBranch        *b_sctl;   //!
        TBranch        *b_scel;   //!
        TBranch        *b_sctr;   //!
        TBranch        *b_scer;   //!

        H10(TTree *tree, std::string experiment);
        virtual ~H10();
        virtual Int_t    Cut(Long64_t entry);
        virtual Int_t    GetEntry(Long64_t entry);
        virtual Long64_t LoadTree(Long64_t entry);
        virtual void     Init(TTree *tree);
        virtual void     Loop(Long64_t ntoproc = -1, Bool_t fastcount = kTRUE, TEntryList *elist = 0);
        virtual Bool_t   Notify();
        virtual void     Show(Long64_t entry = -1);
        void GetUVW(double xyz[3], double uvw[3]);
        void CalcLVs();

        TLorentzVector  lvE0, lvE1, lvP0, lvP1, lvPip, lvPim,
                        lvW, lvq, lvMMp, lvMMppip, lvMMppim, lvMMppippim;
        Float_t E0, nu, Q2,  W,  s, t, t0, t1,
                MMp, MMppip, MMppim, MMppippim,
                cosTheta, phi;
        int esector;
        Int_t np, npip, npim, nother, n0;
        Int_t npos, nneg, nneu;
};
#endif

#ifdef H10_cxx

H10::H10(TTree *tree, std::string fn_config)
{
    cfg = new Config(fn_config.c_str());
    // if (experiment == "e1f") cfg = new Config("input.e16.exp.parms");
    // else if(experiment == "e16") cfg = new Config("input.e1f.exp.parms");
    // else
    // {
    //     std::string emsg = "experiment not recognized! must be e1f or e16";
    //     throw new std::runtime_error(emsg.c_str());
    // }
    esector = 0;
    is_sim = kFALSE;
    run = -1;
    beamEnergy = cfg->GetFloat("beam_energy");
    file_anum = -1;
    filename = "";
    fHandlerChain = new HandlerChain();
    fTreeNumber = -1;
    eventnum = 0;
    fRegExp_run = new TRegexp("[0-9][0-9][0-9][0-9][0-9]");
    fRegExp_Anum = new TRegexp("[Aa][0-9][0-9]");
    E0 = nu = Q2 = s = W = MMp = MMppip = MMppim = MMppippim = cosTheta = phi = t = t0 = t1 = 0;
    np = npip = npim = 0;
    lvE0.SetXYZM(0,0,beamEnergy,MASS_E);
    lvP0.SetXYZM(0,0,0,MASS_P);
    Init(tree);
}


H10::~H10()
{
    if (fChain) delete fChain->GetCurrentFile();
    delete fHandlerChain;
    delete fRegExp_run;
    delete fRegExp_Anum;
    delete cfg;
}


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

    fChain->SetBranchAddress("npart", &npart, &b_npart);
    fChain->SetBranchAddress("evstat", &evstat, &b_evstat);
    fChain->SetBranchAddress("evntid", &evntid, &b_evntid);
    fChain->SetBranchAddress("evntype", &evntype, &b_evntype);
    fChain->SetBranchAddress("evntclas", &evntclas, &b_evntclas);
    fChain->SetBranchAddress("q_l", &q_l, &b_q_l);
    fChain->SetBranchAddress("t_l", &t_l, &b_t_l);
    fChain->SetBranchAddress("tr_time", &tr_time, &b_tr_time);
    fChain->SetBranchAddress("rf_time", &rf_time, &b_rf_time);
    fChain->SetBranchAddress("l2bit", &l2bit, &b_l2bit);
    fChain->SetBranchAddress("l3bit", &l3bit, &b_l3bit);
    fChain->SetBranchAddress("hlsc", &hlsc, &b_hlsc);
    fChain->SetBranchAddress("intt", &intt, &b_intt);
    fChain->SetBranchAddress("gpart", &gpart, &b_gpart);
    fChain->SetBranchAddress("id", id, &b_id);
    fChain->SetBranchAddress("stat", stat, &b_stat);
    fChain->SetBranchAddress("dc", dc, &b_dc);
    fChain->SetBranchAddress("cc", cc, &b_cc);
    fChain->SetBranchAddress("sc", sc, &b_sc);
    fChain->SetBranchAddress("ec", ec, &b_ec);
    fChain->SetBranchAddress("lec", lec, &b_lec);
    fChain->SetBranchAddress("st", st, &b_st);
    fChain->SetBranchAddress("p", p, &b_p);
    fChain->SetBranchAddress("m", m, &b_m);
    fChain->SetBranchAddress("q", q, &b_q);
    fChain->SetBranchAddress("b", b, &b_b);
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
    fChain->SetBranchAddress("tb_st", tb_st, &b_tb_st);
    fChain->SetBranchAddress("dc_xsc", dc_xsc, &b_dc_xsc);
    fChain->SetBranchAddress("dc_ysc", dc_ysc, &b_dc_ysc);
    fChain->SetBranchAddress("dc_zsc", dc_zsc, &b_dc_zsc);
    fChain->SetBranchAddress("dc_cxsc", dc_cxsc, &b_dc_cxsc);
    fChain->SetBranchAddress("dc_cysc", dc_cysc, &b_dc_cysc);
    fChain->SetBranchAddress("dc_czsc", dc_czsc, &b_dc_czsc);
    fChain->SetBranchAddress("dc_vx", dc_vx, &b_dc_vx);
    fChain->SetBranchAddress("dc_vy", dc_vy, &b_dc_vy);
    fChain->SetBranchAddress("dc_vz", dc_vz, &b_dc_vz);
    fChain->SetBranchAddress("dc_vr", dc_vr, &b_dc_vr);
    fChain->SetBranchAddress("tl1_cx", tl1_cx, &b_tl1_cx);
    fChain->SetBranchAddress("tl1_cy", tl1_cy, &b_tl1_cy);
    fChain->SetBranchAddress("tl1_cz", tl1_cz, &b_tl1_cz);
    fChain->SetBranchAddress("tl1_x", tl1_x, &b_tl1_x);
    fChain->SetBranchAddress("tl1_y", tl1_y, &b_tl1_y);
    fChain->SetBranchAddress("tl1_z", tl1_z, &b_tl1_z);
    fChain->SetBranchAddress("tl1_r", tl1_r, &b_tl1_r);
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
    fChain->SetBranchAddress("lec_ein", lec_ein, &b_lec_ein);
    fChain->SetBranchAddress("lec_t", lec_t, &b_lec_t);
    fChain->SetBranchAddress("lec_r", lec_r, &b_lec_r);
    fChain->SetBranchAddress("lec_x", lec_x, &b_lec_x);
    fChain->SetBranchAddress("lec_y", lec_y, &b_lec_y);
    fChain->SetBranchAddress("lec_z", lec_z, &b_lec_z);
    fChain->SetBranchAddress("lec_c2", lec_c2, &b_lec_c2);
    fChain->SetBranchAddress("nprt", &nprt, &b_nprt);
    fChain->SetBranchAddress("pidpart", pidpart, &b_pidpart);
    fChain->SetBranchAddress("xpart", xpart, &b_xpart);
    fChain->SetBranchAddress("ypart", ypart, &b_ypart);
    fChain->SetBranchAddress("zpart", zpart, &b_zpart);
    fChain->SetBranchAddress("epart", epart, &b_epart);
    fChain->SetBranchAddress("pxpart", pxpart, &b_pxpart);
    fChain->SetBranchAddress("pypart", pypart, &b_pypart);
    fChain->SetBranchAddress("pzpart", pzpart, &b_pzpart);
    fChain->SetBranchAddress("qpart", qpart, &b_qpart);
    fChain->SetBranchAddress("nsc1", &nsc1, &b_nsc1);
    fChain->SetBranchAddress("sc1sesctpd", sc1sesctpd, &b_sc1sesctpd);
    fChain->SetBranchAddress("sctl", sctl, &b_sctl);
    fChain->SetBranchAddress("scel", scel, &b_scel);
    fChain->SetBranchAddress("sctr", sctr, &b_sctr);
    fChain->SetBranchAddress("scer", scer, &b_scer);

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
        //printf("TEST1\n");
        run = ((TString)fn(*fRegExp_run)).Atoi();
        //printf("TEST2\n");
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
