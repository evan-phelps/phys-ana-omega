#define H10_cxx
#include "H10.h"
#include <iostream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

H10::H10(TTree *tree, std::string experiment)
{
    // cfg = new Config("input.e16.exp.parms");
    if (experiment == "e1f") cfg = new Config("input.e16.exp.parms");
    else if(experiment == "e16") cfg = new Config("input.e1f.exp.parms");
    else
    {
        std::string emsg = "experiment not recognized! must be e1f or e16";
        throw new std::runtime_error(emsg.c_str());
    }
    is_sim = kFALSE;
    run = -1;
    beamEnergy = cfg->GetFloat("beam_energy");
    file_anum = -1;
    filename = "";
    fHandlerChain = new HandlerChain();
    fTreeNumber = -1;
    eventnum = 0;
    fRegExp_run = new TRegexp("[0-9][0-9][0-9][0-9][0-9]");
    fRegExp_Anum = new TRegexp("[Aa][0-9][0-9]");
    Init(tree);
}


H10::~H10()
{
    if (fChain) delete fChain->GetCurrentFile();
    delete fHandlerChain;
    delete fRegExp_run;
    delete fRegExp_Anum;
    delete cfg;
}


void H10::Loop(Long64_t ntoproc/* = -1*/, Bool_t fastcount/* = kTRUE*/, TEntryList *elist/* = 0 */)
{
    if (fChain == 0) return;
    if (ntoproc == -1)
    {
        if (fastcount) ntoproc = fChain->GetEntriesFast();
        else ntoproc = elist ? elist->GetN() : fChain->GetEntries();
    }
    std::cout << "ntoproc = " << ntoproc << std::endl;
    _ntoprocess = ntoproc;
    //Long64_t nbytes = 0, nb = 0;
    _swmain.Start();
    _swgroup.Start();
    for (Long64_t jentry_el=0; jentry_el<ntoproc; jentry_el++)
    {
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
