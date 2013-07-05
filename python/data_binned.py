# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # Binned Data Notes
# ## 2013-06-10

# <codecell>

import ROOT as r
from ROOT import TFile, THnSparseF
from rootpy.io import root_open as ropen, DoesNotExist
from rootpy.plotting import Hist, Hist2D
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
from numpy import array
import numpy as np

# <codecell>

fin_t = ropen('/home/ephelps/projects/phys-ana-omega/input/acc-per-part.root')
#fin_e = ropen('/data/ephelps.bak/analysis/sandbox/out/h6-exp-varbin.root')
fin_e = ropen('/data/ephelps.bak/analysis/sandbox/out/h6-top1-exp.root')

# <codecell>

h4_t, h4_a1, h4_a = fin_t.Get('hthrown'), fin_t.Get('hacc'), fin_t.Get('hacc0s')
h4_r = h4_t.Clone('h4_r')
h4_r.Multiply(h4_a1)
h6_e = fin_e.Get('hbd_yield')
dims = [0,1,3,4]
h4_e = h6_e.Projection(len(dims), array(dims,'i'))
import new
idims = new.module('idims_namespace')
idims.__dict__.update({'w': 0, 'q2': 1, 't': 2, 'p': 3})
print('  s   |  e  ')
for idim in [0, 1]:
    print('%.3f | %.3f' % (h4_t.GetAxis(idim).GetBinWidth(3), h4_t.GetAxis(idim).GetBinWidth(3)))
    

# <codecell>

# yfmt = FormatStrFormatter('%.1e')
def draw(hist, ncols=1, nrows=1, cell=1, fig=None):
    if fig is None:
        fig = plt.figure(figsize=(14, 5), dpi=100, facecolor='white')
    else:
        plt.figure(fig.number)
    subplot = fig.add_subplot(nrows, ncols, cell)
    subplot.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    
    if isinstance(hist, r.TH2):
        X, Y = np.meshgrid(list(hist.x()), list(hist.y()))
        x = X.ravel()
        y = Y.ravel()
        z = np.array(a.z()).T
        subplot.pcolormesh(X, Y, H)
    else:
        # locs,labels = yticks()
        # yticks(locs, map(lambda x: "%.1f" % x, locs*1e6))
        # text(0.0, 1.01, '1e-6', fontsize=10, transform = gca().transAxes)
        # subplot.yaxis.set_major_formatter(yfmt)
        rplt.errorbar([hist], xerr=False, emptybins=False)
    # plt.show()
    return fig

# <codecell>

ht_w, ht_q2, ht_ct, ht_phi = [asrootpy(h4_t.Projection(idim)) for idim in range(0,4)]
hr_w, hr_q2, hr_ct, hr_phi = [asrootpy(h4_r.Projection(idim)) for idim in range(0,4)]
he_w, he_q2, he_ct, he_phi = [asrootpy(h4_e.Projection(idim)) for idim in range(0,4)]

# <codecell>

ht_q2w, hr_q2w, he_q2w = [asrootpy(h4.Projection(idims.q2, idims.w)) for h4 in [h4_t, h4_r, h4_e]]

# <codecell>

hr_q2w.Draw('colz')

# <codecell>

fig = plt.figure(figsize=(12, 10), dpi=100, facecolor='white')
hts = [ht_w, ht_q2, ht_ct, ht_phi]
hrs = [hr_w, hr_q2, hr_ct, hr_phi]
hes = [he_w, he_q2, he_ct, he_phi]
[draw(h1,3,4,i*3+j+1,fig) for i, h1s in enumerate(zip(hts, hrs, hes)) for j, h1 in enumerate(h1s)]
fig.tight_layout()
plt.show()

# <codecell>

def draw1(q2bin, wbin):
    fig = plt.figure(figsize=(12, 5), dpi=100, facecolor='white')
    h4s = [h4_t, h4_r, h4_e]
    for i, h4 in enumerate(h4s):
        h4.GetAxis(idims.w).SetRange(wbin,wbin+1)
        h4.GetAxis(idims.q2).SetRange(q2bin,q2bin+1)
        h1 = asrootpy(h4.Projection(idims.t))
        h1.Scale(1, 'width')
        draw(h1,3,2,i+1,fig)
        h1 = asrootpy(h4.Projection(idims.p))
        h1.Scale(1, 'width')
        draw(h1,3,2,i+4,fig)
        h4.GetAxis(idims.w).SetRange(0,-1)
        h4.GetAxis(idims.q2).SetRange(0,-1)
    fig.tight_layout()
    plt.show()

# <codecell>

draw1(5, 15)
# [draw1(3, wbin) for wbin in range(h4_e.GetAxis(idims.w).FindBin(1.9), h4_e.GetAxis(idims.w).FindBin(2.2))]
# [draw1(3, wbin) for wbin in [h4_e.GetAxis(idims.w).FindBin(1.9), h4_e.GetAxis(idims.w).FindBin(2.1), h4_e.GetAxis(idims.w).FindBin(2.3), h4_e.GetAxis(idims.w).FindBin(2.7)]]
wbin = h4_e.GetAxis(idims.w).FindBin(1.9)
wbl = h4_e.GetAxis(idims.w).GetBinLowEdge(wbin)
wbw = h4_e.GetAxis(idims.w).GetBinWidth(wbin)
wbh = wbl + wbw
print('w_%d: %.3f-%.3f, %.3f' % (wbin, wbl, wbh, wbw))
q2bin = 5
q2bl = h4_e.GetAxis(idims.q2).GetBinLowEdge(q2bin)
q2bw = h4_e.GetAxis(idims.q2).GetBinWidth(q2bin)
q2bh = q2bl + q2bw
print('q2_%d: %.3f-%.3f, %.3f' % (q2bin, q2bl, q2bh, q2bw))

# <codecell>

he_mmp = asrootpy(h6_e.Projection(5))
fig = draw(he_mmp)

# <codecell>

h4_e.Print()

# <codecell>


