# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # Electron timing correction sample

# <codecell>

from matplotlib import rc
rc('text', usetex=True)
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec

import ROOT as r

from rootpy.io import root_open
from rootpy.plotting import Hist, Hist2D, HistStack
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt

# <markdowncell>

# File contains example polar graph from e16, produced on ifarm as follows:
# ```
# TFile *_file0 = TFile::Open("/home/ephelps/w/2013_sept/e16_test/e1f6/out.root")
# TChain *h10 = (TChain*)_file0->Get("h10_eskim/h10")
# h10->Draw("TMath::Pi()*(tr_time-(sc_t[sc[0]-1]-sc_r[sc[0]-1]/(b[0]*29.979)))>>hetimepol(100,-3.14159,3.14159)","sc_sect[sc[0]-1]==5 && sc_pd[sc[0]-1]==13","goff")
# TH1 *h = hetimepol
# int npoints = h->GetNbinsX()
# double x[100], y[100]
# for (int xbin = 1; xbin <= npoints; xbin++) {
#     x[xbin-1] = h->GetBinCenter(xbin);
#     y[xbin-1] = h->GetBinContent(xbin);
# }
# TGraphPolar *g = new TGraphPolar(npoints, x, y)
# TFile fout("polar.root","create")
# fout.WriteObject(g, "gp_etime_s5p13")
# fout.Close()
# ```
# where out.root was produced by processH10s.py.

# <codecell>

f = root_open("polar.root",'r')
g = f.gp_etime_s5p13

# <markdowncell>

# ROOT's polar graph is difficult to get right, so get lists

# <codecell>

def getXY(gp, i):
    x, y = r.Double(0), r.Double(0)
    gp.GetPoint(i, x, y)
    return [x,y]

xs,ys = zip(*[getXY(g, i) for i in range(0,g.GetN())])
xs,ys = list(xs), list(ys)

# <codecell>

fig = figure(figsize=(4,4))
ax = fig.add_axes([0.1, 0.1, 0.9, 0.9], polar=True)
ax.scatter(xs, ys)
yticks(arange(0,500,100))

show()

# <codecell>

scatter(xs,ys)

# <markdowncell>

# In this case, then, I don't need 2pi normalization, so revert to native -1 to 1.
# 
# Also, look at full cycles by duplicating range.

# <codecell>

xs = xs+list(np.add(xs,[2*pi]*100))+list(np.add(xs,[-2*pi]*100))
ys = 3*ys

# <codecell>

scatter(xs,ys)

# <codecell>

fig = figure(figsize=(4,4))
ax = fig.add_axes([0.1, 0.1, 0.9, 0.9], polar=True)
ax.scatter(xs, ys)
yticks(arange(0,500,100))

show()

# <rawcell>

# Change back to original range

# <codecell>

xs = [x/pi for x in xs[0:100]]
ys = ys[0:100]

# <codecell>

scatter(xs,ys)

# <markdowncell>

# ## find offset

# <markdowncell>

# Cycle the points and accumulate 4-bin window average in center, where maximum should be in case of good timing.
# 
# Wherever the maximum occurs in the 4-bin window averages corresponds to the offset required to match the electron time with the rf signal, +- 2ns as determined by hadron particle id histograms.

# <codecell>

def crot(x, i):
    if i == 0 or i >= len(x): return x
    return x[-i:-1] + [x[-1]] + x[0:len(x)-i]
binwidth = xs[1]-xs[0]
offsetprob = zip(*[(i*binwidth,average(crot(ys,i)[48:52])) for i in range(0,100)])

# <codecell>

scatter(*offsetprob, color='g')
ylim(0)
maxy = max(offsetprob[1])
offset = [x for x,y in zip(*offsetprob) if y == maxy][0]
axvline(offset, color='r')
annotate('%.3f'%offset, xy=(0.5,0.5), xytext=(0.6,0.4), xycoords='axes fraction', textcoords='axes fraction', color='r', size=16, arrowprops=dict(color='r', facecolor='r', shrink=0.05, width=1))
show()

# <markdowncell>

# ## corrected timing

# <codecell>

xscorrd = np.add(xs,[offset]*len(xs))
xscorrd = [x if x<1 else x-2 for x in xscorrd]
scatter(xscorrd,ys,color='g')
scatter(xs,ys,color='r')
ylim(0)

# <codecell>

fig = figure(figsize=(4,4))
ax = fig.add_axes([0.1, 0.1, 0.9, 0.9], polar=True)
ax.scatter([pi*(x+0.96) for x in xs], ys, color='g')
ax.scatter([pi*x for x in xs], ys, color='r')
yticks(arange(0,500,100))

show()

# <codecell>


