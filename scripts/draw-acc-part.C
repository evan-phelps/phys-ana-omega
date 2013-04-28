gROOT->ProcessLine(".L infid.C");

THStack* sliceP(TH3 *ht, Double_t thresh = 100) {
  THStack *hets = new THStack();
  TH1 *h2tmp;
  for (int mbin = 1; mbin <= ht->GetNbinsZ(); mbin++) {
    ht->GetZaxis()->SetRange(mbin,mbin);
    h2tmp = ht->Project3D("yx");
    TString hn = TString::Format("het_%d",mbin);
    h2tmp->SetName(hn.Data());
    h2tmp->SetOption("colz");
    h2tmp->SetDrawOption("colz");
    if (h2tmp->GetEntries()>thresh) hets->Add(h2tmp);
  }
  return hets;
}

void drawAccPart(char *pname, int bin) {
  TFile *_file0 = TFile::Open("acc-per-part.root");
  TString hthrown = TString::Format("ht_%s_mtp",pname);
  TH3 *ht = (TH3*)_file0->Get(hthrown.Data());
  TObjArray *hets = new TObjArray();
  TH1 *h2tmp;
  for (int mbin = 1; mbin <= ht->GetNbinsZ(); mbin++) {
    ht->GetZaxis()->SetRange(mbin,mbin);
    h2tmp = ht->Project3D("yx");
    TString hn = TString::Format("het_%d",mbin);
    h2tmp->SetName(hn.Data());
    if (h2tmp->GetEntries()>100) hets->Add(h2tmp);
  }
  hets->At(bin)->Draw();
  TString hacc = TString::Format("ha_%s_mtp",pname);
  TH3 *ha = (TH3*)_file0->Get(hacc.Data());
  TObjArray *heas = new TObjArray();
  for (int mbin = 1; mbin <= ha->GetNbinsZ(); mbin++) {
    ha->GetZaxis()->SetRange(mbin,mbin);
    h2tmp = ha->Project3D("yx");
    TString hn = TString::Format("heacc_%d",mbin);
    h2tmp->SetName(hn.Data());
    if (h2tmp->GetEntries()>100) heas->Add(h2tmp);
  }
  heas->At(bin)->Draw("samecolz");
  TH2 *htmp = (TH2*)heas->At(bin);
  htmp->Rebin2D(2,2);
  htmp->Scale(0.25);
  htmp->Smooth(1);
  htmp->SetMaximum(1.0);
  htmp->SetMinimum(0.0);
}

THStack* accDist(TH3 *acc, Double_t threshC, Double_t threshE, bool reqfid = false, int pid = 11) {
  Int_t nbX = acc->GetNbinsX();
  Int_t nbY = acc->GetNbinsY();
  Int_t nbZ = acc->GetNbinsZ();
  TString hn = TString::Format("%s_acc",acc->GetName());
  TH1 *hacc = new TH1F(hn.Data(),hn.Data(),400,0,1);
  hn = TString::Format("%s_err",acc->GetName());
  TH1 *herr = new TH1F(hn.Data(),hn.Data(),500,0,1);
  for (int ix = 1; ix <= nbX; ix++) {
    for (int iy = 1; iy <= nbY; iy++) {
      for (int iz = 1; iz <= nbZ; iz++) {
	Double_t bc = acc->GetBinContent(ix,iy,iz);
	Double_t be = bc>0 ? acc->GetBinError(ix,iy,iz)/bc : 0;
	if (bc>threshC && be<threshE) {
	  double mom = acc->GetZaxis()->GetBinCenter(iz);
	  double theta = TMath::DegToRad()*acc->GetYaxis()->GetBinCenter(iy);
	  double phi = TMath::DegToRad()*acc->GetXaxis()->GetBinCenter(ix);
	  if (!reqfid || infid(mom,theta,phi,pid)  ) {
	    hacc->Fill(bc);
	    herr->Fill(be);
	  }
	}
      }
    }
  }
  THStack *hs = new THStack();
  hs->Add(hacc);
  hs->Add(herr);
  return hs;
}

