{
  //assumes ".x h10t3pi_paddles.C++;"
  int nbinsdiff = h2e[0]->GetXaxis()->GetNbins();
  int goodcolors[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  THStack *hse = stack(h2e,"hse","electron paddle hits");
  TCanvas c2d("c2d","paddle vs. momentum");
  hse->Draw("pads");
  TH1 *hpdl[6];
  TH1 *hpdlmax;
  for (int h2idx = 0; h2idx<6; h2idx++) {
    h2e[h2idx]->Sumw2();
    hpdl[h2idx] = h2e[h2idx]->ProjectionY();
    if (h2idx == 0) {
      hpdlmax = (TH1*)hpdl[h2idx]->Clone("hpdlmax");
      hpdlmax->SetTitle("max per bin");
    } else {
      for (int ibin = 1; ibin < hpdlmax->GetNbinsX(); ibin++) {
	hpdlmax->SetBinContent(ibin,TMath::Max(hpdl[h2idx]->GetBinContent(ibin),hpdlmax->GetBinContent(ibin)));
      }
    }
  }
  THStack *hspdl = stack(hpdl,"hspdl","paddle hits");
  TCanvas cP("cP","paddle counts");
  TH1 *hpdlavg = (TH1*)hpdl[0]->Clone("hpdl_avg");
  for (h2idx = 1; h2idx<6; h2idx++) hpdlavg->Add(hpdl[h2idx]);
  hpdlavg->Scale(1.0/6);
  hpdlavg->SetLineWidth(2);
  hpdlavg->SetTitle("average");
  hspdl->Add(hpdlavg);
  for (h2idx = 0; h2idx<6; h2idx++) {;
    hpdl[h2idx]->SetOption("pe");
    hpdl[h2idx]->SetMarkerColor(goodcolors[h2idx]);
    hpdl[h2idx]->SetMarkerStyle(20);
    hpdl[h2idx]->SetMarkerSize(1);
  }
  hspdl->Draw("nostackpe");
  hpdlavg->Draw("sameCpe");

  TH1 *hpdlN[6];
  for (h2idx = 0; h2idx<6; h2idx++) {
    hpdlN[h2idx] = (TH1*)hpdl[h2idx]->Clone();
    TString hnN = hpdlN[h2idx]->GetName();
    TString htN = hpdlN[h2idx]->GetTitle();
    hnN.Append("_N");
    htN.Append(", normalized");
    hpdlN[h2idx]->SetName(hnN.Data());
    hpdlN[h2idx]->SetTitle(htN.Data());
    hpdlN[h2idx]->Scale(1.0/hpdlN[h2idx]->Integral());
    hpdlN[h2idx]->SetMarkerStyle(20);
    hpdlN[h2idx]->SetMarkerColor(goodcolors[h2idx]);
    hpdlN[h2idx]->SetMarkerSize(1);
    hpdlN[h2idx]->SetLineColor(goodcolors[h2idx]);
  }
  TH1 *hpdlNavg = (TH1*)hpdlN[0]->Clone("hpdlN_avg");
  for (h2idx = 1; h2idx<6; h2idx++) hpdlNavg->Add(hpdlN[h2idx]);
  hpdlNavg->Scale(1.0/6);
  hpdlNavg->SetLineWidth(2);
  TCanvas cN("cN","normalized paddle counts");
  THStack *hspdlN = stack(hpdlN,"hspdlN","normalized paddle counts");
  hspdlN->Add(hpdlNavg);
  hspdlN->Draw("nostackpe");
  hpdlNavg->Draw("sameCpe");

  TCanvas cD("cD","bin2bin differences in normalized counts");
  TH1 *hpdlD[6];
  for (h2idx = 0; h2idx<6; h2idx++) {
    TH1 *htmp = hpdlN[h2idx];
    TString hnD = htmp->GetName();
    TString htD = htmp->GetTitle();
    hnD.Append("D");
    htD.Append(", #Deltay");
    hpdlD[h2idx] = new TH1D(hnD.Data(),htD.Data(),nbinsdiff-1,1.5,nbinsdiff+0.5);
    hpdlD[h2idx]->SetOption("pe");
    hpdlD[h2idx]->SetMarkerStyle(20);
    hpdlD[h2idx]->SetMarkerColor(goodcolors[h2idx]);
    hpdlD[h2idx]->SetMarkerSize(1);
    hpdlD[h2idx]->SetLineColor(goodcolors[h2idx]);
    for (int bin = 1; bin < hpdlavg->GetNbinsX(); bin++) {
      float x0 = htmp->GetBinContent(bin);
      float x1 = htmp->GetBinContent(bin+1);
      float x0e = htmp->GetBinError(bin);
      float x1e = htmp->GetBinError(bin+1);
      float dxe = sqrt(pow(x0e,2)+pow(x1e,2));
      hpdlD[h2idx]->SetBinContent(bin,x1-x0);
      hpdlD[h2idx]->SetBinError(bin,dxe);
    }
  }
  TH1 *hpdlDavg = (TH1*)hpdlD[0]->Clone("hpdlD_avg");
  for (h2idx = 1; h2idx<6; h2idx++) hpdlDavg->Add(hpdlD[h2idx]);
  hpdlDavg->Scale(1.0/6);
  hpdlDavg->SetLineWidth(2);
  THStack *hspdlD = stack(hpdlD,"hsepdlD","paddle-to-paddle differences");
  hspdlD->Add(hpdlDavg);
  hspdlD->Draw("nostackpe");
  hpdlDavg->Draw("sameCpe");

  TH1 *hpdlNa[6];
  for (h2idx = 0; h2idx<6; h2idx++) {
    hpdlNa[h2idx] = (TH1*)hpdl[h2idx]->Clone();
    TString hnN = hpdlNa[h2idx]->GetName();
    TString htN = hpdlNa[h2idx]->GetTitle();
    hnN.Append("_Na");
    htN.Append(", per-bin normalized");
    hpdlNa[h2idx]->SetName(hnN.Data());
    hpdlNa[h2idx]->SetTitle(htN.Data());
    hpdlNa[h2idx]->Divide(hpdlavg);
    hpdlNa[h2idx]->SetMarkerStyle(20);
    hpdlNa[h2idx]->SetMarkerColor(goodcolors[h2idx]);
    hpdlNa[h2idx]->SetMarkerSize(1);
    hpdlNa[h2idx]->SetLineColor(goodcolors[h2idx]);
  }
  TCanvas cNa("cNa","per-pdl normalized paddle counts");
  THStack *hspdlNa = stack(hpdlNa,"hspdlNa","per-bin normalized paddle counts");
  hspdlNa->Draw("nostackpe");

  TH1 *hpdlNmax[6];
  for (h2idx = 0; h2idx<6; h2idx++) {
    hpdlNmax[h2idx] = (TH1*)hpdl[h2idx]->Clone();
    TString hnN = hpdlNmax[h2idx]->GetName();
    TString htN = hpdlNmax[h2idx]->GetTitle();
    hnN.Append("_Nmax");
    htN.Append(", per-bin normalized");
    hpdlNmax[h2idx]->SetName(hnN.Data());
    hpdlNmax[h2idx]->SetTitle(htN.Data());
    hpdlNmax[h2idx]->Divide(hpdlmax);
    hpdlNmax[h2idx]->SetMarkerStyle(20);
    hpdlNmax[h2idx]->SetMarkerColor(goodcolors[h2idx]);
    hpdlNmax[h2idx]->SetMarkerSize(1);
    hpdlNmax[h2idx]->SetLineColor(goodcolors[h2idx]);
  }
  TCanvas cNmax("cNmax","per-pdl-max normalized paddle counts");
  THStack *hspdlNmax = stack(hpdlNmax,"hspdlNmax","per-bin normalized paddle counts");
  TH1 *hpdlNmaxavg = hpdlavg->Clone("hpdlNmaxavg");
  hpdlNmaxavg->SetTitle("avg. normalized to max per-bin");
  hpdlNmaxavg->Divide(hpdlmax);
  hspdlNmax->Add(hpdlNmaxavg);
  hspdlNmax->Draw("nostackpe");

  gDirectory->WriteObject(hse,hse->GetName());
  gDirectory->WriteObject(hspdl,hspdl->GetName());
  gDirectory->WriteObject(hspdlN,hspdlN->GetName());
  gDirectory->WriteObject(hspdlD,hspdlD->GetName());
  gDirectory->WriteObject(hspdlNa,hspdlNa->GetName());
  gDirectory->WriteObject(hspdlNmax,hspdlNmax->GetName());
}
