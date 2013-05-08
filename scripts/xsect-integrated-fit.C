//#include "scripts/rootutils.C"
#include "particle-constants.h"
#include <stdio.h>
#include <TSystem.h>
#include <TFile.h>
#include <TObject.h>
#include <TList.h>
#include <TF1.h>
#include <TLine.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TMinuit.h>

using namespace ParticleConstants;

Double_t stepfactor(Double_t x, Double_t x0, Double_t x1)
{
  if (x < x0) return 1;
  else if (x > x1) return 0;
  else return 1 - 1/(x1-x0) * (x-x0);
}

Double_t d_sig(Double_t *x, Double_t *par)
{
  return stepfactor(x[0],par[3],par[4])*par[0]*exp(-0.5*pow(((x[0]-par[1])/par[2]),2));
}

Double_t d_pol4(Double_t *x, Double_t *par)
{
  if ( par[5] < par[6] ) {
    if ( (x[0] > par[5] && x[0] < par[6]) || (x[0] > 0.5 && x[0] < 0.6) ) {
      TF1::RejectPoint();
      //return 0;
    }
  }
  double retval = (par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0])*stepfactor(x[0],par[7],par[8]);
  if (retval<0) {
    //TF1::RejectPoint();
    return 0;
  }
  return retval;
}

TF1 *f_pol4(char *funcn, double min, double max, bool skippeak)
{
  TF1 *retf = new TF1(funcn,&d_pol4,min,max,9);
  if (skippeak) {
    retf->FixParameter(5,0.74);
    retf->FixParameter(6,0.85);
  } else {
    retf->FixParameter(5,0);
    retf->FixParameter(6,0);
  }
  return retf;
}

Double_t d_pol4gaus(Double_t *x, Double_t *par)
{
  double retval = (par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0] + par[5]*exp(-0.5*((x[0]-par[6])/par[7])*((x[0]-par[6])/par[7])))*stepfactor(x[0],par[8],par[9]);
  if ( retval<=0 ) { // || (x[0] > 0.5 && x[0] < 0.6) ) {
    //TF1::RejectPoint();
    return 0;
  }
  if (x[0] > 0.5 && x[0] < 0.6) {
    TF1::RejectPoint();
  }
  return retval;
}

TF1 *f_pol4gaus(char *funcn, double min, double max, TF1 *fpol4, TF1 *fgaus)
{
  TF1 *retf = new TF1(funcn,&d_pol4gaus,min,max,10);
  if (fpol4) {
    for (int i = 0; i < 5; i++) retf->SetParameter(i,fpol4->GetParameter(i));
  }
  if (fgaus) {
    for (int i = 5; i < 8; i++) retf->SetParameter(i,fgaus->GetParameter(i-5));
  }
  return retf;
}

