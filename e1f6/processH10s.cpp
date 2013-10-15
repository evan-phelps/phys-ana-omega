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

int main(int argc, char const *argv[])
{
    TFile *fout = new TFile("skim.root","recreate");
    TChain *c = new TChain("h10");
    c->Add("*.root");
    H10 *h10proc = new H10(c, "e1f");
    h10proc->Add(new DH_Hists_Monitor("mon_raw", fout));
    h10proc->Add(new DH_EC_Hists_PreEid("echists_raw", fout));
    h10proc->Add(new DH_CC_Hists("cchists_raw", fout));
    h10proc->Add(new DH_RunQuality("runquality", fout));
    h10proc->Add(new DH_CC_Hists("cchists_qskim", fout));
    h10proc->Add(new DH_EC_Hists("echists_qskim", fout));
    h10proc->Add(new DH_Hists_Monitor("mon_qskim", fout));
    h10proc->Add(new DH_Eid("eid", fout));
    h10proc->Add(new DH_EC_Hists("echists_eskim", fout));
    h10proc->Add(new DH_CC_Hists("cchists_eskim", fout));
    h10proc->Add(new DH_Hists_Monitor("mon_eskim", fout));
    h10proc->Add(new DH_CloneH10("h10_eskim", fout));;
    h10proc->Add(new DH_SC_Hists_PrePid("scpid", fout));
    h10proc->Loop(-1,kFALSE);
    return 0;
}
