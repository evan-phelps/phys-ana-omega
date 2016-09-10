# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

cfgs = {1:['/data/jobs/skim.20140102/skim.e16.hists.root', 'echists_raw', 'e16.exp', 0.25, 0.7, 4],  # ['EC_Hists_e16.root', 'echists', 'e16.exp', 0.25, 0.7, 4.5],
        2:['/data/jobs/skim.20140102/skim.e1f.hists.root', 'echists_raw', 'e1f.exp', 0.25, 0.7, 4],  # ['EC_Hists_e1f.root', 'echists_raw', 'e1f.exp', 0.25, 0.7, 4.5],
        3:['EC_Hists_e16.sim.root', 'echists_raw', 'e16.sim', 0.2, 0.9, 3.5],
        4:['EC_Hists_e1f.sim.root', 'echists_raw', 'e1f.sim', 0.2, 0.9, 3.5]}
    
finname, din, imgtmpl, losfapprox, fitxlo, fitxhi = _,_,_,_,_,_
def pick(pickme):
    global finname, din, imgtmpl, losfapprox, fitxlo, fitxhi
    finname, din, imgtmpl, losfapprox, fitxlo, fitxhi = cfgs[pickme]
    
pick(2)

# <codecell>

import sys
write = sys.stdout.write

# import warnings
# warnings.filterwarnings('ignore')

from matplotlib import rc
rc('text', usetex=True)
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec

import ROOT as r

from rootpy.io import root_open
from rootpy.plotting import Hist, Hist2D, HistStack
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt

goldenaspect=lambda x, rows=1, cols=1: (x,0.95*x/cols*rows/1.618)

def draw(hist, ncols=1, nrows=1, cell=1, fig=None,
         figsize=goldenaspect(3), **kwargs):
    if fig is None:
        fig = figure(figsize=figsize, dpi=1200, facecolor='white')
    else:
        figure(fig.number)
    subplot = fig.add_subplot(nrows, ncols, cell)
    #subplot.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
    #subplot.ticklabel_format(scilimits=(0, 0))
    if isinstance(hist, r.TH2):
        rplt.hist2d(hist, axes=subplot, **kwargs)
    else:
        rplt.errorbar([hist], xerr=False, emptybins=False, **kwargs)
    # plt.show()
    return fig

# <codecell>

fechists = root_open(finname)
for h in fechists[din]: print(h)
hs_ec_oVi = HistStack(hists=[asrootpy(fechists[din].Get('heo_V_ei_clean_anti_%d'%sector)) for sector in range(1,7)])
hs_ec_sfVp = HistStack(hists=[asrootpy(fechists[din].Get('hsf_V_p_clean_%d'%sector)) for sector in range(1,7)])

# <codecell>

for hs in [hs_ec_oVi, hs_ec_sfVp]:
    for h in hs:
        (imax, jmax) = shape(h)
        write('clearing %s ...' % h)
        for i in range(0, imax): h[i][0] = 0
        for j in range(0, jmax): h[0][j] = 0
        print('CLEARED!')
        #h.SetOption('col')
        #h.GetXaxis().SetRangeUser(0.02,0.1)
        #h.GetYaxis().SetRangeUser(0.02, 0.1)
        #h.SetMaximum(0.5*h.GetMaximum())

# <codecell>

fgaus = r.TF1('fgaus', 'gaus', 0, 0.3)
nr, nc = 2, 3
fig = figure(figsize=goldenaspect(6.5,nr,nc))
gs = gridspec.GridSpec(nr, nc, wspace=0, hspace=0, left=0.1, right=0.95, bottom=0.175)
for irow in range(0,nr):
    for icol in range(0,nc):
        hnum = 3*irow+(icol+1)
        h2 = hs_ec_oVi[hnum-1]
        hx = h2.ProjectionX()
        hx.Fit(fgaus, 'N0', '', 0.01, 0.035)
        ax = subplot(gs[irow, icol])
        rplt.hist2d(h2, axes=ax, norm=LogNorm())
        X = linspace(0, 0.1, 100)
        fscale = fgaus.GetParameter(0)/(0.9*0.3)
        plot(X, [fgaus.Eval(x)/fscale for x in X],
             'k-', linewidth=0.5)
        cutoff = fgaus.GetParameter(1)+5*fgaus.GetParameter(2)
        #plot([0.13,cutoff], [0,cutoff], 'r-')
        #plot([cutoff,cutoff], [cutoff,0.3], 'r-')
        plot([cutoff,cutoff], [0,0.3], 'r-')
#         text(0.9, 0.85, 'sector %d'%hnum, fontsize=12, color='black', transform=ax.transAxes,
#              verticalalignment='top', horizontalalignment='right', bbox=dict(facecolor='gray', pad=5))
        print('%d,%.3f' % (hnum, cutoff))
#         annotate('(%.3f, %.3f) GeV'%(cutoff,cutoff), xy=(cutoff, cutoff),
#                  xytext=(+0.075, +0.2), fontsize=8,
#                  arrowprops=dict(arrowstyle="->",
#                                  connectionstyle="arc3,rad=.2"),
#                  bbox=dict(edgecolor='gray', facecolor='lightgray', alpha=0.85))
        if icol > 0:
            ax.set_yticklabels('')
        else:
            yticks(arange(0.0, 0.30, 0.1))
            if irow == 0:
                yticks(arange(0.0, 0.31, 0.1))
        if irow+1 < nr:
            ax.set_xticklabels('')
        else:
            xticks(arange(0.0, 0.30, 0.1))
            if icol+1 == nc:
                xticks(arange(0.0, 0.31, 0.10))
bigax = fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
xticks([])
yticks([])
ylabel(r'E$_{outer}$ (GeV)')
xlabel(r'E$_{inner}$ (GeV)')
savefig('eid_ei_cut.%s.pdf'%imgtmpl)

# <codecell>

def get_pointgen(gr, xlo, xhi):
    def genpoints():
        for x, y in zip(gr.x(), gsfmeans.y()):
            if x > xlo and x < xhi and y > 0:
                yield (x, y)
    return genpoints

