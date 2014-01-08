#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include "Config.h"
#include "DataHandler.h"
#include "HandlerChain.h"
#include "H10.h"
#include "DH_EC_Hists_PreEid.h"
#include "DH_RunQuality.h"
#include "DH_EC_Hists.h"
#include "DH_CloneH10.h"
#include "DH_Hists_Monitor.h"
#include "DH_Eid.h"
#include "DH_SC_Hists_PrePid.h"
#include "DH_CC_Hists.h"
#include "DH_W_Skim.h"
#include "DH_MMp_Skim.h"

int main(int argc, char const *argv[])
{
    TChain *c = new TChain("h10clone/h10");
    c->Add("*.root");
    TFile *fout = new TFile("skim.root","recreate");
    H10 *h10proc = new H10(c, "input.e1f.exp.parms");
    h10proc->Add(new DH_Hists_Monitor("mon_raw", fout));
    h10proc->Add(new DH_EC_Hists("echists_qskim", fout));
    h10proc->Add(new DH_RunQuality("runquality", fout));
    h10proc->Add(new DH_EC_Hists_PreEid("echists_raw", fout));
    h10proc->Add(new DH_EC_Hists("echists_qskim_2", fout));
    h10proc->Add(new DH_Hists_Monitor("mon_qskim", fout));
    h10proc->Add(new DH_Eid("eid", fout));
    h10proc->Add(new DH_EC_Hists("echists_eskim", fout));
    h10proc->Add(new DH_Hists_Monitor("mon_eskim", fout));
    h10proc->Add(new DH_SC_Hists_PrePid("scpid", fout));
    h10proc->Add(new DH_W_Skim("w_skim", fout));
    h10proc->Add(new DH_MMp_Skim("mmp_skim", fout));
    h10proc->Add(new DH_CloneH10("h10clone", fout));
    h10proc->Add(new DH_Hists_Monitor("mon_mmpskim", fout));
    printf("all handlers added; execute h10proc.Loop\n");
    h10proc->Loop(-1,kFALSE);
    return 0;
}

/*
g++ `root-config --cflags --glibs` -c H10.C
g++ `root-config --cflags --glibs` -c DH_RunQuality.cpp
g++ `root-config --cflags --glibs` -c Config.cpp
g++ `root-config --cflags --glibs` -c HandlerChain.cpp
g++ `root-config --cflags --glibs` *.o processH10s.cpp -o processH10s
*/