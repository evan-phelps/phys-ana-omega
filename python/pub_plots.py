# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

WS = '/home/ephelps/projects/phys-ana-omega'
WS_SETUP = '%s/python/oana_setup.py' % WS
%run $WS_SETUP

import matplotlib.gridspec as gridspec
import rootplot.root2matplotlib as r2m
from matplotlib import rc
rc('text', usetex=True)

import itertools as it

import pylab as pl
from epfuncs import f_bwexpgaus
from epfuncs import f_bwexpgaus_pol4
from epfuncs import RejectWrapper
from epxsectutils import vgflux

from math import pi
mpi0, mpi, mrho0, mphi, mk = 0.1349766, 0.139570, 0.77549, 1.019445, 0.493677

# <codecell>

run.Draw("ntrigs/dq:run>>h_trigsPerQ_run(2000,37000.5,39000.5,1000,0,500000)","","goff")
run.Draw("ntrigs/dq>>h_trigsPerQ(1000,0,500000)","","goff")
h_trigsPerQ_run = asrootpy(r.gROOT.FindObject('h_trigsPerQ_run'))
h_trigsPerQ = asrootpy(r.gROOT.FindObject('h_trigsPerQ'))

# <codecell>

run.Draw("npassed/dq:run>>h_passedPerQ_run(2000,37000.5,39000.5,200,0,5000)","","goff")
run.Draw("npassed/dq>>h_passedPerQ(1000,0,5000)","","goff")
h_passedPerQ = asrootpy(r.gROOT.FindObject('h_passedPerQ'))
h_passedPerQ_run = asrootpy(r.gROOT.FindObject('h_passedPerQ_run'))

# <codecell>

parmobjarr = r.TObjArray()
h_passedPerQ_run.FitSlicesY(0,  0, -1, 1, 'qnr', parmobjarr)
fig = plt.figure(figsize=(16,4), dpi=600)
for iparm in range(0,4):
    parmobjarr[iparm]
    plt.subplot2grid([1,4], [0,iparm])
    parmobjarr[iparm].SetMarkerStyle(24)
    for ibin in range(1, parmobjarr[iparm].GetNbinsX()+1):
        if parmobjarr[iparm].GetBinContent(ibin) == 0:
            parmobjarr[iparm].SetBinContent(ibin, 0)
            parmobjarr[iparm].SetBinError(ibin, 0)
    rplt.errorbar(asrootpy(parmobjarr[iparm]), markersize=0.1)
    ylim(ymin=0)

locut = parmobjarr[1].Clone()
hicut = parmobjarr[1].Clone()
hicut.Add(parmobjarr[2], 3)
locut.Add(parmobjarr[2], -3)

# <codecell>

#gs = gridspec.GridSpec(1,8)
plt.figure(figsize=(12, 8), dpi=600)
plt.subplot2grid((2,5), (0,0), 1, 3)
plt.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
rplt.hist2d(h_trigsPerQ_run, cmin=1)
#r2m.Hist2D(h_trigsPerQ_run).col() #contour()
#ylim([0,600000])
ylim([2e5,4e5])
xlim([37600,38800])
plt.xlabel('run number', fontsize=12)
plt.ylabel(r'triggers/Q_{FC} ($\mu C$)', fontsize=12)
plt.xticks(fontsize=12, rotation=30)
plt.yticks(fontsize=12)
plt.subplot2grid((2,5), (0,3), 1, 2)
#r2m.Hist(h_trigsPerQ)
rplt.hist(h_trigsPerQ)
plt.xticks(fontsize=12, rotation=30)
plt.yticks(fontsize=12)
plt.xlabel(r'triggers/Q_{FC} ($\mu C$)', fontsize=12)
plt.autoscale()
yscale('log')

plt.subplot2grid((2,5), (1,0), 1, 3)
plt.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
rplt.hist2d(h_passedPerQ_run, cmin=1)
#r2m.Hist2D(h_passedPerQ_run).col() #contour()
#ylim([0,600000])
plt.xlabel('run number', fontsize=12)
plt.ylabel(r'N_{exclusive}/Q_{FC} ($\mu C$)', fontsize=12)
plt.xticks(fontsize=12, rotation=30)
plt.yticks(fontsize=12)
rplt.errorbar(asrootpy(hicut), markersize=0.1, xerr=False, yerr=False, emptybins=False)
rplt.errorbar(asrootpy(locut), markersize=0.1, xerr=False, yerr=False, emptybins=False)
htemp = asrootpy(parmobjarr[1])
htemp.SetLineColor(r.kRed)
htemp.SetMarkerColor(r.kRed)
rplt.errorbar(asrootpy(parmobjarr[1]), markersize=0.1, xerr=False, yerr=False, emptybins=False)
ylim([0,5e3])
xlim([37600,38800])
plt.subplot2grid((2,5), (1,3), 1, 2)
#r2m.Hist(h_trigsPerQ)
rplt.hist(h_passedPerQ)
plt.xticks(fontsize=12, rotation=30)
plt.yticks(fontsize=12)
plt.xlabel(r'N_{exclusive}/Q_{FC} ($\mu C$)', fontsize=12)
plt.autoscale()
yscale('log')
plt.tight_layout()

# <codecell>

for iparm in range(0,4): print(parmobjarr[iparm].GetName())

# <codecell>

def drawf(f, fcolor='red', xs = pl.arange(0.6, 0.95, 0.001)):
    ys = pl.array([f.Eval(x) for x in xs])
    plot(xs, ys, linewidth=2.0, color=fcolor)

fgaus = r.TF1('fgaus', 'gaus', 0, 2000)
htemp.Fit(fgaus, 'N0', 'goff')

h_passedPerQ_run.GetXaxis().SetRangeUser(38199.5, 38200.5)
htemp = asrootpy(h_passedPerQ_run.ProjectionY())
rplt.hist(htemp)
drawf(fgaus, xs=pl.arange(0,5000))

# <codecell>

fgaus

# <codecell>

fgaus.Print()

# <codecell>