def get_xslices(h2, window=1):
    hs = []
    for xbin in range(1, h2.GetNbinsX()+1):
        h = h2.ProjectionY('%s_%d'%(h2.GetName(),xbin), xbin, xbin+window-1)
        if h.Integral() < 10:
            h = None
        hs.append((h2.GetXaxis().GetBinCenter(xbin),h))
    return hs

def get_peak_points(h2, xlo, xhi, ylo=0):
    points = []
    hs = get_xslices(h2)
    pfinder = r.TSpectrum(1)
    for h in hs:
        if h[1] is not None and h[0]>xlo and h[0]<xhi:
            pfinder.Search(h[1], 2.5, 'goff')
            y = pfinder.GetPositionX()[0]
            if y > ylo:
                points.append((h[0],pfinder.GetPositionX()[0]))
    return points

fgaus = r.TF1('fgaus', 'gaus', 0.2, 0.4)
def get_peak_points_with_error(h2, xlo, xhi, ylo=0, window=1):
    xvals = []
    yvals = []
    yerrs = []
    chi2s = []
    sgmas = []
    serrs = []
    hs = get_xslices(h2, window)
    pfinder = r.TSpectrum(1)
    for h in hs:
        if h[1] is not None and h[0]>xlo and h[0]<xhi:
            pfinder.Search(h[1], 2.5, 'goff')
            y = pfinder.GetPositionX()[0]
            if y > ylo:
                fgaus.SetParameter(1, y)
                fgaus.SetParLimits(1, y-0.02, y+0.02)
                h[1].Fit(fgaus, 'QN0', 'goff', y-0.04, y+0.04)
                y = fgaus.GetParameter(1)
                yerr = fgaus.GetParError(1)
                chi2 = fgaus.GetChisquare()/fgaus.GetNDF()
                sgma = fgaus.GetParameter(2)
                serr = fgaus.GetParError(2)
                if chi2 < 10: #r.gMinuit.fCstatu.startswith('CONV') and chi2 < 4:
                    xvals.append(h[0])
                    yvals.append(y)
                    yerrs.append(yerr)
                    chi2s.append(chi2s)
                    sgmas.append(sgma)
                    serrs.append(serr)
    return xvals, yvals, yerrs, chi2s, sgmas, serrs

# fit electron peak
#     fsfgaus = r.TF1('fsfpeak', 'gaus', 0.26, 0.37)
#     parmobjarr = r.TObjArray()
#     h2.FitSlicesY(fsfgaus,  0, -1, 1, 'qnr', parmobjarr)
#     gsfmeans = asrootpy(parmobjarr[1])
#     genpoints = get_pointgen(gsfmeans, 1, 4)
#     xy = zip(*list(genpoints()))
#     pars = polyfit(xy[0], xy[1], 3)
    

# <codecell>

def normXslices(h2):
    h2c = h2.Clone()
    nby = h2c.GetNbinsY()
    for xbin in range(1, h2c.GetNbinsX()+1):
        h1 = h2c.ProjectionY('%s_%d'%(h2c.GetName(),xbin), xbin, xbin)
        maxval = h1.GetMaximum()
        for ybin in range(1, nby+1):
            bc = h2c.GetBinContent(xbin,ybin)
            if bc > 0:
                h2c.SetBinContent(h2c.GetBin(xbin,ybin), bc/maxval)
    return h2c

# <codecell>

r.gErrorIgnoreLevel = r.kError
nr, nc = 2, 3
fig = figure(figsize=goldenaspect(6.5,nr,nc))
gs = gridspec.GridSpec(nr, nc, wspace=0, hspace=0, left=0.1, right=0.95, bottom=0.175) #, left=0.05, bottom=0.075)
for irow in range(0,nr):
    for icol in range(0,nc):
        hnum = 3*irow+(icol+1)
        h2 = hs_ec_sfVp[hnum-1]
        
        xvals, yvals, yerrs, chi2s, sgmas, serrs = get_peak_points_with_error(h2, fitxlo, fitxhi, losfapprox, 2)
        #pars = polyfit(xvals, yvals, 3, w=[1/yerr for yerr in yerrs if yerr>0])
        hicutvals = [y+3*s for y,s in zip(yvals,sgmas)]
        locutvals = [y-3*s for y,s in zip(yvals,sgmas)]
        cuterrs = [sqrt(yerr**2+(3*serr)**2) for yerr, serr in zip(yerrs, serrs)]
        #pars = polyfit(xvals, hicut, 3, w=[1/yerr for yerr in yerrs if yerr>0])
        #print('%d\t%.2g\t%.2g\t%.2g\t%.2g' % (hnum, pars[3], pars[2], pars[1], pars[0]))
        
        ax = subplot(gs[irow, icol])
        rplt.hist2d(normXslices(h2), axes=ax, cmin=0.0001) #, norm=LogNorm())
        (_,caps,_) = errorbar(xvals, hicutvals, cuterrs, fmt='k.', ms=0.75, lw=0.5, capsize=1)
        [cap.set_markeredgewidth(0.25) for cap in caps]
        (_,caps,_) = errorbar(xvals, locutvals, cuterrs, fmt='k.', ms=0.75, lw=0.5, capsize=1)
        [cap.set_markeredgewidth(0.25) for cap in caps]
        pars = polyfit(xvals, hicutvals, 3, w=[1/err**2 for err in cuterrs])
        plot(linspace(*xlim()), polyval(pars, linspace(*xlim())), '-', color='black', lw=0.3)
        print('%s\t%d,%.2g,%.2g,%.2g,%.2g' % ('ec_sf_hi_pol3', hnum, pars[3], pars[2], pars[1], pars[0]))
        #ax.fill_between(linspace(*xlim()), polyval(pars, linspace(*xlim())), 0.5, color='lightgray', alpha=0.75)
        pars = polyfit(xvals, locutvals, 3, w=[1/err**2 for err in cuterrs])
        plot(linspace(*xlim()), polyval(pars, linspace(*xlim())), '-', color='black', lw=0.3)
        print('%s\t%d,%.2g,%.2g,%.2g,%.2g' % ('ec_sf_lo_pol3', hnum, pars[3], pars[2], pars[1], pars[0]))
        #ax.fill_between(linspace(*xlim()), 0.0, polyval(pars, linspace(*xlim())), color='lightgray', alpha=0.75)
        #plot(linspace(*xlim()), polyval(pars, linspace(*xlim())), 'k-')
        #(_,caps,_) = errorbar(xvals, sgmas, serrs, fmt='k.', ms=0.5, capsize=1, lw=0.5)
        #(_,caps,_) = errorbar(xvals, yvals, yerrs, fmt='k.', ms=0.75, lw=0.5, capsize=1)
        #[cap.set_markeredgewidth(0.25) for cap in caps]
        #grid()
        
        if icol > 0:
            ax.set_yticklabels('')
        else:
            yticks(arange(0.0, 0.50, 0.1))
            if irow == 0:
                yticks(arange(0.0, 0.51, 0.1))
        if irow+1 < nr:
            ax.set_xticklabels('')
        else:
            xticks(arange(0.0, 5, 1))
            if icol+1 == nc:
                xticks(arange(0.0, 5.1, 1))

