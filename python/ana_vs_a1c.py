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
from multiprocessing import Process

from root_numpy import root2rec, root2array

np.set_printoptions(precision=4, suppress=True)
plt.rc('text', usetex=True)

d2r = pi/180
nbins = 100

# <markdowncell>

# # Load data from pickle files
# ## (if available)

# <codecell>

dfkin = pd.DataFrame.load('../testdata/ana_vs_a1c.p')

# <markdowncell>

# ### ... and continue below the "Save dataframes" section

# <markdowncell>

# # Load data from ROOT trees

# <codecell>

# tree data information for import
# branches = ['mcnpart', 'mcid', 'mcm', 'mcp', 'mctheta', 'mcphi']
branches = ['gpart', 'id', 'p', 'cx', 'cy', 'cz']
rootfiles = ['a1c.10.runindex.root', 'a1c.38121.runindexe1f.root', 'ana.10.runindexe1f.root', 'ana.10.runindex.root']
# data set names corresponding to each of rootfiles
dfnames = ['a1c10', 'a1c38121', 'ana10', 'ana10e1f']
# import tree data as 3 numpy structured records
treerecs = [root2rec('/data/e1f/anaVa1c/%s' % fn, treename='h10', branches=branches, selection='id[0]==11 && id[1]==2212 && id[2]==211 && id[3]==-211') for fn in rootfiles]
# associate data sets with dfnames
dfdict = dict(zip(dfnames, [pd.DataFrame(treerec) for treerec in treerecs]))
# create single data frame with multi-index
df = pd.Panel.from_dict(dfdict, orient='minor').swapaxes().to_frame()
del treerecs

# <codecell>

#for cname in ['gpart', 'id']:
#    df[cname] = df[cname].astype(int)
#for cname in ['p', 'cx', 'cy', 'cz']:
#    df[cname] = df[cname].astype(float)
df

# <markdowncell>

# # Add lab-frame 4-momentum components

# <codecell>

df['pz'] = df.p*df.cz
df['py'] = df.p*df.cy
df['px'] = df.p*df.cx
masses = {11:0.000511, 211:0.13957, -211:0.13957, 2212:0.93827, 2112:0.939565, 0:0, 22:0, 321:0.493667, -321:0.493667, 45:0, 47:0, 49:0}
getM2 = lambda x: [masses[pid]**2 for pid in x]
df['energy'] = (df.p*df.p + df.id.apply(getM2)).apply(lambda x: np.sqrt(x))

# <markdowncell>

# # Add particle index columns

# <codecell>

"""df.mcid is a series of arrays, where each array contains the particle ids of the
event.  Other particle-level df columns contain arrays with particle property values
in the same order."""
# define curry function that takes as argument desired particle ids
# pidxs_l = lambda partids: lambda x: [np.where(x==partid)[0][0] for partid in partids]
def pidxs_l(partids):
    def pidxs(x):
        retval = np.empty(5)
        for i, partid in enumerate(partids):
            w = np.where(x==partid)[0]
            retval[i] = w[0] if len(w) > 0 else -1
        return retval
            # yield w[0] if len(w) > 0 else -1
    return pidxs
# pidxs returns a list of particle indexes
# in this case the order is...
pidxs = pidxs_l(np.array([11, 2212, 211, -211, 111]))
# to correpsond to column labels
cnames = ['idxE', 'idxP', 'idxPip', 'idxPim', 'idxPi0']
# apply pidxs to the mcid series; result is a series of lists, but I want a series
# for each part id, so...
# pull out the series of mcid lists as a 2d numpy array, which provides nice slicing
# features (could have been another dataframe, I suppose?)
arr2d = np.array(list(df.id.apply(pidxs).values), dtype=np.short)
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

df

# <codecell>

