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

#include "tkin.h"
#include <TH2.h>
#include <TStyle.h>


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

   TString option = GetOption();
   Int_t bins2[] = { 80, 10, 40, 20, 18, 160 };
   hdim = 6;
   Double_t xmin2[] = { 1.6, 0.5, 0, -1, -Pi(), 0.4 };
   Double_t xmax2[] = { 3.2, 5.5, 8,  1,  Pi(), 2.0 };
   THnSparse* hbd = new THnSparseF("yield", "W, Q^{2}, t', cos(#theta), #phi, mmp", 6, bins2, xmin2, xmax2);
   hbd->Sumw2();

}

Bool_t tkin::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either tkin::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  bdcoord[0] = W;
  bdcoord[1] = Q2;
  bdcoord[2] = t1;
  bdcoord[3] = ct;
  bdcoord[4] = phi;
  bdcoord[5] = mmp;
  
  hbd->Fill(bdcoord, simevtw(t1));
  
  return kTRUE;
}

void tkin::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void tkin::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

Double_t tkin::dsimw(Double_t *x, Double_t *par)
{
  Double_t retval = 1;
  if (!dsimw_fsim) {
    dsimw_fsim = new TF1("dsimw_fsim","expo(0)+pol1(2)",0,10);
    dsimw_fsim->SetParameters(par[4],par[5],par[6],par[7]);
  }
  if (!dsimw_fexp) {
    dsimw_fexp = new TF1("dsimw_fexp","expo(0)+pol1(2)",0,10);
    dsimw_fexp->SetParameters(par[0],par[1],par[2],par[3]);
  }
  if (!dsimw_simw_gt4) {
    dsimw_simw_gt4 = new TF1("dsimw_fexp","expo",0,10);
    dsimw_simw_gt4->SetParameters(1.95,-4.91489e-1);
  }
  if (x[0]<4) {
    Double_t vsim = dsimw_fsim->Eval(x[0]);
    Double_t vexp = dsimw_fexp->Eval(x[0]);
    if (vsim>0 && vexp>0) retval = vexp/vsim;
    else if (vexp==0) retval = vsim;
  } else {
    retval = dsimw_simw_gt4->Eval(x[0]);
  }
  return retval;
}

TF1 *tkin::dsimw_fsimw() {
  if (!dsimw_simw) {
    dsimw_simw = new TF1("dsimw_simw",&dsimw,0,8,8);
    dsimw_simw->SetParameters(-4.55,-3.99e-1,-3.689e-3,4.6016e-4,-3.835,-3.04248,1.277e-3,-2.458e-4);
  }
  return dsimw_simw;
}

Double_t tkin::simevtw(Double_t t1) {
  return dsimw_fsimw()->Eval(t1);
}
