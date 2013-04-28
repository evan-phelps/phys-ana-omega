#ifndef STUDYFID_H
#define STUDYFID_H
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <TMinuit.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TString.h>
#include <THStack.h>
#include <TLine.h>
#include <TKey.h>

using namespace std;

extern TMinuit *gMinuit;

/* ************** 2d tools *********************************** */
TH2* normXslices(TH2 *h) {
  TH2D *h2 = (TH2D*)h->Clone();
  Int_t nby = h2->GetYaxis()->GetNbins();
  for (Int_t bx = 1; bx < h2->GetXaxis()->GetNbins(); bx++) {
    h2->GetXaxis()->SetRange(bx,bx);
    TH1 *h1 = h2->ProjectionY();
    Double_t max = h1->GetMaximum();
    for (Int_t by = 1; by <= nby; by++) {
      Double_t bc = h2->GetBinContent(bx,by);
      if (bc > 0) {
	Int_t global_bin = h2->GetBin(bx,by);
	h2->SetBinContent(global_bin,bc/max);
      }
    }
  }
  h2->GetXaxis()->SetRange(0,h2->GetXaxis()->GetNbins()+1);
  return h2;
}

class StudyFid
{
 public:
  static const int NSECT = 6;
  static const int STAT_THRESH_PHI = 100;
  static const int MAX_PARMS = 20;
  static const char* BN_DEF;
  static const char* HNTMPL_DEF;
  TString fBaseName;
  float fP0, fP1;
  vector<float> pbinvals;
  vector<TH3*> fH3;
  vector< vector<TCanvas*> > fC;
  vector< vector<TH2*> > fH2;
  vector<TH2*> fHtVps;
  vector< vector<TH1*> > fHt;
  vector< vector< vector<TGraph*> > > fH1parms;
  vector< vector< vector<TH1*> > > fH1;
  StudyFid(const char* basename = BN_DEF) {
    fBaseName = basename;
    fP0 = 0;
    fP1 = -1;
  }
  virtual ~StudyFid() {
    //printf("CLEANUP!!\n");
    for(vector<int>::size_type i = 0; i < fH3.size(); i++) {
      //printf("fH3[%ld], %s\n",i,fH3[i]->GetName());
      delete fH3[i];
      //printf("fHtVps[%ld], %s\n",i,fHtVps[i]->GetName());
      delete fHtVps[i];
      for(vector<int>::size_type j = 0; j < fH2[i].size(); j++) {
	//printf("fH2[%ld][%ld], %s\n",i,j,fH2[i][j]->GetName());
	delete fH2[i][j];
	//printf("fHt[%ld][%ld], %s\n",i,j,fHt[i][j]->GetName());
	delete fHt[i][j];
	for(vector<int>::size_type k = 0; k < fH1[i][j].size(); k++) {
	  //printf("fH1[%ld][%ld][%ld], %s\n",i,j,k,fH1[i][j][k]->GetName());
	  delete fH1[i][j][k];
	}
	for(vector<int>::size_type l = 0; l < fH1parms[i][j].size(); l++) {
	  //printf("fH1parms[%ld][%ld][%ld], %s\n",i,j,l,fH1parms[i][j][l]->GetName());
	  delete fH1parms[i][j][l];
	}
      }
    }
  }
  void PrintParms();
  static TCanvas* DrawH2ang(int sector, float pval, const char *bn, TDirectory *dexp, TDirectory *dsim, const char *pname);
  void Save(TDirectory *dir);
  void Load(TDirectory *dir, const char* bn);
  void Draw();
  void PopH3s(TDirectory *dir, const char* hntmpl = HNTMPL_DEF,
	      char *hbasename = "h3p");
  void PopH2s(float pbsize = 0.200, float pstart = 0, float pstop = 4, bool smooth = false);
  void PopH1s();
  void FitH1s(TF1 *fitfunc = NULL, int ntries = 1);
  void FitH2s(TF1 *fitfunc = NULL, int ntries = 1);
  void PopHts();
  void PopHtVps(float philow = -1, float phihigh = 1);
  static Double_t dTrap(Double_t *x, Double_t *par);
  static double xvalthresh(TH1 *h1, double thresh = 0.6, bool l2r = true) {
    int nbins = h1->GetNbinsX();
    double max = h1->GetMaximum();
    double retval = 0;
    int bin = 0;
    if (l2r) {
      for (bin = 1; bin < nbins; bin++) {
	if (h1->GetBinContent(bin) > max*thresh) break;
      }
    } else {
      for (bin = nbins; bin > 0; bin--) {
	if (h1->GetBinContent(bin) > max*thresh) break;
      }
    }
    retval = h1->GetBinCenter(bin);
    return retval;
  }
  static bool fitphi(TH1 *h1, TF1 *fitfunc, int ntries = 1) {
    bool converged = false;
    while (--ntries >= 0) {
      h1->Fit(fitfunc,"Q","goff");
      if (gMinuit && gMinuit->fCstatu == "CONVERGED ") {
	converged = true;
	break;
      }
    }
    return converged;
  }
  static void fitedge(TGraphErrors *g, TF1 *fitfunc, int ntries = 1) {
    while (--ntries >= 0) {
      g->Fit(fitfunc,"Q0","goff");
      if (gMinuit && gMinuit->fCstatu == "CONVERGED ") break;
    }
  }
  static TH2* projang(TH3 *h3) {
    TH2 *h2 = (TH2*)h3->Project3D("xy");
    return h2;
  }
  static TH2* projtvp(TH3 *h3) {
    TH2 *h2 = (TH2*)h3->Project3D("yz");
    return h2;
  }
  static TH1* projtheta(TH2 *h2) {
    float dt = 2.5;
    int b0 = h2->GetYaxis()->GetBinUpEdge(dt);
    int b1 = h2->GetYaxis()->GetBinLowEdge(-dt);
    TString hn = h2->GetName();
    hn.Append("_pT");
    return h2->ProjectionX(hn.Data(),b0,b1);
  }
  static vector<TH1*> projphi(TH2 *h2) {
    vector<TH1*> v;
    int max = h2->GetNbinsX();
    for (int bin = 1; bin < max; bin++) {
      TH1D *h1 = h2->ProjectionY("_py",bin,bin);
      if (h1->Integral(2,h1->GetNbinsX()-1) < STAT_THRESH_PHI) {
	delete h1;
      } else {
	TString hn = h1->GetName();
	float tval = h2->GetXaxis()->GetBinCenter(bin);
	hn += bin;
	TString ht = TString::Format("#phi for #theta = %.3f",tval);
	h1->SetName(hn.Data());
	h1->SetTitle(ht.Data());
	v.push_back(h1);
      }
    }
    return v;
  }
  static THStack* stack(vector<TH1*> v) {
    THStack *hs = new THStack();
    for(vector<TH1*>::size_type i = 0; i < v.size(); i++) {
      hs->Add(v[i]);
    }
    return hs;
  }
};
#endif // STUDYFID_H

