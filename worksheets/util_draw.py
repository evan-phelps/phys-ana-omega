import sys
from matplotlib import rc
rc('text', usetex=True)
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.colors import LogNorm
from rootpy import asrootpy
import numpy as np

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
        rplt.errorbar(hist, xerr=False, emptybins=False, **kwargs)
    #fig.set_tight_layout(True)
    return fig

def mdraw(hists, ncols=1, nrows=1, figwidth=6.5, **kwargs):
    fig = plt.figure(figsize=goldenaspect(figwidth, nrows, ncols))
    for (ih, h) in enumerate(hists):
        draw(h, ncols, nrows, ih+1, fig=fig, figsize=None, **kwargs)
    #fig.set_tight_layout(True)
    return fig

def sdraw(hists, xlabel='', ylabel='', xlims=None, ylims=None, space=0, grid=False, **kwargs):
    nr, nc = 2, 3
    fig = plt.figure(figsize=goldenaspect(6.5,nr,nc))
    gs = gridspec.GridSpec(nr, nc, wspace=space, hspace=space, left=0.1, right=0.95, bottom=0.175)
    for irow in range(0,nr):
        for icol in range(0,nc):
            hnum = 3*irow+(icol+1)
            h = hists[hnum-1]
            ax = plt.subplot(gs[irow, icol])
            rplt.hist2d(h, axes=ax, **kwargs) #, norm=LogNorm())
            if xlims is not None:
                plt.xlim(xlims)
            if ylims is not None:
                plt.ylim(ylims)
            if icol > 0:
                ax.set_yticklabels('')
            if irow+1 < nr:
                ax.set_xticklabels('')
            if grid is True:
                plt.grid()
    fig.add_axes([0.06, 0.105, 0.94, 0.895], frameon=False)
    plt.xticks([])
    plt.yticks([])
    plt.ylabel(ylabel)
    plt.xlabel(xlabel)
    return fig


def norm_x_slices(h2, range_of_max=None):
    h2c = h2.Clone()
    nby = h2c.GetNbinsY()
    for xbin in range(1, h2c.GetNbinsX()+1):
        h1 = h2c.ProjectionY('%s_%d'%(h2c.GetName(),xbin), xbin, xbin)
        if range_of_max is not None:
            h1.GetXaxis().SetRangeUser(range_of_max[0], range_of_max[1])
        maxval = h1.GetMaximum()
        h1.GetXaxis().SetRange(0,0)
        maxval = maxval if maxval>0 else h1.GetMaximum()
        for ybin in range(1, nby+1):
            bc = h2c.GetBinContent(xbin,ybin)
            berr = h2c.GetBinError(xbin,ybin)
            if bc > 0:
                h2c.SetBinContent(h2c.GetBin(xbin,ybin), bc/maxval)
                h2c.SetBinError(h2c.GetBin(xbin,ybin), berr/maxval)
        h2c.SetMaximum(maxval)
    return asrootpy(h2c)


def norm_y_slices(h2, range_of_max=None):
    h2c = h2.Clone()
    nbx = h2c.GetNbinsX()
    for ybin in range(1, h2c.GetNbinsY()+1):
        h1 = h2c.ProjectionX('%s_%d'%(h2c.GetName(),ybin), ybin, ybin)
        if range_of_max is not None:
            h1.GetXaxis().SetRangeUser(range_of_max[0], range_of_max[1])
        maxval = h1.GetMaximum()
        h1.GetXaxis().SetRange(0,0)
        maxval = maxval if maxval>0 else h1.GetMaximum()
        for xbin in range(1, nbx+1):
            bc = h2c.GetBinContent(xbin,ybin)
            berr = h2c.GetBinError(xbin,ybin)
            if bc > 0:
                h2c.SetBinContent(h2c.GetBin(xbin,ybin), bc/maxval)
                h2c.SetBinError(h2c.GetBin(xbin,ybin), berr/maxval)
        h2c.SetMaximum(maxval)
    return asrootpy(h2c)


