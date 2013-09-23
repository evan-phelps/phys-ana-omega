# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

%pylab inline

# <markdowncell>

# # Data-skim chains
# 
# ## 2013-06-13
# 
# ### Setting things up and testing
# 
# Here I'll start experimenting with the idea of "pythonized" interactive analysis of the data generated by `phys-ana-omega/main.cpp`.  Using IPython Notebook, if proven viable, will provide handy aggregation of notes, $\LaTeX$, and dynamically generated plots and simple HTML export of static snapshots.
# 
# The script, `oana_setup.py`, is responsible for several preliminary setup tasks:
# 
# * load all required libraries, including, for example, all or part of the following modules:
#   + ROOT
#   + rootpy
#   + matplotlib
#   + numpy
# * load references to all data chains and trees
# * build the data hierarcy by assigning *friend* trees/chains to the main *h10* chain
# * load C++ modules that provide functionality from my `projects/phys-ana-omega` codebase
# * set convenience aliases on h10
# * define python-specific helper classes

# <codecell>

WS = '/home/ephelps/projects/phys-ana-omega'
WS_SETUP = '%s/python/oana_setup.py' % WS
%run $WS_SETUP

import itertools as it

import pylab as pl
from epfuncs import f_bwexpgaus
from epfuncs import f_bwexpgaus_pol4
from epfuncs import RejectWrapper
from epxsectutils import vgflux

from math import pi
mpi0, mpi, mrho0, mphi, mk = 0.1349766, 0.139570, 0.77549, 1.019445, 0.493677

# <markdowncell>

# I'll first explore a subset of events selected from the $p \pi^{+} X$ topology (see *el1* `EventList` for cut conditions).  I'll also define a cut string that allows me to look into one $W,Q^{2}$ bin with boundaries consistent with one bin of the 6D histogram produced by `projects/phys-ana-omega/h6maker.h`.

# <codecell>

h10.SetEntryList(el1)
cut_1bin  = cuts( ('W',1.98,2.00), ('Q2',1.8,2.1) )
cut_5bins = cuts( ('W',1.98,2.08), ('Q2',1.8,2.1) )
n2proc = None

# <codecell>

ostrs = ['lf.omega.'+ostr for ostr in ['E()', 'Px()', 'Py()', 'Pz()']]
pipstrs = ['sqrt(p[h10idx_pip]^2+%f^2)' % mk, 'p[h10idx_pip]*cx[h10idx_pip]', 'p[h10idx_pip]*cy[h10idx_pip]', 'p[h10idx_pip]*cz[h10idx_pip]']
dstr = 'sqrt((%s-%s)^2-(%s-%s)^2-(%s-%s)^2-(%s-%s)^2)' % (ostrs[0], pipstrs[0], ostrs[1], pipstrs[1], ostrs[2], pipstrs[2], ostrs[3], pipstrs[3])
print(dstr)
htop1_k = rh1('%s:mmp' % dstr, cut_5bins, 'htop1_k', binning=(80, 0.4, 1.2, 110, -0.2, 2.0), N=n2proc)

# <codecell>

htop1_k.Draw('colz')

# <codecell>

htop1 = rh1('k.mmppip:k.mmp', '', 'htop1', binning=(160, 0.4, 2.0, 180, 0.2, 2.0), N=n2proc)
htop1_20MeV = rh1('k.mmppip:k.mmp', cut_1bin, 'htop1_1bin', binning=(80, 0.4, 1.2, 80, 0.2, 1.0), N=n2proc)
htop1_100MeV = rh1('k.mmppip:k.mmp', cut_5bins, 'htop1_5bins', binning=(80, 0.4, 1.2, 80, 0.2, 1.0), N=n2proc)

# <codecell>

fig_top_20_100 = draw(htop1, 3, 2, 1, figsize=(15, 10))
fig_top_20_100 = draw(htop1_100MeV, 3, 2, 2, fig_top_20_100)
for i in range(2,7):
    plt.axhline(i*mpi, color='black', lw=2.0, ls='dashed')
    plt.axvline((i+1)*mpi, color='black', lw=2.0, ls='dashed')
