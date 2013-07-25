# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # Playing with tabular data
# 
# Currently, my analysis workflow involves increasingly high-level programming constructs as the data progresses through stages of reduction.
# 
# * C++ is used to perform performance sensitive tasks on large sets of event-level data, for example:
#     * simulations and reconstruction
#     * particle identification
#     * momentum corrections
#     * energy loss corrections
#     * fiducial cuts
#     * detector-level efficiencies
#     * skim event selection
#     * transformation into multidimensional binned data
# * C++/Cint or Pyroot scripts are used for binned data analysis
#     * acceptance corrections
#     * hole corrections
#     * fitting
#     * background subtraction
#     * cross-section extraction
#     * generation of tabular data
# * Python with Pandas and rootpy
#     * analysis of tabular data
#     * plotting of tabular data
# 
# The final bullet point above is only an idea, which I'm exploring here.

# <markdowncell>

# ## Data format
# 
# The following sections require an existing CSV file produced by ana_bd.py, which has the following columns:
# 
#     Wbin, Q2bin, W, Wlo, Whi, Q2, Q2lo, Q2hi,
#     weight3s, weightsb, fbgstat, fbgchi2, fsigstat, fsigchi2,
#     fbgpar0, fbgpar1, fbgpar2, fbgpar3, fbgpar4, fbgpar5,
#     fsigpar0, fsigpar1, fsigpar2, fsigpar3, hsigint3s,
#     xsect, err

# <markdowncell>

# ## ROOT
# 
# ROOT's TTree and TGraphError already support direct input of tabular data, and it is excellent for quick plotting of final data.  The column names and types can be provided at the command line or on the first line of the file.  I usually add a header line:
# 
# > Wbin/I,Q2bin,W/F,Wlo,Whi,Q2,Q2lo,Q2hi,weight3s,weightsb,
# > fbgstat/C,fbgchi2/F,fsigstat/C,fsigchi2/F,
# > fbgpar0,fbgpar1,fbgpar2,fbgpar3,fbgpar4,fbgpar5,
# > fsigpar0,fsigpar1,fsigpar2,fsigpar3,hsigint3s,xsect,err
# 
# TGraph and TGraphErrors can take a file name and format string encoding the column numbers to plot, but has no way to select specific rows.  The current data file includes data across many $W$ and $Q^{2}$ bins, so we cannot use the TGraph variants directly.  Instead we will use TTree's ReadFile function and then produce plots via it's Draw() function.
# 
# To illustrate ROOT's builtin mechanism here, I'll use the PyRoot wrapper so that it shows up in this notebook.  The analogous C++-flavored commands will work in CInt.

# <codecell>

from array import array
from ROOT import gPad, TTree, TGraphErrors

t = TTree()
t.ReadFile('out/ana_bd_root.dat', '', ',')
t.Draw('W:xsect:err', 'Q2bin==5 && fsigchi2 < 5', 'goff')
npoints = t.GetSelectedRows()
passbyreftrick = array('d', [0]*npoints) # not needed in CInt
g = TGraphErrors(npoints, t.GetV1(), t.GetV2(), passbyreftrick, t.GetV3())
g.Draw('ae*')  # draws TCanvas external to this notebook

# to draw inline
from IPython.core.display import Image
imgfn = 'ipn_tabular_tgraph.png'
gPad.SaveAs(imgfn)
Image(filename=imgfn)

# <markdowncell>

# ## Python using DataFrames and matplotlib
# 
# Aside from Python's core high-level programming mechanisms, it offers integration to pre-existing statistical analysis and plotting mechanisms, like
# 
# * ROOT via primitive wrapper classes of PyRoot as in previous section or via the more Pythonized rootpy
# * R via RPy and RPy2 for all the power of decades the language made from the ground up for statistical analysis and used widely in other fields.
# 
# It also provides powerful native libraries for every aspect of scientific analysis from simulation/modeling and data analysis to symbolic analysis.  For tabular data, traditional R-like statistical programming concepts -- e.g., data frames, slicing, indexing, subsetting, group, etc. -- are very well suited.  While there are many Python-oriented options along these lines, I'll use the Pandas project, part of SciPy, here to illustrate functionality equivalent to the previous ROOT example.

# <codecell>

import pandas as pd
import matplotlib.pylab as plt

df = pd.read_csv('out/ana_bd.dat')
dftmp = df[['W', 'Q2', 'xsect', 'err', 'fsigchi2']][(df['fsigchi2']<3) & (df['Q2bin']==5)]
plt.errorbar(dftmp.W, dftmp.xsect, yerr=dftmp.err, ls='none', marker='o')
plt.show()

# <markdowncell>m

# ### Deconstructing the subsetting line
# 
# The most notable aspect of the above snippet is the data selection syntax inherited from, or at least strongly influenced by, a traditional "data frame" concept as implemented in statistical languages like S, R, and their variants.  Let's explore the anatomy of the data frame and the subsetting features used above.  First look at the main data frame:

# <codecell>

# a summary of the data is printed when there's "too much" to display
print(df)

# <codecell>

# in this "too much" case, use "columns" and "values"
# to peak at first 3 rows of data
print(df.columns)
print(df[0:3].values)

# <codecell>

# or better yet, change what "too much" means
pd.set_printoptions(max_columns=27)
print(df[0:3])

# <markdowncell>

# Now for the subsetting features...

# <codecell>

# create a data frame consisting of a subset of columns
dfsubsetcols = df[['W', 'Q2', 'xsect', 'err']]
print(dfsubsetcols)

