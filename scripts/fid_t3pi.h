//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jan 26 17:59:53 2013 by ROOT version 5.34/03
// from TChain 3pi-tree/t3pi/
//////////////////////////////////////////////////////////

#ifndef t3pi_h
#define t3pi_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH3.h>
#include "/home/ephelps/analysis/omega/fid.h"
// Header file for the classes stored in the TTree if any.
#include <TLorentzVector.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class t3pi {
public :

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   
   TFile *fout;
   static const int SECTORS = 6;
   static const int NPARTS = 4;
   enum { E, P, PIP, PIM };
   TH3 *h3[NPARTS][SECTORS];
   TLorentzVector* lv[NPARTS];

   // Declaration of leaf types
   TLorentzVector  *w4;
   TLorentzVector  *q4;
   TLorentzVector  *e0;
   TLorentzVector  *e1;
   TLorentzVector  *p0;
   TLorentzVector  *p1;
   TLorentzVector  *pip;
   TLorentzVector  *pim;
   TLorentzVector  *pi0;
   TLorentzVector  *omega;
   TLorentzVector  *ppip;
   TLorentzVector  *ppim;
   TLorentzVector  *pippim;
   TLorentzVector  *pippi0;
   TLorentzVector  *pimpi0;
   TLorentzVector  *mmppip;
   TLorentzVector  *mmppim;
   Int_t           h10idx_e;
   Int_t           h10idx_p;
   Int_t           h10idx_pip;
   Int_t           h10idx_pim;

   // List of branches
   TBranch        *b_w4;   //!
   TBranch        *b_q4;   //!
   TBranch        *b_e0;   //!
   TBranch        *b_e1;   //!
   TBranch        *b_p0;   //!
   TBranch        *b_p1;   //!
   TBranch        *b_pip;   //!
   TBranch        *b_pim;   //!
   TBranch        *b_pi0;   //!
   TBranch        *b_omega;   //!
   TBranch        *b_ppip;   //!
   TBranch        *b_ppim;   //!
   TBranch        *b_pippim;   //!
   TBranch        *b_pippi0;   //!
   TBranch        *b_pimpi0;   //!
   TBranch        *b_mmppip;   //!
   TBranch        *b_mmppim;   //!
   TBranch        *b_h10idx_e;   //!
   TBranch        *b_h10idx_p;   //!
   TBranch        *b_h10idx_pip;   //!
   TBranch        *b_h10idx_pim;   //!

   t3pi(TTree *tree=0);
   virtual ~t3pi();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef t3pi_cxx
t3pi::t3pi(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("3pi-tree/t3pi",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      //TChain * chain = new TChain("3pi-tree/t3pi","");
      //chain->Add("omega.root/3pi-tree/t3pi");
      TChain * chain = new TChain("t3pi","");
      chain->Add("1901?.root/3pi-tree-recon/t3pi");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

t3pi::~t3pi()
{
  if (fout) delete fout;
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t t3pi::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t t3pi::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void t3pi::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
  for (int i = 0; i < NPARTS; i++) lv[i] = 0;
   w4 = 0;
   q4 = 0;
   e0 = 0;
   e1 = 0;
   p0 = 0;
   p1 = 0;
   pip = 0;
   pim = 0;
   pi0 = 0;
   omega = 0;
   ppip = 0;
   ppim = 0;
   pippim = 0;
   pippi0 = 0;
   pimpi0 = 0;
   mmppip = 0;
   mmppim = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("w4", &w4, &b_w4);
   fChain->SetBranchAddress("q4", &q4, &b_q4);
   fChain->SetBranchAddress("e0", &e0, &b_e0);
   fChain->SetBranchAddress("e1", &lv[E], &b_e1); //&e1, &b_e1);
   fChain->SetBranchAddress("p0", &p0, &b_p0);
   fChain->SetBranchAddress("p1", &lv[P], &b_p1); //&p1, &b_p1);
   fChain->SetBranchAddress("pip", &lv[PIP], &b_pip); //&pip, &b_pip);
   fChain->SetBranchAddress("pim", &lv[PIM], &b_pim); //&pim, &b_pim);
   fChain->SetBranchAddress("pi0", &pi0, &b_pi0);
   fChain->SetBranchAddress("omega", &omega, &b_omega);
   fChain->SetBranchAddress("ppip", &ppip, &b_ppip);
   fChain->SetBranchAddress("ppim", &ppim, &b_ppim);
   fChain->SetBranchAddress("pippim", &pippim, &b_pippim);
   fChain->SetBranchAddress("pippi0", &pippi0, &b_pippi0);
   fChain->SetBranchAddress("pimpi0", &pimpi0, &b_pimpi0);
   fChain->SetBranchAddress("mmppip", &mmppip, &b_mmppip);
   fChain->SetBranchAddress("mmppim", &mmppim, &b_mmppim);
   fChain->SetBranchAddress("h10idx_e", &h10idx_e, &b_h10idx_e);
   fChain->SetBranchAddress("h10idx_p", &h10idx_p, &b_h10idx_p);
   fChain->SetBranchAddress("h10idx_pip", &h10idx_pip, &b_h10idx_pip);
   fChain->SetBranchAddress("h10idx_pim", &h10idx_pim, &b_h10idx_pim);
   Notify();
}

Bool_t t3pi::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void t3pi::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t t3pi::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef t3pi_cxx
