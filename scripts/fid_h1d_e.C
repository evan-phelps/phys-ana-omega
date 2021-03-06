{
  gROOT->Reset();
  gROOT->ProcessLine(".L ~/analysis/omega/fid.cpp+");
  gROOT->ProcessLine(".L ~/analysis/omega/scripts/rootutils.C+");
  gROOT->SetStyle("Modern");
  gStyle->SetPadBottomMargin(0.16);
  //gStyle->SetTitleOffset(0.8,"Y");
  const bool SAVE = true; //true;
  const char *IN = "fide.root";
  const int REBINX = 2;
  const int REBINY = 2;
  const int REBINZ = 10;
  const char *XY = "xy";
  const char *DIR = "4400-MeV";
  const float PVAL = 4.4;
  const float T0TIGHT = 0;
  const float PHITIGHT = 1;
  const int SECTOR = 1;
  const float DMIDTHETA = 2;
  const int THRESHOLD = 100;
  const char *HN3TMPL = "h3e_pVthetaVphi_s%d";
  const char *HN2TMPL = "h2e_thetaVphi_s%d_p%d";
  const char *HN1TMPL = "h1e_phi_s%d_p%d_t_%d";
  const char *HNTHETATMPL = "h1e_theta_s%d_p%d";
  const char *AXTHETA = "#theta (degrees)";
  const char *AXPHI = "#phi (degrees)";
  const char *HTTHETA = "p #in [%.3f,%.3f), #phi #in (%.1f,%.1f), sector %d";
  const char *HTANG = "p #in [%.3f,%.3f), sector %d";
  const char *HTPHI = "p #in [%.3f,%.3f), #theta #in [%.1f,%.1f), #sector %d";
  
  TFile fio(IN,"update");
  TH3 *h3e[6];
  TH2 *h2e;
  TH1 *h1e_theta, *h1e_theta2;
  //TH1 *h1e_tPphi;
  THStack *hsphi = new THStack("hsphi","hsphi");
  for (int i = 0; i < 6; i++) {
    TString hn = TString::Format(HN3TMPL,i+1);
    h3e[i] = (TH3*)fio.Get(hn.Data());
    if (REBINX+REBINY+REBINZ>3) h3e[i]->Rebin3D(REBINX,REBINY,REBINZ);
  }
  int isect = SECTOR-1;
  int pbin = h3e[isect]->GetZaxis()->FindBin(PVAL);
  float pval = h3e[isect]->GetZaxis()->GetBinCenter(pbin);
  float plow = h3e[isect]->GetZaxis()->GetBinLowEdge(pbin);
  float phigh = h3e[isect]->GetZaxis()->GetBinUpEdge(pbin);
  TString ht_theta = TString::Format(HTTHETA,plow,phigh,-DMIDTHETA/2,DMIDTHETA/2,SECTOR);
  TString htang = TString::Format(HTANG,plow,phigh,SECTOR);

  if ( !(fio.cd(DIR) || (fio.mkdir(DIR) && fio.cd(DIR))) ) {
    printf("Could not make directory!");
  } else {
    TH3 *htmp = h3e[isect];
    htmp->GetZaxis()->SetRange(pbin,pbin);
    h2e = (TH2*)htmp->Project3D(XY);
    TString hn = TString::Format(HN2TMPL,SECTOR,pbin);
    h2e->SetName(hn.Data());
    h2e->SetTitle(htang.Data());
    h2e->GetXaxis()->SetTitle(AXTHETA);
    h2e->GetYaxis()->SetTitle(AXPHI);
    hn = TString::Format(HNTHETATMPL,SECTOR,pbin);
    h2e->GetYaxis()->SetRangeUser(-DMIDTHETA/2,DMIDTHETA/2);
    h1e_theta = h2e->ProjectionX(hn.Data());
    h1e_theta->SetTitle(ht_theta.Data());
    h1e_theta->GetXaxis()->SetTitle(AXTHETA);
    //h1e_tPphi = (TH1*)h1e_theta->Clone("h1e_tPphi");
    //h1e_tPphi->SetTitle("#theta/#Delta#phi");
    h2e->GetYaxis()->SetRangeUser(0,-1);
    hn.Append("_fid");
    h1e_theta2 = (TH1*)h1e_theta->Clone(hn.Data());
    //h1e_theta->DrawCopy();
    Fid *fid = Fid::Instance();
    TF1 *fphi = fid->fPhiFid(pval,SECTOR,1,PHITIGHT,T0TIGHT);
    TF1 *fphi2 = fid->fPhiFid(pval,SECTOR,-1,PHITIGHT,T0TIGHT);
    TF1 *ft0 = fid->fThetaMin(SECTOR);
    Double_t t0 = ft0->Eval(pval) + T0TIGHT;
    //TF1 *fphi3 = fid->fPhiFid(pval,SECTOR,1,PHITIGHT,-5);
    //h1e_tPphi->Divide(fphi3);
    h1e_theta2->SetFillColor(kGreen+1);
    h1e_theta2->SetFillStyle(3002);
    h1e_theta2->GetXaxis()->SetRangeUser(t0,-1);
    h1e_theta2->SetOption("same");
    h1e_theta2->SetDrawOption("same");
    //h1e_theta->DrawCopy("same");
    h1e_theta->GetListOfFunctions()->Add(h1e_theta2);
    h1e_theta->GetXaxis()->SetRangeUser(10,70);
    int t0bin = h1e_theta->GetXaxis()->FindBin(t0);
    int maxbin = h1e_theta->GetNbinsX();
    for (int bin = t0bin+1; bin < maxbin; bin++) {
      if (h1e_theta->GetBinContent(bin) < THRESHOLD) continue;
      TString hn = TString::Format(HN1TMPL,SECTOR,pbin,bin);
      h2e->GetXaxis()->SetRange(bin,bin);
      TH1 *h1e = h2e->ProjectionY(hn.Data());
      double tlow = h2e->GetXaxis()->GetBinLowEdge(bin);
      double thigh = h2e->GetXaxis()->GetBinUpEdge(bin);
      double tmid = h2e->GetXaxis()->GetBinCenter(bin);
      TString htphi = TString::Format(HTPHI,plow,phigh,tlow,thigh,SECTOR);
      h1e->GetXaxis()->SetTitle(AXPHI);
      h1e->SetTitle(htphi.Data());
      hn.Append("_fid");
      float tval = h2e->GetXaxis()->GetBinCenter(bin);
      TH1 *h1e2 = (TH1*)h1e->Clone(hn.Data());
      float bphi = fphi->Eval(tval);
      h1e2->GetXaxis()->SetRangeUser(-bphi,bphi);
      h1e2->SetFillColor(kGreen+1);
      h1e2->SetFillStyle(3002);
      h1e->GetListOfFunctions()->Add(h1e2);
      h1e2->SetOption("same");
      h1e2->SetDrawOption("same");
      //text box with theta
      double ymax = h1e->GetMaximum();
      TString str_tmid = TString::Format("%.2f^{o}",tmid);
      TLatex *tt = new TLatex(-6,0.2*ymax,str_tmid);
      tt->SetTextSize(0.11);
      tt->SetTextColor(kRed+1);
      h1e->GetListOfFunctions()->Add(tt->Clone());
      delete tt;
      hsphi->Add(h1e);
      h2e->GetXaxis()->SetRange(0,-1);
    }
    TCanvas *ct0 = new TCanvas("ct0","#theta_{min} cut",899,530,700,369);
    h1e_theta->Draw();
    TCanvas *chsphi = new TCanvas("chsphi","#phi cuts",568,52,1206,584);
    hsphi->Draw("pads");
    fphi->SetNpx(1000);
    fphi2->SetNpx(1000);
    TCanvas *cAng = new TCanvas("cang","#phi vs. #theta",2,64,564,835);
    cAng.Divide(1,2);
    cAng.cd(1);
    h2e->GetListOfFunctions()->Add(fphi);
    h2e->GetListOfFunctions()->Add(fphi2);
    h2e->GetYaxis()->SetTitleOffset(0.8);
    h2e->GetXaxis()->SetRangeUser(10,70);
    h2e->Draw("col");
    TH2 *h2en =  normXslices(h2e);
    TString h2en_name = h2e->GetName();
    h2en_name.Append("_norm");
    h2en->SetName(h2en_name.Data());
    h2en->SetMaximum(1);
    h2en->GetXaxis()->SetRangeUser(10,70);
    cAng.cd(2);
    h2en->Draw("col");
    if (SAVE) {
      for (int i = 0; i < 6; i++) delete h3e[i];
      htmp = 0;
      gDirectory->WriteObject(hsphi,"hsphi");
      gDirectory->WriteObject(ct0,"ct0");
      gDirectory->WriteObject(cang,"cang");
      gDirectory->WriteObject(chsphi,"chsphi");
      fio.Write();
    }
  }
}
