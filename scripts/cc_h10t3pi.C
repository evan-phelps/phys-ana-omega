{
  //Reset ROOT and connect tree file
  gROOT->Reset();
  gROOT->ProcessLine(".L ~/analysis/omega/scripts/cc_utils.C");
  gROOT->ProcessLine(".L ~/analysis/omega/scripts/rootutils.C");
  gROOT->ProcessLine(".L ~/analysis/omega/fid.cpp++");
  const int NTOPROC = 1000000;
  const char *infile = "omega.37x_380-2.root";
  const float R2D = TMath::RadToDeg();
  const float D2R = TMath::DegToRad();
  const float PI = TMath::Pi();
  //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data/e1f/exp.pcor.root");
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile); //_data/exp.root");
  if (!f) {
    //f = new TFile("/data/e1f/exp.pcor.root");
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

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  //   t3pi->SetBranchStatus("*",0);  // disable all branches
  //   t3pi->SetBranchStatus("e1",1);  // activate branchname

  Long64_t nentries = NTOPROC;
  printf("NTOPROC = %lld\n",nentries);
  if (nentries<=0) nentries = h10->GetEntries();
  printf("nentries = %lld\n",nentries);
  Long64_t eventnum = 0;

  const int MONSEC = 1;
  const int NPMTS = 36;
  const int NSECTS = 6;
  TCanvas *cnphe = new TCanvas("cnphe","mean number photo-electrons");
  cnphe->Divide(3);
  TH2 *hang_meanNphe[NSECTS][3];
  Fid *fid = Fid::Instance();
  TH1 *hnphe[NSECTS][NPMTS];
  for (int sectidx = 0; sectidx < NSECTS; sectidx++) {
    TString hn_ang1 = TString::Format("hang1_meanNphe_s%d",sectidx+1);
    TString hn_ang2 = TString::Format("hang1_meanNphe_s%d_pass",sectidx+1);
    TString hn_ang3 = TString::Format("hang1_meanNphe_s%d_fail",sectidx+1);
    TString ht_ang1 = TString::Format("CC mean num. of photo-electron, S%d",sectidx+1);
    TString ht_ang2 = TString::Format("CC mean num. of photo-electron, S%d, fid passed",sectidx+1);
    TString ht_ang3 = TString::Format("CC mean num. of photo-electron, S%d, fid failed",sectidx+1);
    hang_meanNphe[sectidx][0] = new TH2F(hn_ang1.Data(),ht_ang1,140,0,70,120,-30,30);
    hang_meanNphe[sectidx][1] = new TH2F(hn_ang2.Data(),ht_ang1,140,0,70,120,-30,30);
    hang_meanNphe[sectidx][2] = new TH2F(hn_ang3.Data(),ht_ang1,140,0,70,120,-30,30);
    for(int hidx = 0; hidx < 3; hidx++) hang_meanNphe[sectidx][hidx]->SetBit(TH1::kIsAverage);
    for (int pmtidx = 0; pmtidx < NPMTS; pmtidx++) {
      TString hn = TString::Format("hnphe_s%d_pmt%d",sectidx+1,pmtidx+1);
      TString ht = TString::Format("S%d, Pmt%d",sectidx+1,pmtidx+1);
      hnphe[sectidx][pmtidx] = new TH1D(hn.Data(),ht.Data(),72,-0.5,35.5);
    }
  }

  printf("created histograms\n");

  TStopwatch swmain;
  TStopwatch swgroup;
  const Int_t BLOCKSIZE = 100000;

  Long64_t nbytes = 0;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += h10->GetEntry(i);
    //nbytes += t3pi->GetEntry(i);

    if ( (++eventnum) % BLOCKSIZE == 0 ) {
      Float_t gtime = swgroup.RealTime();
      Float_t ttime = swmain.RealTime();
      Double_t percentProcessed = (((Double_t)eventnum)/(Double_t)nentries)*100;
      Float_t remaining = (100.0/percentProcessed*ttime-ttime)/60.0;
      printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(BLOCKSIZE/gtime),remaining);
      cnphe->cd(1);
      hang_meanNphe[MONSEC-1][0]->Draw("colz");
      gPad->Modified();
      gPad->Update();
      cnphe->cd(2);
      hang_meanNphe[MONSEC-1][1]->Draw("colz");
      gPad->Modified();
      gPad->Update();
      cnphe->cd(3);
      hang_meanNphe[MONSEC-1][2]->Draw("colz");
      gPad->Modified();
      gPad->Update();
      gSystem->ProcessEvents();
      swgroup.Start();
      swmain.Start(kFALSE);
    }

    int ccidx = cc[0]-1;
    int scidx = sc[0]-1;
    int sector_cc = cc_sect[ccidx];
    int sector_sc = sc_sect[scidx];
    Double_t mom = e1->P();
    Double_t phi = R2D*e1->Phi();
    Double_t theta = R2D*e1->Theta();
    if (phi<-30) phi+=360;

    //determine sector
    Double_t sectphi = phi + 30;
    Int_t iSector = sectphi/60; //relies on truncation
    iSector = iSector < 6 ? iSector : 0;
    sectphi -= 60*iSector+30;
    int sector_kin = iSector+1;

    //printf("p=%.2f, theta=%.2f, phi=%.2f, (%d,%d,%d)\n",mom,theta,phi,sector_cc,sector_sc,sector_kin);
    int segment = (cc_segm[ccidx]%1000)/10;
    int iSegment = segment-1;
    int pmt_hit = (cc_segm[ccidx]/1000)-1;
    int iPmt = segment*2 - (pmt_hit==1?0:1) - 1;
    //printf("*** (segment,pmt) = (%d,%d)\n",segment,iPmt+1);
    if (sector_sc == sector_cc && sector_sc == sector_kin && mom > 0 && mom < 6 && pmt_hit!=0) {
      if (w4.M()>1.6 && omega.M()>0.450) {
	int dcidx = dc[0]-1;
	float sc_x = dc_xsc[dcidx];
	float sc_y = dc_ysc[dcidx];
	float sc_z = dc_zsc[dcidx];
	float sc_cx = dc_cxsc[dcidx];
	float sc_cy = dc_cysc[dcidx];
	float sc_cz = dc_czsc[dcidx];
	float cct = cctheta(sc_x,sc_y,sc_z,sc_cx,sc_cy,sc_cz);
	float ccp = ccphi(sc_x,sc_y,sc_z,sc_cx,sc_cy,sc_cz);   
	float npe = nphe[ccidx]/10.0;
	//printf("p=%.2f, theta=%.2f, phi=%.2f, cctheta=%.2f, ccphi=%.2f, nphe=%.2f\n",mom,theta,phi,cct,ccp,npe);
	//printf("  (x,y,z)=(%.2f,%.2f,%.2f), (cx,cy,cz)=(%.2f,%.2f,%.2f)\n",sc_x,sc_y,sc_z,sc_cx,sc_cy,sc_cz);
	hang_meanNphe[iSector][0]->Fill(cct,ccp,npe);
	if (fid->InFid(mom,D2R*theta,D2R*phi,sector_kin,11)) {
	  hang_meanNphe[iSector][1]->Fill(cct,ccp,npe);
	  hnphe[iSector][iPmt]->Fill(npe);
	} else {
	  hang_meanNphe[iSector][2]->Fill(cct,ccp,npe);
	}
      }
    }
  }
}
