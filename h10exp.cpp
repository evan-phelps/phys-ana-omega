#include "h10exp.h"

h10exp::h10exp() : Data()
{
	is_sim = false;
}
h10exp::~h10exp()
{
}
bool h10exp::CheapPop(Long64_t ientry) const
{
	//nb = fChain->GetEntry(jentry);   nbytes += nb;
	b_evntid->GetEntry(ientry);
	b_gpart->GetEntry(ientry);
	b_id->GetEntry(ientry);
	b_p->GetEntry(ientry);
	b_cx->GetEntry(ientry);
	b_cy->GetEntry(ientry);
	b_cz->GetEntry(ientry);
	b_q->GetEntry(ientry);
	b_sc->GetEntry(ientry);
	b_cc->GetEntry(ientry);
	b_ec->GetEntry(ientry);
	b_dc->GetEntry(ientry);
	b_stat->GetEntry(ientry);
	b_q_l->GetEntry(ientry);
	b_dc_stat->GetEntry(ientry);
	b_sc_sect->GetEntry(ientry);
	b_etot->GetEntry(ientry);
	b_ec_ei->GetEntry(ientry);
	return true;
}
bool h10exp::BigPop(Long64_t ientry) const
{
	return true;
}
bool h10exp::Bind(TTree *fChain, int cachesize /* = -1 */)
{	
	//Int_t cachesize = 10000000; //10 MBytes
	if (cachesize>0) cachesize *= 1000000; //Int_t cachesize = 500000000;
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

	fChain->SetBranchStatus("*",1);
	
	if (cachesize > 0) {
		fChain->SetCacheSize(cachesize);     //<<<
		fChain->StopCacheLearningPhase();    //<<<
		fChain->AddBranchToCache(b_evntid,kTRUE);
		fChain->AddBranchToCache(b_gpart,kTRUE);
		fChain->AddBranchToCache(b_id,kTRUE);
		fChain->AddBranchToCache(b_p,kTRUE);
		fChain->AddBranchToCache(b_cx,kTRUE);
		fChain->AddBranchToCache(b_cy,kTRUE);
		fChain->AddBranchToCache(b_cz,kTRUE);
		fChain->AddBranchToCache(b_q,kTRUE);
		fChain->AddBranchToCache(b_sc,kTRUE);
		fChain->AddBranchToCache(b_cc,kTRUE);
		fChain->AddBranchToCache(b_ec,kTRUE);
		fChain->AddBranchToCache(b_dc,kTRUE);
		fChain->AddBranchToCache(b_stat,kTRUE);
		fChain->AddBranchToCache(b_q_l,kTRUE);
		fChain->AddBranchToCache(b_dc_stat,kTRUE);
		fChain->AddBranchToCache(b_sc_sect,kTRUE);
		fChain->AddBranchToCache(b_etot,kTRUE);
		fChain->AddBranchToCache(b_ec_ei,kTRUE);
	}
	return true;
}