# <codecell>

dfsubsetrows = df[0:3]
print(dfsubsetrows)

# <codecell>

dfsubsetrowscols = df[['W', 'Q2', 'xsect', 'err']][0:3]
# OR using loc[row_indexer, col_indexer], but noting the inclusive nature
#   of the indexers:
# dfsubsetrowscols = dfsubsetcols.loc[0:2]
# dfsubsetrowscols = dfsubsetcols.loc[0:2,['W', 'Q2', 'xsect', 'err']]
print(dfsubsetrowscols)

# <codecell>

# columns can be accessed as members or keys
print(df.W[0:3])
print(df['W'][0:3])

# <codecell>

# operations are vectorized
print(df.fsigchi2[0:3] < 5)
print(df.W[0:3])
print(df.W[0:3]*1000)
# boolean vectors can serve as selection criteria
#  -- think of the vector as signifying which row indices to keep
print(dfsubsetcols[(df.Wlo >= 2.0) & (df.Whi <= 2.04)])

# <markdowncell>

# Now we can make sense of this line:
# 
# > dftmp = df[['W', 'Q2', 'xsect', 'err', 'fsigchi2']][(df['fsigchi2']<3) & (df['Q2bin']==5)]
# 
# df[[...]] returns a dataframe with all rows but only a subset of columns, and it is, in turn, operated on my the row selection criteria of [() & ()] which is, by itself, a boolean array given by the bitwise vector operation of two boolean arrays.  It could have been decomposed into this:

# <codecell>

dfxsects = df[['W', 'Q2', 'xsect', 'err', 'fsigchi2']]
goodfits = dfxsects.fsigchi2 < 3
# df can also be used to define a valid boolean array to apply to dfxsects,
# because, at this point, it has the ordered elements that are in complete
# correspondence with dfxsects...
q2bin5 = df.Q2bin == 5
select = goodfits & q2bin5
print(dfxsects[select])

# <markdowncell>

# ### Multiple plots
# 
# In ROOT, plotting multiple groups of data would be accomplished procedural control sctructures, TTree iteration, and multiple TGraphErrors/TMultiGraph or TH1s/THStack.
# 
# In Python with DataFrames, we'll just *groupby* and iterate.

# <codecell>

dfgb = dfxsects[(dfxsects['fsigchi2']<3) & (df['fsigstat'] == 'CONVERGED ')].groupby('Q2')

# <codecell>

fig = plt.figure(figsize=(18, 12))
gtitle = '$Q^2 = %.3f$ $GeV^2$'
for gid, g in dfgb:
    plt.errorbar(g.W, g.xsect, g.err, ls='none', marker='o', markersize=4, label=gtitle%gid)
plt.ylim((0, 2500))
plt.xlabel('$W$  $(GeV)$', fontsize=20)
plt.ylabel('$\sigma$  ($nb^{-1}$)', fontsize=20)
plt.legend(fontsize=20)
plt.show()

# or on individual canvases
# for gid, g in dfgb:
#     fig = plt.figure(figsize=(12, 8))
#     plt.errorbar(g.W, g.xsect, g.err, ls='none', marker='o', ms=4)
#     plt.ylim((0, 2500))
#     plt.xlim((1.6, 3.0))
#     plt.title(gtitle%gid, fontsize=32)
#     plt.xlabel('W (GeV)', fontsize=20)
#     plt.ylabel('$\sigma$  ($nb^{-1}$)', fontsize=20)
#     plt.show()
    
# or on individual subpads
nrows, ncols = 2, 4
fig = plt.figure(figsize=(18, 12))
for cell, (gid, g) in enumerate(dfgb):
    subplot = fig.add_subplot(nrows, ncols, cell+1)
    plt.errorbar(g.W, g.xsect, g.err, ls='none', marker='o', ms=4)
    plt.ylim((0, 2500))
    plt.xlim((1.6, 3.0))
    plt.title(gtitle%gid, fontsize=20)
    plt.xlabel('W (GeV)', fontsize=16)
    plt.ylabel('$\sigma$  ($nb^{-1}$)', fontsize=20)
plt.tight_layout()
plt.show()
    

# <markdowncell>

# ### Fitting
# 
# Python with Scipy has a robust, and again R/S-reminiscent, set of optimization (including fitting) tools, but the beauty of using Python is that you can combine tools that that you prefer for each subtask.  Perhaps we can use Python for its brevity (compared to equivalent C++), matplotlib for nice-looking plots, Pandas for data frame tools, and ROOT for fitting (and of course for any event-level out-of-memory processing)... or some combination therein according to personal preference.
# 
# The following example is a bit of a mutt.

# <codecell>

import rootpy.plotting.root2matplotlib as rplt
from rootpy.plotting import Graph
from rootpy import asrootpy

# use a numpy recarray
ra = dftmp.to_records()

# use rootpy's matplotlib-plottable Graph
g = Graph(ra.shape[0])
for n, (w, x, e) in enumerate(zip(ra.W, ra.xsect, ra.err)):
    g.SetPoint(n, w, x)
    g.SetPointError(n, 0, 0, e, e)

# use ROOT to Fit
g.Fit('pol8')

# plot Graph
rplt.errorbar([g])

# plot fit function
# Plottable TF1 not yet implemented, so...
f = g.GetListOfFunctions()[0]
xs = arange(1.6, 3.0, 0.001)
ys = [f.Eval(x) for x in xs]
plt.plot(xs, ys)

# <codecell>


