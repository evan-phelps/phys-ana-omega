#define h10t3pi_sel_sim_cxx
#include "h10t3pi_sel_sim.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>

void h10t3pi_sel_sim::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
}

void h10t3pi_sel_sim::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
   Setup();
}

Bool_t h10t3pi_sel_sim::Process(Long64_t entry)
{
  if ( (++eventnum) % BLOCKSIZE == 0 ) {
    if (nentries==-1) nentries = fChain->GetEntriesFast();
    Float_t gtime = swgroup.RealTime();
    Float_t ttime = swmain.RealTime();
    Double_t percentProcessed = (((Double_t)eventnum)/(Double_t)nentries)*100;
    Float_t remaining = (100.0/percentProcessed*ttime-ttime)/60.0;
    printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(BLOCKSIZE/gtime),remaining);
    swgroup.Start();
    swmain.Start(kFALSE);
  }
  GetEntry(entry);
  Process();
  return kTRUE;
}

void h10t3pi_sel_sim::SlaveTerminate()
{
}

void h10t3pi_sel_sim::Terminate()
{
  Float_t gtime = swgroup.RealTime();
  Float_t ttime = swmain.RealTime();
  Double_t percentProcessed = (((Double_t)eventnum)/(Double_t)nentries)*100;
  Float_t remaining = (100.0/percentProcessed*ttime-ttime)/60.0;
  printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(BLOCKSIZE/gtime),remaining);
  Finalize();
}
