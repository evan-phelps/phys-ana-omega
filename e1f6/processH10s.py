#!/usr/bin/python

import ROOT as r

for dep in ['Config.cpp', 'DataHandler.h', 'HandlerChain.cpp', 'H10.C',
            'DH_EC_Hists_PreEid.h', 'DH_RunQuality.cpp', 'DH_EC_Hists.h',
            'DH_CloneH10.h', 'DH_Hists_Monitor.h', 'DH_Eid.h',
            'DH_SC_Hists_PrePid.h', 'DH_CC_Hists.h']:
    r.gROOT.ProcessLine('.L %s+' % dep)

fout = r.TFile('test.root', 'RECREATE')

#handlers = [r.DH_RunQuality('dhrq', fout), r.DH_CloneH10('h10', fout)]
# handlers = [r.DH_EC_Hists('echists', fout)]

chain = r.TChain('h10')
chain.Add('~/w/2013_sept/e16_test/*.root')
processor = r.H10(chain, "e16")

# for dh in handlers:
#     processor.Add(dh)

processor.Add(r.DH_Hists_Monitor("mon_raw", fout))
processor.Add(r.DH_EC_Hists_PreEid("echists_raw", fout))
processor.Add(r.DH_CC_Hists("cchists_raw", fout))
processor.Add(r.DH_RunQuality("runquality", fout))
processor.Add(r.DH_CC_Hists("cchists_qskim", fout))
processor.Add(r.DH_EC_Hists("echists_qskim", fout))
processor.Add(r.DH_Hists_Monitor("mon_qskim", fout))
processor.Add(r.DH_Eid("eid", fout))
processor.Add(r.DH_EC_Hists("echists_eskim", fout))
processor.Add(r.DH_CC_Hists("cchists_eskim", fout))
processor.Add(r.DH_Hists_Monitor("mon_eskim", fout))
processor.Add(r.DH_CloneH10("h10_eskim", fout));
processor.Add(r.DH_SC_Hists_PrePid("scpid_eskim", fout))

processor.Loop(1000, False)