plt.axvline(mphi, color='red', lw=2.0, ls='dashed')
fig_top_20_100 = draw(htop1_20MeV, 3, 2, 3, fig_top_20_100)
(hmmp, hmmp_100MeV, hmmp_20MeV) = [asrootpy(h.ProjectionX()) for h in [htop1, htop1_100MeV, htop1_20MeV]]
fig_top_20_100 = draw(hmmp, 3, 2, 4, fig_top_20_100)
plt.axvline(0.547, color='purple', lw=2.0, ls='dashed')
plt.axvline(0.783, color='green', lw=2.0, ls='dashed')
plt.axvline(0.958, color='blue', lw=2.0, ls='dashed')
plt.axvline(mphi, color='red', lw=2.0, ls='dashed')
plt.axvline(1.285, color='yellow', lw=2.0, ls='dashed')
fig_top_20_100 = draw(hmmp_100MeV, 3, 2, 5, fig_top_20_100)
plt.axvline(0.547, color='purple', lw=2.0, ls='dashed')
plt.axvline(0.783, color='green', lw=2.0, ls='dashed')
plt.axvline(0.958, color='blue', lw=2.0, ls='dashed')
plt.axvline(mphi, color='red', lw=2.0, ls='dashed')
fig_top_20_100 = draw(hmmp_20MeV, 3, 2, 6, fig_top_20_100)
plt.axvline(0.547, color='purple', lw=2.0, ls='dashed')
plt.axvline(0.783, color='green', lw=2.0, ls='dashed')
plt.axvline(0.958, color='blue', lw=2.0, ls='dashed')
plt.axvline(mphi, color='red', lw=2.0, ls='dashed')
for ax in fig_top_20_100.axes[:3]:
    ax.set_ylabel("$MM_{p\pi^{+}}$ (GeV)") #, horizontalalignment="right", x=1, labelpad=20)
    ax.set_xlabel("$MM_{p}$ (GeV)") #, horizontalalignment="right", y=1, labelpad=32)

# <markdowncell>

# * purple: eta
# * green: omega
# * blue: eta'
# * red: phi
# * yellow: f2

# <codecell>

hphi = rh1('k.phi', '', 'hphi', binning=(360, -pi, pi))
hphi_20MeV = rh1('k.phi', cut_1bin, 'hphi_1bin', binning=(360, -pi, pi))
hphi_100MeV = rh1('k.phi', cut_5bins, 'hphi_5bins', binning=(360, -pi, pi))

# <codecell>

def phist(h):
    print('   name: %s' % h.GetName())
    print('  title: %s' % h.GetTitle())
    print('entries: %d' % h.GetEntries())

# <codecell>

phist(hphi_20MeV)
phist(hphi_100MeV)

# <codecell>

hang = rh1('ct:phi', '', 'hang', binning=(25, -pi, pi, 25, -1, 1))
hang_20MeV = rh1('ct:phi', cut_1bin, 'hang_1bin', binning=(15, -pi, pi, 20, -1, 1))
hang_100MeV = rh1('ct:phi', cut_5bins, 'hang_5bins', binning=(15, -pi, pi, 20, -1, 1))

# <codecell>

fig_ang_20_100 = draw(hang, 3, 2, 1, figsize=(15,10))
fig_ang_20_100 = draw(hang_100MeV, 3, 2, 2, fig_ang_20_100)
fig_ang_20_100 = draw(hang_20MeV, 3, 2, 3, fig_ang_20_100)
fig_ang_20_100 = draw(hphi, 3, 2, 4, fig_ang_20_100)
fig_ang_20_100 = draw(hphi_100MeV, 3, 2, 5, fig_ang_20_100)
fig_ang_20_100 = draw(hphi_20MeV, 3, 2, 6, fig_ang_20_100)

# <markdowncell>

# ## 2013-06-14
# 
# ### Sideband subtraction walk-through
# 
# With a simplified fitting scheme, to be replaced with the previously developed signal+background fitting scheme, I'll walk through the sideband subtraction process for a single $W, Q^{2}$ bin.

# <codecell>

etarange = (0.5, 0.6)
omegarange = (0.74, 0.85)
fitrange = (0.4, 0.975)
fbgskip = [etarange, omegarange]

# <codecell>

def fitbg(h):
    # get function with fbgskip regions neglected in fit
    bgwrapper = RejectWrapper(r.TF1('fbg', 'pol4', 0.4, 1.2), fbgskip)
    fbgrej = bgwrapper.newtf1
    fbg = bgwrapper.tf1
    h.Fit(fbgrej, '', '', *fitrange)
    # get yaxis maximum
    r.gPad.Update()
    ymax = r.gPad.GetFrame().GetY2()
    #lines at edges of skip-ranges
    vlines = [r.TLine(x,0,x,ymax) for x in it.chain(*fbgskip)]
    for l in vlines: l.Draw()
    r.gPad.Update()
    return fbg

