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
    # plt.show()
    return fig

def mdraw(hists, ncols=1, nrows=1, **kwargs):
    fig = plt.figure(figsize=goldenaspect(6.5, nrows, ncols))
    for (ih, h) in enumerate(hists):
        draw(h, ncols, nrows, ih, fig=fig)
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

# u'$\Delta t$ (ns) assuming proton mass'
# u'momentum (GeV)'