bigax = fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
#bigax = axes(frameon=False)
xticks([])
yticks([])
ylabel(r'E$_{EC}$/(pc)')
xlabel('p (GeV/c)')
savefig('eid_sf.%s.pdf'%imgtmpl)

# <codecell>

hicutvals = [y+3*s for y,s in zip(yvals,sgmas)]
locutvals = [y-3*s for y,s in zip(yvals,sgmas)]
cuterrs = [sqrt(yerr**2+(3*serr)**2) for yerr, serr in zip(yerrs, serrs)]

# <codecell>

r.gErrorIgnoreLevel = r.kError
nr, nc = 1, 2
fig = figure(figsize=goldenaspect(6.5,nr,nc))
gs = gridspec.GridSpec(nr, nc, wspace=0, hspace=0, left=0.1, right=0.95, bottom=0.175) #, left=0.05, bottom=0.075)

fechists = root_open(finname)
din = 'echists_qskim_2'
sector = 1
h2 = asrootpy(fechists[din].Get('hsf_V_p_%d'%sector))

ax = subplot(1,2,1)
rplt.hist2d(h2, axes=ax, cmin=0.0001) #normXslices(h2), axes=ax, cmin=0.0001) #, norm=LogNorm())
ax = subplot(1,2,2)
rplt.hist2d(normXslices(h2), axes=ax, cmin=0.0001) #, norm=LogNorm())
hp = asrootpy(root_open('/home/ephelps/c1.root').hp)
hp.Scale(0.4/hp.GetMaximum())
hp.linecolor = 'gray'
rplt.hist(hp)
ylim(0,0.5)
# plot([0.139,0.139],[0.0,0.5], 'k-')
# plot([0.494,0.494],[0.0,0.5], 'k-')
    
bigax = fig.add_axes([0.075, 0.01, 0.9, 0.95], frameon=False)

#bigax = axes(frameon=False)
xticks([])
yticks([])
ylabel(r'E$_{EC}$/(pc)')
xlabel('p (GeV/c)')
savefig('eid_sf.cands.%s.pdf'%imgtmpl, bbox_inches='tight')

# <codecell>

r.gErrorIgnoreLevel = r.kError
nr, nc = 2, 3
fig = figure(figsize=goldenaspect(6,nr,nc))
gs = gridspec.GridSpec(nr, nc, wspace=0, hspace=0, left=0.1, right=0.95, bottom=0.175) #, left=0.05, bottom=0.075)

fechists = root_open(finname)
din = 'echists_qskim_2'
for h in fechists[din]: print(h)
hs_ec_sfVp_cands = HistStack(hists=[asrootpy(fechists[din].Get('hsf_V_p_%d'%sector)) for sector in range(1,7)])

for irow in range(0,nr):
    for icol in range(0,nc):
        hnum = 3*irow+(icol+1)
        h2 = hs_ec_sfVp_cands[hnum-1]
        
        xvals, yvals, yerrs, chi2s, sgmas, serrs = get_peak_points_with_error(h2, fitxlo, fitxhi, losfapprox, 2)
        ax = subplot(gs[irow, icol])
        rplt.hist2d(h2, axes=ax, cmin=0.0001) #normXslices(h2), axes=ax, cmin=0.0001) #, norm=LogNorm())
        
        if icol > 0:
            ax.set_yticklabels('')
        else:
            yticks(arange(0.0, 0.50, 0.1))
            if irow == 0:
                yticks(arange(0.0, 0.51, 0.1))
        if irow+1 < nr:
            ax.set_xticklabels('')
        else:
            xticks(arange(0.0, 5, 1))
            if icol+1 == nc:
                xticks(arange(0.0, 5.1, 1))

bigax = fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
#bigax = axes(frameon=False)
xticks([])
yticks([])
ylabel(r'E$_{EC}$/(pc)')
xlabel('p (GeV/c)')
savefig('eid_sf.cands.%s.pdf'%imgtmpl)

# <codecell>

from rootpy.plotting import Hist2D
from rootpy.io import root_open
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt

fin = root_open('/home/ephelps/documents/texws/thesis/figs/dtVp_candidates_100m.root')
h2 = asrootpy(fin.dtVp_cands)

# <codecell>

from matplotlib import rc
rc('text', usetex=True)
goldenaspect=lambda x, rows=1, cols=1: (x,0.95*x/cols*rows/1.618)
fig = figure(figsize=goldenaspect(6.5,1,1))
f = rplt.hist2d(h2, figure=fig) #normXslices(h2), figure=fig)
xlim(0,5)
xlabel('p (GeV/c)')
ylabel('$\Delta t$ (ns)')
fig.savefig('dtVp_candidates_100m.pdf')

# <markdowncell>