const char* StudyFid::BN_DEF = "exp_proton";
const char* StudyFid::HNTMPL_DEF = "h3p_pVthetaVphi_s%d";

void StudyFid::PopH3s(TDirectory *dir, const char* hntmpl /*= HNTMPL_DEF */, char *hbasename /* = "h3p" */)
{
  for (int i = 0; i < NSECT; i++) {
    TString hn = TString::Format(hntmpl,i+1);
    TString bn = hbasename;
    bn += i+1;
    printf("getting %s...",hn.Data());
    TH3 *h3 = (TH3*)dir->Get(hn.Data());
    h3->SetName(bn.Data());
    printf(" %s\n",h3->GetName());
    fH3.push_back(h3);
  }
}

void StudyFid::PopH2s(float pbsize /* = 0.200 */, float pstart /* = 0 */, float pstop /* = 4 */, bool smooth /* = false */)
{
  fH2.resize(fH3.size());
  for(vector<int>::size_type i = 0; i < fH2.size(); i++) {
    float bs = fH3[i]->GetZaxis()->GetBinWidth(1);
    int nbins = pbsize/bs;
    pbsize = nbins*bs;
    if (nbins==0) nbins = 1;
    int pbstart = fH3[i]->GetZaxis()->FindBin(pstart);
    int pbstop = fH3[i]->GetZaxis()->FindBin(pstop);
    for(int pbin = pbstart; pbin < pbstop; pbin+=nbins) {
      float pval = fH3[i]->GetZaxis()->GetBinLowEdge(pbin)+pbsize/2;
      pbinvals.push_back(pval);
      fH3[i]->GetZaxis()->SetRange(pbin,pbin+nbins-1);
      TH2 *h2 = (TH2*)fH3[i]->Project3D("xy");
      if (smooth) h2->Smooth();
      TString hn = TString::Format("%s_%d",h2->GetName(),(int)(pval*1000));
      TString ht = TString::Format("%.3f GeV",pval);
      h2->SetName(hn.Data());
      h2->SetTitle(ht.Data());
      fH2[i].push_back(h2);
    }
    fH3[i]->GetZaxis()->SetRange(0,-1);
  }
}