# <markdowncell>

# #### Simplified P4 background fit
# 
# ... for 100- and 20-MeV W ranges as defined above.

# <codecell>

xs = pl.arange(0.4, 0.975, 0.001)
fbg_100 = fitbg(hmmp_100MeV)
ys = pl.array([fbg_100.Eval(x) for x in xs])
fig = draw(hmmp_100MeV, 2, 1, 1, figsize=(15, 5))
plot(xs, ys, linewidth=1.0, color='red')
plt.axvline(0.783, color='g', lw=2.0, ls='dashed')
hmmp_100MeV_bgs = asrootpy(hmmp_100MeV.Clone('hmmp_100MeV_bgs'))
hmmp_100MeV_bgs.Add(fbg_100, -1)
for b in it.chain(*[range(hmmp_100MeV.FindBin(0.975), hmmp_100MeV.GetNbinsX()+1),
                    range(1, hmmp_100MeV.FindBin(0.723))]):
    hmmp_100MeV_bgs.SetBinContent(b,0)
    hmmp_100MeV_bgs.SetBinError(b,0)
hmmp_100MeV_bgs.markercolor = 'green'
hmmp_100MeV_bgs.linecolor = 'green'
fig = draw(hmmp_100MeV_bgs, 2, 1, 1, fig)

fbg_20 = fitbg(hmmp_20MeV)
ys = pl.array([fbg_20.Eval(x) for x in xs])
fig = draw(hmmp_20MeV, 2, 1, 2, fig)
plot(xs, ys, linewidth=1.0, color='red')
plt.axvline(0.783, color='g', lw=2.0, ls='dashed')
hmmp_20MeV_bgs = asrootpy(hmmp_20MeV.Clone('hmmp_20MeV_bgs'))
hmmp_20MeV_bgs.Add(fbg_20, -1)
for b in it.chain(*[range(hmmp_20MeV.FindBin(0.975), hmmp_20MeV.GetNbinsX()+1),
                    range(1, hmmp_20MeV.FindBin(0.723))]):
    hmmp_20MeV_bgs.SetBinContent(b,0)
    hmmp_20MeV_bgs.SetBinError(b,0)
hmmp_20MeV_bgs.markercolor = 'green'
hmmp_20MeV_bgs.linecolor = 'green'
fig = draw(hmmp_20MeV_bgs, 2, 1, 2, fig)
for ax in fig.axes:
    ax.grid(True)
    ax.set_autoscaley_on(False)
    ax.set_ylim([0,ax.get_ylim()[1]])
    ax.set_xlabel('$MM_{p}$ (GeV)')

# <codecell>

hmmp_20MeV_bgs.Draw()

# <markdowncell>

# Note the small jump around 1 GeV:  Could this be $\phi(1020)$?
# <table>
#     <tr><th>$\Gamma_{i}$</th><th>mode</th><th>fraction $(\Gamma_{i}/\Gamma)$</th><th>p (MeV/c)</th></tr>
#     <tr><td>$\Gamma_{1}$</td><td>$K^{+}K^{-}$</td><td>(48.9 ± 0.5) %</td><td>127</td></tr>
#     <tr><td>$\Gamma_{2}$</td><td>$K_{L}^{+}K_{L}^{-}$</td><td>(34.2 ± 0.4) %</td><td>110</td></tr> 
#     <tr><td>$\Gamma_{3}$</td><td>$\rho\pi$<br/>$+\pi^{+}\pi^{-}\pi^{0}$</td><td>(15.32 ± 0.32) %</td><td>178<br/>462</td></tr>
# </table>
# This could be from kaons misidentified as pions or from the physical $\Gamma_{3}$ decay mode, which, however, is kinematically suppressed in this case.  *(Look at plots under assumption that all pions are kaons, which will also give an idea of background shape due to pion-kaon misidentification.)*

# <codecell>