def get_x_slices(h2, window=1, xlow=None, xhigh=None):
    hs = []
    xbin_lo = 1
    xbin_hi = h2.xaxis.get_nbins()+1
    if xlow is not None:
        xbin_lo = h2.xaxis.find_bin(xlow)
    if xhigh is not None:
        xbin_hi = h2.xaxis.find_bin(xhigh)+1

    for xbin in range(xbin_lo, xbin_hi):
        h = h2.projection_y('%s_%d'%(h2.name, xbin), xbin, xbin+window-1)
        if h.GetEntries() > 10:
            xmid = sum([h2.xaxis.get_bin_center(i+xbin) for i in range(0,window)])/window
            hs.append((xmid,h))
    return hs

def get_y_slices(h2, window=1, ylow=None, yhigh=None):
    hs = []
    ybin_lo = 1
    ybin_hi = h2.yaxis.get_nbins()+1
    if ylow is not None:
        ybin_lo = h2.yaxis.find_bin(ylow)
    if yhigh is not None:
        ybin_hi = h2.yaxis.find_bin(yhigh)+1

    for ybin in range(ybin_lo, ybin_hi):
        h = h2.projection_x('%s_%d'%(h2.name, ybin), ybin, ybin+window-1)
        if h.GetEntries() > 10:
            ymid = sum([h2.yaxis.get_bin_center(i+ybin) for i in range(0,window)])/window
            hs.append((ymid,h))

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
        for x, y in zip(gr.x(), gr.y()):
            if x > xlo and x < xhi and y > 0:
                yield (x, y)
    return genpoints


def get_points_from_TF1(f, xmin, xmax, npoints=100):
    X = np.linspace(xmin, xmax, npoints)
    return [(x,f.Eval(x)) for x  in X]


def get_edges_1d(h, loose=0, method='falling', threshold=0.001):
    # TODO incorporate "plateau" functions as a method
    # in the edge-finding methods
    thresh_abs = threshold*h.get_maximum()
    ymin = h.get_bin_low_edge(1)
    ymax = h.get_bin_low_edge(h.get_nbins_x()+1)
    ymin_err = 0
    if method == 'falling':
        ymin0 = h.get_bin_low_edge(h.find_first_bin_above(thresh_abs))
        ymin = h.get_bin_low_edge(h.get_maximum_bin())
        ymin = ymin-loose*(ymin-ymin0)
        yminerr = 0.25*(ymin-ymin0)
        ymax = h.get_bin_low_edge(1+h.find_last_bin_above(thresh_abs))
    return (ymin, ymax, yminerr)


def get_yedges_2d(h2, loose=0, xlow=None, xhigh=None, window=1,
                  method='falling', threshold=0.001):
    # TODO incorporate "plateau" functions as a method
    # in the edge-finding methods
    h1_projs = get_x_slices(h2, window, xlow, xhigh)
    edge_loY, edge_hiY, edge_loY_err = [], [], []
    for h1 in h1_projs:
        edge_x = h1[0]
        [edge_y_low, edge_y_high, edge_y_low_err] = get_edges_1d(asrootpy(h1[1]), loose=loose)
        if edge_y_low is not None:
            edge_loY.append((edge_x, edge_y_low))
            edge_loY_err.append(edge_y_low_err)
        if edge_y_high is not None:
            edge_hiY.append((edge_x, edge_y_high))
    return [edge_loY, edge_hiY, edge_loY_err]


def get_ylims_of_xmid(h2, method='falling', threshold=0.001):
    xmean = h2.projection_x().GetMean()
    xmeanbin = h2.xaxis.find_bin(xmean)
    h1mid3 = asrootpy(h2.projection_y('%s_py_mid3'%h2.name, xmeanbin-1, xmeanbin+1))
    thresh_abs = threshold*h1mid3.get_maximum()

    ymin = h1mid3.get_bin_low_edge(1)
    ymax = h1mid3.get_bin_low_edge(h1mid3.get_nbins_x()+1)
    if method == 'falling':
        ymin = h1mid3.get_bin_low_edge(h1mid3.get_maximum_bin())
        ymax = h1mid3.get_bin_low_edge(1+h1mid3.find_last_bin_above(thresh_abs))
    return (ymin, ymax)


