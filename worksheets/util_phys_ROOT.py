import ROOT
from rootpy.io import root_open
from rootpy.plotting import Hist, Hist2D, Hist3D, HistStack
from rootpy.tree import TreeChain, Tree
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt
from matplotlib import rc
rc('text', usetex=True)
from matplotlib.colors import LogNorm
import matplotlib.gridspec as gridspec

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
    if isinstance(hist, R.TH2):
        rplt.hist2d(hist, axes=subplot, **kwargs)
    else:
        rplt.errorbar([hist], xerr=False, emptybins=False, **kwargs)
    # plt.show()
    return fig

def twos_comp(val):
    if (val & (1 << 7)) != 0:
        val = val - (1 << 8)
    return val

