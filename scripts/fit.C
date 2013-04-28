#include "scripts/rootutils.C"
#include <stdio.h>
#include <TSystem.h>
#include <TFile.h>
#include <TObject.h>
#include <TList.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TMinuit.h>

void fitmmp(TH1 *hmmp, bool verbose = false) {
  TString options;
  if (verbose) options = "";
  else options = "Q";
  TF1 *fbg = f_pol4("fbg",0.4,2,true);
  hmmp->Fit(fbg,options.Data(),"",0.42,2);
  //printf("%s\n",gMinuit->fCstatu.Data());
  if (true) { //gMinuit->fCstatu.Contains("CONVER")) {
    TF1 *fbg2 = f_pol4("fbg",0.4,2,false);
    fbg2->SetParameters(fbg->GetParameters());
    //fbg2->Draw("same");
    fbg2->SetParameter(5,0);
    fbg2->SetParameter(6,0);
    TH1 *htmp = (TH1*)hmmp->Clone("hmmp_bgsub");
    htmp->Add(fbg2,-1);
    //htmp->Draw();
    TF1 *fgaus = new TF1("fgaus","gaus",0.4,2);
    htmp->Fit(fgaus,options.Data(),"",0.74,0.85);
    TF1 *f = f_pol4gaus("f",0.4,2,fbg2,fgaus);
    f->SetNpx(500);
    hmmp->Fit(f,options.Data(),"",0.42,2);
    fgaus->SetRange(0.4,2);
    for (int i = 0; i < 3; i++) fgaus->SetParameter(i,f->GetParameter(i+5));
    for (int i = 0; i < 5; i++) fbg2->SetParameter(i,f->GetParameter(i));
    fbg2->SetLineStyle(2);
    fbg2->SetLineColor(kRed+1);
    fgaus->SetLineStyle(2);
    fgaus->SetLineColor(kGreen+1);
    hmmp->GetListOfFunctions()->Add(fbg2->Clone());
    hmmp->GetListOfFunctions()->Add(fgaus->Clone());
    delete fbg2;
    delete htmp;
    delete fgaus;
    delete f;
  } else hmmp->GetListOfFunctions()->Delete();
  delete fbg;
}

float *getwq(TH1 *h) {
  TString delim = "_";
  TString hn = h->GetName();
  TObjArray *tokens = hn.Tokenize(delim);
  TObjString *wstro = (TObjString*)tokens->At(1);
  TObjString *qstro = (TObjString*)tokens->At(2);
  TString wstr = wstro->GetString();
  TString qstr = qstro->GetString();
  double wval = wstr.Atof();
  double qval = qstr.Atof();
  float *ret = new float[2];
  ret[0] = wval;
  ret[1] = qval;
  delete tokens;
  return ret;
}

void fit() {
  FILE *ofile;
  ofile = fopen("xsect-integrated-me.txt","w");
  TFile *_file0 = TFile::Open("h3maker-hn.root","update");
  _file0->Delete("*_f;*");
  TH2 *h2xsect = new TH2("hq2wXsect","Q^2:W",32,1.6,3.2,7,1.5,5.1);
  Double_t qbinedges[] = { 1.5, 1.6, 1.8, 2.1, 2.4, 2.76, 3.3, 5.1 };
  h2xsect->GetYaxis()->Set(7,qbinedges);
  TH3 *h3 = (TH3*)_file0->Get("hq2wmmp");
  int qbins = h3->GetZaxis()->GetNbins();
  int wbins = h3->GetYaxis()->GetNbins();
  fprintf(ofile, "W\tQ2\txsect\terror\tpol4p0\tpol4p1\tpol4p2\tpol4p3\tpol4p4\tgN\tgM\tgS\n");
  for (int iq = 0; iq < qbins; iq++) {
    TString hsn = TString::Format("hs%d",iq);
    THStack *hs = (THStack*)_file0->Get(hsn.Data());
    TIter next(hs->GetHists());
    //while (TObject *obj = next()) {
    //TH1 *h = (TH1*)obj;
    while (TH1 *h = (TH1*)next()) {
      float *wq = getwq(h);
      float wval = wq[0];
      float qval = wq[1];
      fitmmp(h);
      TH1 *htmp = (TH1*)h->Clone("hbgsubtracted");
      TF1 *fbg = (TF1*)h->GetListOfFunctions()->FindObject("fbg");
      htmp->Add(fbg,-1);
      double N = htmp->Integral(34,43);
      double qwidth = h3->GetZaxis()->GetBinWidth(iq+1);
      int wbin = h3->GetYaxis()->FindBin(wval);
      double wwidth = h3->GetYaxis()->GetBinWidth(wbin);
      double xsect = N/(0.891*wwidth*qwidth*19.844);
      double err2 = 0;
      for (int immp = 34; immp < 44; immp++) err2 += htmp->GetBinError(immp)*htmp->GetBinError(immp);
      //fprintf(ofile, "%.3f\t%.3f\t%.0f\t%.0f",wval,qval,xsect/(1e6), sqrt(err2)/(1e6));
      fprintf(ofile, "%.3f\t%.3f\t%.3e\t%.3e",wval,qval,xsect/(1e6), sqrt(err2)/(1e6));
      TF1 *ftmp = (TF1*)h->GetListOfFunctions()->At(0);
      int npar = ftmp->GetNpar();
      for (int ipar = 0; ipar < npar; ipar++) fprintf(ofile, "\t%.3e", ftmp->GetParameter(ipar));
      fprintf(ofile, "\n");
    }
    hsn.Append("_f");
    _file0->WriteObject(hs,hsn.Data());
    delete hs;
  }
  fclose(ofile);
  delete _file0;
}

void parms(TH1 *h) {
  float *wq = getwq(h);
  float wval = wq[0];
  float qval = wq[1];
  printf("%.3e, %.3e\n",wval,qval);
}

//void (*somefn)(TH1 *h);
//void (*somefn);
// void hmmp_each( void(*somefn)(TH1 *h) ) {
//   TFile *_file0 = TFile::Open("h3maker-hn.root");
//   TH3 *h3 = (TH3*)_file0->Get("hq2wmmp");
//   int qbins = h3->GetZaxis()->GetNbins();
//   int wbins = h3->GetYaxis()->GetNbins();
//   for (int iq = 0; iq < qbins; iq++) {
//     TString hsn = TString::Format("hs%d",iq);
//     THStack *hs = (THStack*)_file0->Get(hsn.Data());
//     TIter next(hs->GetHists());
//     while (TH1 *h = (TH1*)next()) {
//       somefn(h);
//     }
//     delete hs;
//   }
//   delete _file0;
// }
