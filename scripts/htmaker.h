#include "h10t3pi_sel.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <THStack.h>
#include <TH2.h>
#include "particle-constants.h"
#include "scripts/xsect-utils.C"
#include "fid.h"
#include "scripts/simw.C"
#include "input/cc_eff_lazy_programmer.h"

using namespace TMath;
using namespace ParticleConstants;

class h3maker : public h10t3pi_sel {
 public:
  Fid *fid;
  TFile *fout;
  TH3 *h3[7];
  bool w8ed;

 h3maker() : h10t3pi_sel() {
    printf("in h3maker::h3maker()\n");
    w8ed = true;
  }

  ~h3maker() {
    printf("in h3maker::h3maker()\n");
    delete h3;
    delete fout;
  }

  void Setup() {
    printf("in h3maker::Setup()\n");
    TString option = GetOption();
    w8ed = !option.Contains("now8");
    fid = Fid::Instance();
    fout = new TFile("htmaker-hn.root","recreate");
    Double_t qbins[] = { 1.5, 1.6, 1.8, 2.1, 2.4, 2.76, 3.3, 5.1 };
    for (int i = 0; i < 4; i++) {
      h3 = new TH3F("hwtmmp","W:t:mmp",

    }
    h3->GetZaxis()->Set(7,qbins);
    h3->Sumw2();
  }
  void Process() {
    int segment = (cc_segm[cc[h10idx_e]-1]%1000)/10;
    int pmt_hit = (cc_segm[cc[h10idx_e]-1]/1000)-1;
    int sector = cc_sect[cc[h10idx_e]-1];

    double weight = 1;
    double invw = 1;
    float err = 0;
    float wacc = fid->acc->GetAcc(kin.W,kin.Q2,kin.ct,kin.phi,err);

    if (!w8ed) h3->Fill(kin.mmp,kin.W,kin.Q2);
    else {
      invw = kin.vgflux*ccw8(sector,segment,pmt_hit)*wacc;
      if (invw > 0) {
	weight = 1/invw;
	int ibin = h3->FindBin(kin.mmp,kin.W,kin.Q2);
      
	double binerror = h3->GetBinError(ibin);
	h3->Fill(kin.mmp,kin.W,kin.Q2,weight);

	float err2 = binerror*binerror + weight*weight + err*err/(wacc*wacc*wacc*wacc);

	h3->SetBinError(ibin,sqrt(err2));
      }
    }
  }
  void Finalize() {
    printf("in h3maker::Finalize()\n");
    h3->Write();
    TAxis *wax = h3->GetYaxis();
    TAxis *q2ax = h3->GetZaxis();
    int wbinof = wax->GetNbins()+1;
    int q2binof = q2ax->GetNbins()+1;
    for (int q2bin = 1; q2bin < q2binof; q2bin++) {
      hs[q2bin-1] = new THStack();
      for (int wbin = 1; wbin < wbinof; wbin++) {
	float wvals[] = { wax->GetBinCenter(wbin), wax->GetBinLowEdge(wbin), wax->GetBinLowEdge(wbin+1) };
	float q2vals[] = { q2ax->GetBinCenter(q2bin), q2ax->GetBinLowEdge(q2bin), q2ax->GetBinLowEdge(q2bin+1) };
	TString hn = TString::Format("mmp_%.3f_%.3f",wvals[0],q2vals[0]);
	TString ht = TString::Format("W = (%.3f,%.3f), Q^2 = (%.3f,%.3f)",wvals[1],wvals[2],q2vals[1],q2vals[2]);
	TH1 *hmmp = h3->ProjectionX(hn.Data(),wbin,wbin,q2bin,q2bin);
	//printf("%s\n",hn.Data());
	if (hmmp->Integral()>0) {
	  //printf("+++ %s\n",ht.Data());
	  hmmp->SetTitle(ht.Data());
	  hs[q2bin-1]->Add((TH1F*)hmmp->Clone());
	}
      }
      TString hsn = TString::Format("hs%d",q2bin-1);
      fout->WriteObject(hs[q2bin-1],hsn.Data());
    }
  }
};
