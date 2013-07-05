# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # IPython Notebook with PyROOT?
# 
# PyROOT exposes the ROOT framework as Python modules, which, in addition to providing access to ROOT through a higher-level language, unleashes the full variety of scientific and infrastructure libraries of Python, including non-PROOF distributed processing; interfaces to R, GnuPlot, symbolic math packages, etc.; the entire SciPy suite; and more.
# 
# In the case of this "Notebook," I would like to test the compatibility of PyROOT with SciPy's IPython Notebook, toward more easily reproducible research in a quasi-interactive environment.
# 
# First, let's try to open a ROOT file and perform some basic operations.

# <codecell>

import ROOT
from ROOT import TFile
fin_38729 = TFile('/data/e1f/skim/38729.root')

# <markdowncell>

# Notice that external functions (e.g., wrapped C++ functions) that print to stdout/stderr only print to the console that launched the notebook rather than to the notebook itself:

# <codecell>

fin_38729.ls()

# <markdowncell>

# So to print the hierarchy, I'll define a python function.

# <codecell>

from ROOT import TDirectory

def printkeys(ks, lvl=0):
    for k in ks:
        print('%s%s;%d\t%s' % ('  '*lvl, k.GetName(),
                               k.GetCycle(), k.GetTitle()))
        o = k.ReadObj()
        if isinstance(o, TDirectory): printkeys(o.GetListOfKeys(), lvl+1)

printkeys(fin_38729.GetListOfKeys())

# <codecell>

t = fin_38729.Get('3pi-tree/t3pi_physfrm')
print(t.GetEntries())

# <codecell>

from ROOT import TH1F
from ROOT import TLorentzVector
t.SetBranchStatus('*', 0)
t.SetBranchStatus('omega', 1)
# for some reason, SetBranchAddress must be called due to known bug
# see http://root.cern.ch/phpBB3/viewtopic.php?f=14&t=9450
lvomega = TLorentzVector()
t.SetBranchAddress('omega', lvomega)

h = TH1F('htest', 'hist test', 80, 0.4, 1.2)

for i, evt in enumerate(t):
    h.Fill(evt.omega.M())
    if i % 10000 == 0 and i > 0: print(i)
    
print(h.GetEntries())

# <markdowncell>

# Now, here's a limitation -- ROOT canvases will not print in the IPython Notebook.

# <codecell>

h.Draw()

# <markdowncell>

# So lets install the relatively new "pythonized pyroot" called rootpy (see https://github.com/rootpy/rootpy), assuming you already have dependencies (e.g., matplotlib) installed.

# <codecell>

# !!sudo pip2 install rootpy

# <markdowncell>

# First, let's make sure we can plot with rootpy.

# <codecell>

from rootpy.plotting import Canvas, Hist

def make_plot():
    canvas = Canvas(400, 300)
    hist = Hist(100, -3, 3)
    hist.FillRandom('gaus')
    hist.Draw()
    return canvas

canvas = make_plot()
# empty canvas since the histogram
# has been garbage collected!
canvas.Draw()

# <markdowncell>

# But it's still not printing into the notebook.  Let's try the matplotlib interface.

# <codecell>

import rootpy.plotting.root2matplotlib as rplt
import matplotlib.pyplot as plt

hist = Hist(100, -3, 3)
hist.FillRandom('gaus')
fig = plt.figure(figsize=(7, 5), dpi=100, facecolor='white')
rplt.errorbar(hist, xerr=False, emptybins=False)
plt.show()

# <markdowncell>

# Great! Now for a the originally intended histogram.

# <codecell>

del h
del canvas

h = Hist(80, 0.4, 1.2)

for i, evt in enumerate(t):
    h.Fill(evt.omega.M())
    if i+1 % 10000 == 0: print('processed %d' % i+1)
        
fig_mmp = plt.figure(figsize=(7, 5), dpi=100, facecolor='white')
rplt.errorbar(h, xerr=False, emptybins=False)
plt.show()

