# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

%run ../scratch_intQ2.py
# %run /home/ephelps/lhcbstyle.py
import ROOT as r
r.gROOT.ProcessLine('.x /home/ephelps/Dropbox/Notebooks/omega/Miscellany/Linux_OS/styPUB.C')
r.gROOT.SetStyle("PUB")
# import matplotlib as mplt
# mplt.use('TkAgg')
import operator as op
from rootpy.plotting import Hist, HistStack, Canvas
import pandas as pd
ress = pd.DataFrame(doeach(trim=False))
dffits = pd.concat([df.W, df.Q2]+[ress[i] for i in [0,1,2,4,5]], axis=1)
for i in range(84,91): print(dffits.W[i],dffits.Q2[i],[dffits[0][i].GetParameter(j) for j in range(0,5)])
dffits.columns = ['W', 'Q2', 'fitfunc', 'flegs', 'ft', 'h', 'status']
dfQ2s = [dffits[['W','Q2','fitfunc','status']][dffits.Q2==Q2] for Q2 in dffits.Q2.unique()]
# each q2 in q2s contains dffit records for W = 1.79, 1.91, 2.03, 2.15, 2.27
# wselect = [0,6,12,18,24]
wselect = [0,12,24]
q2s = [dffits[(dffits.Q2==q2) & (dffits.W>1.78) & (dffits.W<2.28)].iloc[wselect] for q2 in dffits.Q2.unique()]

# <codecell>

# %run /home/ephelps/lhcbstyle.py
import operator as op
from rootpy.plotting import Hist, HistStack, Canvas
import pandas as pd
ress = pd.DataFrame(doeach(trim=False))
dffits = pd.concat([df.W, df.Q2, df.Wlo, df.Whi, df.Q2lo, df.Q2hi]+[ress[i] for i in [0,1,2,4,5]], axis=1)
for i in range(84,91): print(dffits.W[i],dffits.Q2[i],[dffits[0][i].GetParameter(j) for j in range(0,5)])
dffits.columns = ['W', 'Q2', 'Wlo', 'Whi', 'Q2lo', 'Q2hi', 'fitfunc', 'flegs', 'ft', 'h', 'status']
dfQ2s = [dffits[['W','Q2','Wlo','Whi','Q2lo','Q2hi','fitfunc','status']][dffits.Q2==Q2] for Q2 in dffits.Q2.unique()]
# each q2 in q2s contains dffit records for W = 1.79, 1.91, 2.03, 2.15, 2.27
# wselect = [0,6,12,18,24]
wselect = [0,12,24]
q2s = [dffits[(dffits.Q2==q2) & (dffits.W>1.78) & (dffits.W<2.28)].iloc[wselect] for q2 in dffits.Q2.unique()]

# <codecell>

import rootpy.plotting.root2matplotlib as rplt
import matplotlib.pyplot as plt
from rootpy import asrootpy

# <codecell>

from matplotlib.patches import PathPatch
from matplotlib import rc
rc('text', usetex=True)