TH3* applyThresholds(TH3 *acc, Double_t threshC = 0.5, Double_t threshE = 0.25) {
  Int_t nbX = acc->GetNbinsX();
  Int_t nbY = acc->GetNbinsY();
  Int_t nbZ = acc->GetNbinsZ();
  TString hn = TString::Format("%s_thresh",acc->GetName());
  TH3 *haccthresh = (TH3*)acc->Clone(hn.Data());
  for (int ix = 1; ix <= nbX; ix++) {
    for (int iy = 1; iy <= nbY; iy++) {
      for (int iz = 1; iz <= nbZ; iz++) {
	Double_t bc = acc->GetBinContent(ix,iy,iz);
	Double_t be = bc>0 ? acc->GetBinError(ix,iy,iz)/bc : 0;
	if (!(bc>threshC && be<threshE)) {
	  haccthresh->SetBinContent(ix,iy,iz,0.0);
	  haccthresh->SetBinError(ix,iy,iz,0.0);
	}
      }
    }
  }
  return haccthresh;
}

void drawDists(TH3 *hae, const char *plabel = "e", const char *pn = "e^{-}") {
  TString cn = TString::Format("acc_eff_%s",plabel);
  TCanvas *ceff = new TCanvas(cn.Data(),cn.Data());
  cn = TString::Format("acc_err_%s",plabel);
  TCanvas *cerr = new TCanvas(cn.Data(),cn.Data());
  TString hstitle = TString::Format("%s detection",pn);

  THStack *hsefull = accDist(hae,0,2);
  TH1 *ha_e_acc = (TH1*)hsefull->GetHists()->At(0);;
  TH1 *ha_e_err = (TH1*)hsefull->GetHists()->At(1);;
  ha_e_acc->SetTitle("full volume");
  ha_e_err->SetTitle("full volume");
  TH1 *e_full_eff = (TH1*)ha_e_acc->Clone("e_full_eff");
  TH1 *e_full_err = (TH1*)ha_e_err->Clone("e_full_err");
  delete ha_e_acc;
  delete ha_e_err;
  THStack *hsefid = accDist(hae,0,2,true);
  TH1 *ha_e_acc = (TH1*)hsefid->GetHists()->At(0);;
  TH1 *ha_e_err = (TH1*)hsefid->GetHists()->At(1);;
  ha_e_acc->SetTitle("fiducial volume");
  ha_e_err->SetTitle("fiducial volume");
  TH1 *e_fid_eff = (TH1*)ha_e_acc->Clone("e_fid_eff");
  TH1 *e_fid_err = (TH1*)ha_e_err->Clone("e_fid_err");
  delete hsefull;
  delete hsefid;

  THStack *hse_eff = new THStack("hse_eff",hstitle.Data());
  hse_eff->Add(e_full_eff);
  hse_eff->Add(e_fid_eff);
  e_full_eff->SetLineColor(kRed+1);
  e_fid_eff->SetLineColor(kGreen+1);
  e_full_eff->SetLineWidth(2);
  e_fid_eff->SetLineWidth(2);
  ceff->cd();
  hse_eff->Draw("nostack");
  gPad->BuildLegend(0.569,0.763,0.879,0.881);
  gPad->SetLogy();
  hse_eff->GetHistogram()->GetXaxis()->SetTitle("efficiency");

  THStack *hse_err = new THStack("hse_err",hstitle.Data());
  hse_err->Add(e_full_err);
  hse_err->Add(e_fid_err);
  e_full_err->SetLineColor(kRed+1);
  e_fid_err->SetLineColor(kGreen+1);
  e_full_err->SetLineWidth(2);
  e_fid_err->SetLineWidth(2);
  cerr->cd();
  hse_err->Draw("nostack");
  gPad->BuildLegend(0.569,0.763,0.879,0.881);
  gPad->SetLogy();
  hse_err->GetHistogram()->GetXaxis()->SetTitle("relative error");
}
