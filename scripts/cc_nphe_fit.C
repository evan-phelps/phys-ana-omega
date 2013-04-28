{
  gROOT->ProcessLine(".L ~/analysis/omega/scripts/rootutils.C");
  TFile *fin = TFile::Open("out/cc_nphe.root","update");
  const bool SAVE2FILE = false;
  enum { BETAFUN, EGAMMAFUN };
  const bool LLFIT = true;
  char* FITOPTS;
  FITOPTS = LLFIT?"QNM":"QNL";
  const int mode = EGAMMAFUN;
  const int NSECTS = 6;
  TString dirname;
  if (mode==BETAFUN) {
    dirname = "betadist";
  } else if (mode==EGAMMAFUN) {
    dirname = "expgamma";
  }
  if (LLFIT) dirname.Append("LL");
  fin.mkdir(dirname.Data());
  fin.cd(dirname.Data());
  THStack *hs[6];
  TCanvas *cs[6];
  //TCanvas ctmp;
  TF1 *ffit;
  if (mode==BETAFUN) ffit = new TF1("fnphetmp",&d_betaf,0,35,5);
  else if(mode==EGAMMAFUN) ffit = new TF1("fnphetmp","[0]*([1]**(x/[2])/TMath::Gamma(x/[2]+1))*exp(-[1])",0,35);
  for (int j = 0; j < NSECTS; j++) {
    TString hsn = TString::Format("hsnphe_s%d",j+1);
    TString hst = TString::Format("num photo-electrons s%d",j+1);
    hs[j] = new THStack(hsn.Data(),hst.Data());
    for (int i = 0; i < 36; i++) {
      TString hn = TString::Format("hnphe_s%d_pmt%d",j+1,i+1);
      TString ht = TString::Format("s%d, pmt%d",j+1,i+1);
      TH1 *htmp = (TH1*)fin->Get(hn.Data());
      htmp->SetDirectory(gDirectory);
      hn.Append("_cut");
      TH1 *htmp2 = htmp->Clone(hn.Data());
      htmp2->GetXaxis()->SetRangeUser(3,-1);
      htmp2->SetFillColor(kGreen+1);
      htmp2->SetFillStyle(3002);
      htmp2->SetOption("same");
      htmp->GetXaxis()->SetRangeUser(-0.5,35.5);
      htmp->GetListOfFunctions()->Add(htmp2);
      hs[j]->Add(htmp);
      //ctmp->cd();
      float p0factor = (mode==BETAFUN)?35.0:10.0;
      float par0 = htmp->Integral()/p0factor;
      if (mode==BETAFUN) {
	ffit->SetParameters(0,35,2,5,400);
	ffit->FixParameter(0,0);
	ffit->SetParLimits(1,10,50);
	ffit->SetParLimits(2,1,50);
	ffit->SetParLimits(3,2,70);
	ffit->SetParLimits(4,20,1000);
      } else if(mode==EGAMMAFUN) ffit->SetParameters(par0,3,3);
      htmp->Fit(ffit,FITOPTS,"",3.5,30);
      //TF1 *ffit2 = new TF1("fnphe","[0]*([1]**(x/[2])/TMath::Gamma(x/[2]+1))*exp(-[1])",0,35);
      TF1 *ffit2 = (TF1*)ffit->Clone("fnphe");
      ffit2->SetParameters(ffit->GetParameters());
      htmp->GetListOfFunctions()->Add(ffit2);
    }
    TString cn = TString::Format("csnphe_s%d",j+1);
    TString ct = TString::Format("sector %d",j+1);
    cs[j] = new TCanvas(cn.Data(),ct.Data());
    hs[j]->Draw("pads");
  }
  if (SAVE2FILE) {
    for (int jj = 0; jj < NSECTS; jj++) {
      gDirectory->WriteObject(hs[jj],hs[jj]->GetName());
    }
  }
}
