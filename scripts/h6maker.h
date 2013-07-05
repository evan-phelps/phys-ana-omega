#include "h10t3pi_sel.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include "particle-constants.h"
#include "scripts/xsect-utils.C"
#include "fid.h"
#include "scripts/simw.C"
#include "input/cc_eff_lazy_programmer.h"

using namespace TMath;
using namespace ParticleConstants;

class h6maker : public h10t3pi_sel {
 public:
  bool issim, w8ed;
  Fid *fid;
  TFile *fout;
  TH1 *ht1, *ht1w;
  THnSparseF *hbd;
  Double_t bdcoord[6];
 h6maker() : h10t3pi_sel() {
    printf("in h6maker::h6maker()\n");
  }
  ~h6maker() {
    printf("in h6maker::h6maker()\n");
    delete hbd;
    delete ht1;
    delete ht1w;
    delete fout;
  }
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
    //Int_t retval = fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
    //if (t3pi) t3pi->GetEntry(entry, getall);
    TString option = GetOption();
    issim = (option.Contains("sim"));
    w8ed = (option.Contains("w8"));
    Int_t retval = 0;
    retval += b_h10idx_e->GetEntry(entry);
    retval += b_h10idx_p->GetEntry(entry);
    retval += b_h10idx_pip->GetEntry(entry);
    retval += b_h10idx_pim->GetEntry(entry);
    if (issim) retval += tkin->GetTree()->GetEntry(entry);
    else retval += tkin->GetEntry(entry);
    if (w8ed) {
      retval += b_cc_sect->GetEntry(entry);
      retval += b_cc_hit->GetEntry(entry);
      retval += b_cc_segm->GetEntry(entry);
    }
    //printf("*** entry = %lld\t",entry);
    return retval;
  }
  void Setup() {
    printf("in h6maker::Setup()\n");
    fid = Fid::Instance();
    fout = new TFile("h6maker-hn.root","create");
    ht1 = new TH1F("ht1","t' yield",800,0,8);
    ht1w = new TH1F("ht1simw","t' yield, weighted",800,0,8);
    Int_t bins2[] = { 80, 7, 8, 12, 18, 160 };
    Double_t xmin2[] = { 1.6, 1.5, 0, -1, -Pi(), 0.4 };
    Double_t xmax2[] = { 3.2, 3.1, 8,  1,  Pi(), 2.0 };
    hbd = new THnSparseF("hbd_yield", "W, Q^{2}, t', cos(#theta), #phi, mmp", 6, bins2, xmin2, xmax2);
    //Double_t qbins[] = { 1.5, 1.6, 1.713, 1.843, 1.997, 2.183, 2.422, 2.753, 3.296, 5.1 };
    Double_t qbins[] = { 1.5, 1.6, 1.8, 2.1, 2.4, 2.76, 3.3, 5.1 };
    //Double_t tbins[] = { 0.1, 0.25, 0.42, 0.618, 0.853, 1.142, 1.518, 2.06, 3, 8 };
    Double_t tbins[] = { 0.1, 0.25, 0.45, 0.65, 0.85, 1.15, 1.5, 2.06, 3, 8 };
    Double_t cbins[] = { -1,-0.9,-0.8,-0.6,-0.4,-0.2,-0.0,0.2,0.4,0.6,0.8,0.9,1 };
    hbd->SetBinEdges(1,qbins);
    hbd->SetBinEdges(2,tbins);
    hbd->SetBinEdges(3,cbins);
    hbd->Sumw2();
  }
  void Process() {
    bdcoord[0] = kin.W;
    bdcoord[1] = kin.Q2;
    bdcoord[2] = -kin.t1;
    bdcoord[3] = kin.ct;
    bdcoord[4] = kin.phi;
    bdcoord[5] = kin.mmp;
    int segment = (cc_segm[cc[h10idx_e]-1]%1000)/10;
    int pmt_hit = (cc_segm[cc[h10idx_e]-1]/1000)-1;
    int sector = cc_sect[cc[h10idx_e]-1];
    //for (int i = 0; i < 6; i++) printf("%.3f\t",bdcoord[i]);
    //printf("\n");
    double weight = 1;
    if (issim) weight = simevtw(-kin.t1);
    else if (w8ed) weight = 1/(kin.vgflux*ccw8(sector,segment,pmt_hit));

    hbd->Fill(bdcoord,weight);
    ht1->Fill(-kin.t1);
    ht1w->Fill(-kin.t1,weight);
  }
  void Finalize() {
    printf("in h6maker::Finalize()\n");
    hbd->Write();
    ht1->Write();
    ht1w->Write();
  }
};