void StudyFid::PopHtVps(float philow /* = -1 */, float phihigh /* = 1 */)
{
  fHtVps.resize(fH3.size());
  for(vector<int>::size_type i = 0; i < fH3.size(); i++) fH3[i]->GetXaxis()->SetRangeUser(philow,phihigh);
  transform(fH3.begin(),fH3.end(),fHtVps.begin(),projtvp);
  for(vector<int>::size_type i = 0; i < fH3.size(); i++) fH3[i]->GetXaxis()->SetRangeUser(0,-1);
}

void StudyFid::PopH1s()
{
  fH1.resize(fH2.size());
  for(vector<int>::size_type i = 0; i < fH2.size(); i++) {
    fH1[i].resize(fH2[i].size());
    //for(vector<int>::size_type j = 0; j < fH2[i].size(); j++) {
    transform(fH2[i].begin(),fH2[i].end(),fH1[i].begin(),projphi);
    //}
  }
}

void StudyFid::FitH1s(TF1 *fitfunc /* = NULL */, int ntries /* = 1*/)
{
  bool usedefaults = (fitfunc == NULL);
  if (usedefaults) fitfunc = new TF1("fitfunc",&StudyFid::dTrap,-30,30,6);
  fH1parms.resize(fH1.size());
  for(vector<int>::size_type i = 0; i < fH1.size(); i++) {
    fH1parms[i].resize(fH1[i].size());
    for(vector<int>::size_type j = 0; j < fH1[i].size(); j++) {
      vector<TGraph*> vfitgraphs;
      TGraph *gchi2 = new TGraph();
      vfitgraphs.push_back(gchi2);
      for (int par = 0; par < fitfunc->GetNpar(); par++) {
	TGraph *g = new TGraphErrors();
	g->SetMarkerStyle(20);
	TString gn = fitfunc->GetParName(par);
	gn += i+1;
	g->SetName(gn.Data());
	vfitgraphs.push_back(g);
      }
      for (vector<int>::size_type k = 0; k < fH1[i][j].size(); k++) {
	if (usedefaults) {
	  double h0 = fH1[i][j][k]->GetMaximum();
	  double edgeest = xvalthresh(fH1[i][j][k])+2;
	  double alim0 = edgeest-5;
	  double alim1 = edgeest+5;
	  if (alim0<-29) alim0 = -29;
	  if (alim1>-1) alim1 = -1;
	  fitfunc->SetParameters(edgeest,-edgeest,0.5,0.5,h0*0.9,0);
	  fitfunc->SetParLimits(4,h0/3.0,h0);
	  fitfunc->SetParLimits(2,0.1,0.9);
	  fitfunc->SetParLimits(3,0.1,0.9);
	  fitfunc->SetParLimits(0,alim0,alim1);
	  fitfunc->SetParLimits(1,-alim1,-alim0);
	  fitfunc->SetParLimits(5,0,0.1*h0);
	}
	bool success = fitphi(fH1[i][j][k],fitfunc,ntries);
	double thetaval = 0;
	const char* hphinameC = fH1[i][j][k]->GetTitle();
	char hphiname[256];
	strcpy(hphiname,hphinameC);
	int i1, i2;
	i2=0;
	for (i1=0; i1 < strlen(hphiname); i1++) {
	  if (isdigit(hphiname[i1]) || hphiname[i1] == '.') hphiname[i2++] = hphiname[i1];
	}
	hphiname[i2] = '\0';
	sscanf(hphiname,"%lf",&thetaval);;
	double chi2 = fitfunc->GetChisquare()/fitfunc->GetNDF();
	vfitgraphs[0]->SetPoint(k,thetaval,chi2);
	for (int par = 0; par < fitfunc->GetNpar(); par++) {
	  int gidx = par+1;
	  double *fitpars = fitfunc->GetParameters();
	  double *fiterrs = fitfunc->GetParErrors();
	  vfitgraphs[gidx]->SetPoint(k,thetaval,fitpars[par]);
	  double err = fiterrs[par]<0.75 ? 0.75 : fiterrs[par];
	  //if (!success) err = 10;
	  //double err = 1;
	  ((TGraphErrors*)vfitgraphs[gidx])->SetPointError(k,0,err);
	}
      }
      fH1parms[i][j] = vfitgraphs;
    }
  }
}
void StudyFid::FitH2s(TF1 *fitfunc /* = NULL */, int ntries /* = 1*/)
{
  bool usedefaults = (fitfunc == NULL);
  if (usedefaults) fitfunc = new TF1("fitfunc","[0]*(1-exp(-[1]*(x-[2])))",0,120);
  for(vector<TH2*>::size_type i = 0; i < fH1parms.size(); i++) {
    for(vector<TH1*>::size_type j = 0; j < fH1parms[i].size(); j++) {
      float t0 = xvalthresh(fHt[i][j],0.3);
      float t1 = 50;
      if (usedefaults) {
	fitfunc->SetParLimits(0,-29,-20);
	fitfunc->SetParLimits(1,0.08,0.5);
	fitfunc->SetParLimits(2,4,30);
	fitfunc->SetParameters(-24,0.1,5);
      }
      fH1parms[i][j][1]->Fit(fitfunc,"Q0","",t0,t1);
      fH1parms[i][j][1]->GetListOfFunctions()->Add(fitfunc->Clone());
      if (usedefaults) {
	fitfunc->SetParLimits(0,20,29);
	fitfunc->SetParLimits(1,0.08,0.5);
	fitfunc->SetParLimits(2,4,30);
	fitfunc->SetParameters(24,0.1,5);
      }
      fH1parms[i][j][2]->Fit(fitfunc,"Q0","",t0,t1);
      fH1parms[i][j][2]->GetListOfFunctions()->Add(fitfunc->Clone());
      TLine *lt0 = new TLine(t0,-30,t0,30);
      lt0->SetLineWidth(2);
      fH2[i][j]->GetListOfFunctions()->Add(lt0);
    }
  }
}