mmppip_phi_thresh = mrho0 + 3*mpi
# mmppip_phi_thresh = 2*mk-mpi
fig_top_20_100 = draw(htop1, 3, 1, 1, figsize=(15, 5))
plt.axvline(mphi, color='black', lw=2.0, ls='dashed')
plt.axhline(mmppip_phi_thresh, color='black', lw=2.0, ls='dashed')
fig_top_20_100 = draw(htop1_100MeV, 3, 1, 2, fig_top_20_100)
plt.axvline(mphi, color='black', lw=2.0, ls='dashed')
plt.axhline(mmppip_phi_thresh, color='black', lw=2.0, ls='dashed')
fig_top_20_100 = draw(htop1_20MeV, 3, 1, 3, fig_top_20_100)
plt.axvline(mphi, color='black', lw=2.0, ls='dashed')
plt.axhline(mmppip_phi_thresh, color='black', lw=2.0, ls='dashed')
for ax in fig_top_20_100.axes:
    ax.set_ylabel("$MM_{p\pi^{+}}$ (GeV)") #, horizontalalignment="right", x=1, labelpad=20)
    ax.set_xlabel("$MM_{p}$ (GeV)") #, horizontalalignment="right", y=1, labelpad=32)

# <markdowncell>

# 
# The two simplest sideband weighting schemes:
# 
# * use integral of background function in each region -- low sideband, high sideband, and signal region -- to determine weights
# * use integral of background function in signal region but integral of bin content in sidebands to determine weights

# <codecell>

def bgInt(x0, x1, f=fbg_100, h=hmmp_100MeV):
    b0, b1 = h.FindBin(x0), h.FindBin(x1)
    x0, x1 = h.GetBinLowEdge(b0), h.GetBinLowEdge(b1+1)
    return (f.Integral(x0, x1), h.Integral(b0, b1, 'width'))

gsig = 0.0135 # sigma of gauss fit on 20MeV hist from 0.76 to 0.805
cwidth = 3
x0, x1, x2, x3 = 0.6, 0.783 - cwidth*gsig, 0.783 + cwidth*gsig, 0.95
print(x0,x1,x2,x3)
int01, int12, int23 = bgInt(x0, x1)[0], bgInt(x1, x2)[0], bgInt(x2, x3)[0]
print(int01)
print(int12)
print(int23)
factorSB = (int01 + int23)/int12
print(factorSB) # need this for each q2, w bin

# <codecell>

cut_5bins_SB = cut_5bins + ' && ((' + cuts(('mmp',x0,x1)) + ') || (' + cuts(('mmp',x2,x3)) + '))'
cut_5bins_sig = cut_5bins + ' && (' + cuts(('mmp',x1,x2)) + ')'
print(cut_5bins_SB)
print(cut_5bins_sig)

# <codecell>

hang_5bins_SB = rh1('ct:phi', cut_5bins_SB, 'hang_5bins_SB', binning=(15, -pi, pi, 15, -1, 1))
hang_5bins_sig = rh1('ct:phi', cut_5bins_sig, 'hang_5bins_sig', binning=(15, -pi, pi, 15, -1, 1))

# <codecell>

fig = draw(hang_5bins_SB, 2, 1, 1, figsize=(10, 5))
fig = draw(hang_5bins_sig, 2, 1, 2, fig)

# <codecell>

c1 = r.TCanvas()
c1.Divide(2)
c1.cd(1)
hang_5bins_SB.Draw('colz')
c1.cd(2)
hang_5bins_sig.Draw('colz')

# <codecell>

hang_sbs = hang_5bins_sig.Clone('hang_sbs')
hang_sb = hang_5bins_SB.Clone('hang_sb')
weight_sb = 1/3.2
hang_sb.Scale(weight_sb)
hang_sbs.Add(hang_sb, -1)
c2 = r.TCanvas()
c2.Divide(3)
c2.cd(1)
hang_sb.Draw('colz')
c2.cd(2)
hang_5bins_sig.Draw('colz')
c2.cd(3)
hang_sbs.Draw('colz')

# <codecell>

import rootplot
import rootplot.root2matplotlib as r2m

def draw(hist, ncols=1, nrows=1, cell=1, fig=None, figsize=(10, 5)):
    if fig is None:
        fig = plt.figure(figsize=figsize, dpi=100, facecolor='white')
    else:
        plt.figure(fig.number)
    subplot = fig.add_subplot(nrows, ncols, cell)
    subplot.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
    if isinstance(hist, r.TH2):
        # rplt.hist2d(hist, axes=subplot)
        r2m.Hist2D(hist).colz()
    else:
        rplt.errorbar([hist], xerr=False, emptybins=False)
    # plt.show()
    return fig

# <codecell>

fig = draw(hang_sb, 3, 1, 1, figsize=(18, 5))
fig = draw(hang_5bins_sig, 3, 1, 2, fig)
fig = draw(hang_sbs, 3, 1, 3, fig)