fig = plt.figure(dpi=1200, figsize=(14,10))
pnum = 1
for q2_xxxx in op.itemgetter(1,3,5)(q2s):
    for w, wlo, whi, q2, q2lo, q2hi, h in zip(q2_xxxx.W, q2_xxxx.Wlo, q2_xxxx.Whi,
                                              q2_xxxx.Q2, q2_xxxx.Q2lo, q2_xxxx.Q2hi, asrootpy(q2_xxxx.h)):
        sp = plt.subplot(3, 3, pnum)
        pnum += 1
        rplt.errorbar(h, xerr=False)
        if pnum-1 in [7,8,9]: plt.xlabel(r'$\mathrm{cos}\,\theta^{*}$', fontsize=18)
        # if pnum-1 in [1,4,7]: plt.ylabel(r'$\frac{d\sigma}{d(\mathrm{cos}\,\theta^*\!)}\,\mathrm{(nb)}$', fontsize=22)
        if pnum-1 in [1,4,7]: plt.ylabel(r'$\displaystyle\frac{d\sigma}{d(\mathrm{cos}\,\theta^*\!)}\,\mathrm{(nb)}$', fontsize=16)
        ymax = h.GetMaximum()
        plt.ylim(ymin=-0.15*ymax)
        y0, y1 = plt.ylim()
        x0, x1 = plt.xlim()
        xr, yr = x1-x0, y1-y0
        plt.text(x0+0.183*xr, y0+0.85*yr, r'$W = [%.3f, %.3f)\,\mathrm{GeV}$'%(wlo,whi),
                 fontsize=16, horizontalalignment='left', verticalalignment='bottom')
        plt.text(x0+0.175*xr, y0+0.725*yr, r'$Q^2 = [%.3f, %.3f)\,\mathrm{GeV^2}$'%(q2lo,q2hi),
                 fontsize=16, horizontalalignment='left', verticalalignment='bottom')
        f3 = h.GetListOfFunctions()[2]
        X = np.linspace(-1,1, num=500)
        Y = [f3.Eval(x) for x in X]
        # plt.plot(X,Y,'.r')
        p = plt.fill_between(X, 0, Y)
        p.set_facecolors('none')
        for path in p.get_paths():
            p1 = PathPatch(path, fc='none', hatch='\\\\\\', color='red')
            sp.add_patch(p1)
            p1.set_zorder(p.get_zorder()-0.1)
        f1 = h.GetListOfFunctions()[0]
        Y = [f1.Eval(x) for x in X]
        plt.plot(X,Y,'-k')
        f2 = h.GetListOfFunctions()[1]
        Y = [f2.Eval(x) for x in X]
        plt.plot(X,Y,'--b', lw=2)
plt.tight_layout()
plt.text(-5.5,2600,'PRELIMINARY',fontsize=108,rotation=30,alpha=0.175)
plt.text(-3.75,1600,'PRELIMINARY',fontsize=108,rotation=30,alpha=0.175)
plt.savefig('cost_samples_tmp.pdf')
plt.savefig('cost_samples_tmp.eps')

# <codecell>

dfm = pd.read_table('/home/ephelps/Dropbox/Notebooks/omega/Projects/Omega/Cross-sections/Morand/xsect_integrated_morand.txt')

# <codecell>

plt.figure(dpi=600, figsize=(14,7))
wlim = 2.7
ylim = 3000
picks = (df.Q2==1.7) & (df.fsigchi2<3) & (df.W<wlim)
plt.errorbar(df[picks].W, df[picks].xsect, df[picks].err, fmt='D', fillstyle='full', ms=5, )
picks = (df.Q2==2.25) & (df.fsigchi2<3) & (df.W<wlim)
plt.errorbar(df[picks].W, df[picks].xsect, df[picks].err, fmt='s', fillstyle='full', ms=5)
picks = (df.Q2==3.03) & (df.fsigchi2<3) & (df.W<wlim)
plt.errorbar(df[picks].W, df[picks].xsect, df[picks].err, fmt='o', fillstyle='full', ms=5)
picks = (dfm.Q2<1.8) & (dfm.W<wlim)
plt.errorbar(dfm.W[picks].values, dfm.xsect[picks].values, dfm.error[picks].values, fmt='*', ms=15)
picks = (dfm.Q2>2.3) & (dfm.Q2<2.4) &(dfm.W<wlim)
plt.errorbar(dfm.W[picks].values, dfm.xsect[picks].values, dfm.error[picks].values, fmt='*', ms=15)
picks = (dfm.Q2>2.76) & (dfm.Q2<3.3) &(dfm.W<wlim)
plt.errorbar(dfm.W[picks].values, dfm.xsect[picks].values, dfm.error[picks].values, fmt='*', ms=15)
plt.ylim([0,2500])

