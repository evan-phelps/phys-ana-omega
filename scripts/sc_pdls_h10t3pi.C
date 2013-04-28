{
  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->ProcessLine(".L ~/analysis/omega/scripts/rootutils.C");
  gROOT->ProcessLine(".L ~/analysis/omega/fid.cpp++");
  const int NTOPROC = -1; //500000;
  const char *infile = "omega.root";
  const float R2D = TMath::RadToDeg();
  const float D2R = TMath::DegToRad();
  const float PI = TMath::Pi();
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile);
  if (!f) {
    f = new TFile(infile); //_data/exp.root");
  }
  TDirectory *dirh10 = (TDirectory*)f->Get("h10clone");
  TDirectory *dir = (TDirectory*)f->Get("3pi-tree");
  TTree *t3pi, *h10;
  dirh10->GetObject("h10",h10);
  dir->GetObject("t3pi",t3pi);
  h10->AddFriend(t3pi);
  //Declaration of leaves types
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
  Short_t         id[22];
  Char_t          stat[22];
  UChar_t         dc[22];
  UChar_t         cc[22];
  UChar_t         sc[22];
  UChar_t         ec[22];
  UChar_t         lec[22];
  Float_t         p[22];
  Float_t         m[22];
  Char_t          q[22];
  Float_t         b[22];
  Float_t         cx[22];
  Float_t         cy[22];
  Float_t         cz[22];
  Float_t         vx[22];
  Float_t         vy[22];
  Float_t         vz[22];
  Int_t           dc_part;
  UChar_t         dc_sect[20];
  UChar_t         dc_trk[20];
  Char_t          dc_stat[20];
  Float_t         dc_xsc[20];
  Float_t         dc_ysc[20];
  Float_t         dc_zsc[20];
  Float_t         dc_cxsc[20];
  Float_t         dc_cysc[20];
  Float_t         dc_czsc[20];
  Float_t         dc_xec[20];
  Float_t         dc_yec[20];
  Float_t         dc_zec[20];
  Float_t         dc_thcc[20];
  Float_t         dc_c2[20];
  Int_t           ec_part;
  UShort_t        ec_stat[21];
  UChar_t         ec_sect[21];
  Int_t           ec_whol[21];
  Int_t           ec_inst[21];
  Int_t           ec_oust[21];
  Float_t         etot[21];
  Float_t         ec_ei[21];
  Float_t         ec_eo[21];
  Float_t         ec_t[21];
  Float_t         ec_r[21];
  Float_t         ech_x[21];
  Float_t         ech_y[21];
  Float_t         ech_z[21];
  Float_t         ec_m2[21];
  Float_t         ec_m3[21];
  Float_t         ec_m4[21];
  Float_t         ec_c2[21];
  Int_t           sc_part;
  UChar_t         sc_sect[21];
  UChar_t         sc_hit[21];
  UChar_t         sc_pd[21];
  UChar_t         sc_stat[21];
  Float_t         edep[21];
  Float_t         sc_t[21];
  Float_t         sc_r[21];
  Float_t         sc_c2[21];
  Int_t           cc_part;
  UChar_t         cc_sect[20];
  UChar_t         cc_hit[20];
  Int_t           cc_segm[20];
  UShort_t        nphe[20];
  Float_t         cc_t[20];
  Float_t         cc_r[20];
  Float_t         cc_c2[20];
  Int_t           lac_part;
  Int_t           lec_sect[20];
  Int_t           lec_hit[20];
  Int_t           lec_stat[20];
  Float_t         lec_etot[20];
  Float_t         lec_t[20];
  Float_t         lec_r[20];
  Float_t         lec_x[20];
  Float_t         lec_y[20];
  Float_t         lec_z[20];
  Float_t         lec_c2[20];

  // Set branch addresses.
  h10->SetBranchAddress("npart",&npart);
  h10->SetBranchAddress("evstat",&evstat);
  h10->SetBranchAddress("evntid",&evntid);
  h10->SetBranchAddress("evtype",&evtype);
  h10->SetBranchAddress("evntclas",&evntclas);
  h10->SetBranchAddress("evthel",&evthel);
  h10->SetBranchAddress("evntclas2",&evntclas2);
  h10->SetBranchAddress("q_l",&q_l);
  h10->SetBranchAddress("t_l",&t_l);
  h10->SetBranchAddress("tr_time",&tr_time);
  h10->SetBranchAddress("rf_time1",&rf_time1);
  h10->SetBranchAddress("rf_time2",&rf_time2);
  h10->SetBranchAddress("gpart",&gpart);
  h10->SetBranchAddress("id",id);
  h10->SetBranchAddress("stat",stat);
  h10->SetBranchAddress("dc",dc);
  h10->SetBranchAddress("cc",cc);
  h10->SetBranchAddress("sc",sc);
  h10->SetBranchAddress("ec",ec);
  h10->SetBranchAddress("lec",lec);
  h10->SetBranchAddress("p",p);
  h10->SetBranchAddress("m",m);
  h10->SetBranchAddress("q",q);
  h10->SetBranchAddress("b",b);
  h10->SetBranchAddress("cx",cx);
  h10->SetBranchAddress("cy",cy);
  h10->SetBranchAddress("cz",cz);
  h10->SetBranchAddress("vx",vx);
  h10->SetBranchAddress("vy",vy);
  h10->SetBranchAddress("vz",vz);
  h10->SetBranchAddress("dc_part",&dc_part);
  h10->SetBranchAddress("dc_sect",dc_sect);
  h10->SetBranchAddress("dc_trk",dc_trk);
  h10->SetBranchAddress("dc_stat",dc_stat);
  h10->SetBranchAddress("dc_xsc",dc_xsc);
  h10->SetBranchAddress("dc_ysc",dc_ysc);
  h10->SetBranchAddress("dc_zsc",dc_zsc);
  h10->SetBranchAddress("dc_cxsc",dc_cxsc);
  h10->SetBranchAddress("dc_cysc",dc_cysc);
  h10->SetBranchAddress("dc_czsc",dc_czsc);
  h10->SetBranchAddress("dc_xec",dc_xec);
  h10->SetBranchAddress("dc_yec",dc_yec);
  h10->SetBranchAddress("dc_zec",dc_zec);
  h10->SetBranchAddress("dc_thcc",dc_thcc);
  h10->SetBranchAddress("dc_c2",dc_c2);
  h10->SetBranchAddress("ec_part",&ec_part);
  h10->SetBranchAddress("ec_stat",ec_stat);
  h10->SetBranchAddress("ec_sect",ec_sect);
  h10->SetBranchAddress("ec_whol",ec_whol);
  h10->SetBranchAddress("ec_inst",ec_inst);
  h10->SetBranchAddress("ec_oust",ec_oust);
  h10->SetBranchAddress("etot",etot);
  h10->SetBranchAddress("ec_ei",ec_ei);
  h10->SetBranchAddress("ec_eo",ec_eo);
  h10->SetBranchAddress("ec_t",ec_t);
  h10->SetBranchAddress("ec_r",ec_r);
  h10->SetBranchAddress("ech_x",ech_x);
  h10->SetBranchAddress("ech_y",ech_y);
  h10->SetBranchAddress("ech_z",ech_z);
  h10->SetBranchAddress("ec_m2",ec_m2);
  h10->SetBranchAddress("ec_m3",ec_m3);
  h10->SetBranchAddress("ec_m4",ec_m4);
  h10->SetBranchAddress("ec_c2",ec_c2);
  h10->SetBranchAddress("sc_part",&sc_part);
  h10->SetBranchAddress("sc_sect",sc_sect);
  h10->SetBranchAddress("sc_hit",sc_hit);
  h10->SetBranchAddress("sc_pd",sc_pd);
  h10->SetBranchAddress("sc_stat",sc_stat);
  h10->SetBranchAddress("edep",edep);
  h10->SetBranchAddress("sc_t",sc_t);
  h10->SetBranchAddress("sc_r",sc_r);
  h10->SetBranchAddress("sc_c2",sc_c2);
  h10->SetBranchAddress("cc_part",&cc_part);
  h10->SetBranchAddress("cc_sect",cc_sect);
  h10->SetBranchAddress("cc_hit",cc_hit);
  h10->SetBranchAddress("cc_segm",cc_segm);
  h10->SetBranchAddress("nphe",nphe);
  h10->SetBranchAddress("cc_t",cc_t);
  h10->SetBranchAddress("cc_r",cc_r);
  h10->SetBranchAddress("cc_c2",cc_c2);
  h10->SetBranchAddress("lac_part",&lac_part);
  h10->SetBranchAddress("lec_sect",lec_sect);
  h10->SetBranchAddress("lec_hit",lec_hit);
  h10->SetBranchAddress("lec_stat",lec_stat);
  h10->SetBranchAddress("lec_etot",lec_etot);
  h10->SetBranchAddress("lec_t",lec_t);
  h10->SetBranchAddress("lec_r",lec_r);
  h10->SetBranchAddress("lec_x",lec_x);
  h10->SetBranchAddress("lec_y",lec_y);
  h10->SetBranchAddress("lec_z",lec_z);
  h10->SetBranchAddress("lec_c2",lec_c2);

  //Declaration of leaves types
  TLorentzVector  *w4 = 0;
  TLorentzVector  *q4 = 0;
  TLorentzVector  *e0 = 0;
  TLorentzVector  *e1 = 0;
  TLorentzVector  *p0 = 0;
  TLorentzVector  *p1 = 0;
  TLorentzVector  *pip = 0;
  TLorentzVector  *pim = 0;
  TLorentzVector  *pi0 = 0;
  TLorentzVector  *omega = 0;
  TLorentzVector  *ppip = 0;
  TLorentzVector  *ppim = 0;
  TLorentzVector  *pippim = 0;
  TLorentzVector  *pippi0 = 0;
  TLorentzVector  *pimpi0 = 0;
  TLorentzVector  *mmppip = 0;
  TLorentzVector  *mmppim = 0;
  Int_t           h10idx_e;
  Int_t           h10idx_p;
  Int_t           h10idx_pip;
  Int_t           h10idx_pim;

  // Set branch addresses.
  t3pi->SetBranchAddress("w4",&w4);
  t3pi->SetBranchAddress("q4",&q4);
  t3pi->SetBranchAddress("e0",&e0);
  t3pi->SetBranchAddress("e1",&e1);
  t3pi->SetBranchAddress("p0",&p0);
  t3pi->SetBranchAddress("p1",&p1);
  t3pi->SetBranchAddress("pip",&pip);
  t3pi->SetBranchAddress("pim",&pim);
  t3pi->SetBranchAddress("pi0",&pi0);
  t3pi->SetBranchAddress("omega",&omega);
  t3pi->SetBranchAddress("ppip",&ppip);
  t3pi->SetBranchAddress("ppim",&ppim);
  t3pi->SetBranchAddress("pippim",&pippim);
  t3pi->SetBranchAddress("pippi0",&pippi0);
  t3pi->SetBranchAddress("pimpi0",&pimpi0);
  t3pi->SetBranchAddress("mmppip",&mmppip);
  t3pi->SetBranchAddress("mmppim",&mmppim);
  t3pi->SetBranchAddress("h10idx_e",&h10idx_e);
  t3pi->SetBranchAddress("h10idx_p",&h10idx_p);
  t3pi->SetBranchAddress("h10idx_pip",&h10idx_pip);
  t3pi->SetBranchAddress("h10idx_pim",&h10idx_pim);

  Long64_t nentries = NTOPROC;
  printf("NTOPROC = %lld\n",nentries);
  if (nentries<=0) nentries = h10->GetEntries();
  printf("nentries = %lld\n",nentries);
  Long64_t eventnum = 0;

  const int NPDLS = 48;
  const int NSECTS = 6;
  TH2D *h2e[NSECTS], *h2p[NSECTS], *h2pip[NSECTS], *h2pim[NSECTS];
  Fid *fid = Fid::Instance();
  TFile fout("sc_pdls_h10t3pi.root","recreate");
  for (int sidx = 0; sidx < NSECTS; sidx++) {
    TString hn = TString::Format("h_pdlVp_%s_s%d","e",sidx+1);
    TString ht = TString::Format("paddle:momentum, %s, S%d","e^{-}",sidx+1);
    h2e[sidx] = new TH2D(hn.Data(),ht.Data(),60,0,6,23,0.5,23+0.5);
    hn = TString::Format("h_pdlVp_%s_s%d","p",sidx+1);
    ht = TString::Format("paddle:momentum, %s, S%d","p",sidx+1);
    h2p[sidx] = new TH2D(hn.Data(),ht.Data(),60,0,6,NPDLS,0.5,NPDLS+0.5);
    hn = TString::Format("h_pdlVp_%s_s%d","pip",sidx+1);
    ht = TString::Format("paddle:momentum, %s, S%d","#pi^{+}",sidx+1);
    h2pip[sidx] = new TH2D(hn.Data(),ht.Data(),60,0,6,NPDLS,0.5,NPDLS+0.5);
    hn = TString::Format("h_pdlVp_%s_s%d","pim",sidx+1);
    ht = TString::Format("paddle:momentum, %s, S%d","#pi^{-}",sidx+1);
    h2pim[sidx] = new TH2D(hn.Data(),ht.Data(),60,0,6,NPDLS,0.5,NPDLS+0.5);
  }
  
  printf("created histograms\n");

  TStopwatch swmain;
  TStopwatch swgroup;
  const Int_t BLOCKSIZE = 100000;

  Long64_t nbytes = 0;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += h10->GetEntry(i);

    if ( (++eventnum) % BLOCKSIZE == 0 ) {
      Float_t gtime = swgroup.RealTime();
      Float_t ttime = swmain.RealTime();
      Double_t percentProcessed = (((Double_t)eventnum)/(Double_t)nentries)*100;
      Float_t remaining = (100.0/percentProcessed*ttime-ttime)/60.0;
      printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(BLOCKSIZE/gtime),remaining);
      swgroup.Start();
      swmain.Start(kFALSE);
    }

    if (h10idx_e==0 && h10idx_p>0 && h10idx_pip>0 && h10idx_pim>0 && w4->M()>1.6 && mmppip->M()>0.280 && mmppim->M()>0.280) {
      int ccidx = cc[0]-1;
      int scidx = sc[0]-1;
      int sector_cc = cc_sect[ccidx]-1;
      int sector_sc = sc_sect[scidx]-1;
      int scidxp = sc[h10idx_p]-1;
      int scidxpip = sc[h10idx_pip]-1;
      int scidxpim = sc[h10idx_pim]-1;
      int sect_sc_p = scidxp>=0 ? sc_sect[scidxp]-1 : -1;
      int sect_sc_pip = scidxpip>=0 ? sc_sect[scidxpip]-1 : -1;
      int sect_sc_pim = scidxpim>=0 ? sc_sect[scidxpim]-1 : -1;
      if (sector_cc == sector_sc) {// && fid->InFid(e1->P(),e1->Theta(),e1->Phi(),sector_sc+1,11)) {
	h2e[sector_sc]->Fill(e1->P(),sc_pd[scidx]);
	if (scidxp >= 0) {// && fid->InFid(p1->P(),p1->Theta(),p1->Phi(),sect_sc_p+1,2212)) {
	  h2p[sect_sc_p]->Fill(p1->P(),sc_pd[scidxp]);
	}
	if (scidxpip >= 0) { // && fid->InFid(pip->P(),pip->Theta(),pip->Phi(),sect_sc_pip+1,211)) {
	  h2pip[sect_sc_pip]->Fill(pip->P(),sc_pd[scidxpip]);
	}
	if (scidxpim >= 0) {// && fid->InFid(pim->P(),pim->Theta(),pim->Phi(),sect_sc_pim+1,-211)) {
	  h2pim[sect_sc_pim]->Fill(pim->P(),sc_pd[scidxpim]);
	}
      }
    }
  }
}
