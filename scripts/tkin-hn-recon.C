#define tkin_cxx
// The class definition in tkin.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("tkin.C")
// Root > T->Process("tkin.C","some options")
// Root > T->Process("tkin.C+")
//

#include "tkin-hn-recon.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include "simw.C"

using namespace TMath;

void tkin::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void tkin::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
  fid = Fid::Instance();
  fout = new TFile("tkin-hn.root","create");
  ht1 = new TH1F("ht1r","t' reconstructed",800,0,8);
  ht1w = new TH1F("ht1t","t' thrown",800,0,8);
   TString option = GetOption();
   Int_t bins2[] = { 80, 10, 40, 20, 18, 160 };
   Double_t xmin2[] = { 1.6, 0.5, 0, -1, -Pi(), 0.4 };
   Double_t xmax2[] = { 3.2, 5.5, 8,  1,  Pi(), 2.0 };
   hbd = new THnSparseF("hbd_thrown", "W, Q^{2}, t', cos(#theta), #phi, mmp", 6, bins2, xmin2, xmax2);
   hbd->Sumw2();
}

Bool_t tkin::Process(Long64_t entry)
{
  GetEntry(entry);
  b_thrown_t1->GetEntry(entry);
  if (e_p>0 && p_p>0 && pip_p>0) {
    if (fid->InFid(e_p,e_theta,e_phi,11)
	&& fid->InFid(p_p,p_theta,p_phi,2212)
	&& fid->InFid(pip_p,pip_theta,pip_phi,211)) {
      bdcoord[0] = W;
      bdcoord[1] = Q2;
      bdcoord[2] = -t1;
      bdcoord[3] = ct;
      bdcoord[4] = phi;
      bdcoord[5] = mmp;
      double weight =  simevtw(-thrown_t1);
      hbd->Fill(bdcoord,weight);
      ht1->Fill(-t1,weight);
      ht1w->Fill(-thrown_t1,weight);
    }
  }

  return kTRUE;
}

void tkin::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  fout->cd();
  hbd->Write();
  ht1->Write();
  ht1w->Write();
}

void tkin::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
