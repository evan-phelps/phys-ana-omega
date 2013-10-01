#!/usr/bin/python

import ROOT as r

for dep in ['Config.cpp', 'DataHandler.h', 'HandlerChain.cpp', 'H10.C',
            'DH_RunQuality.cpp', 'DH_CloneH10.h', 'DH_EC_Hists.h',
            'DH_EC_Hists_PreEid.h', 'DH_Hists_Monitor.h']:
    r.gROOT.ProcessLine('.L %s+' % dep)

fout = r.TFile('test.root', 'RECREATE')

#handlers = [r.DH_RunQuality('dhrq', fout), r.DH_CloneH10('h10', fout)]
handlers = [r.DH_EC_Hists('echists', fout)]

chain = r.TChain('h10clone/h10')
chain.Add('/data/e1f/skim/3812?.root')

processor = r.H10(chain, "e1f")

for dh in handlers:
    processor.Add(dh)

processor.Loop(1000)
