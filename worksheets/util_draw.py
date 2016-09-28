from matplotlib import rc
rc('text', usetex=True)
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.colors import LogNorm

import ROOT as R

import rootpy.plotting.root2matplotlib as rplt

goldenaspect=lambda x, rows=1, cols=1: (x,0.95*x/cols*rows/1.618)

def draw(hist, ncols=1, nrows=1, cell=1, fig=None,
         figsize=goldenaspect(3), **kwargs):
    if fig is None:
        fig = plt.figure(figsize=figsize, dpi=1200, facecolor='white')
    else:
        plt.figure(fig.number)
    subplot = fig.add_subplot(nrows, ncols, cell)
    #subplot.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
    #subplot.ticklabel_format(scilimits=(0, 0))
    if isinstance(hist, R.TH2):
        rplt.hist2d(hist, axes=subplot, **kwargs)
    else:
        rplt.errorbar([hist], xerr=False, emptybins=False, **kwargs)
    fig.set_tight_layout(True)
    return fig

def mdraw(hists, ncols=1, nrows=1, figwidth=6.5, **kwargs):
    fig = plt.figure(figsize=goldenaspect(figwidth, nrows, ncols))
    for (ih, h) in enumerate(hists):
        draw(h, ncols, nrows, ih+1, fig=fig)
    fig.set_tight_layout(True)
    return fig

def sdraw(hists, xlabel='', ylabel='', xlims=None, ylims=None, space=0):
    nr, nc = 2, 3
    fig = plt.figure(figsize=goldenaspect(6.5,nr,nc))
    gs = gridspec.GridSpec(nr, nc, wspace=space, hspace=space, left=0.1, right=0.95, bottom=0.175)
    for irow in range(0,nr):
        for icol in range(0,nc):
            hnum = 3*irow+(icol+1)
            h = hists[hnum-1]
            ax = plt.subplot(gs[irow, icol])
            rplt.hist2d(h, axes=ax) #, norm=LogNorm())
            if xlims is not None:
                plt.xlim(xlims)
            if ylims is not None:
                plt.ylim(ylims)
            if icol > 0:
                ax.set_yticklabels('')
            if irow+1 < nr:
                ax.set_xticklabels('')
    fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
    plt.xticks([])
    plt.yticks([])
    plt.ylabel(ylabel)
    plt.xlabel(xlabel)
    return fig


def norm_x_slices(h2):
    h2c = h2.Clone()
    nby = h2c.GetNbinsY()
    for xbin in range(1, h2c.GetNbinsX()+1):
        h1 = h2c.ProjectionY('%s_%d'%(h2c.GetName(),xbin), xbin, xbin)
        maxval = h1.GetMaximum()
        for ybin in range(1, nby+1):
            bc = h2c.GetBinContent(xbin,ybin)
            if bc > 0:
                h2c.SetBinContent(h2c.GetBin(xbin,ybin), bc/maxval)
    return asrootpy(h2c)


def norm_y_slices(h2):
    h2c = h2.Clone()
    nbx = h2c.GetNbinsX()
    for ybin in range(1, h2c.GetNbinsY()+1):
        h1 = h2c.ProjectionX('%s_%d'%(h2c.GetName(),ybin), ybin, ybin)
        maxval = h1.GetMaximum()
        for xbin in range(1, nbx+1):
            bc = h2c.GetBinContent(xbin,ybin)
            if bc > 0:
                h2c.SetBinContent(h2c.GetBin(xbin,ybin), bc/maxval)
    return asrootpy(h2c)


def get_x_slices(h2, window=1):
    hs = []
    for xbin in range(1, h2.xaxis.get_nbins()+1):
        h = h2.projection_y('%s_%d'%(h2.name, xbin), xbin, xbin+window-1)
        if sum(h) < 10:
            h = None
        hs.append((h2.xaxis.get_bin_center(xbin),h))
    return hs


def get_peak_points(h2, xlo, xhi, ylo=0):
    points = []
    hs = get_xslices(h2)
    pfinder = R.TSpectrum(1)
    for h in hs:
        if h[1] is not None and h[0]>xlo and h[0]<xhi:
            pfinder.Search(h[1], 2.5, 'goff')
            y = pfinder.GetPositionX()[0]
            if y > ylo:
                points.append((h[0],pfinder.GetPositionX()[0]))
    return points


def get_pointgen(gr, xlo, xhi):
    def genpoints():
        for x, y in zip(gr.x(), gsfmeans.y()):
            if x > xlo and x < xhi and y > 0:
                yield (x, y)
    return genpoints