def get_ymin_of_xmid(h2, method='falling', threshold=0.001):
    return get_ylims_of_xmid(h2, method, threshold)[0]


def get_ymax_of_xmid(h2, method='falling', threshold=0.001):
    return get_ylims_of_xmid(h2, method, threshold)[1]


def funcR_plateau(x, par):
    '''ROOT-friendly piece-wise "plateau" function.'''
    arg = x[0]
    fitval = 0
    if arg < par[2] or arg > par[3]: fitval = 0
    if arg >= par[2] and arg < par[0]: fitval = par[4]*(arg-par[2])/(par[0]-par[2])
    if arg >= par[0] and arg <= par[1]: fitval = par[4]
    if arg > par[1] and arg <= par[3]: fitval = par[4]*(arg-par[3])/(par[1]-par[3])
    return fitval


def get_plateau_edges(h1, loose=0, threshold=10, fitopts='RQSNW'):
    if h1.GetEntries() < threshold:
        return ([None, None], None)
    h1_xmin = h1.get_bin_low_edge(1)
    h1_xmax = h1.get_bin_low_edge(h1.get_nbins_x()+1)
    h1_bwidth = h1.get_bin_width(1)
    fplat = R.TF1('%s_fplat'%h1.name, funcR_plateau, h1_xmin, h1_xmax, 5)
    p0_xmin = b0_xmax = h1.get_bin_center(h1.find_first_bin_above(0.75*h1.get_maximum()))
    p1_xmax = b1_xmin = h1.get_bin_center(h1.find_last_bin_above(0.75*h1.get_maximum()))
    p0_xmax = p1_xmin = (p1_xmax+p0_xmin)/2
    b0_xmin = h1.get_bin_center(h1.find_first_bin_above(0))
    b1_xmax = h1.get_bin_center(h1.find_last_bin_above(0))
    fplat.SetParameters(p0_xmin, p1_xmax,
                        b0_xmax-h1_bwidth,
                        b1_xmin+h1_bwidth,
                        h1.get_maximum())
    fplat.SetParLimits(0, p0_xmin, p0_xmax)
    fplat.SetParLimits(1, p1_xmin+h1_bwidth, p1_xmax)
    fplat.SetParLimits(2, b0_xmin, b0_xmax-h1_bwidth)
    fplat.SetParLimits(3, b1_xmin+h1_bwidth, b1_xmax)
    fplat.SetParLimits(4, 0.75*h1.get_maximum(), h1.get_maximum())

    res = h1.fit(fplat, fitopts, '')

    b0, b1 = fplat.GetParameter(2), fplat.GetParameter(3)
    p0, p1 = fplat.GetParameter(0), fplat.GetParameter(1)
    d0, d1 = p0-b0, b1-p1
    x0, x1 = p0-d0*loose, p1+d1*loose
    return ([x0, x1], fplat)


def get_plateau_edges_2d(h2, loose=0, window=1, ylow=None, yhigh=None,
                         chi2ndf_thresh=None, transpose=False):
    get_slices = get_x_slices if transpose is True else get_y_slices
    h1_projs = get_slices(h2, window, ylow, yhigh)
    edge_loX, edge_hiX = [], []
    for h1 in h1_projs:
        edge_y = h1[0]
        ([edge_x_low, edge_x_high], fplat) = get_plateau_edges(asrootpy(h1[1]), loose=loose)
        if chi2ndf_thresh is None or chi2ndf_thresh > fplat.GetChisquare()/fplat.GetNDF():
            if edge_x_low is not None:
                edge_loX.append((edge_x_low, edge_y))
            if edge_x_high is not None:
                edge_hiX.append((edge_x_high, edge_y))
    return [edge_loX, edge_hiX]


def get_tgraph(X, Y):
    g = R.TGraph(len(X), array('d', X), array('d', Y))
    return g