# # CC Cuts

# <codecell>

from rootpy.plotting import Hist, Hist2D, HistStack
from rootpy.io import root_open
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt

# <codecell>

def gethists_nphe(ptype='p', sector=1):
    # ptype, sector = 'p', 1
    hnpheS = []
    ntmpl = 'hnphe_V_pmt_%s_s%d'
    with root_open('CC_Hists.root') as fin:
        h2n = ntmpl%(ptype,sector)
        h2 = asrootpy(fin.Get(h2n))
        for b in range(1,37):
            hn = h2n+'_pmt%d'%b
            h = asrootpy(h2.ProjectionY(hn,b,b+1))
            h.SetDirectory(0)
            hnpheS.append(h)
    return hnpheS

# <codecell>

xmax = 300
xtmarks = arange(0,80,10)
xtlabls = ('',1,'','',4,'','',7,'')
hnphe_p_1 = gethists_nphe('p',1)
hnphe_pip_1 = gethists_nphe('pip',1)
hnphe_pim_1 = gethists_nphe('pim',1)
hnphe_e_1 = gethists_nphe('e',1)
hnphe_not_e_1 = [h1+h2+h3 for (h1, h2, h3) in zip(hnphe_p_1, hnphe_pip_1, hnphe_pim_1)]

# <codecell>

fig = figure(figsize=(10,10))
fig.subplots_adjust(hspace=0.1, wspace=0.1)
for i, (h,he) in enumerate(zip(hnphe_not_e_1,hnphe_e_1),1):
    ax = subplot(6,6,i)
    if i > 30:
        xticks(xtmarks, xtlabls)
    else:
        ax.set_xticklabels('')
    ax.set_yticklabels('')
    h.Scale(1/h.GetMaximum())
    he.Scale(1/he.GetMaximum())
    h.linecolor = 'red'
    he.linecolor = 'blue'
    rplt.hist(h)
    rplt.hist(he)
    he = he-h
    he.linecolor = 'green'
    rplt.hist(he)
    text(0.8, 0.75, 'PMT %d'%i, transform=ax.transAxes, ha='right', va='center')
    xlim(0,80)
    ylim(0)

bigax = fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
xticks([])
yticks([])
xlabel('number of photo-electrons')

# <codecell>

import ROOT as R

def d_gaus(x, par):
    return par[0]*R.TMath.Gaus(x[0], par[1], par[2])

def d_pois(x, par):
    return par[0]*R.TMath.Poisson(par[2]*x[0],par[1]*par[2])

def d_gausexp_p1(v, par):
    """Convolution of gauss and exponential using analytic expression.
    parameters:
        par[0] (mag):    magnitude
        par[1] (gmean):  gauss mean
        par[2] (gsigma): gauss sqrt(variance)
        par[3] (ehl):  exponential decay half-life.
        par[4] (p0)
        par[5] (p1)
    """
    x, mag, gmean, gsigma, ehl, p0, p1 = v[0], par[0], par[1], par[2], par[3], par[4], par[5]
    if mag <= 0 or gsigma <= 0 or ehl <= 0: return 0
    erate = R.TMath.Log(2)/ehl
    erfcarg = (gmean + erate*gsigma**2 - x)/(gsigma*R.TMath.Sqrt(2))
    factor = erate/2 * R.TMath.Exp(erate/2 * (2*gmean + erate*gsigma**2 - 2*x))
    val = mag * factor * R.TMath.Erfc(erfcarg) + p0*exp(-(x/p1)) # (p0/(x+p1))
    return val if val > 0 else 0

def d_gausexp_p1_pois(v, par):
    """Convolution of gauss and exponential using analytic expression.
    parameters:
        par[3] (mag):    magnitude
        par[4] (gmean):  gauss mean
        par[5] (gsigma): gauss sqrt(variance)
        par[6] (ehl):  exponential decay half-life.
        par[7] (p0)
        par[8] (p1)
        par[0] (pmag): magnitude of poisson
        par[1] (pmean): mean of poisson
        par[2] (pstretch): x-scaling
    """
    x, mag, gmean, gsigma, ehl, p0, p1 = v[0], par[3], par[4], par[5], par[6], par[7], par[8]
    if mag <= 0 or gsigma <= 0 or ehl <= 0: return 0
    erate = R.TMath.Log(2)/ehl
    erfcarg = (gmean + erate*gsigma**2 - x)/(gsigma*R.TMath.Sqrt(2))
    factor = erate/2 * R.TMath.Exp(erate/2 * (2*gmean + erate*gsigma**2 - 2*x))
    val = mag * factor * R.TMath.Erfc(erfcarg) + p0*exp(-(x/p1)) # (p0/(x+p1))
    val = val if val > 0 else 0
    val += par[0]*R.TMath.Poisson(par[2]*x,par[1]*par[2])
    return val 

def getFunNoise():
    ff = R.TF1('f2', d_gausexp_p1, 0, 300, 6)
    pars = [mag, gmean, gsigma, ehl, p0, p1] = [R.Double(14), R.Double(6), R.Double(2.5),
                                                R.Double(5), R.Double(0), R.Double(5)] #, R.Double(0.1), R.Double(-0.001)]
    ff.SetParNames('magnitude', 'mean', 'sigma', 'decay', 'a0', 'a1')
    #pars_f = [R.Double(1), R.Double(10), R.Double(3)]
    #f.SetParameters(pars_f[0], pars_f[1], pars_f[2])
    ff.SetParameters(pars[0], pars[1], pars[2], pars[3], pars[4], pars[5])
    ff.SetParLimits(0,R.Double(1.0),R.Double(30.0))
    ff.SetParLimits(2,R.Double(0.1),R.Double(10))
    ff.SetParLimits(3,R.Double(0.1),R.Double(10))
    ff.SetParLimits(4,R.Double(0),R.Double(0.2))
    ff.SetParLimits(5,R.Double(30),R.Double(200))
#     ff.SetParLimits(4,R.Double(0),R.Double(0.5))
#     ff.SetParLimits(5,R.Double(-0.01),R.Double(0))
    return ff

