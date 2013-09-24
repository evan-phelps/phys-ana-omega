#!/usr/bin/python

import ROOT as r

r.gROOT.ProcessLine('.L DataHandler.h+')
r.gROOT.ProcessLine('.L HandlerChain.cpp+')
r.gROOT.ProcessLine('.L H10.C+')
r.gROOT.ProcessLine('.L DH_RunQuality.cpp+')

chain = r.TChain('h10clone/h10')
chain.Add('/data/e1f/skim/3812?.root')

dhrq = r.DH_RunQuality('dhrq')
processor = r.H10(chain)
processor.Add(dhrq)
processor.Loop()
