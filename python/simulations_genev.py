# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # All imports and some settings

# <codecell>

from math import atan2
import functools as ft
import numpy as np
import numpy.linalg as la
from scipy import optimize
import pandas as pd
import matplotlib.pyplot as plt

from root_numpy import root2rec, root2array

np.set_printoptions(precision=4, suppress=True)
plt.rc('text', usetex=True)

d2r = pi/180
nbins = 500

# <markdowncell>

# # Load data from pickle files
# ## (if available)

# <codecell>

dfkin = pd.DataFrame.load('../testdata/genev_dfkin.p')

# <markdowncell>

# ### ... and continue below the "Save dataframes" section

# <markdowncell>

# # Load data from ROOT trees

# <codecell>

# tree data information for import
branches = ['mcnpart', 'mcid', 'mcm', 'mcp', 'mctheta', 'mcphi']
rootfiles = ['genev000.root', 'genev100.root', 'genev200.root', 'genev101.root', 'genev102.root']
# data set names corresponding to each of rootfiles
dfnames = ['000', '100', '200', '101', '102']
# import tree data as 3 numpy structured records
treerecs = [root2rec('../testdata/%s' % fn, branches=branches) for fn in rootfiles]
# associate data sets with dfnames
dfdict = dict(zip(dfnames, [pd.DataFrame(treerec) for treerec in treerecs]))
# create single data frame with multi-index
df = pd.Panel.from_dict(dfdict, orient='minor').swapaxes().to_frame()
del treerecs

# <markdowncell>

# # Add lab-frame 4-momentum components

# <codecell>

dfrad = df[['mctheta','mcphi']]*d2r
dfcos = dfrad.applymap(np.cos)
dfsin = dfrad.applymap(np.sin)
df['pz'] = df.mcp*dfcos.mctheta
df['py'] = df.mcp*dfsin.mctheta*dfsin.mcphi
df['px'] = df.mcp*dfsin.mctheta*dfcos.mcphi
df['energy'] = (df.mcp*df.mcp + df.mcm*df.mcm).apply(lambda x: np.sqrt(x))
del dfrad
del dfcos
del dfsin

# <markdowncell>

# # Add particle index columns

# <codecell>

"""df.mcid is a series of arrays, where each array contains the particle ids of the
event.  Other particle-level df columns contain arrays with particle property values
in the same order."""
# define curry function that takes as argument desired particle ids
pidxs_l = lambda partids: lambda x: [np.where(x==partid)[0][0] for partid in partids]
# pidxs returns a list of particle indexes
# in this case the order is...
pidxs = pidxs_l(np.array([11, 2212, 211, -211, 111]))
# to correpsond to column labels
cnames = ['idxE', 'idxP', 'idxPip', 'idxPim', 'idxPi0']
# apply pidxs to the mcid series; result is a series of lists, but I want a series
# for each part id, so...
# pull out the series of mcid lists as a 2d numpy array, which provides nice slicing
# features (could have been another dataframe, I suppose?)
arr2d = np.array(list(df.mcid.apply(pidxs).values))
# each row corresponds to an event, each column to the ith particle in the event
# so I'll add a column for each particle type; the values will be the position
# of the particle type in the arrays of other columns/series in the df.
for cn, vals in zip(cnames, [arr2d[:,icol] for icol in range(0,5)]):
    df[cn] = pd.Series(vals, name=cn, index=df.index)
del arr2d

# <markdowncell>

# # Calculate kinematic variables
# 
# ## Define Lorentz rotation

# <codecell>

sqrt2 = lambda x: sqrt(x) if x >= 0 else -sqrt(-x)
g = np.array([1, -1, -1, -1])
boost = lambda beta, gamma: np.array([ [gamma,0,0,-beta*gamma], [0,1,0,0], [0,0,1,0], [-beta*gamma,0, 0, gamma] ])
def R(e04, e14, p04):
    e0, e1 = e04[1:], e14[1:]
    q = (e0-e1)
    v3 = q/la.norm(q)
    v2 = ft.partial(lambda x: x/la.norm(x), np.cross(e0,e1))()
    v1 = np.cross(v2, v3)
    v = np.array([v1, v2, v3])
    rot4_3rot = np.hstack(([[1], [0], [0], [0]], np.vstack(([[0, 0, 0]], v))))
    w4 = e04-e14+p04
    E, p = w4[0], la.norm(w4[1:])
    m = sqrt2(E*E-p*p)
    beta, gamma = p/E, E/m
    return boost(beta, gamma).dot(rot4_3rot)

# <markdowncell>

# ## Define kinematic variables

# <codecell>

e04 = np.array([5.497, 0, 0, 5.497])
p04 = np.array([0.938, 0, 0, 0])
v4idxs = ['energy', 'px', 'py', 'pz']
def getkin(x):
    [iE, iPx, iPy, iPz] = v4idxs
    e4 = np.array([x[iE][0], x[iPx][0], x[iPy][0], x[iPz][0]])
    p4 = np.array([x[iE][1], x[iPx][1], x[iPy][1], x[iPz][1]])
    pip4 = np.array([x[iE][2], x[iPx][2], x[iPy][2], x[iPz][2]])
    pim4 = np.array([x[iE][3], x[iPx][3], x[iPy][3], x[iPz][3]])
    pi04 = np.array([x[iE][4], x[iPx][4], x[iPy][4], x[iPz][4]])
    q4 = e04-e4
    w4 = q4+p04
    Q2 = -np.sum(q4*g*q4)
    s = np.sum(w4*g*w4)
    W = sqrt2(s)
    o4 = w4-p4
    mmp = sqrt2(np.sum(o4*g*o4))
    t4 = p04-p4
    u4 = q4-p4
    t = np.sum(t4*g*t4)
    u = np.sum(u4*g*u4)
    rot4 = R(e04, e4, p04)
    p04cm, p4cm = rot4.dot(p04), rot4.dot(p4)
    t0 = 2*(0.938**2 - p04cm[0]*p4cm[0] + la.norm(p04cm[1:])*la.norm(p4cm[1:]))
    t1 = t-t0
    o4cm = (rot4.dot(o4))
    o3cm = o4cm[1:]
    costheta = o3cm[2]/la.norm(o3cm)
    phi = atan2(o3cm[1],o3cm[0])
    return pd.Series(data=(rot4, W, Q2, costheta, phi, s, t, u, t0, t1, e04, p04, e4, p4, pip4, pim4, pi04, q4, w4, mmp))