# <codecell>


# <markdowncell>

# ## 2013-06-17
# 
# I couldn't figure out how to get the TChain's tree offset, so I'll create new entry lists from the single `tkin` tree and add friends to that tree rather than to the h10 chain.

# <codecell>

WS = '/home/ephelps/projects/phys-ana-omega'
WS_SETUP = '%s/python/oana_setup_friendInvert.py' % WS
%run $WS_SETUP

import itertools as it

import pylab as pl
from epfuncs import f_bwexpgaus
from epfuncs import f_bwexpgaus_pol4
from epfuncs import RejectWrapper
from epxsectutils import vgflux

from math import pi
mpi0, mpi, mrho0, mphi, mk = 0.1349766, 0.139570, 0.77549, 1.019445, 0.493677

from array import array

# <codecell>


# <codecell>


# <codecell>

tkin.Draw(">>el1_single", "top1pass && mmthreshpass && ccpass && fidpass", "entrylist")

# <codecell>

el1 = gROOT.FindObject('el1_single')

# <codecell>

tkin.SetEntryList(el1)
cut_1bin  = cuts( ('W',1.98,2.00), ('Q2',1.8,2.1) )
cut_5bins = cuts( ('W',1.98,2.08), ('Q2',1.8,2.1) )

# <codecell>

htop1 = rh1('mmppip:mmp', '', 'htop1', binning=(160, 0.4, 2.0, 180, 0.2, 2.0), t=tkin)
htop1_20MeV = rh1('mmppip:mmp', cut_1bin, 'htop1_1bin', binning=(80, 0.4, 1.2, 80, 0.2, 1.0), t=tkin)
htop1_100MeV = rh1('mmppip:mmp', cut_5bins, 'htop1_5bins', binning=(80, 0.4, 1.2, 80, 0.2, 1.0), t=tkin)

# <codecell>

fig_top_20_100 = draw(htop1, 3, 2, 1, figsize=(15, 10))
fig_top_20_100 = draw(htop1_100MeV, 3, 2, 2, fig_top_20_100)
fig_top_20_100 = draw(htop1_20MeV, 3, 2, 3, fig_top_20_100)
(hmmp, hmmp_100MeV, hmmp_20MeV) = [asrootpy(h.ProjectionX()) for h in [htop1, htop1_100MeV, htop1_20MeV]]
fig_top_20_100 = draw(hmmp, 3, 2, 4, fig_top_20_100)
fig_top_20_100 = draw(hmmp_100MeV, 3, 2, 5, fig_top_20_100)
fig_top_20_100 = draw(hmmp_20MeV, 3, 2, 6, fig_top_20_100)
for ax in fig_top_20_100.axes[:3]:
    ax.set_ylabel("$MM_{p\pi^{+}}$ (GeV)") #, horizontalalignment="right", x=1, labelpad=20)
    ax.set_xlabel("$MM_{p}$ (GeV)") #, horizontalalignment="right", y=1, labelpad=32)

# <codecell>

print('%d events to process' % el1.GetN())

# <codecell>

htmp = Hist2D(160, 0.4, 2.0, 180, 0.2, 2.0)
tkin.SetBranchStatus('*', 0)
tkin.SetBranchStatus('e', 1)
tkin.SetBranchStatus('p', 1)
tkin.SetBranchStatus('pip', 1)

# <codecell>

omega = r.TLorentzVector()
pip = r.TLorentzVector()

# <codecell>

# force branch caching
# x, y = array('f', [0]), array('f', [0])
# tkin.SetBranchAddress('mmp', x)
tkin.SetBranchAddress('mmppip', y)
tkin.SetBranchAddress('pf.omega', r.AddressOf(omega))
tkin.SetBranchAddress('pf.pip', r.AddressOf(pip))
for idx_entry in range(0, el1.GetN()):
    idx_event = el1.GetEntry(idx_entry)
    tkin.GetEntry(idx_event)
    if idx_entry > 10: break
    if eventnum % 250000 == 0:
        print('%d processed' % eventnum)
    print(omega.M())
    # htmp.Fill(x[0], y[0])

# <codecell>

htmp.Draw('colz')

# <codecell>

tkin.Print()

# <codecell>

el1.Print()

# <codecell>

print(el1)

# <codecell>

tkin.GetEntry(el1.GetEntry(1))

# <codecell>


# <codecell>

array(