e04 = np.array([5.497, 0, 0, 5.497], dtype=np.float16)
p04 = np.array([0.938, 0, 0, 0], dtype=np.float16)
v4idxs = ['energy', 'px', 'py', 'pz']
def getkin(x):
    [iE, iPx, iPy, iPz] = v4idxs
    e4 = np.array([x[iE][0], x[iPx][0], x[iPy][0], x[iPz][0]], dtype=np.float16)
    p4 = np.array([x[iE][1], x[iPx][1], x[iPy][1], x[iPz][1]], dtype=np.float16)
    pip4 = np.array([x[iE][2], x[iPx][2], x[iPy][2], x[iPz][2]], dtype=np.float16)
    pim4 = np.zeros(5, dtype=np.float16)  # np.array([x[iE][3], x[iPx][3], x[iPy][3], x[iPz][3]])
    pi04 = np.zeros(5, dtype=np.float16)  # np.array([x[iE][4], x[iPx][4], x[iPy][4], x[iPz][4]])
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
    return pd.Series(data=(rot4, float16(W), Q2, costheta, phi, s, t, u, t0, t1, e04, p04, e4, p4, pip4, pim4, pi04, q4, w4, mmp))

# <markdowncell>

# ## Apply and add columns

# <codecell>

df

# <codecell>

# apply to data frame
dfkin = df[(df.idxE==0) & (df.idxP>0) & (df.idxPip>0)].apply(getkin, axis=1)
dfkin.columns = ['R', 'W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'e04', 'p04', 'e4', 'p4', 'pip4', 'pim4', 'pi04', 'q4', 'w4', 'mmp']
# convert non-4-vectors to floats
dfkin[['W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'mmp']] = dfkin[['W', 'Q2', 'costheta', 'phi', 's', 't', 'u', 't0', 't1', 'mmp']].astype(float16)
dfkin['theta'] = np.arccos(dfkin.costheta)
# dfkin['px'] = df['px']
# dfkin['py'] = df['py']
# dfkin['pz'] = df['pz']
# dfkin['energy'] = df['energy']
del df

# <markdowncell>

# # Save dataframes

# <codecell>

dfkin.save('../testdata/ana_vs_a1c.p')

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

grpd.mmp.agg(np.mean)

# <codecell>

grpd.mmp.agg([np.mean,np.std])

# <codecell>

fabove = lambda x: np.sum(x>=0.795)
fbelow = lambda x: np.sum(x<0.795)
smmry = grpd.mmp.agg({'> 0.795':fabove, '< 0.795':fbelow})
smmry['%'] = 100*smmry['> 0.795']/(smmry['> 0.795']+smmry['< 0.795'])
smmry

# <codecell>

fabove = lambda x: np.sum(x>=0.84)
fbelow = lambda x: np.sum(x<0.84)
smmry = grpd.mmp.agg({'> 0.84':fabove, '< 0.84':fbelow})
smmry['%'] = 100*smmry['> 0.84']/(smmry['> 0.84']+smmry['< 0.84'])
smmry

# <codecell>

hmmps = compareDists('mmp', nbins=100, xlim=[0.7,0.866], figsize=(12,8), ymin=0)

# <markdowncell>

# # Missing momentum angles

# <codecell>

hphis = compareDists('phi', xlabel=r'$\phi$ $(radians)$', figsize=(12,8), ymin=0, nbins=40)
hcoss = compareDists('costheta', xlabel=r'$cos(\theta)$', fnum=2, yscale='log', figsize=(12,8), nbins=40)
hthetas = compareDists('theta', xlabel=r'$\theta$ $(radians)$', fnum=3, figsize=(12,8), ymin=0, nbins=40)

# <codecell>

hthetasZoom = compareDists('costheta', xlabel=r'$\theta$ $(radians)$', fnum=3,
                           figsize=(12,8), xlim=[0.9,1], nbins=20, yscale='linear')

# <markdowncell>

# # t-slope

# <codecell>

ht1s = compareDists('t1', groups=grpd, xlabel=r'$t-t_{o}$', figsize=(14,8), nbins=40)

# <codecell>

dfkin.t1

# <codecell>