# <codecell>

from matplotlib import rc
rc('text', usetex=True)
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec
goldenaspect=lambda x, rows=1, cols=1: (x,x/cols*rows/1.618)

def drawCCfits(pmtnums=[18], save=False, polbg=True, w=6.5):
    nrows = len(pmtnums)
    fig = figure(figsize=goldenaspect(w,nrows,3))
    gs = gridspec.GridSpec(nrows, 3, wspace=0.1, hspace=0, left=0.1, right=0.95, bottom=0.175)
    
    for rownum, pmtnum in enumerate(pmtnums):
        h = hnphe_not_e_1[pmtnum-1]
        h.color='black'
        h.Scale(1/h.GetMaximum())
        ff = getFunNoise()
        
        if not polbg:
            ff.FixParameter(4,0)
            ff.FixParameter(5,1)
        
        h.Fit(ff, 'N0', '', 5, 350)
        ff.SetRange(0,350)
        pars = [ff.GetParameter(i) for i in range(0,ff.GetNpar())]
        X = linspace(0,350,200)
        Y = [d_gausexp_p1([R.Double(x)],pars) for x in X]
        Xc = pars[1]+3*(pars[2]+pars[3])
        
        ax1 = subplot(gs[rownum,0])
        xtmarks = arange(0,80,10)
        xtlabls = ('',1,'',3,'',5,'',7,'')
        xticks(xtmarks, xtlabls)
        yticks([])
        rplt.hist(h)
        plot(X,Y, 'r')
        axvline(Xc, color='red', linestyle='dashed')
        text(0.9, 0.85, r'$\tilde{\chi}^2 = %.2f$'%(ff.GetChisquare()/ff.GetNDF()), ha='right', va='top', transform=ax1.transAxes)
        if rownum==0:
            text(0.5, 1.075, 'non-electron noise', transform=ax1.transAxes, va='bottom', ha='center')
        xlim(0,80)
        
        ff2 = R.TF1('ff2', d_gausexp_p1_pois, 0, 350, 9)
        for i, p in enumerate(pars,3):
            if i == 8:
                ff2.FixParameter(i,p)
            elif i != 7:
                tol = 0.5 if i in [4,5,6] else 0.5
                (lf, rf) = (1-tol,1+tol) if p > 0 else (1+tol,1-tol)
                ff2.SetParameter(i,p)
                (lo,hi) = (lf*p,rf*p)
                ff2.SetParLimits(i,lo,hi)
        ff2.SetParameter(0, 1)
        ff2.SetParameter(1, 150)
        ff2.SetParameter(2, 0.04)
        ff2.SetParLimits(2, 0.001, 1)
        
        if not polbg:
            ff2.FixParameter(7,0)
            ff2.FixParameter(8,1)
        
        h = hnphe_e_1[pmtnum-1]
        h.color='black'
        h.Scale(1/h.GetMaximum())
        h.Fit(ff2, 'N0', '', 5, 350)
        
        pars = [ff2.GetParameter(i) for i in range(0,ff2.GetNpar())]
        X = linspace(0, 350, 350)
        Y = [d_gausexp_p1_pois([R.Double(x)],pars) for x in X]
        Y2 = [d_gausexp_p1([R.Double(x)],pars[3:]) for x in X]
        Y3 = [d_pois([R.Double(x)],pars[0:3]) for x in X]
        
        ax1 = subplot(gs[rownum,1])
        ax1.set_yscale('log')
        xtmarks = (70, 140, 210, 280)
        xtlabls = (7, 14, 21, 28)
        xticks(xtmarks, xtlabls)
        yticks([])
        axvline(Xc, color='red', linestyle='dashed')
        plot(X,Y,'gray', linewidth=2)
        rplt.hist(h)
        plot(X,Y2,'r')
        plot(X,Y3,'g')
        text(0.9, 0.85, r'$\tilde{\chi}^2 = %.2f$'%(ff2.GetChisquare()/ff2.GetNDF()), ha='right', va='top', transform=ax1.transAxes)
        if rownum==0:
            text(0.5, 1.075, 'electrons with noise', transform=ax1.transAxes, va='bottom', ha='center')
        text(0.97, 0.3, 'LOG', transform=ax1.transAxes, rotation=90, va='bottom', ha='right')
        xlim(0,350)
        ylim(0.001,1.1)
        
        h2 = h.Clone()
        ff.SetRange(0,350)
        [ff.SetParameter(i,ff2.GetParameter(i+3)) for i in range(0,ff.GetNpar())]
        h2.Add(ff,-1)
        fpois = R.TF1('fpois',d_pois,0,350,3)
        fpois.SetParameters(pars[0],pars[1],pars[2])
        h2.Fit(fpois,'N0','',30,300)
        Y = [d_pois([R.Double(x)],[fpois.GetParameter(i) for i in range(0,3)]) for x in X] # pars[0:3]) for x in X]
        h2.GetXaxis().SetRangeUser(30,350)
        ymax = 1.2*h2.GetMaximum()
        h2.GetXaxis().SetRangeUser(0,-1)
        
        ax2 = subplot(gs[rownum,2])
        xticks(xtmarks, xtlabls)
        yticks([])
        h.color='black'
        h.color='red'
        rplt.hist([h,h2], stacked=False)
        axvline(Xc, color='red', linestyle='dashed')
        plot(X,Y, 'g')
        #plot(X,Y2,'r')
        text(0.9, 0.85, r'$\tilde{\chi}^2 = %.2f$'%(fpois.GetChisquare()/fpois.GetNDF()), ha='right', va='top', transform=ax2.transAxes)
        if rownum==0:
            text(0.5, 1.075, 'electrons', transform=ax2.transAxes, va='bottom', ha='center')
        xlim(0,350)
        ylim(0,ymax)
        text(1.05, 0.5, 'PMT %d'%pmtnum, fontsize=12, color='0.25',
             transform=ax2.transAxes, rotation=-90, va='center', ha='left')
        
        fpois = R.TF1('fpois', d_pois, 0, 350, 3)
        fpois.SetParameters(pars[0],pars[1],pars[2])
        print('Efficiency = %.3f'%(fpois.Integral(30,350)/fpois.Integral(0,350)))
    bigax = fig.add_axes([0.085, 0.12, 0.875, 0.85], frameon=False)
    xticks([])
    yticks([])
    xlabel('number of photo-electrons')
    ylabel('relative photo-electron count frequency')
    
    if save: savefig('eid_cc_fit_sample_%s.pdf'%str(pmtnums), bbox_inches='tight')

