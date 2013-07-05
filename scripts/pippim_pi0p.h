#include "h10t3pi_sel.h"
#include <TMath.h>
#include <TFile.h>
#include <TCutG.h>
#include <TH2.h>
#include "acc.h"

class pippim_pi0p : public h10t3pi_sel {
 public:
  TH2 *h_dalitz, *h_dalitz_cut, *h_cost_w, *h_cost_w_acc;
  TFile *f_cut;
  TCutG *cut_delta_rho;
  Acc *acc;
 pippim_pi0p() : h10t3pi_sel() {
    printf("in pippim_pi0p::pippim_pi0p()\n");
    h_dalitz = NULL;
    h_dalitz_cut = NULL;
    h_cost_w = NULL;
    h_cost_w_acc = NULL;
    acc = new Acc();
    f_cut = new TFile("cutg.root");
    cut_delta_rho = (TCutG*)f_cut->Get("CUTG");
  }
  ~pippim_pi0p() {
    printf("in pippim_pi0p::pippim_pi0p()\n");
    delete cut_delta_rho;
    delete f_cut;
    delete h_dalitz;
    delete h_dalitz_cut;
    delete h_cost_w;
    delete h_cost_w_acc;
    delete acc;
  }
  void Setup() {
    printf("in pippim_pi0p::Setup()\n");
    h_dalitz = new TH2D("h_dalitz", "h_dalitz", 200, 0.4, 1.2, 200, 0.8, 1.4);
    h_dalitz = new TH2D("h_dalitz_cut", "h_dalitz_cut", 200, 0.4, 1.2, 200, 0.8, 1.4);
    h_cost_w =  new TH2D("h_cost_w", "h_cost_w", 11, -1, 1, 50, 1.7, 3.2);
    h_cost_w =  new TH2D("h_cost_w_acc", "h_cost_w_acc", 11, -1, 1, 50, 1.7, 3.2);
  }
  void Process() {
    TLorentzVector lv_pi0p = *w4 - *pippim;
    Double_t cost = TMath::Cos(pippim->Angle(lv_pi0p.Vect()));
    Double_t mpippim = pippim->M();
    Double_t mpi0p = lv_pi0p.M();
    h_dalitz->Fill(mpippim, mpi0p);
    if (cut_delta_rho->IsInside(mpippim, mpi0p)) {
      h_cost_w->Fill(cost, kin.W);
      Double_t accerr = 0;
      accfact = acc.GetAcc(kin.W, kin.Q2, kin.ct, kin.phi, accerr);
      if (accfact > 0.001) {
        Int_t ibin = h_cost_w_acc->FindBin(cost, kin.W);
        Double_t binerror = h_cost_w_acc->GetBinError(ibin);
        weight = 1/accfact;
        Double_t err2 = binerror*binerror + weight*weight + accerr*accerr/(weight*weight*weight*weight);
        h_cost_w_acc->Fill(cost, kin.W, weight);
        h_cost_w_acc->SetBinError(ibin, sqrt(err2));
      }
    }
  }
  void Finalize() {
    printf("in pippim_pi0p::Finalize()\n");
    h_dalitz->Draw("colz");
  }
};