# <codecell>

import matplotlib.gridspec as gridspec
rc('text', usetex=True)
plt.figure(dpi=1200, figsize=(14,4.5))
gs = gridspec.GridSpec(1,8)
wlim = 2.7
ylim = 3000
picks = (df.Q2==2.25) & (df.fsigchi2<3) & (df.W<wlim)
# sp = plt.subplot(1, 3, 1)
sp = plt.subplot(gs[0,0:4], alpha=0.0)
plt.errorbar(df[picks].W, df[picks].xsect, df[picks].err, label=r'$Q^2\! = [2.100,2.400)\,\mathrm{GeV^2\!,}\,\,\mathrm{Phelps, E1F}$', fmt='ok', fillstyle='none', ms=8)
picks = (dfm.Q2>2.3) & (dfm.Q2<2.4) &(dfm.W<wlim)
plt.errorbar(dfm.W[picks].values, dfm.xsect[picks].values, dfm.error[picks].values, label=r'$Q^{2}\! = [2.200,2.500)\,\mathrm{GeV^2\!,}\,\,\mathrm{Morand, E16}$', fmt='db', fillstyle='full', ms=10, lw=1)
plt.xlabel(r'$W \mathrm{(GeV)}$', fontsize=18)
plt.ylabel(r'$\sigma \mathrm{(nb)}$', fontsize=18)
plt.legend(fontsize=14).get_frame().set_linewidth(1)
plt.ylim([0,2000])
sp = plt.subplot(gs[0,4:])
picks = df.W==1.89
plt.errorbar(df[picks].Q2, df[picks].xsect, df[picks].err, label=r'$W = [1.880,1.900)\,\mathrm{GeV,}\,\,\mathrm{Phelps,\,\,E1F}$', fmt='ok', fillstyle='none', ms=8)
plt.xlabel(r'$Q^2 \mathrm{(GeV^2)}$', fontsize=18)
plt.ylabel(r'$\sigma \mathrm{(nb)}$', fontsize=18)
picks = (dfm.W==1.85) | (dfm.W==1.86)
plt.errorbar(dfm[picks].Q2, dfm[picks].xsect, dfm[picks].error, label=r'$\left<W\right> \approx 1.85\,\mathrm{GeV\!,}\,\,\mathrm{Morand,\,\,E16}$', fmt='db', fillstyle='full', ms=10)
plt.legend(fontsize=14).get_frame().set_linewidth(1)
plt.ylim([0,2000])
plt.xlim([1.5,5.5])
plt.tight_layout()
plt.text(-2.5,1300,'PRELIMINARY',fontsize=48,rotation=30,alpha=0.175)
plt.text(2.25,1300,'PRELIMINARY',fontsize=48,rotation=30,alpha=0.175)
# plt.xlim([1.5,5.5])
# sp = plt.subplot(1, 3, 3)
# picks = df.W==2.23
# plt.errorbar(df[picks].Q2, df[picks].xsect, df[picks].err, fmt='s', fillstyle='full', ms=5)
# plt.ylim([0,1000])
# plt.xlim([1.5,5.5])
plt.savefig('xsects_wq2_sample.eps')
plt.savefig('xsects_wq2_sample.pdf')

# <codecell>

arange(1.6,3.1,0.3)

# <codecell>

np.sort(dfm.Q2.unique())

# <codecell>

#np.sort(dfm.W.unique())
np.sort(dfm.Q2[(dfm.W==1.85) | (dfm.W==1.86)])

# <codecell>

df[df.fsigchi2<5].fsigchi2.hist(bins=30)

# <codecell>

dfm[picks].W  #, dfm[picks].xsect, dfm[picks].error

# <codecell>

df[['Q2lo','Q2hi']][df.Q2==2.25]

# <codecell>

dfm.sort(columns='Q2')

# <codecell>

(3.1-1.6)/5

# <codecell>


# <codecell>


