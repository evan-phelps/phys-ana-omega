#!/usr/bin/python

import ROOT as r

r.gROOT.ProcessLine('.L DataHandler.h+')
r.gROOT.ProcessLine('.L HandlerChain.cpp+')
r.gROOT.ProcessLine('.L H10.C+')
r.gROOT.ProcessLine('.L DH_RunQuality.cpp+')
r.gROOT.ProcessLine('.L DH_CloneH10.h+')

chain = r.TChain('h10clone/h10')
chain.Add('/data/e1f/skim/3812?.root')

fout = r.TFile('test.root', 'RECREATE')
processor = r.H10(chain)
handlers = [r.DH_RunQuality('dhrq', fout), r.DH_CloneH10('h10', fout)]

for dh in handlers:
    processor.Add(dh)

processor.Loop(1000)
