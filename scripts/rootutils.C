#include <TROOT.h>
#include <TMath.h>
#include <TF1.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TVirtualPad.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>

/* *************** sector histogram tools ******************** */
void pophists(const char* tmpl, TH1** hists, char* bn = NULL, TDirectory *din = gDirectory) {
  //if (!bn) bn = (char*)tmpl;
  for (int i = 0; i < 6; i++) {
    TString hn = TString::Format(tmpl,i+1);
    hists[i] = (TH1*)din->Get(hn.Data());
    if (hists[i]) {
      if (bn) {
	TString bnstr = bn;
	bnstr.Append(TString::Format("_s%d",i+1));
	hists[i]->SetName(bnstr.Data());
      }
      hists[i]->SetDirectory(gDirectory);
      hists[i]->Print();
    } else {
      printf("Could not find %s!!!\n",hn.Data());
    }
  }
}

void projs(TH3** hsin, TH1** hsout, const char* dims, char* dids = NULL) {
  for (int i = 0; i < 6; i++) {
    hsout[i] = hsin[i]->Project3D(dims);
    if (dids) {
      TString hn = hsin[i]->GetName();
      hn.Append("_").Append(dids);
      hsout[i]->SetName(hn.Data());
    }
    hsout[i]->Print();
  }
}

THStack *stack(TH1** hists, char *hsn = "histstack", char *htn = "stack o hists") {
  int goodcolors[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  THStack *hs = new THStack(hsn,htn);
  for (int i = 0; i < 6; i++) {
    hists[i]->SetLineColor(goodcolors[i]);
    if (hists[i]->InheritsFrom("TH2")) hists[i]->SetOption("colz");
    hs->Add(hists[i]);
  }
  return hs;
}

/* ************** 2d tools *********************************** */
TH2* normXslices(TH2 *h) {
  TH2D *h2 = (TH2D*)h->Clone();
  Int_t nby = h2->GetYaxis()->GetNbins();
  for (Int_t bx = 1; bx <= h2->GetXaxis()->GetNbins(); bx++) {
    h2->GetXaxis()->SetRange(bx,bx);
    TH1 *h1 = h2->ProjectionY();
    Double_t max = h1->GetMaximum();
    for (Int_t by = 1; by <= nby; by++) {
      Double_t bc = h2->GetBinContent(bx,by);
      if (bc > 0) {
	Int_t global_bin = h2->GetBin(bx,by);
	h2->SetBinContent(global_bin,bc/max);
	h2->SetBinError(global_bin, bc/max*TMath::Sqrt(bc/TMath::Power(bc,2)+max/TMath::Power(max,2)));
      }
    }
  }
  h2->GetXaxis()->SetRange(0,h2->GetXaxis()->GetNbins()+1);
  h2->SetMaximum(1);
  return h2;
}
TH2* normYslices(TH2 *h) {
  TH2D *h2 = (TH2D*)h->Clone();
  Int_t nbx = h2->GetXaxis()->GetNbins();
  for (Int_t by = 1; by <= h2->GetYaxis()->GetNbins(); by++) {
    h2->GetYaxis()->SetRange(by,by);
    TH1 *h1 = h2->ProjectionX();
    Double_t max = h1->GetMaximum();
    for (Int_t bx = 1; bx <= nbx; bx++) {
      Double_t bc = h2->GetBinContent(bx,by);
      if (bc > 0) {
	Int_t global_bin = h2->GetBin(bx,by);
	h2->SetBinContent(global_bin,bc/max);
      }
    }
  }
  h2->GetYaxis()->SetRange(0,h2->GetYaxis()->GetNbins()+1);
  h2->SetMaximum(1);
  return h2;
}

/* *********** multi-pad canvases ******************** */
void setLogy(TCanvas *c, bool on = true) {
    c->Modified();
    c->Update();
    //gSystem->ProcessEvents();
    TObject *obj;
    TIter    next(c->GetListOfPrimitives());
    while ((obj = next())) {
      if (obj->InheritsFrom(TVirtualPad::Class())) {
	TVirtualPad *pad = (TVirtualPad*)obj;
	pad->Modified();
	pad->Update();
	pad->SetLogy(on);
	pad->Modified();
	pad->Update();
      }
    }
}

void setLogz(TCanvas *c, bool on = true) {
    c->Modified();
    c->Update();
    //gSystem->ProcessEvents();
    TObject *obj;
    TIter    next(c->GetListOfPrimitives());
    while ((obj = next())) {
      if (obj->InheritsFrom(TVirtualPad::Class())) {
	TVirtualPad *pad = (TVirtualPad*)obj;
	pad->Modified();
	pad->Update();
	pad->SetLogz(on);
	pad->Modified();
	pad->Update();
      }
    }
}

/* *************** general purpose math functions ******************* */
Double_t d_betaf(Double_t *x, Double_t *par)
{
  Double_t start = par[0];
  Double_t scale = par[1]-par[0];
  if (x[0] > par[5] && x[0] < par[6]) {
    //TF1::RejectPoint();
    return 0;
  }
  if (x[0]<=start || x[0]>=par[1]) return 0;
  else {
    return par[4]*TMath::BetaDist((x[0]/scale)-start,par[2],par[3]);
    //return par[4]*ROOT::Math::beta_pdf((x[0]/scale)-start,par[2],par[3]);
    //return par[4]*TMath::BetaIncomplete((x[0]/scale)-start,par[2],par[3]);
  }
}

TF1 *f_betaf(char *funcn, double min, double max, bool skippeak = true)
{
  TF1 *retf = new TF1(funcn,&d_betaf,min,max,7);
  if (skippeak) {
    retf->FixParameter(5,0.74);
    retf->FixParameter(6,0.85);
  } else {
    retf->FixParameter(5,0);
    retf->FixParameter(6,0);
  }
  return retf;
}

Double_t d_pol4(Double_t *x, Double_t *par)
{
  if ( par[5] < par[6] ) {
    if ( (x[0] > par[5] && x[0] < par[6]) || (x[0] > 0.5 && x[0] < 0.6) ) {
      TF1::RejectPoint();
      //return 0;
    }
  }
  double retval = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0];
  if (retval<0) {
    //TF1::RejectPoint();
    return 0;
  }
  return retval;
}

TF1 *f_pol4(char *funcn, double min, double max, bool skippeak)
{
  TF1 *retf = new TF1(funcn,&d_pol4,min,max,7);
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
  double retval = par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0] + par[5]*exp(-0.5*((x[0]-par[6])/par[7])*((x[0]-par[6])/par[7]));
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
  TF1 *retf = new TF1(funcn,&d_pol4gaus,min,max,8);
  if (fpol4) {
    for (int i = 0; i < 5; i++) retf->SetParameter(i,fpol4->GetParameter(i));
  }
  if (fgaus) {
    for (int i = 5; i < 8; i++) retf->SetParameter(i,fgaus->GetParameter(i-5));
  }
  return retf;
}
