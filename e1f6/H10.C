#define H10_cxx
#include "H10.h"
#include <iostream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void H10::Loop(Long64_t ntoproc/* = -1*/, Bool_t fastcount/* = kTRUE*/, TEntryList *elist/* = 0 */)
{
    if (fChain == 0) return;
    if (ntoproc == -1) {
        if (fastcount) ntoproc = fChain->GetEntriesFast();
        else ntoproc = elist ? elist->GetN() : fChain->GetEntries();
    }
    std::cout << "ntoproc = " << ntoproc << std::endl;
    _ntoprocess = ntoproc;
    //Long64_t nbytes = 0, nb = 0;
    _swmain.Start();
    _swgroup.Start();
    for (Long64_t jentry_el=0; jentry_el<ntoproc; jentry_el++) {
        jentry = elist ? elist->GetEntry(jentry_el) : jentry_el;
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        PrintProgress(jentry);
        //data->CheapPop(ientry);
        GetEntry(jentry);
        fHandlerChain->Process(this);
    }

    fHandlerChain->Finalize(this);
    Float_t ttime = _swmain.RealTime();
    Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
    printf("Total: (%.2f) %lld/%.2f = %i events/sec\n",percentProcessed,eventnum,ttime,(Int_t)(eventnum/ttime));
}
