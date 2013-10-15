#!/usr/bin/python

import ROOT as r

for dep in ['Config.cpp', 'DataHandler.h', 'HandlerChain.cpp', 'H10.C',
            'DH_EC_Hists_PreEid.h', 'DH_RunQuality.cpp', 'DH_EC_Hists.h',
            'DH_CloneH10.h', 'DH_Hists_Monitor.h', 'DH_Eid.h',
            'DH_SC_Hists_PrePid.h', 'DH_CC_Hists.h']:
    r.gROOT.ProcessLine('.L %s+' % dep)

handlers = {
    "mon_raw": r.DH_Hists_Monitor,
    "echists_raw": r.DH_EC_Hists_PreEid,
    "cchists_raw": r.DH_CC_Hists,
    "runquality": r.DH_RunQuality,
    "cchists_qskim": r.DH_CC_Hists,
    "echists_qskim": r.DH_EC_Hists,
    "mon_qskim": r.DH_Hists_Monitor,
    "eid": r.DH_Eid,
    "echists_eskim": r.DH_EC_Hists,
    "cchists_eskim": r.DH_CC_Hists,
    "mon_eskim": r.DH_Hists_Monitor,
    "h10_eskim": r.DH_CloneH10,
    "scpid_eskim": r.DH_SC_Hists_PrePid
}

fout = r.TFile('test.root', 'RECREATE')
chain = r.TChain('h10')
chain.Add('~/w/2013_sept/e16_test/*.root')
processor = r.H10(chain, "input.e16.exp.parms")

for name, handler in handlers.items():
    processor.Add(handler(name, fout))

processor.Loop(1000, False)