void StudyFid::PopHts()
{
  fHt.resize(fH2.size());
  for(vector<TH2*>::size_type i = 0; i < fH2.size(); i++) {
    fHt[i].resize(fH2[i].size());
    transform(fH2[i].begin(),fH2[i].end(),fHt[i].begin(),projtheta);
  }
}

Double_t StudyFid::dTrap(Double_t *x, Double_t *par)
{
  double phi = x[0];
  double fitval = 0;
  double b = par[0];
  double c = par[1];
  double a = -30+(b+30)*par[2];
  double d = 30-(30-c)*par[3];
  double h = par[4];
  double y0 = par[5];
  if (phi < a || phi > d) fitval = y0;
  else if (phi >= a && phi < b) fitval = y0+h*(phi-a)/(b-a);
  else if (phi >= b && phi <= c) fitval = y0+h;
  else if (phi > c && phi <= d) fitval = y0+h*(phi-d)/(c-d);
  return fitval;
}

void StudyFid::Save(TDirectory *dir)
{
  TDirectory *dtop = dir->mkdir(fBaseName.Data());
  for(vector<int>::size_type i = 0; i < fH3.size(); i++) {
    fH3[i]->SetDirectory(dir);
    TString dn = TString::Format("s%d",(int)i+1);
    TDirectory *dsec = dtop->mkdir(dn.Data());
    fHtVps[i]->SetDirectory(dsec);
    for(vector<int>::size_type j = 0; j < fH2[i].size(); j++) {
      fH2[i][j]->SetDirectory(dsec);
      //fH2[i][j]->GetListOfFunctions()->Add(fH1parms[i][j][1]);
      //fH2[i][j]->GetListOfFunctions()->Add(fH1parms[i][j][2]);
      dn = TString::Format("s%d",(int)i+1);
      TDirectory *dmom = dsec->mkdir(fH2[i][j]->GetTitle());
      fHt[i][j]->SetDirectory(dmom);
      for(vector<int>::size_type k = 0; k < fH1[i][j].size(); k++) {
	fH1[i][j][k]->SetDirectory(dmom);
      }
      for(vector<int>::size_type k = 0; k < fH1parms[i][j].size(); k++) {
	dmom->WriteObject(fH1parms[i][j][k],fH1parms[i][j][k]->GetName());
      }
    }
  }
  dtop->Write();
}

