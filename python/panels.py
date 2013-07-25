# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # All imports

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

# <markdowncell>

# # Load data from ROOT trees

# <codecell>

# tree data information for import
branches = ['mcnpart', 'mcid', 'mcm', 'mcp', 'mctheta', 'mcphi']
rootfiles = ['genev000_a.root', 'genev100_a.root', 'genev200_a.root']
# data set names corresponding to each of rootfiles
dfnames = ['000', '100', '200']
# import tree data as 3 numpy structured records
treerecs = [root2rec('../testdata/%s' % fn, branches=branches) for fn in rootfiles]
# associate data sets with dfnames
dfdict = dict(zip(dfnames, [pd.DataFrame(treerec) for treerec in treerecs]))
# create single data frame with multi-index
df = pd.Panel.from_dict(dfdict, orient='minor').swapaxes().to_frame()

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
    o4cm = (R(e04, e4, p04).dot(o4))
    o3cm = o4cm[1:]
    costheta = o3cm[2]/la.norm(o3cm)
    phi = atan2(o3cm[1],o3cm[0])
    return pd.Series(data=(W, Q2, costheta, phi, s, e04, p04, e4, p4, pip4, pim4, pi04, q4, w4))

# <markdowncell>

# ## Apply and add columns

# <codecell>

# apply to data frame
dfkin = df.apply(getkin, axis=1)
dfkin.columns = ['W', 'Q2', 'costheta', 'phi', 's', 'e04', 'p04', 'e4', 'p4', 'pip4', 'pim4', 'pi04', 'q4', 'w4']
# convert non-4-vectors to floats
dfkin[['W', 'Q2', 'costheta', 'phi', 's']] = dfkin[['W', 'Q2', 'costheta', 'phi', 's']].astype(float64)

# <markdowncell>

# # Missing mass of proton

# <codecell>

mmp4 = dfkin.w4-dfkin.p4
v4inv = lambda x: sqrt2(x.dot(g*x))
dfkin['mmp'] = mmp4.apply(v4inv)

# <codecell>

grpd = dfkin[['W', 'Q2', 'costheta', 'phi', 'mmp']].groupby(level=0)
hmmps = {}
plt.figure(1, figsize=(15,5))
for i, (k, v) in enumerate(grpd):
    h = hmmps[k] = histogram(v.mmp, bins=1000)
    Y = h[0]
    X = (h[1][1:]+h[1][:-1])/2
    plt.subplot(1,3,i+1)
    plt.errorbar(X, Y, yerr=np.sqrt(Y), fmt='ro', ms=2)
    plt.xlim([0.765, 0.8])
    plt.title(k, fontsize=24)
    plt.xlabel('$m_\omega$ $(GeV)$', fontsize=15)
plt.tight_layout()
grpd['mmp'].agg([np.mean, np.std])

# <codecell>


# <codecell>

# already did this once, so lets make a function
def compareDists(cname, grpd=grpd, fnum=1, xlim=None, xlabel=None, yscale='linear'):
    hs = {}
    plt.figure(fnum, figsize=(12,4))
    for i, (k, v) in enumerate(grpd):
        xlim = [v[cname].min(), v[cname].max()] if xlim is None else xlim
        xlabel = k if xlabel is None else xlabel
        h = hs[k] = histogram(v[cname], bins=1000)
        Y = h[0]
        X = (h[1][1:]+h[1][:-1])/2
        plt.subplot(1,3,i+1)
        plt.errorbar(X, Y, yerr=np.sqrt(Y), fmt='ro', ms=2)
        plt.xlim(xlim)
        plt.title(k, fontsize=24)
        plt.xlabel(xlabel, fontsize=15)
        plt.yscale(yscale)
    plt.tight_layout()
    return hs

hphis = compareDists('phi', xlabel=r'$\phi$ $(radians)$')
hcoss = compareDists('costheta', xlabel=r'$cos(\theta)$', fnum=2, yscale='log')

# <codecell>

h = hcoss['000']
Y = h[0]
X = (h[1][1:]+h[1][:-1])/2
plt.figsize(12,8)
ymax4loX = 2*np.max(Y[0:len(Y)/2])
ymax4hiX = 2*np.max(Y)
xrs = [[0.5,1],[0.9,1],[-1,-0.5],[-1,-0.9]]
yrs = [[20,ymax4hiX],[100,ymax4hiX],[1,ymax4loX],[1,ymax4loX]]
for i, (xlims, ylims) in enumerate(zip(xrs,yrs)):
    print ylims
    plt.subplot(2, 2, i+1)
    plt.errorbar(X, Y, yerr=np.sqrt(Y), fmt='ro', ms=4)
    plt.xlim(xlims)
    plt.yscale('log')
    plt.ylim(ylims)
    plt.title('000', fontsize=24)
    plt.xlabel(r'$cos(\theta)$', fontsize=15)
    plt.tight_layout()

# <markdowncell>

# # Notes

# <codecell>

plt.figure(1, figsize=(15, 5))
plt.subplot(131)
hphi = hist(dfkin.phi, bins=100)
plt.subplot(132)
hcos = hist(dfkin.costheta, bins=100)
plt.subplot(133)
hcos = hist(dfkin.costheta.apply(lambda x: np.arccos(x)), bins=100)

# <codecell>

class Parameter:
    def __init__(self, value):
            self.value = value

    def set(self, value):
            self.value = value

    def __call__(self):
            return self.value

def fit(function, parameters, y, x = None):
    def f(params, x, y):
        i = 0
        for p in parameters:
            p.set(params[i])
            i += 1
        return y - function(x)

    # if x is None: x = np.arange(y.shape[0])
    p = [param() for param in parameters]
    return optimize.leastsq(f, p, args=(x, y))

# <codecell>

mmp4 = dfkin.w4-dfkin.p4
v4inv = lambda x: sqrt2(x.dot(g*x))
mmp = mmp4.apply(v4inv)
hmmp = histogram(mmp, bins=100)  # hist(mmp, bins=100)

# set initial parameters and define function
mu = Parameter(0.77)  # mmp.mean())
sigma = Parameter(0.01)  # mmp.std())
height = Parameter(2000)  # hmmp[1].max())
def gauss(x): return height() * exp(-((x-mu())/sigma())**2)

# fit
Y = hmmp[0]
X = (hmmp[1][1:]+hmmp[1][:-1])/2
fitresult = fit(gauss, [mu, sigma, height], Y, X)
print(fitresult[0])
print(fitresult[1])
Xl = np.linspace(hmmp[1].min(), hmmp[1].max(), 1000)
plt.figure(1, figsize=(9,6))
plt.errorbar(X, Y, yerr=np.sqrt(Y), fmt='ro', ms=2)
plt.plot(Xl, [gauss(x) for x in Xl], 'g-', lw=2)

# <codecell>


