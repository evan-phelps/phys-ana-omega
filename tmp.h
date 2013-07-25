//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jul  6 00:41:34 2013 by ROOT version 5.34/08
// from TChain h10clone/h10/
//////////////////////////////////////////////////////////

#ifndef tmp_h
#define tmp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class tmp {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UChar_t         npart;
   UChar_t         evstat;
   UInt_t          evntid;
   Char_t          evtype;
   Char_t          evntclas;
   Char_t          evthel;
   Int_t           evntclas2;
   Float_t         q_l;
   Float_t         t_l;
   Float_t         tr_time;
   Float_t         rf_time1;
   Float_t         rf_time2;
   Int_t           gpart;
   Short_t         id[22];   //[gpart]
   Char_t          stat[22];   //[gpart]
   UChar_t         dc[22];   //[gpart]
   UChar_t         cc[22];   //[gpart]
   UChar_t         sc[22];   //[gpart]
   UChar_t         ec[22];   //[gpart]
   UChar_t         lec[22];   //[gpart]
   Float_t         p[22];   //[gpart]
   Float_t         m[22];   //[gpart]
   Char_t          q[22];   //[gpart]
   Float_t         b[22];   //[gpart]
   Float_t         cx[22];   //[gpart]
   Float_t         cy[22];   //[gpart]
   Float_t         cz[22];   //[gpart]
   Float_t         vx[22];   //[gpart]
   Float_t         vy[22];   //[gpart]
   Float_t         vz[22];   //[gpart]
   Int_t           dc_part;
   UChar_t         dc_sect[20];   //[dc_part]
   UChar_t         dc_trk[20];   //[dc_part]
   Char_t          dc_stat[20];   //[dc_part]
   Float_t         dc_xsc[20];   //[dc_part]
   Float_t         dc_ysc[20];   //[dc_part]
   Float_t         dc_zsc[20];   //[dc_part]
   Float_t         dc_cxsc[20];   //[dc_part]
   Float_t         dc_cysc[20];   //[dc_part]
   Float_t         dc_czsc[20];   //[dc_part]
   Float_t         dc_xec[20];   //[dc_part]
   Float_t         dc_yec[20];   //[dc_part]
   Float_t         dc_zec[20];   //[dc_part]
   Float_t         dc_thcc[20];   //[dc_part]
   Float_t         dc_c2[20];   //[dc_part]
   Int_t           ec_part;
   UShort_t        ec_stat[21];   //[ec_part]
   UChar_t         ec_sect[21];   //[ec_part]
   Int_t           ec_whol[21];   //[ec_part]
   Int_t           ec_inst[21];   //[ec_part]
   Int_t           ec_oust[21];   //[ec_part]
   Float_t         etot[21];   //[ec_part]
   Float_t         ec_ei[21];   //[ec_part]
   Float_t         ec_eo[21];   //[ec_part]
   Float_t         ec_t[21];   //[ec_part]
   Float_t         ec_r[21];   //[ec_part]
   Float_t         ech_x[21];   //[ec_part]
   Float_t         ech_y[21];   //[ec_part]
   Float_t         ech_z[21];   //[ec_part]
   Float_t         ec_m2[21];   //[ec_part]
   Float_t         ec_m3[21];   //[ec_part]
   Float_t         ec_m4[21];   //[ec_part]
   Float_t         ec_c2[21];   //[ec_part]
   Int_t           sc_part;
   UChar_t         sc_sect[21];   //[sc_part]
   UChar_t         sc_hit[21];   //[sc_part]
   UChar_t         sc_pd[21];   //[sc_part]
   UChar_t         sc_stat[21];   //[sc_part]
   Float_t         edep[21];   //[sc_part]
   Float_t         sc_t[21];   //[sc_part]
   Float_t         sc_r[21];   //[sc_part]
   Float_t         sc_c2[21];   //[sc_part]
   Int_t           cc_part;
   UChar_t         cc_sect[20];   //[cc_part]
   UChar_t         cc_hit[20];   //[cc_part]
   Int_t           cc_segm[20];   //[cc_part]
   UShort_t        nphe[20];   //[cc_part]
   Float_t         cc_t[20];   //[cc_part]
   Float_t         cc_r[20];   //[cc_part]
   Float_t         cc_c2[20];   //[cc_part]
   Int_t           lac_part;
   Int_t           lec_sect[20];   //[lac_part]
   Int_t           lec_hit[20];   //[lac_part]
   Int_t           lec_stat[20];   //[lac_part]
   Float_t         lec_etot[20];   //[lac_part]
   Float_t         lec_t[20];   //[lac_part]
   Float_t         lec_r[20];   //[lac_part]
   Float_t         lec_x[20];   //[lac_part]
   Float_t         lec_y[20];   //[lac_part]
   Float_t         lec_z[20];   //[lac_part]
   Float_t         lec_c2[20];   //[lac_part]

   // List of branches
   TBranch        *b_npart;   //!
   TBranch        *b_evstat;   //!
   TBranch        *b_evntid;   //!
   TBranch        *b_evtype;   //!
   TBranch        *b_evntclas;   //!
   TBranch        *b_evthel;   //!
   TBranch        *b_evntclas2;   //!
   TBranch        *b_q_l;   //!
   TBranch        *b_t_l;   //!
   TBranch        *b_tr_time;   //!
   TBranch        *b_rf_time1;   //!
   TBranch        *b_rf_time2;   //!
   TBranch        *b_gpart;   //!
   TBranch        *b_id;   //!
   TBranch        *b_stat;   //!
   TBranch        *b_dc;   //!
   TBranch        *b_cc;   //!
   TBranch        *b_sc;   //!
   TBranch        *b_ec;   //!
   TBranch        *b_lec;   //!
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
   TBranch        *b_dc_xsc;   //!
   TBranch        *b_dc_ysc;   //!
   TBranch        *b_dc_zsc;   //!
   TBranch        *b_dc_cxsc;   //!
   TBranch        *b_dc_cysc;   //!
   TBranch        *b_dc_czsc;   //!
   TBranch        *b_dc_xec;   //!
   TBranch        *b_dc_yec;   //!
   TBranch        *b_dc_zec;   //!
   TBranch        *b_dc_thcc;   //!
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
   TBranch        *b_lec_t;   //!
   TBranch        *b_lec_r;   //!
   TBranch        *b_lec_x;   //!
   TBranch        *b_lec_y;   //!
   TBranch        *b_lec_z;   //!
   TBranch        *b_lec_c2;   //!

   tmp(TTree *tree=0);
   virtual ~tmp();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tmp_cxx