# <codecell>

drawCCfits([32, 19, 6], True) #, False, 18)

# <markdowncell>

# # All CC PMTs

# <codecell>

def getNpheHists(sect):
    hnphe_p_1 = gethists_nphe('p',sect)
    hnphe_pip_1 = gethists_nphe('pip',sect)
    hnphe_pim_1 = gethists_nphe('pim',sect)
    hnphe_e_1 = gethists_nphe('e',sect)
    hnphe_not_e_1 = [h1+h2+h3 for (h1, h2, h3) in zip(hnphe_p_1, hnphe_pip_1, hnphe_pim_1)]
    return (hnphe_e_1, hnphe_not_e_1)

# <codecell>

def fitCCpmts(sect=1):
    hchi2, hmean, hsig, hNint = Hist(36,0.5,36.5), Hist(36,0.5,36.5), Hist(36,0.5,36.5), Hist(36,0.5,36.5)
    hnphe_e, hnphe_not_e = getNpheHists(sect)
    xtmarks = (60, 170, 280)
    xtlabls = (6, 17, 28)
    
    fig = figure(figsize=(6.5,8.5))
    fig.subplots_adjust(hspace=0.1, wspace=0.1)
    for pnum, (h,he) in enumerate(zip(hnphe_not_e,hnphe_e),1):
        h.Scale(1/h.GetMaximum())
        he.Scale(1/he.GetMaximum())
        fbg = getFunNoise()
        ff2 = R.TF1('ff2', d_gausexp_p1_pois, 0, 350, 9)
        
        h.Fit(fbg,'N0','',5,350)
        pars = [fbg.GetParameter(i) for i in range(0,fbg.GetNpar())]
        Xc = pars[1]+3*(pars[2]+pars[3])
        
        for i, p in enumerate(pars,3):
            if i == 8:
                ff2.FixParameter(i,p)
            elif i != 7:
                tol = 0.5 if i in [4,5,6] else 0.5
                (lf, rf) = (1-tol,1+tol) if p > 0 else (1+tol,1-tol)
                ff2.SetParameter(i,p)
                (lo,hi) = (lf*p,rf*p)
                ff2.SetParLimits(i,lo,hi)
        ff2.SetParameter(0, 1)
        ff2.SetParameter(1, 150)
        ff2.SetParameter(2, 0.04)
        ff2.SetParLimits(2, 0.001, 1)
        
        he.Fit(ff2,'N0','',5,350)
        
        pars = [ff2.GetParameter(i) for i in range(0,ff2.GetNpar())]
        fpois = R.TF1('fpois', d_pois, 0, 350, 3)
        fpois.SetParameters(pars[0],pars[1],pars[2])
        
        he2 = he.Clone()
        fbg.SetRange(0,350)
        [fbg.SetParameter(i,ff2.GetParameter(i+3)) for i in range(0,fbg.GetNpar())]
        he2.Add(fbg,-1)
        ax = subplot(6,6,pnum)
        if pnum > 30:
            xticks(xtmarks, xtlabls)
        else:
            ax.set_xticklabels('')
        ax.set_yticklabels('')
        X = linspace(0, 350, 350)
        Y = [d_pois([R.Double(x)],pars[0:3]) for x in X]
        he2.color='black'
        rplt.hist(he2)
        he.color='gray'
        rplt.hist(he)
        plot(X,Y,'b')
        axvline(Xc, color='red')
        he2.GetXaxis().SetRangeUser(30,300)
        ymax = 1.4*he2.GetMaximum()
        he2.GetXaxis().SetRangeUser(0,-1)
        xlim(0,350)
        ylim(0,ymax)
        
        Nint = fpois.Integral(0,300)
        pmean = fpois.GetParameter(1)
        psig = fpois.GetParameter(2)
        eff = fpois.Integral(30,300)/Nint
        ch2 = ff2.GetChisquare()/ff2.GetNDF()
        
        hchi2.Fill(pnum,ch2)
        hmean.Fill(pnum,pmean)
        hsig.Fill(pnum,psig)
        hNint.Fill(pnum,Nint)
        
        print('%d,%d,%.1f,%.3f'%(sect,pnum,Xc,eff))
        
        text(0.9, 0.2, r'%d'%(36*(sect-1)+pnum), fontsize=10,
             transform=ax.transAxes, ha='right', va='bottom')
        text(0.9, 0.9, r'$\tilde{\chi}^{2} = %.2f$'%ch2, fontsize=8,
             transform=ax.transAxes, ha='right', va='top')
        text(0.9, 0.75, r'$%.1f\%%$'%(100*eff), fontsize=10,
             transform=ax.transAxes, ha='right', va='top')
        
    bigax = fig.add_axes([0.1, 0.1, 0.88, 0.895], frameon=False)
    xticks([])
    yticks([])
    xlabel('number of photo-electrons')
    ylabel('relative photo-electron count frequency')
    savefig('eid_cc_fits_%d.pdf'%sect, bbox_inches='tight')
    return hchi2, hmean, hsig, hNint

# <codecell>

# hpars = fitCCpmts(1)
hpars = [[],[],[],[],[],[]]
for sect in range(1,7):
    hpars[sect-1] = fitCCpmts(sect)

# <codecell>