void StudyFid::Load(TDirectory *dir, const char* bn)
{
  fBaseName = dir->GetName();
  //TDirectory *dtop = dir->GetDirectory(fBaseName.Data());
  TDirectory *dtop = dir;
  dtop->ls();
  fH2.resize(NSECT);
  fH3.resize(NSECT);
  fH1.resize(NSECT);
  fHt.resize(NSECT);
  fHtVps.resize(NSECT);
  fH1parms.resize(NSECT);
  for(int s = 1; s <= NSECT; s++) {
    TString sdirname = TString::Format("s%d",s);
    printf("%s\n",sdirname.Data());
    TDirectory *sdir = dtop->GetDirectory(sdirname.Data());
    sdir->ls();
    TList *sobs = sdir->GetListOfKeys();
    TIter next(sobs);
    int pbin = 0;
    fH1[s-1].resize(1);
    fH1parms[s-1].resize(1);
    while (TKey *key = (TKey*)next()) {
      TObject *o = key->ReadObj();
      if(o->IsFolder()) {
	TDirectory *pdir = (TDirectory*)o;
	pdir->ls();
	if (fH1[s-1].size() < pbin+1) {
	  fH1[s-1].resize(pbin+1);
	  fH1parms[s-1].resize(pbin+1);
	}
	float pval;
	sscanf(o->GetName(),"%f%*s",&pval);
	printf("p = %.3f\n",pval);
	pbinvals.push_back(pval);
	TString hnang = TString::Format("%s%d_xy_%d",bn,s,(int)(pval*1000));
	printf("hnang = %s\n",hnang.Data());
	fH2[s-1].push_back((TH2*)sdir->Get(hnang.Data()));
	TList *pobs = pdir->GetListOfKeys();
	TIter nextpob(pobs);
	while (TKey *key = (TKey*)nextpob()) {
	  TObject *pob = key->ReadObj();
	  TString str = pob->GetName();
	  printf("+%s\t",str.Data());
	  TString pat = TString::Format("%s%d_xy_%d_py",bn,s,(int)(pval*1000));
	  printf("*%s\t",pat.Data());
	  TSubString ss = str.SubString(pat);
	  if (!ss.IsNull()) {
	    fH1[s-1][pbin].push_back((TH1*)pob);
	    printf("yes\n");
	  } else printf("no\n");
	}
	fH1parms[s-1][pbin].push_back((TGraph*)pdir->Get("TGraph"));
	printf("max number of parameters = %d\n",MAX_PARMS);
	for (int npar = 0; npar < MAX_PARMS; npar++) {
	  printf("p%d\t",npar);
	  TString gpn = TString::Format("p%d%d",npar,s);
	  printf("%s\n",gpn.Data());
	  TGraphErrors *g = (TGraphErrors*)pdir->Get(gpn.Data());
	  if (g == NULL) {
	    printf("%d parameters\n",npar);
	    npar = MAX_PARMS;
	    break;
	  } else fH1parms[s-1][pbin].push_back(g);
	}
	TString hn = TString::Format("%s%d_xy_%d_pT",bn,s,(int)(pval*1000));
	printf("%s\n",hn.Data());
	fHt[s-1].push_back((TH1*)pdir->Get(hn.Data()));
	printf("done with pbin = %d\n",pbin);
	pbin++;
      }
    }
    TString hntvp = TString::Format("%s%d_yz",bn,s);
    printf("*** %s\n",hntvp.Data());
    fHtVps.push_back((TH2*)sdir->Get(hntvp.Data()));
  }
}

void StudyFid::Draw()
{
  fC.resize(fH2.size());
  for (vector<int>::size_type isect = 0; isect < fH2.size(); isect++) {
    for (vector<int>::size_type p = 0; p < fH2[isect].size(); p++) {
      TString cn = TString::Format("c_%ld_%ld",isect+1,p);
      TCanvas *c = new TCanvas(cn.Data(),cn.Data());
      printf("%s\n",cn.Data());
      fH2[isect][p]->Draw("colz");
      fH1parms[isect][p][1]->Draw("same*e");
      fH1parms[isect][p][2]->Draw("same*e");
      fC[isect].push_back(c);
    }
  }
}

