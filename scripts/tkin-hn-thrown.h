//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Mar 17 22:21:29 2013 by ROOT version 5.34/05
// from TChain tkin/
//////////////////////////////////////////////////////////

#ifndef tkin_h
#define tkin_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <THnSparse.h>
#include <TH1.h>
#include <TF1.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class tkin : public TSelector {
 public :
  TFile *fout;
  TH1 *ht1, *ht1w;
  THnSparseF *hbd;
  Double_t bdcoord[6];

  TTree          *fChain;   //!pointer to the analyzed TTree or TChain

  // Declaration of leaf types
  Float_t         s;
  Float_t         t;
  Float_t         u;
  Float_t         W;
  Float_t         Q2;
  Float_t         ct;
  Float_t         phi;
  Float_t         mmp;
  Float_t         mmppip;
  Float_t         mmppim;
  Float_t         mmppippim;
  Float_t         t0;
  Float_t         t1;
  Float_t         xb;
  Float_t         nu;
  Float_t         eps;
  Float_t         vgflux;
  Float_t         e_p;
  Float_t         e_theta;
  Float_t         e_phi;
  Float_t         p_p;
  Float_t         p_theta;
  Float_t         p_phi;
  Float_t         pip_p;
  Float_t         pip_theta;
  Float_t         pip_phi;
  Float_t         pim_p;
  Float_t         pim_theta;
  Float_t         pim_phi;

  // List of branches
  TBranch        *b_s;   //!
  TBranch        *b_t;   //!
  TBranch        *b_u;   //!
  TBranch        *b_W;   //!
  TBranch        *b_Q2;   //!
  TBranch        *b_ct;   //!
  TBranch        *b_phi;   //!
  TBranch        *b_mmp;   //!
  TBranch        *b_mmppip;   //!
  TBranch        *b_mmppim;   //!
  TBranch        *b_mmppippim;   //!
  TBranch        *b_t0;   //!
  TBranch        *b_t1;   //!
  TBranch        *b_xb;   //!
  TBranch        *b_nu;   //!
  TBranch        *b_eps;   //!
  TBranch        *b_vgflux;   //!
  TBranch        *b_e;   //!
  TBranch        *b_p;   //!
  TBranch        *b_pip;   //!
  TBranch        *b_pim;   //!

 tkin(TTree * /*tree*/ =0) : fChain(0) { }
  virtual ~tkin() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  ClassDef(tkin,0);
};

#endif

#ifdef tkin_cxx
void tkin::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("s", &s, &b_s);
  fChain->SetBranchAddress("t", &t, &b_t);
  fChain->SetBranchAddress("u", &u, &b_u);
  fChain->SetBranchAddress("W", &W, &b_W);
  fChain->SetBranchAddress("Q2", &Q2, &b_Q2);
  fChain->SetBranchAddress("ct", &ct, &b_ct);
  fChain->SetBranchAddress("phi", &phi, &b_phi);
  fChain->SetBranchAddress("mmp", &mmp, &b_mmp);
  fChain->SetBranchAddress("mmppip", &mmppip, &b_mmppip);
  fChain->SetBranchAddress("mmppim", &mmppim, &b_mmppim);
  fChain->SetBranchAddress("mmppippim", &mmppippim, &b_mmppippim);
  fChain->SetBranchAddress("t0", &t0, &b_t0);
  fChain->SetBranchAddress("t1", &t1, &b_t1);
  fChain->SetBranchAddress("xb", &xb, &b_xb);
  fChain->SetBranchAddress("nu", &nu, &b_nu);
  fChain->SetBranchAddress("eps", &eps, &b_eps);
  fChain->SetBranchAddress("vgflux", &vgflux, &b_vgflux);
  fChain->SetBranchAddress("e", &e_p, &b_e);
  fChain->SetBranchAddress("p", &p_p, &b_p);
  fChain->SetBranchAddress("pip", &pip_p, &b_pip);
  fChain->SetBranchAddress("pim", &pim_p, &b_pim);
}

Bool_t tkin::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

#endif // #ifdef tkin_cxx
