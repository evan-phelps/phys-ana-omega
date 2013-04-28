//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Nov 18 20:14:45 2012 by ROOT version 5.34/02
// from TChain h10/
//////////////////////////////////////////////////////////
#ifndef h10sim_h
#define h10sim_h
#include "TTree.h"
#include "TBranch.h"
#include "data.h"
class h10sim : public Data
{
public :
	static const int MAX_PARTS = 40;
	h10sim();
	virtual ~h10sim();
	virtual bool CheapPop(Long64_t ientry) const;
	virtual bool BigPop(Long64_t ientry) const;
	virtual bool Bind(TTree *fChain, int cachesize = -1);
	// Declaration of leaf types
	UChar_t         npart;
	UChar_t         evstat;
	UInt_t          evntid;
	Char_t          evntype;
	Int_t           evntclas;
	Float_t         q_l;
	Float_t         t_l;
	Float_t         tr_time;
	Float_t         rf_time;
	Int_t           l2bit;
	Int_t           l3bit;
	Int_t           hlsc;
	Int_t           intt;
	Int_t           gpart;
	Int_t           id[MAX_PARTS];   //[gpart]
	Int_t           stat[MAX_PARTS];   //[gpart]
	Int_t           dc[MAX_PARTS];   //[gpart]
	Int_t           cc[MAX_PARTS];   //[gpart]
	Int_t           sc[MAX_PARTS];   //[gpart]
	Int_t           ec[MAX_PARTS];   //[gpart]
	Int_t           lec[MAX_PARTS];   //[gpart]
	Int_t           st[MAX_PARTS];   //[gpart]
	Float_t         p[MAX_PARTS];   //[gpart]
	Float_t         m[MAX_PARTS];   //[gpart]
	Int_t           q[MAX_PARTS];   //[gpart]
	Float_t         b[MAX_PARTS];   //[gpart]
	Float_t         cx[MAX_PARTS];   //[gpart]
	Float_t         cy[MAX_PARTS];   //[gpart]
	Float_t         cz[MAX_PARTS];   //[gpart]
	Float_t         vx[MAX_PARTS];   //[gpart]
	Float_t         vy[MAX_PARTS];   //[gpart]
	Float_t         vz[MAX_PARTS];   //[gpart]
	Int_t           dc_part;
	Int_t           dc_sect[MAX_PARTS];   //[dc_part]
	Int_t           dc_trk[MAX_PARTS];   //[dc_part]
	Int_t           dc_stat[MAX_PARTS];   //[dc_part]
	Int_t           tb_st[MAX_PARTS];   //[dc_part]
	Float_t         dc_xsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_ysc[MAX_PARTS];   //[dc_part]
	Float_t         dc_zsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_cxsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_cysc[MAX_PARTS];   //[dc_part]
	Float_t         dc_czsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_vx[MAX_PARTS];   //[dc_part]
	Float_t         dc_vy[MAX_PARTS];   //[dc_part]
	Float_t         dc_vz[MAX_PARTS];   //[dc_part]
	Float_t         dc_vr[MAX_PARTS];   //[dc_part]
	Float_t         tl1_cx[MAX_PARTS];   //[dc_part]
	Float_t         tl1_cy[MAX_PARTS];   //[dc_part]
	Float_t         tl1_cz[MAX_PARTS];   //[dc_part]
	Float_t         tl1_x[MAX_PARTS];   //[dc_part]
	Float_t         tl1_y[MAX_PARTS];   //[dc_part]
	Float_t         tl1_z[MAX_PARTS];   //[dc_part]
	Float_t         tl1_r[MAX_PARTS];   //[dc_part]
	Float_t         dc_c2[MAX_PARTS];   //[dc_part]
	Int_t           ec_part;
	Int_t           ec_stat[MAX_PARTS];   //[ec_part]
	Int_t           ec_sect[MAX_PARTS];   //[ec_part]
	Int_t           ec_whol[MAX_PARTS];   //[ec_part]
	Int_t           ec_inst[MAX_PARTS];   //[ec_part]
	Int_t           ec_oust[MAX_PARTS];   //[ec_part]
	Float_t         etot[MAX_PARTS];   //[ec_part]
	Float_t         ec_ei[MAX_PARTS];   //[ec_part]
	Float_t         ec_eo[MAX_PARTS];   //[ec_part]
	Float_t         ec_t[MAX_PARTS];   //[ec_part]
	Float_t         ec_r[MAX_PARTS];   //[ec_part]
	Float_t         ech_x[MAX_PARTS];   //[ec_part]
	Float_t         ech_y[MAX_PARTS];   //[ec_part]
	Float_t         ech_z[MAX_PARTS];   //[ec_part]
	Float_t         ec_m2[MAX_PARTS];   //[ec_part]
	Float_t         ec_m3[MAX_PARTS];   //[ec_part]
	Float_t         ec_m4[MAX_PARTS];   //[ec_part]
	Float_t         ec_c2[MAX_PARTS];   //[ec_part]
	Int_t           sc_part;
	Int_t           sc_sect[MAX_PARTS];   //[sc_part]
	Int_t           sc_hit[MAX_PARTS];   //[sc_part]
	Int_t           sc_pd[MAX_PARTS];   //[sc_part]
	Int_t           sc_stat[MAX_PARTS];   //[sc_part]
	Float_t         edep[MAX_PARTS];   //[sc_part]
	Float_t         sc_t[MAX_PARTS];   //[sc_part]
	Float_t         sc_r[MAX_PARTS];   //[sc_part]
	Float_t         sc_c2[MAX_PARTS];   //[sc_part]
	Int_t           cc_part;
	Int_t           cc_sect[MAX_PARTS];   //[cc_part]
	Int_t           cc_hit[MAX_PARTS];   //[cc_part]
	Int_t           cc_segm[MAX_PARTS];   //[cc_part]
	Int_t           nphe[MAX_PARTS];   //[cc_part]
	Float_t         cc_t[MAX_PARTS];   //[cc_part]
	Float_t         cc_r[MAX_PARTS];   //[cc_part]
	Float_t         cc_c2[MAX_PARTS];   //[cc_part]
	Int_t           lac_part;
	Int_t           lec_sect[MAX_PARTS];   //[lac_part]
	Int_t           lec_hit[MAX_PARTS];   //[lac_part]
	Int_t           lec_stat[MAX_PARTS];   //[lac_part]
	Float_t         lec_etot[MAX_PARTS];   //[lac_part]
	Float_t         lec_ein[MAX_PARTS];   //[lac_part]
	Float_t         lec_t[MAX_PARTS];   //[lac_part]
	Float_t         lec_r[MAX_PARTS];   //[lac_part]
	Float_t         lec_x[MAX_PARTS];   //[lac_part]
	Float_t         lec_y[MAX_PARTS];   //[lac_part]
	Float_t         lec_z[MAX_PARTS];   //[lac_part]
	Float_t         lec_c2[MAX_PARTS];   //[lac_part]
	Int_t           vidmvrt;
	Int_t           ntrmvrt;
	Float_t         xmvrt;
	Float_t         ymvrt;
	Float_t         zmvrt;
	Float_t         ch2mvrt;
	Float_t         cxxmvrt;
	Float_t         cxymvrt;
	Float_t         cxzmvrt;
	Float_t         cyymvrt;
	Float_t         cyzmvrt;
	Int_t           stamvrt;
	Int_t           mcnentr;
	UChar_t         mcnpart;
	Int_t           mcst[MAX_PARTS];   //[mcnentr]
	Int_t           mcid[MAX_PARTS];   //[mcnentr]
	Int_t           mcpid[MAX_PARTS];   //[mcnentr]
	Float_t         mctheta[MAX_PARTS];   //[mcnentr]
	Float_t         mcphi[MAX_PARTS];   //[mcnentr]
	Float_t         mcp[MAX_PARTS];   //[mcnentr]
	Float_t         mcm[MAX_PARTS];   //[mcnentr]
	Float_t         mcvx[MAX_PARTS];   //[mcnentr]
	Float_t         mcvy[MAX_PARTS];   //[mcnentr]
	Float_t         mcvz[MAX_PARTS];   //[mcnentr]
	Float_t         mctof[MAX_PARTS];   //[mcnentr]
	Int_t           nprt;
	Int_t           pidpart[MAX_PARTS];   //[nprt]
	Float_t         xpart[MAX_PARTS];   //[nprt]
	Float_t         ypart[MAX_PARTS];   //[nprt]
	Float_t         zpart[MAX_PARTS];   //[nprt]
	Float_t         epart[MAX_PARTS];   //[nprt]
	Float_t         pxpart[MAX_PARTS];   //[nprt]
	Float_t         pypart[MAX_PARTS];   //[nprt]
	Float_t         pzpart[MAX_PARTS];   //[nprt]
	Float_t         qpart[MAX_PARTS];   //[nprt]
	Int_t           Ipart10[MAX_PARTS];   //[nprt]
	Float_t         Rpart11[MAX_PARTS];   //[nprt]
	Float_t         Rpart12[MAX_PARTS];   //[nprt]
	Int_t           Ipart13[MAX_PARTS];   //[nprt]
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
	TBranch        *b_vidmvrt;   //!
	TBranch        *b_ntrmvrt;   //!
	TBranch        *b_xmvrt;   //!
	TBranch        *b_ymvrt;   //!
	TBranch        *b_zmvrt;   //!
	TBranch        *b_ch2mvrt;   //!
	TBranch        *b_cxxmvrt;   //!
	TBranch        *b_cxymvrt;   //!
	TBranch        *b_cxzmvrt;   //!
	TBranch        *b_cyymvrt;   //!
	TBranch        *b_cyzmvrt;   //!
	TBranch        *b_stamvrt;   //!
	TBranch        *b_mcnentr;   //!
	TBranch        *b_mcnpart;   //!
	TBranch        *b_mcst;   //!
	TBranch        *b_mcid;   //!
	TBranch        *b_mcpid;   //!
	TBranch        *b_mctheta;   //!
	TBranch        *b_mcphi;   //!
	TBranch        *b_mcp;   //!
	TBranch        *b_mcm;   //!
	TBranch        *b_mcvx;   //!
	TBranch        *b_mcvy;   //!
	TBranch        *b_mcvz;   //!
	TBranch        *b_mctof;   //!
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
	TBranch        *b_Ipart10;   //!
	TBranch        *b_Rpart11;   //!
	TBranch        *b_Rpart12;   //!
	TBranch        *b_Ipart13;   //!

};
#endif
