{
  gROOT->Reset();
  gROOT->ProcessLine(".L crootutils.C");
  bool norm = false;
  bool smooth = false;
  //TFile *fin[] = { new TFile("out/sc_pdls.exp.nofid.root"), new TFile("out/sc_pdls.sim.nofid.root") };
  TFile *fin[] = { new TFile("out/sc_pdls.sim.nofid.root"), new TFile("out/sc_pdls.exp.nofid.root") };
  const int goodcolors[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  const int NPART = 4;
  const int NSECT = 6;
  const int NTYPE = 2;
  enum { E, P, PIP, PIM };
  const char* PPART[NPART] = { "e", "p", "pip", "pim" };
  enum { EXP, SIM };
  const float PDLHI[NPART] = { -1, 35, -1, -1 };
  const float PHI[NPART] = { 4, 5, 4, 4 };
  const char* PTYPE[NTYPE] = { "exp", "sim" };
  TCanvas *c[NSECT];
  TCanvas *c1[NSECT];
  TCanvas *c2 = new TCanvas("callsectors","All Sectors",600,600);
  c2->Divide(2,2);
  const char* HS2T[NPART] = { "e^{-} count", "proton count", "#pi^{+} count", "#pi^{-} count" };
  const char* HS2N[NPART] = { "hs_scpdls_e",  "hs_scpdls_p",  "hs_scpdls_pip",  "hs_scpdls_pim" };
  THStack *hs[NPART] = { new THStack(), new THStack(), new THStack(), new THStack() };
  THStack *hs2[NPART] = { new THStack(), new THStack(), new THStack(), new THStack() };
  TH2 *h2[NSECT][NPART][NTYPE];
  TH1 *h1[NSECT][NPART][NTYPE];
  TH1 *h1r[NSECT][NPART];
  const float CPOS[6][2] = { 0,0, 520,0, 1040,0, 0,450, 520,450, 1040,450 };/*{ 0, 520, 1040, 0, 520, 1040,
                             0,   0,  0, 420, 420,  420 };*/
  for (int isect = 0; isect < NSECT; isect++) {
    TString cn = TString::Format("c%d",isect+1);
    c[isect] = new TCanvas(cn.Data(),cn.Data());
    c[isect]->Divide(2,4);
    cn = TString::Format("c1%d",isect+1);
    c1[isect] = new TCanvas(cn.Data(),cn.Data(),CPOS[isect][0],CPOS[isect][1],520,400);
    c1[isect]->Divide(2,2);
    int pnum = 1;
    int pnum1 = 1;
    for (int ipart = 0; ipart < NPART; ipart++) {
      for (int itype = 0; itype < NTYPE; itype++) {
	c[isect]->cd(pnum++);
	TString hpath = TString::Format("h_pdlVp_%s_s%d",PPART[ipart],isect+1);
	TString hdesc = TString::Format("%s, %s",PPART[ipart],PTYPE[itype]);
	TString hname = TString::Format("%s_%d_%s",PPART[ipart],isect+1,PTYPE[itype]);
	TH2 *htmp = (TH2*)fin[itype]->Get(hpath.Data());
	h2[isect][ipart][itype] = norm ? normXslices(htmp) : htmp;
	if (smooth) h2[isect][ipart][itype]->Smooth();
	h2[isect][ipart][itype]->SetName(hname.Data());
	h2[isect][ipart][itype]->SetTitle(hdesc.Data());
	h2[isect][ipart][itype]->GetYaxis()->SetRangeUser(0,PDLHI[ipart]);
	h2[isect][ipart][itype]->GetXaxis()->SetRangeUser(0,PHI[ipart]);
	h2[isect][ipart][itype]->Draw("colz");
	h1[isect][ipart][itype] = h2[isect][ipart][itype]->ProjectionY();
	h1[isect][ipart][itype]->Sumw2();
      }
      c1[isect]->cd(pnum1++);
      h1[isect][ipart][EXP]->SetLineColor(kBlue+1);
      h1[isect][ipart][SIM]->SetLineColor(kGreen+1);
      h1[isect][ipart][EXP]->Scale(1.0/h1[isect][ipart][EXP]->Integral()); //GetMaximum()); //Integral(6,-1));
      h1[isect][ipart][SIM]->Scale(1.0/h1[isect][ipart][SIM]->Integral()); //GetMaximum()); //Integral(6,-1));
      TString hn2 = TString::Format("%s2",h1[isect][ipart][EXP]->GetName());
      TH1 *h1clone = h1[isect][ipart][EXP]->Clone(hn2.Data());
      h1clone->SetLineColor(goodcolors[isect]);
      h1clone->SetOption("e0p");
      h1clone->SetMarkerStyle(20);
      h1clone->SetMarkerSize(0.5);
      h1clone->SetMarkerColor(goodcolors[isect]);
      hs[ipart]->Add(h1clone);
      if (h1[isect][ipart][EXP]->GetMaximum()>h1[isect][ipart][SIM]->GetMaximum()) {
	h1[isect][ipart][EXP]->Draw();
	h1[isect][ipart][SIM]->Draw("same");
      } else {
	h1[isect][ipart][SIM]->Draw();
	h1[isect][ipart][EXP]->Draw("same");
      }
    }
  }
  TFile fout("drawscpdls.tmp.root","update");
  TCanvas *c2b = new TCanvas("callsectorsb","All Sectors",1000,850);
  c2b->Divide(2,2);
  for (int ipart = 0; ipart < NPART; ipart++) {
    for (int isect = 0; isect < NSECT; isect++) {
      h1r[isect][ipart] = (TH1*)hs[ipart]->GetHists()->At(isect)->Clone();
      TString hn = TString::Format("%s_r",h1r[isect][ipart]->GetName());
      h1r[isect][ipart]->SetName(hn.Data());
    }
    TH1 *htmp1 = (TH1*)h1r[0][ipart]->Clone("htmp1");
    for (int bin = 1; bin <= h1r[0][ipart]->GetNbinsX(); bin++) {
      double max = 0;
      double maxerr = 0;
      for (int isect = 0; isect < NSECT; isect++) {
	double val = h1r[isect][ipart]->GetBinContent(bin);
	double err = h1r[isect][ipart]->GetBinError(bin);
	max = val>max ? val : max;
	maxerr = val>max ? err : maxerr;
      }
      htmp1->SetBinContent(bin,max);
      htmp1->SetBinError(bin,maxerr);
    }
    for (int isect = 0; isect < NSECT; isect++) {
      h1r[isect][ipart]->Divide(htmp1);
      TString hn = TString::Format("sector %d",isect+1);
      h1r[isect][ipart]->SetTitle(hn.Data());
      hs2[ipart]->Add(h1r[isect][ipart]);
    }
    delete htmp1;
    c2->cd(ipart+1);
    hs[ipart]->Draw("nostack");
    c2b->cd(ipart+1);
    hs2[ipart]->SetTitle(HS2T[ipart]);
    hs2[ipart]->SetName(HS2N[ipart]);
    hs2[ipart]->Draw("nostack");
    gPad->Update();
    if (ipart==0) {
      gPad->BuildLegend(0.23,0.22,0.43,0.5576);
    }
    hs2[ipart]->GetXaxis()->SetTitle("SC paddle");
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->Update();
    fout.WriteObject(hs2[ipart],hs2[ipart]->GetName());
  }
}
