#ifndef h10exp_h
#define h10exp_h
#include "TTree.h"
#include "TBranch.h"
#include "data.h"
class h10exp : public Data
{
public :
	static const int MAX_PARTS = 22;
	h10exp();
	virtual ~h10exp();
	virtual bool CheapPop(Long64_t ientry) const;
	virtual bool BigPop(Long64_t ientry) const;
	virtual bool Bind(TTree *fChain, int cachesize = -1);
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
	Short_t         id[MAX_PARTS];   //[gpart]
	Char_t          stat[MAX_PARTS];   //[gpart]
	UChar_t         dc[MAX_PARTS];   //[gpart]
	UChar_t         cc[MAX_PARTS];   //[gpart]
	UChar_t         sc[MAX_PARTS];   //[gpart]
	UChar_t         ec[MAX_PARTS];   //[gpart]
	UChar_t         lec[MAX_PARTS];   //[gpart]
	Float_t         p[MAX_PARTS];   //[gpart]
	Float_t         m[MAX_PARTS];   //[gpart]
	Char_t          q[MAX_PARTS];   //[gpart]
	Float_t         b[MAX_PARTS];   //[gpart]
	Float_t         cx[MAX_PARTS];   //[gpart]
	Float_t         cy[MAX_PARTS];   //[gpart]
	Float_t         cz[MAX_PARTS];   //[gpart]
	Float_t         vx[MAX_PARTS];   //[gpart]
	Float_t         vy[MAX_PARTS];   //[gpart]
	Float_t         vz[MAX_PARTS];   //[gpart]
	Int_t           dc_part;
	UChar_t         dc_sect[MAX_PARTS];   //[dc_part]
	UChar_t         dc_trk[MAX_PARTS];   //[dc_part]
	Char_t          dc_stat[MAX_PARTS];   //[dc_part]
	Float_t         dc_xsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_ysc[MAX_PARTS];   //[dc_part]
	Float_t         dc_zsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_cxsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_cysc[MAX_PARTS];   //[dc_part]
	Float_t         dc_czsc[MAX_PARTS];   //[dc_part]
	Float_t         dc_xec[MAX_PARTS];   //[dc_part]
	Float_t         dc_yec[MAX_PARTS];   //[dc_part]
	Float_t         dc_zec[MAX_PARTS];   //[dc_part]
	Float_t         dc_thcc[MAX_PARTS];   //[dc_part]
	Float_t         dc_c2[MAX_PARTS];   //[dc_part]
	Int_t           ec_part;
	UShort_t        ec_stat[MAX_PARTS];   //[ec_part]
	UChar_t         ec_sect[MAX_PARTS];   //[ec_part]
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
	UChar_t         sc_sect[MAX_PARTS];   //[sc_part]
	UChar_t         sc_hit[MAX_PARTS];   //[sc_part]
	UChar_t         sc_pd[MAX_PARTS];   //[sc_part]
	UChar_t         sc_stat[MAX_PARTS];   //[sc_part]
	Float_t         edep[MAX_PARTS];   //[sc_part]
	Float_t         sc_t[MAX_PARTS];   //[sc_part]
	Float_t         sc_r[MAX_PARTS];   //[sc_part]
	Float_t         sc_c2[MAX_PARTS];   //[sc_part]
	Int_t           cc_part;
	UChar_t         cc_sect[MAX_PARTS];   //[cc_part]
	UChar_t         cc_hit[MAX_PARTS];   //[cc_part]
	Int_t           cc_segm[MAX_PARTS];   //[cc_part]
	UShort_t        nphe[MAX_PARTS];   //[cc_part]
	Float_t         cc_t[MAX_PARTS];   //[cc_part]
	Float_t         cc_r[MAX_PARTS];   //[cc_part]
	Float_t         cc_c2[MAX_PARTS];   //[cc_part]
	Int_t           lac_part;
	Int_t           lec_sect[MAX_PARTS];   //[lac_part]
	Int_t           lec_hit[MAX_PARTS];   //[lac_part]
	Int_t           lec_stat[MAX_PARTS];   //[lac_part]
	Float_t         lec_etot[MAX_PARTS];   //[lac_part]
	Float_t         lec_t[MAX_PARTS];   //[lac_part]
	Float_t         lec_r[MAX_PARTS];   //[lac_part]
	Float_t         lec_x[MAX_PARTS];   //[lac_part]
	Float_t         lec_y[MAX_PARTS];   //[lac_part]
	Float_t         lec_z[MAX_PARTS];   //[lac_part]
	Float_t         lec_c2[MAX_PARTS];   //[lac_part]
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
	
	/* dirty trick */
	Int_t           mcnentr;
	UChar_t         mcnpart;
	Int_t           mcst[0];   //[mcnentr]
	Int_t           mcid[0];   //[mcnentr]
	Int_t           mcpid[0];   //[mcnentr]
	Float_t         mctheta[0];   //[mcnentr]
	Float_t         mcphi[0];   //[mcnentr]
	Float_t         mcp[0];   //[mcnentr]
	Float_t         mcm[0];   //[mcnentr]
	Float_t         mcvx[0];   //[mcnentr]
	Float_t         mcvy[0];   //[mcnentr]
	Float_t         mcvz[0];   //[mcnentr]
	Float_t         mctof[0];   //[mcnentr]

};
#endif