TCanvas* StudyFid::DrawH2ang(int sector, float pval, const char *bn,
			     TDirectory *dexp, TDirectory *dsim, const char *pname) {
  TString hn = TString::Format("s%d/%s%d_xy_%d",sector,bn,sector,(int)(pval*1000));
  TString gL = TString::Format("s%d/%.3f GeV/p0%d",sector,pval,sector);
  TString gU = TString::Format("s%d/%.3f GeV/p1%d",sector,pval,sector);
  TH2D *hexp = (TH2D*)dexp->Get(hn.Data());
  TGraphErrors *gexpL = (TGraphErrors*)dexp->Get(gL.Data());
  TGraphErrors *gexpU = (TGraphErrors*)dexp->Get(gU.Data());
  gexpL->SetName("gexpL");
  gexpU->SetName("gexpU");
  printf("%s\t%s\t%s",hn.Data(),gL.Data(),gU.Data());
  hexp->Print();
  gexpL->Print();
  gexpU->Print();
  TH2D *hsim = (TH2D*)dsim->Get(hn.Data());
  TGraphErrors *gsimL = (TGraphErrors*)dsim->Get(gL.Data());
  TGraphErrors *gsimU = (TGraphErrors*)dsim->Get(gU.Data());
  gsimL->SetName("gsimL");
  gsimU->SetName("gsimU");
  TGraphErrors *gs[] = { gexpL, gexpU, gsimL, gsimU };
  gexpL->SetMarkerColor(kRed);
  gexpU->SetMarkerColor(kRed);
  gexpU->SetLineColor(kRed);
  gexpL->SetLineColor(kRed);
  gsimL->SetMarkerColor(kBlack);
  gsimU->SetMarkerColor(kBlack);
  gsimL->SetLineColor(kBlack);
  gsimU->SetLineColor(kBlack);
  TF1* fsimL = (TF1*)gsimL->GetListOfFunctions()->FindObject("fitfunc");
  TF1* fsimU = (TF1*)gsimU->GetListOfFunctions()->FindObject("fitfunc");
  TF1* fexpL = (TF1*)gexpL->GetListOfFunctions()->FindObject("fitfunc");
  TF1* fexpU = (TF1*)gexpU->GetListOfFunctions()->FindObject("fitfunc");
  fsimL->SetName("fsimL");
  fsimU->SetName("fsimU");
  fexpL->SetName("fexpL");
  fexpU->SetName("fexpU");
  ((TF1*)gsimL->GetListOfFunctions()->FindObject("fitfunc"))->SetLineColor(kBlack);
  ((TF1*)gsimU->GetListOfFunctions()->FindObject("fitfunc"))->SetLineColor(kBlack);
  fsimL->SetLineColor(kBlack);
  fsimU->SetLineColor(kBlack);
  for (int ig = 0; ig < 4; ig++) {
    gs[ig]->SetMarkerSize(0.6);
    gs[ig]->SetMarkerStyle(3);
    //hexp->GetListOfFunctions()->Add(gs[ig]);
    //hsim->GetListOfFunctions()->Add(gs[ig]);
  }
  TH1 *hthetaexp = hexp->ProjectionX();
  TH1 *hthetasim = hsim->ProjectionX();
  double xmaxexp = xvalthresh(hthetaexp,0.001,false);
  double xmaxsim = xvalthresh(hthetasim,0.001,false);
  double xmax = xmaxsim > xmaxexp ? xmaxsim : xmaxexp;
  xmax += 8;
  hexp->GetXaxis()->SetTitle("#theta (degrees)");
  hsim->GetXaxis()->SetTitle("#theta (degrees)");
  hexp->GetYaxis()->SetTitle("#phi (degrees)");
  hsim->GetYaxis()->SetTitle("#phi (degrees)");
  hexp->GetXaxis()->SetRangeUser(0,xmax);
  hsim->GetXaxis()->SetRangeUser(0,xmax);
  TString cn = TString::Format("%s_s%d_p%d",bn,sector,(int)(pval*1000));
  TString ct = TString::Format("%s s%d p = %.3f",bn,sector,pval);

  TH2 *hexpN = normXslices(hexp);
  TH2 *hsimN = normXslices(hsim);
  hexpN->GetXaxis()->SetRangeUser(0,xmax);
  hsimN->GetXaxis()->SetRangeUser(0,xmax);

  TCanvas *c = new TCanvas(cn.Data(),ct.Data());
  //c->Divide(3);
  c->Divide(2,2);
  c->cd(1);
  /*  TVirtualPad *curpad = gPad;
  curpad->Divide(1,2);
  curpad->cd(1);*/
  TString htexp = TString::Format("%s, S%d, p = %s, EXP",pname,sector,hexp->GetTitle());
  hexp->SetTitle(htexp.Data());
  TLine *tl0exp = (TLine*)hexp->GetListOfFunctions()->At(0);
  tl0exp->SetLineColor(kRed);
  TLine *tl0sim = (TLine*)hsim->GetListOfFunctions()->At(0);
  tl0sim->SetLineColor(kBlack);
  ((TLine*)hexpN->GetListOfFunctions()->At(0))->SetLineColor(kRed);
  ((TLine*)hexp->GetListOfFunctions()->At(0))->SetLineWidth(2);
  ((TLine*)hexpN->GetListOfFunctions()->At(0))->SetLineWidth(2);
  ((TLine*)hsim->GetListOfFunctions()->At(0))->SetLineWidth(2);
  ((TLine*)hsimN->GetListOfFunctions()->At(0))->SetLineWidth(2);
  hexp->Draw("colz");
  tl0sim->Draw("same");
  tl0exp->Draw("same");
  gexpL->Draw("same*e");
  gexpU->Draw("same*e");
  gsimL->Draw("same*e");
  gsimU->Draw("same*e");
  //curpad->cd(2);
  c->cd(2);
  TString htsim = TString::Format("%s, S%d, p = %s, SIM",pname,sector,hsim->GetTitle());
  hsim->SetTitle(htsim.Data());
  hsim->Draw("colz");
  tl0sim->Draw("same");
  tl0exp->Draw("same");
  gsimL->Draw("same*e");
  gsimU->Draw("same*e");
  gexpL->Draw("same*e");
  gexpU->Draw("same*e");
  //c->cd(2);
  c->cd(3);
  /*curpad = gPad;
  curpad->Divide(1,2);
  curpad->cd(1);*/
  hexpN->Draw("colz");
  tl0sim->Draw("same");
  tl0exp->Draw("same");
  gexpL->Draw("same*e");
  gexpU->Draw("same*e");
  gsimL->Draw("same*e");
  gsimU->Draw("same*e");
  //curpad->cd(2);
  c->cd(4);
  hsimN->Draw("colz");
  tl0sim->Draw("same");
  tl0exp->Draw("same");
  gsimL->Draw("same*e");
  gsimU->Draw("same*e");
  gexpL->Draw("same*e");
  gexpU->Draw("same*e");
  /*c->cd(3);
  TH2 *heff = (TH2*)hexpN->Clone("heff");
  heff->GetXaxis()->SetRangeUser(0,xmax);
  heff->Divide(hsimN);
  heff->SetMaximum(2);
  heff->SetMinimum(0);
  //heff->Smooth(1);
  heff->Draw("colz");
  gsimL->Draw("same*e");
  gsimU->Draw("same*e");
  gexpL->Draw("same*e");
  gexpU->Draw("same*e");
  */
  return c;
}

void StudyFid::PrintParms()
{
  for (int isect = 0; isect < fH1parms.size(); isect++) {
    for (int imom = 0; imom < fH1parms[isect].size(); imom++) {
      TF1 *fhi = (TF1*)fH1parms[isect][imom][2]->GetListOfFunctions()->At(0);
      TF1 *flo = (TF1*)fH1parms[isect][imom][1]->GetListOfFunctions()->At(0);
      TLine *t0 = (TLine*)fH2[isect][imom]->GetListOfFunctions()->FindObject("TLine");
      printf("%s, %d, %.3f, %.2f",fBaseName.Data(),isect+1,pbinvals[imom],t0->GetX1());
      for (int ipar = 0; ipar < fhi->GetNpar(); ipar++) {
	char *delim = ", ";
	printf("%s%.3f",delim,fhi->GetParameter(ipar));
      }
      for (int ipar = 0; ipar < flo->GetNpar(); ipar++) {
	char *delim = ", ";
	printf("%s%.3f",delim,flo->GetParameter(ipar));
      }
      printf("\n");
    }
  }
}