void fitmmp(TH1 *hmmp, float step_x0 = 2, float step_x1 = 2.1, float wval = 0, bool verbose = false) {
  float fnrangemax = 1.1;
  float mmplow = 0.4;
  //float bgp2 = 2.1e8;
  float gsig = -0.0065+0.013*wval;
  float pol2cutoff = 4; //wval under which to use pol2
  float fitcutoff = 1.1;
  TString options;
  if (verbose) options = "";
  else options = "Q";
  TF1 *fbg = f_pol4("fbg",0.4,fnrangemax,true);
  fbg->FixParameter(7,step_x0);
  fbg->FixParameter(8,step_x1);
  if (wval<pol2cutoff) {
    fitcutoff = 1.1;
    fbg->FixParameter(3,0);
    fbg->FixParameter(4,0);
  } else fitcutoff = 2;
  hmmp->Fit(fbg,options.Data(),"",mmplow,fitcutoff);
  if (true) { //gMinuit->fCstatu.Contains("CONVER")) {
    TF1 *fbg2 = f_pol4("fbg",0.4,fnrangemax,false);
    fbg2->SetParameters(fbg->GetParameters());
    fbg2->FixParameter(7,step_x0);
    fbg2->FixParameter(8,step_x1);
    if (wval<pol2cutoff) {
      fbg2->FixParameter(3,0);
      fbg2->FixParameter(4,0);
    }
    fbg2->SetParameter(5,0);
    fbg2->SetParameter(6,0);
    TH1 *htmp = (TH1*)hmmp->Clone("hmmp_bgsub");
    htmp->Add(fbg2,-1);
    TF1 *fgaus = new TF1("fgaus","gaus",0.4,fnrangemax);
    fgaus->FixParameter(2,gsig);
    fgaus->FixParameter(1,0.783);
    float gmax = 0.85;
    if (gmax > step_x0) gmax = step_x0;
    htmp->Fit(fgaus,options.Data(),"",0.74,gmax);
    TF1 *f = f_pol4gaus("f",0.4,fnrangemax,fbg2,fgaus);
    f->FixParameter(8,step_x0);
    f->FixParameter(9,step_x1);
    if (wval<pol2cutoff) {
      f->FixParameter(3,0);
      f->FixParameter(4,0);
    }
    f->SetNpx(500);
    f->FixParameter(6,0.783);
    f->FixParameter(7,gsig);
    hmmp->Fit(f,options.Data(),"",mmplow,fitcutoff);

    //f->FixParameter(2,0.9*f->GetParameter(2));
    //hmmp->Fit(f,options.Data(),"",mmplow,fitcutoff);

    fgaus->SetRange(0.4,fnrangemax);
    for (int i = 0; i < 3; i++) fgaus->SetParameter(i,f->GetParameter(i+5));
    for (int i = 0; i < 5; i++) fbg2->SetParameter(i,f->GetParameter(i));
    fbg2->SetLineStyle(2);
    fbg2->SetLineColor(kRed+1);
    fgaus->SetLineStyle(2);
    fgaus->SetLineColor(kGreen+1);
    hmmp->GetListOfFunctions()->Add(fbg2->Clone());
    hmmp->GetListOfFunctions()->Add(fgaus->Clone());
    float ymax = 1.1*hmmp->GetMaximum();
    hmmp->GetYaxis()->SetRangeUser(0,ymax);
    TLine *l1 = new TLine(step_x0,0,step_x0,ymax);
    TLine *l2 = new TLine(step_x1,0,step_x1,ymax);
    l1->SetLineColor(kBlue+1);
    l2->SetLineColor(kBlue+1);
    hmmp->GetListOfFunctions()->Add(l1);
    hmmp->GetListOfFunctions()->Add(l2);
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

void fit(float bgpar2smudge=1.0) {
  TF1 *fsig = new TF1("fsig",&d_sig,0.4,2,5);
  FILE *ofile;
  ofile = fopen("xsect-integrated-me.txt","w");
  TFile *_file0 = TFile::Open("h3maker-hn.root","update");
  _file0->Delete("*_f;*");
  TH2 *h2xsect = new TH2("hq2wXsect","Q^2:W",32,1.6,3.2,7,1.5,5.1);
  Double_t qbinedges[] = { 1.5, 1.6, 1.8, 2.1, 2.4, 2.76, 3.3, 5.1 };
  h2xsect->GetYaxis()->Set(7,qbinedges);
  TH3 *h3 = (TH3*)_file0->Get("hq2wmmp");
  int qbins = h3->GetZaxis()->GetNbins();
  //int wbins = h3->GetYaxis()->GetNbins();
  fprintf(ofile, "W\tQ2\txsect\terror\tpol4p0\tpol4p1\tpol4p2\tpol4p3\tpol4p4\tgN\tgM\tgS\tstepx0\tstepx1\txsectFn\n");
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
      delete [] wq;
      int wbin = h3->GetYaxis()->FindBin(wval);
      float wlow = h3->GetYaxis()->GetBinLowEdge(wbin);
      float step_x0 = sqrt(wlow*wlow+MASS_P*MASS_P-2*wlow*MASS_P);
      float whigh = h3->GetYaxis()->GetBinLowEdge(wbin+1);
      float step_x1 = sqrt(whigh*whigh+MASS_P*MASS_P-2*whigh*MASS_P);
      fitmmp(h,step_x0,step_x1,wval);
      TH1 *htmp = (TH1*)h->Clone("hbgsubtracted");
      TF1 *fbg = (TF1*)h->GetListOfFunctions()->FindObject("fbg");
      htmp->Add(fbg,-1);
      double N = htmp->Integral(34,43);
      double qwidth = h3->GetZaxis()->GetBinWidth(iq+1);
      //int wbin = h3->GetYaxis()->FindBin(wval);
      double wwidth = h3->GetYaxis()->GetBinWidth(wbin);

      TF1 *ftmp = (TF1*)h->GetListOfFunctions()->At(0);
      fsig->SetParameter(0,ftmp->GetParameter(5));
      fsig->SetParameter(1,ftmp->GetParameter(6));
      fsig->SetParameter(2,ftmp->GetParameter(7));
      fsig->SetParameter(3,step_x0);
      fsig->SetParameter(4,step_x1);
      fsig->SetLineWidth(2);
      fsig->SetLineColor(kBlue+1);
      h->GetListOfFunctions()->Add((TF1*)fsig->Clone("fsig"));
      //fsig->Print();
      double Nfn = 0;
      for (int b = 1; b < h->GetNbinsX(); b++) {
        double x = h->GetXaxis()->GetBinCenter(b);
        Nfn += fsig->Eval(x);
      }
      //printf("**** %.3e\t\%.3e\n",Nfn,N);
      double xsect = N/(0.891*wwidth*qwidth*19.844);
      double xsectFn = Nfn/(0.891*wwidth*qwidth*19.844);
      double err2 = 0;
      for (int immp = 34; immp < 44; immp++) err2 += htmp->GetBinError(immp)*htmp->GetBinError(immp);
      //fprintf(ofile, "%.3f\t%.3f\t%.0f\t%.0f",wval,qval,xsect/(1e6), sqrt(err2)/(1e6));
      fprintf(ofile, "%.3f\t%.3f\t%.3e\t%.3e",wval,qval,xsect/(1e6), sqrt(err2)/(1e6));
      int npar = ftmp->GetNpar();
      for (int ipar = 0; ipar < npar; ipar++) fprintf(ofile, "\t%.3e", ftmp->GetParameter(ipar));
      fprintf(ofile,"\t%.3e",xsectFn/(1e6));
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
  delete [] wq;
  printf("%.3e, %.3e\n",wval,qval);
}