# <markdowncell>

# ## Apply and add columns

# <codecell>

# apply to data frame
dfkin = df.apply(getkin, axis=1)
dfkin.columns = ['R', 'W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'e04', 'p04', 'e4', 'p4', 'pip4', 'pim4', 'pi04', 'q4', 'w4', 'mmp']
# convert non-4-vectors to floats
dfkin[['W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'mmp']] = dfkin[['W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'mmp']].astype(float64)
dfkin['theta'] = np.arccos(dfkin.costheta)
dfkin['px'] = df['px']
dfkin['py'] = df['py']
dfkin['pz'] = df['pz']
dfkin['energy'] = df['energy']
del df

# <markdowncell>

# # Save dataframes

# <codecell>

dfkin.save('../testdata/genev_dfkin.p')

# <markdowncell>

# # Group by simulation

# <codecell>

grpd = dfkin[['W', 'Q2', 'costheta', 'phi', 'mmp', 'theta', 't1']].groupby(level=0)

# <codecell>

def compareDists(cname, groups=grpd, nbins=nbins, edges=None, figsize=(12,4), layout=[2,3], fnum=1, xlim=None, ylim=None, ymin=None, xlabel=None, yscale='linear'):
    hs = {}
    plt.figure(fnum, figsize=figsize)
    for i, (k, v) in enumerate(groups):
        xlim = [v[cname].min(), v[cname].max()] if xlim is None else xlim
        xlabel = k if xlabel is None else xlabel
        nbins = nbins if edges is None else edges
        h = hs[k] = histogram(v[cname], bins=nbins, range=xlim)
        Y = h[0]
        X = (h[1][1:]+h[1][:-1])/2
        plt.subplot(*(layout+[i+1]))
        plt.errorbar(X, Y, yerr=np.sqrt(Y), fmt='ro', ms=2)
        plt.xlim(xlim)
        if ylim: plt.ylim(ylim)
        plt.title(k, fontsize=24)
        plt.xlabel(xlabel, fontsize=15)
        plt.yscale(yscale)
        if ymin is not None:
            plt.ylim(ymin=ymin)
    plt.tight_layout()
    return hs

# <markdowncell>

# # Missing mass of proton

# <codecell>

grpd.mmp.agg([np.mean,np.std])

# <codecell>

fabove = lambda x: np.sum(x>=0.795)
fbelow = lambda x: np.sum(x<0.795)
smmry = grpd.mmp.agg({'> 0.795':fabove, '< 0.795':fbelow})
smmry['%'] = 100*smmry['> 0.795']/(smmry['> 0.795']+smmry['< 0.795'])
smmry

# <codecell>

hmmps = compareDists('mmp', edges=np.arange(0.765,1.0,0.000235), xlim=[0.765,0.81], figsize=(12,8), ymin=0)

# <markdowncell>

# # Missing momentum angles

# <codecell>

hphis = compareDists('phi', xlabel=r'$\phi$ $(radians)$', figsize=(12,8), ymin=0)
hcoss = compareDists('costheta', xlabel=r'$cos(\theta)$', fnum=2, yscale='log', figsize=(12,8))
hthetas = compareDists('theta', xlabel=r'$\theta$ $(radians)$', fnum=3, figsize=(12,8), ymin=0)

# <codecell>

hthetasZoom = compareDists('costheta', xlabel=r'$\theta$ $(radians)$', fnum=3,
                           figsize=(12,8), xlim=[0.9,1], nbins=100, yscale='log')

# <codecell>

df10x = dfkin.ix['100':'102']
hcts10xhi = compareDists('costheta', fnum=1, groups=df10x.loc[df10x.mmp>0.797].groupby(level=0), layout=[1,3], nbins=100, ymin=0)
hcts10xlo = compareDists('costheta', fnum=2, groups=df10x.loc[df10x.mmp<=0.797].groupby(level=0), layout=[1,3], nbins=100, ymin=0)

# <codecell>

hphis10xhi = compareDists('phi', fnum=1, groups=df10x.loc[df10x.mmp>0.797].groupby(level=0), layout=[1,3], nbins=100, ymin=0)
hphis10xlo = compareDists('phi', fnum=2, groups=df10x.loc[df10x.mmp<=0.797].groupby(level=0), layout=[1,3], nbins=100, ymin=0)

# <markdowncell>

# # t-slope

# <codecell>

ht1s = compareDists('t1', groups=grpd, xlabel=r'$t-t_{o}$', figsize=(14,10), nbins=500)

# <codecell>

grpd.mmp.hist(bins=np.arange(0.7, 1, 0.0005), pads=True)
plt.autoscale(True, 'both')

# <codecell>