figure(figsize=(15,4))
subplot(141)
rplt.hist(hpars[0][0])
axvline(3.5)
ylim(0,5)
subplot(142)
rplt.hist(hpars[0][1])
axvline(3.5)
ylim(0,200)
subplot(143)
rplt.hist(hpars[0][2])
axvline(3.5)
ylim(0,0.1)
subplot(144)
rplt.hist(hpars[0][3])
axvline(3.5)
ylim(0,180)

# <codecell>

hpars[0][1].Draw()

# <codecell>

y = lambda x:20.28+6.74*x
y(3)

# <markdowncell>

# # Scratch

# <codecell>

from matplotlib import rc
rc('text', usetex=True)
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec
goldenaspect=lambda x, rows=1, cols=1: (x,x/cols*rows/1.618)

def drawCCfits2(pmtnums=[18], polbg=True, save=False):
    nrows = len(pmtnums)
    fig = figure(figsize=goldenaspect(6.5,nrows,3))
    gs = gridspec.GridSpec(nrows, 3, wspace=0.1, hspace=0, left=0.1, right=0.95, bottom=0.175)
    
    for rownum, pmtnum in enumerate(pmtnums):
        h = hnphe_not_e_1[pmtnum-1]
        h.color='black'
        h.Scale(1/h.GetMaximum())
        ff = getFunNoise2()
        
        ff.FixParameter(5,0)
        #ff.FixParameter(4,0.1)
        
        h.Fit(ff, 'N0', '', [7:8]5, 90)
        ff.SetRange(0,350)
        pars = [ff.GetParameter(i) for i in range(0,ff.GetNpar())]
        X = linspace(0,100,200)
        Y = [d_gausexp_2([R.Double(x)],pars) for x in X]
        Xc = pars[1]+3*(pars[2]+pars[3])
        
        ax1 = subplot(gs[rownum,0])
        xtmarks = arange(0,80,10)
        xtlabls = ('',1,'',3,'',5,'',7,'')
        xticks(xtmarks, xtlabls)
        yticks([])
        rplt.hist(h)
        plot(X,Y, 'r')
        axvline(Xc, color='red', linestyle='dashed')
        # text(0.95, 0.9, 'non-electrons,\nshape of noise peak', transform=ax1.transAxes, va='top', ha='right')
        if rownum==0:
            text(0.5, 1.075, 'non-electron noise', transform=ax1.transAxes, va='bottom', ha='center')
        xlim(0,80)
        
        ff2 = R.TF1('ff2', d_gausexp_2_pois, 0, 350, 9)
        for i, p in enumerate(pars,3):
#             print('%s = %.3e'%(ff.GetParName(i-3),p))
            if i != 3:
                ff2.FixParameter(i,p)
            else:
                tol = 0.5 if i in [4,5,6] else 0.5
                (lf, rf) = (1-tol,1+tol) if p > 0 else (1+tol,1-tol)
                ff2.SetParameter(i,p)
                (lo,hi) = (lf*p,rf*p)
                ff2.SetParLimits(i,lo,hi)
        ff2.SetParameter(0, 1)
        ff2.SetParameter(1, 150)
        ff2.SetParameter(2, 0.04)
        ff2.SetParLimits(2, 0.001, 1)
        
        ff2.FixParameter(8,0)
        #ff.FixParameter(4,0.1)
        
        h = hnphe_e_1[pmtnum-1]
        h.color='black'
        h.Scale(1/h.GetMaximum())
        h.Fit(ff2, 'N0', '', 5, 350)
        
        pars = [ff2.GetParameter(i) for i in range(0,ff2.GetNpar())]
        X = linspace(0, 350, 350)
        Y = [d_gausexp_2_pois([R.Double(x)],pars) for x in X]
        Y2 = [d_gausexp_2([R.Double(x)],pars[3:]) for x in X]
        Y3 = [d_pois([R.Double(x)],pars[0:3]) for x in X]
        
        ax1 = subplot(gs[rownum,1])
        ax1.set_yscale('log')
        xtmarks = (70, 140, 210, 280)
        xtlabls = (7, 14, 21, 28)
        xticks(xtmarks, xtlabls)
        yticks([])
        axvline(Xc, color='red', linestyle='dashed')
        plot(X,Y,'gray', linewidth=2)
        rplt.hist(h)
        plot(X,Y2,'r')
        plot(X,Y3,'g')
        #text(0.95, 0.9, 'contaminated\nelectrons', transform=ax1.transAxes, va='top', ha='right')
        if rownum==0:
            text(0.5, 1.075, 'electrons with noise', transform=ax1.transAxes, va='bottom', ha='center')
        text(0.98, 0.2, 'LOG', transform=ax1.transAxes, rotation=90, va='bottom', ha='right')
        xlim(0,350)
        ylim(0.001,1.1)
        
        h2 = h.Clone()
        ff.SetRange(0,350)
        [ff.SetParameter(i,ff2.GetParameter(i+3)) for i in range(0,ff.GetNpar())]
        h2.Add(ff,-1)
        Y = [d_pois([R.Double(x)],pars[0:3]) for x in X]
        h2.GetXaxis().SetRangeUser(30,350)
        ymax = 1.2*h2.GetMaximum()
        h2.GetXaxis().SetRangeUser(0,-1)
        
        ax2 = subplot(gs[rownum,2])
        xticks(xtmarks, xtlabls)
        yticks([])
        h.color='black'
        h.color='red'
        rplt.hist([h,h2], stacked=False)
        axvline(Xc, color='red', linestyle='dashed')
        plot(X,Y, 'g')
        #plot(X,Y2,'r')
        #text(0.95, 0.9, 'noise\nsubtracted', transform=ax2.transAxes, va='top', ha='right')
        if rownum==0:
            text(0.5, 1.075, 'electrons', transform=ax2.transAxes, va='bottom', ha='center')
        xlim(0,350)
        ylim(0,ymax)
        text(1.05, 0.5, 'PMT %d'%pmtnum, fontsize=12, color='0.25',
             transform=ax2.transAxes, rotation=-90, va='center', ha='left')
        