tmp::tmp(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("h10clone/h10",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("h10clone/h10","");
      chain->Add("/data/e1f/skim/37658.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37659.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37661.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37662.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37664.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37665.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37666.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37667.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37670.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37672.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37673.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37674.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37675.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37677.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37678.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37679.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37680.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37681.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37683.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37684.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37685.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37686.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37687.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37688.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37689.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37690.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37691.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37692.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37693.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37694.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37698.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37699.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37700.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37701.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37702.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37703.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37704.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37705.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37706.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37707.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37708.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37709.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37710.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37711.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37712.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37713.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37714.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37715.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37716.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37717.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37719.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37721.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37722.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37723.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37724.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37725.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37740.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37744.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37745.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37746.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37747.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37748.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37750.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37753.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37762.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37763.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37766.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37767.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37769.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37770.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37772.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37773.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37775.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37776.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37778.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37780.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37781.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37782.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37783.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37784.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37785.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37788.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37789.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37790.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37801.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37802.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37803.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37804.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37805.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37806.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37807.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37808.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37809.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37810.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37811.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37812.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37813.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37814.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37815.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37816.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37817.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37818.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37819.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37820.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37822.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37823.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37824.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37825.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37828.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37831.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37832.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37833.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37844.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37845.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37846.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37847.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37848.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37849.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37850.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37851.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37852.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37854.root/h10clone/h10");
      chain->Add("/data/e1f/skim/37856.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38046.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38047.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38048.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38049.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38050.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38051.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38052.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38053.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38070.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38071.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38072.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38074.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38075.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38076.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38077.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38078.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38079.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38080.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38081.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38082.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38083.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38084.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38085.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38086.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38089.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38090.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38091.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38092.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38093.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38094.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38095.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38096.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38097.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38098.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38099.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38100.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38111.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38112.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38114.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38117.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38118.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38119.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38120.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38121.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38122.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38131.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38132.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38133.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38134.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38135.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38136.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38137.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38138.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38139.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38140.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38141.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38142.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38143.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38144.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38146.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38172.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38173.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38174.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38175.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38176.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38177.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38182.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38183.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38184.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38185.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38186.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38187.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38188.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38189.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38190.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38191.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38192.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38194.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38195.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38196.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38197.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38198.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38199.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38200.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38201.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38203.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38204.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38205.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38206.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38207.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38208.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38209.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38210.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38211.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38212.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38213.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38214.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38215.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38216.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38217.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38218.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38219.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38220.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38221.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38222.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38223.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38225.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38226.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38265.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38266.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38268.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38271.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38272.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38273.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38274.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38275.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38276.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38277.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38278.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38283.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38284.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38285.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38286.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38288.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38289.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38290.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38300.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38301.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38302.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38303.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38304.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38305.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38306.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38307.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38309.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38310.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38312.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38313.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38314.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38316.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38317.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38318.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38320.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38321.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38322.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38328.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38329.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38331.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38337.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38338.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38339.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38340.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38341.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38342.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38344.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38346.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38347.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38350.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38351.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38352.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38353.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38354.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38355.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38356.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38359.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38360.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38364.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38365.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38378.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38379.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38380.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38381.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38382.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38383.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38384.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38385.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38387.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38388.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38389.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38390.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38391.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38392.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38393.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38394.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38395.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38396.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38397.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38398.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38399.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38400.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38401.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38402.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38403.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38404.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38405.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38408.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38409.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38410.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38411.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38412.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38415.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38417.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38418.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38419.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38420.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38421.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38422.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38423.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38424.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38425.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38426.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38427.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38429.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38430.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38431.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38432.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38433.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38434.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38435.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38436.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38437.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38438.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38439.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38440.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38441.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38443.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38446.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38447.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38449.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38450.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38451.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38452.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38453.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38454.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38455.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38456.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38457.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38458.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38459.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38460.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38461.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38462.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38463.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38464.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38465.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38466.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38467.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38468.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38469.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38470.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38471.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38472.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38473.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38474.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38475.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38476.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38477.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38478.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38479.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38480.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38483.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38484.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38485.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38486.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38487.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38488.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38489.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38490.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38491.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38492.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38493.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38494.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38495.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38497.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38498.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38499.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38500.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38501.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38507.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38508.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38509.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38510.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38511.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38512.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38513.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38514.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38515.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38516.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38517.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38518.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38519.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38520.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38521.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38522.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38523.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38524.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38525.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38526.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38527.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38528.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38529.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38530.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38531.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38534.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38536.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38537.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38538.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38539.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38540.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38541.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38542.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38543.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38544.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38545.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38548.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38549.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38550.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38551.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38552.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38553.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38554.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38558.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38559.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38560.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38561.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38562.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38563.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38568.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38571.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38572.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38573.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38574.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38575.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38576.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38577.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38578.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38579.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38580.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38581.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38582.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38583.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38584.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38585.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38587.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38588.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38590.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38596.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38597.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38598.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38600.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38601.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38602.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38603.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38604.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38606.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38607.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38608.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38609.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38610.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38611.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38612.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38613.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38614.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38616.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38617.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38618.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38619.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38620.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38621.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38622.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38623.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38624.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38625.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38626.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38627.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38628.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38629.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38630.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38631.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38632.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38633.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38634.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38635.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38636.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38637.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38638.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38639.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38640.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38641.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38642.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38645.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38646.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38647.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38648.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38649.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38650.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38651.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38652.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38653.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38654.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38655.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38656.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38658.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38659.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38660.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38661.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38662.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38663.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38664.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38665.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38666.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38667.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38668.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38670.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38671.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38672.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38673.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38674.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38675.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38676.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38677.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38681.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38682.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38684.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38685.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38686.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38687.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38689.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38690.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38691.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38692.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38693.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38694.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38696.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38697.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38698.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38699.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38700.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38701.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38702.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38703.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38704.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38705.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38706.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38707.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38708.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38709.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38710.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38711.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38712.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38713.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38714.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38715.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38716.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38717.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38719.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38720.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38721.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38722.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38723.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38724.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38725.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38727.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38728.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38729.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38730.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38731.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38732.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38733.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38734.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38735.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38737.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38738.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38739.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38740.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38743.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38744.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38745.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38746.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38748.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38749.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38750.root/h10clone/h10");
      chain->Add("/data/e1f/skim/38751.root/h10clone/h10");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

tmp::~tmp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tmp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tmp::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tmp::Init(TTree *tree)
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
   Notify();
}

Bool_t tmp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tmp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tmp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tmp_cxx