#         print('-----------------')
#         for i, (pname, pval) in enumerate(zip(['pois_amp','pois_mean','pois_stretch'],pars[0:3])):
#             print('%s = %.3e'%(pname,pval))
#         print('-----------------')
        fpois = R.TF1('fpois', d_pois, 0, 350, 3)
        fpois.SetParameters(pars[0],pars[1],pars[2])
#         print('Total = %.3f'%fpois.Integral(0,350))
#         print('Cut = %.3f'%fpois.Integral(0,30))
#         print('Kept = %.3f'%fpois.Integral(30,350))
        print('Efficiency = %.3f'%(fpois.Integral(30,350)/fpois.Integral(0,350)))
#         print('-----------------')
    bigax = fig.add_axes([0.085, 0.12, 0.875, 0.85], frameon=False)
    xticks([])
    yticks([])
    xlabel('number of photo-electrons')
    ylabel('relative photo-electron count frequency')
    
    if save: savefig('eid_cc_fit_sample_%s.pdf'%str(pmtnums), bbox_inches='tight')

# <codecell>

drawCCfits2([32, 19, 6], False)

# <codecell>

def d_pois(x, par):
    return par[0]*R.TMath.Poisson(par[2]*x[0],par[1])

h = hnphe_not_e_1[5]
h.Scale(1/h.GetMaximum())
ff = R.TF1('ff', d_pois, 0, 100, 3)
ff.SetParameters(1, 10, 3)
h.Fit(ff, 'N0', '', 5, 30)
pars = [ff.GetParameter(i) for i in range(0,ff.GetNpar())]
X = linspace(0,100,200)
Y = [d_pois([R.Double(x)],pars) for x in X]

rplt.hist(h)
plot(X,Y)
xlim(0,80)

# <codecell>

%run ../python/oana_setup.py

# <codecell>

import ROOT as R
h10.SetEntryList(0)
h10.Draw('>>elist', 'top3pass && mmthreshpass && mmpi0pass && fidpass', 'entrylist')
elist = R.gDirectory().Get('elist')
h10.SetEntryList(elist)

# <codecell>

h = rh1('nphe[cc[0]-1]', 'ccseg==5 && ccsect==1', binning=(300,0,300))

# <codecell>

rplt.hist(h)

# <codecell>

h10.Draw('ccseg>>hseg(19,-0.5,18.5)', '','',1000)

# <codecell>

print(h2.GetTitle())

# <codecell>

from rootpy.plotting import Hist2D
from rootpy.io import root_open
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt
from matplotlib.colors import LogNorm

fin = root_open('/data/jobs/skim.20140102/skim.e1f.hists.root')
h = fin.scpid.hdt_V_p_neg_e_s1_p1.clone()
for i in range(2,30):
    h = h + asrootpy(fin.scpid.Get('hdt_V_p_neg_e_s1_p%d'%i))

tmph2 = rplt.hist2d(asrootpy(h), norm=LogNorm())
ylim(-2,2)

# <codecell>

%run /home/ephelps/projects/phys-ana-omega/python/oana_setup.py

# <codecell>

h10.SetAlias("c",'29.97925')
h10.SetAlias("mp",'0.9382721')
h10.SetAlias("mpip",'0.1395702')
h10.SetAlias("bifpip","sqrt(p*p/(p*p+mpip*mpip))")
h10.SetAlias("dtifpip","sc_t[sc-1]-sc_r[sc-1]/(c*bifpip)-tr_time")
h10.SetAlias("dtife","sc_t[sc-1]-sc_r[sc-1]/(c*c)-tr_time")

# <codecell>

cp ~/projects/phys-ana-omega/worksheets/eid_cc_fit_sample_\[32\,\ 19\,\ 6\].pdf  eid_cc_fit_samples.pdf
h10.Draw('sc_t[sc-1]-sc_r[sc-1]/29.97925-tr_time:p>>hdt(110,0,5.5,100,-1,1)','sc>0 && q<0 && id!=11','colz')

# <codecell>

fin_missingfiles = root_open('~/Dropbox/missingfiles.root')

# <codecell>

fin_lbs = root_open = root_open('/data/e1f/skim/skims.root')

# <codecell>

import pandas as pd

# <codecell>

h2sf0 = hs_ec_sfVp[1]
h2sf0.Draw('colz')

# <codecell>

fsfgaus = r.TF1('fsfpeak', 'gaus', 0.27, 0.32)
parmobjarr = r.TObjArray()
h2sf0.FitSlicesY(fsfgaus,  0, -1, 1, 'qnr', parmobjarr)
gsfmeans = asrootpy(parmobjarr[1])

def get_pointgen(gr, xlo, xhi):
    def genpoints():
        for x, y in zip(gr.x(), gsfmeans.y()):
            if x > xlo and x < xhi and y > 0:
                yield (x, y)
    return genpoints

genpoints = get_pointgen(gsfmeans, 0.640, 5)

xy = zip(*list(genpoints()))
pars = polyfit(xy[0], xy[1], 2)

rplt.errorbar(gsfmeans, emptybins=False)
plot(linspace(*xlim()), polyval(pars, linspace(*xlim())), 'k')
ylim(0, 0.5)

# <codecell>


# <codecell>


# <codecell>

pars

# <codecell>

X = pd.Series([h2sf0.GetXaxis().GetBinCenter(ibin) for ibin in range(1, h2sf0.GetNbinsX()+1)])
Y = pd.Series([h2sf0.GetYaxis().GetBinCenter(ibin) for ibin in range(1, h2sf0.GetNbinsY()+1)])

# <codecell>

errorbar(Y, h2sf0[100], yerr=np.sqrt(h2sf0[100]), fmt='k.')

# <codecell>

#p0, p1 = 1, 1
fx = lambda x,p0=1,p1=1: p0*exp(-(x/p1))  # p0/(x+p1)
X = linspace(0,350,350)
Y = [fx(x,.1,100) for x in X]
plot(X,Y)
ylim(0,1)

# <codecell>

print(exp(0),exp(1))

# <codecell>

fx(3)

# <codecell>


