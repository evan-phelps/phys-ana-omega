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

# from IPython.core.display import Image

np.set_printoptions(precision=4, suppress=True)

# <markdowncell>

# # Load TTree into DataFrame

# <codecell>

ar = root2rec('../testdata/genev100_a.root',
              branches=['mcnpart', 'mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])

# <codecell>

df = pd.DataFrame(ar)
dftmp = df[0:100]

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
    df[cn] = pd.Series(vals, name=cn)

# <markdowncell>

# # Perform some column-wise operations

# <codecell>

# technically, these are element-wise (applymap versus apply)
d2r = pi/180
# dfrad = df[['mctheta','mcphi']].applymap(lambda x: np.cos(d2r*x))
dfrad = df[['mctheta','mcphi']]*d2r
dfcos = dfrad.applymap(np.cos)
dfsin = dfrad.applymap(np.sin)

# <codecell>

# these are column-wise
df['pz'] = df.mcp*dfcos.mctheta
df['py'] = df.mcp*dfsin.mctheta*dfsin.mcphi
df['px'] = df.mcp*dfsin.mctheta*dfcos.mcphi
df['energy'] = (df.mcp*df.mcp + df.mcm*df.mcm).apply(lambda x: np.sqrt(x))

# <markdowncell>

# # Perform some row-wise operations

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

# TODO: separate 4-vector series creation from operations that can be performed column-wise
# after having them!  This row-wise calculation might be expensive (test it).
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
dfkin = df.apply(getkin, axis=1)
dfkin.columns = ['W', 'Q2', 'costheta', 'phi', 's', 'e04', 'p04', 'e4', 'p4', 'pip4', 'pim4', 'pi04', 'q4', 'w4']

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


# <markdowncell>

# # Return to column-wise operations

# <codecell>


# <markdowncell>

# # OLD NOTES/TESTS

# <codecell>

import numpy as np
from scipy.optimize import curve_fit
def func(x, a, b, c):
    return a*np.exp(-b*x) + c

x = np.linspace(0,4,50)
y = func(x, 2.5, 1.3, 0.5)
yn = y + 0.2*np.random.normal(size=len(x))

popt, pcov = curve_fit(func, x, yn)
print(popt)
print(pcov)

# <codecell>

fitresult

# <codecell>

sqrt2 = lambda x: sqrt(x) if x >= 0 else -sqrt(-x)
g = np.matrix(np.zeros((4,4)))
g[0,0] = 1
for i in range(1,4): g[i,i] = -1
boost = lambda beta, gamma: np.matrix([ [gamma,0,0,-beta*gamma], [0,1,0,0], [0,0,1,0], [-beta*gamma,0, 0, gamma] ])
def R(e04, e14, p04):
    e0, e1 = e04[1:], e14[1:]
    q = (e0-e1)
    v3 = q/la.norm(q)
    v2 = ft.partial(lambda x: x/la.norm(x), np.cross(e0,e1))()
    v1 = ft.partial(lambda x: x/la.norm(x), np.cross(v2,v3))()
    v = np.matrix([v1, v2, v3])
    rot4_3rot = np.matrix(np.hstack(([[1], [0], [0], [0]], np.vstack(([[0, 0, 0]], v)))).T)
    print(rot4_3rot.T*np.matrix(e04-e14).T)
    w4 = e04-e14+p04
    E, p = w4[0], la.norm(w4[1:])
    m = sqrt2(E*E-p*p)
    beta, gamma = p/E, E/m
    bst = boost(beta, gamma)
    print(rot4_3rot)
    print(bst)
    print(gamma)
    print('+++++++')
    print(bst*boost(-1*beta, gamma).T)
    print('+++++++')
    print(rot4_3rot*rot4_3rot.T)
    return bst*rot4_3rot
e04 = dftmp.e04.values[0]
e14 = dftmp.e4.values[0]
p04 = dftmp.p04.values[0]
rot4 = R(e04, e14, p04)
print(w4)
print(rot4)
print(rot4.T)
print(rot4.T*np.matrix(w4).T)
print(rot4.T*rot4)

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
    beta, gamma = E/p, E/m
    print(rot4_3rot)
    return boost(beta, gamma).dot(rot4_3rot)

# <codecell>

from numpy.linalg import norm
la.norm([3, 3])
np.set_printoptions(precision=3, suppress=True)
dftmp = dfkin[0:2]
e04 = dftmp.e04.values[0]
e14 = dftmp.e4.values[0]
p04 = dftmp.p04.values[0]
w4 = e04-e14+p04
E = w4[0]
p = sqrt(w4[1:].dot(w4[1:]))
m = sqrt(E*E-p*p)
bst = boost(E/p, E/m)
rot4 = R(e04, e14, p04)
for k, v in { 'e04':e04, 'e14':e14, 'p04':p04, 'w4':w4, 'E':E, 'p':p, 'm':m, 'boost':bst, 'rot4':rot4 }.items():
    print(k, v)

# <codecell>

# http://stackoverflow.com/questions/6802577/python-rotation-of-3d-vector
# unutbu says
import numpy as np

def rotation_matrix(axis, phi):
    axis = axis/np.sqrt(np.dot(axis,axis))
    a = np.cos(phi/2)
    b,c,d = -axis*np.sin(phi/2)
    return np.array([[a*a+b*b-c*c-d*d, 2*(b*c-a*d), 2*(b*d+a*c)],
                     [2*(b*c+a*d), a*a+c*c-b*b-d*d, 2*(c*d-a*b)],
                     [2*(b*d-a*c), 2*(c*d+a*b), a*a+d*d-b*b-c*c]])

v = np.array([1,0,0])
axis = np.array([0,1,0])
phi = pi/2

print(np.dot(rotation_matrix(axis,phi),v))
# same page, juniper- says: using weave gives 20x improvement

# <codecell>

import numpy as np
import numpy.linalg as la
import functools as ft

def R(e0, e1):
    q = (e0-e1)
    v3 = q/la.norm(q)
    v2 = ft.partial(lambda x: x/la.norm(x), np.cross(e0,e1))()
    v1 = np.cross(v2, v3)
    v = np.array([v1, v2, v3])
    return v

e0 = np.array([0, 0, 1])
e1 = np.array([0, 1, 0])

print(rot(e0, e1))

# <codecell>

import numpy as np
import numpy.linalg as la
import functools as ft
# from new coordinate system
# u->v

u = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]])

e0 = np.array([0, 0, 1])
e1 = np.array([0, 1, 0])
q = (e0-e1)
v3 = q/la.norm(q)
v2 = ft.partial(lambda x: x/la.norm(x), np.cross(e0,e1))()
v1 = np.cross(v2, v3)
v = np.array([v1, v2, v3])

def rot(e0, e1):
    q = (e0-e1)
    v3 = q/la.norm(q)
    v2 = ft.partial(lambda x: x/la.norm(x), np.cross(e0,e1))()
    v1 = np.cross(v2, v3)
    v = np.array([v1, v2, v3])
    return v

print(e0)
print(e1)
print(q)
print(v1)
print(v2)
print(v3)
print(v)
print(u)
np.dot(u, v)

# <codecell>

"""
From my C++ code, evt.h...

	void CalcRot() {
		if (_3rot) delete _3rot;
		if (_4rot) delete _4rot;
		if (_boost) delete _boost;
		TVector3 uz = _q->Vect().Unit();
		TVector3 ux = (_e0->Vect().Cross(_e1->Vect())).Unit();
		ux.Rotate(-PiOver2(),uz);
		_3rot = new TRotation();
		_3rot->SetZAxis(uz,ux).Invert();
		//_w and _q are in z-direction
		_boost = new TVector3(-1*_w->BoostVector());
		_4rot = new TLorentzRotation(*_3rot); //*_boost);
		*_4rot *= *_boost; //*_3rot;
	}
"""


# <markdowncell>

# ## Apply only works with same shape return value!

# <codecell>

# df.apply(lambda x: x['mcp'][x['idxE']], axis=1)
# df[['energy','px','py','pz']].shape
print(df.energy.shape)
print(df.px.shape)
print(df.py.shape)
print(df.pz.shape)
print(df[['energy','px','py','pz']].shape)
g = np.array([1, -1, -1, -1])
e04 = np.array([5.497, 0, 0, 5.497])
p04 = np.array([0.938, 0, 0, 0])
sqrt2 = lambda x: sqrt(x) if x >= 0 else -sqrt(-x)
#v4idxs = [10, 9, 8, 7]
v4idxs = ['energy', 'px', 'py', 'pz']
def test(x):
    i = v4idxs
    e4 = np.array([x[i[0]][0], x[i[1]][0], x[i[2]][0], x[i[3]][0]])
    p4 = np.array([x[i[0]][1], x[i[1]][1], x[i[2]][1], x[i[3]][1]])
    pip4 = np.array([x[i[0]][2], x[i[1]][2], x[i[2]][2], x[i[3]][2]])
    pim4 = np.array([x[i[0]][3], x[i[1]][3], x[i[2]][3], x[i[3]][3]])
    pi04 = np.array([x[i[0]][4], x[i[1]][4], x[i[2]][4], x[i[3]][4]])
    q4 = e04-e4
    w4 = q4+p04
    Q2 = -np.sum(q4*g*q4)
    s = np.sum(w4*g*w4)
    W = sqrt2(s)
    return pd.Series(data=(e04, p04, e4, p4, pip4, pim4, pi04, q4, w4, Q2, W, s))
    #return pd.DataFrame( {'e4':e4, 'p4':p4, 'pip4':pip4, 'pim4':pim4, 'pi04':pi04, 'q4':q4, 'w4':w4, 'Q2':Q2, 'W':W, 's':s} )
    # return pd.Series(np.array([x[1][2], x[2][2], x[3][2], x[4][2]]))
# pd.concat([ pd.DataFrame(x) for x in [df.energy, df.px, df.py, df.pz] ], axis=1).shape
dftest = df.apply(test, axis=1)
# dftest.columns = ['e4', 'p4', 'pip4', 'pim4', 'pi04', 'q4', 'w4', 'Q2', 'W', 's']
#dftest = df[['energy','px','py','pz']].reset_index().apply(lambda x: pd.Series(x[2]), axis=1)

# <codecell>

dftest

# <codecell>

def test(x):
    i = v4idxs
    e4 = np.array([x[i[0]][0], x[i[1]][0], x[i[2]][0], x[i[3]][0]])
    p4 = np.array([x[i[0]][1], x[i[1]][1], x[i[2]][1], x[i[3]][1]])
    pip4 = np.array([x[i[0]][2], x[i[1]][2], x[i[2]][2], x[i[3]][2]])
    pim4 = np.array([x[i[0]][3], x[i[1]][3], x[i[2]][3], x[i[3]][3]])
    pi04 = np.array([x[i[0]][4], x[i[1]][4], x[i[2]][4], x[i[3]][4]])
    q4 = e04-e4
    w4 = q4+p04
    Q2 = -np.sum(q4*g*q4)
    s = np.sum(w4*g*w4)
    W = sqrt2(s)
    # return (e4, p4, pip4, pim4, pi04, q4, w4, Q2, W, s)
    return pd.DataFrame( {'e04':e04, 'p04':p04, 'e4':e4, 'p4':p4, 'pip4':pip4, 'pim4':pim4, 'pi04':pi04, 'q4':q4, 'w4':w4, 'Q2':Q2, 'W':W, 's':s} )

def c4v(icomp):
    i = ['energy', 'px', 'py', 'pz']
    return lambda x: np.array([x[i[0]][icomp], x[i[1]][icomp], x[i[2]][icomp], x[i[3]][icomp]])

# dftest = df.groupby(level=0).apply(test)
# dftest = df[0:3].groupby(level=0).apply(lambda x: pd.DataFrame({'pzB':x['pz'][0], 'eB':x['energy'][0]}))
# dftest = df[0:3].groupby(level=0).agg({'e4':c4v(0), 'p4':c4v(1)})
grpd = df[0:3].groupby(level=0)
# .agg([lambda x,y: x, lambda x,y: x[0]] ) # [c4v(0), c4v(1)])
grpd.apply(lambda x: (x.energy.values))

# <codecell>

dftest['pzB'][0][

# <markdowncell>

# ## Load data frame
# ### (if already pickled, per next section)

# <codecell>

import pandas as pd
df = pd.DataFrame.load('../testdata/genev100_a.p')
print(df)

# <codecell>

dftmp = df[0:20]

# <codecell>

df.px

# <codecell>


# <markdowncell>

# ## Add LorentzVectors to DataFrame

# <codecell>

d2r = pi/180.0
lvs = {'pz':np.cos(d2r*df.mctheta)*df.mcp,
       'py':np.sin(d2r*df.mctheta)*np.sin(d2r*df.mcphi)*df.mcp,
       'px':np.sin(d2r*df.mctheta)*np.cos(d2r*df.mcphi)*df.mcp,
       'E': np.sqrt(df.mcp*df.mcp + df.mcm*df.mcm)}

# <codecell>

for k,v in lvs.items():
    df[k] = v

df.columns

# <codecell>

from rootpy.math.physics.vector import LorentzVector

# <codecell>

def getlv(px, py, pz, e):
    return LorentzVector(px, py, pz, e)

vgetlv = np.vectorize(getlv)

# <codecell>

lvs = vgetlv(df.px, df.py, df.pz, df.E)

# <codecell>

df['lv'] = lvs
df.columns

# <codecell>

df.save('../testdata/genev100_a.p')

# <markdowncell>

# ## Munge tree data and pickle

# <codecell>

from rootpy.io import root_open
from rootpy import asrootpy
from rootpy.tree import Tree

from rootpy.math.physics.vector import LorentzVector
from rootpy.math.physics.vector import LorentzRotation
from rootpy.math.physics.vector import Vector3
from rootpy.math.physics.vector import Rotation

from root_numpy import root2rec, root2array

BEAM_E1F = 5.497
E0_E1F = LorentzVector(0, 0, BEAM_E1F, BEAM_E1F)

# <codecell>

ar = root2rec('../testdata/genev100_a.root',
              branches=['mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])
nparts = root2array('../testdata/genev100_a.root',
              branches=['mcnpart']).astype(int)

# <codecell>

import pandas as pd
df = pd.DataFrame(ar)
#idxevt = np.array([df.index]*5).T.flatten()
idxevt = np.repeat(df.index, nparts)
idxprt = np.array([np.arange(0, npart) for npart in nparts]).flatten()

# <codecell>

# convert series of arrays into multiindexed series of numbers
ss = [reduce(lambda x, y: pd.Series(x).append(pd.Series(y)), s).reindex([idxevt, idxprt], level=1) for s in [df[cname].values for cname in df.columns]]
# SLOW.  probably much faster to convert each series of arrays into 2d numpy array and then slice

# <codecell>

df2 = pd.DataFrame(dict(zip(df.columns, ss)))

# <codecell>

print(df2)
df2.describe()

# <codecell>

df2.save('../testdata/genev100_a.p')

# <markdowncell>

# ## Scratch

# <codecell>

np.array([np.arange(0, npart) for npart in nparts]).flatten()

# <codecell>

smcid = pd.Series(df['mcid'][0], index=[[0]*5, range(0,5)]).append(pd.Series(df['mcid'][1], index=[[1]*5, range(0,5)]))
smcp = pd.Series(df['mcp'][0], index=[[0]*5, range(0,5)]).append(pd.Series(df['mcp'][1], index=[[1]*5, range(0,5)]))
df2 = pd.DataFrame({'mcid':smcid, 'mcp':smcp})
df2

# <codecell>

# in this case, mcid always contains the same 5 values, so a simple reshape would do
# pd.Series(df.mcid[0]).reindex([idxevt, range(0,5)*df.shape[0]], level=1)
# but lets test reduce
reduce(lambda x, y: pd.Series(x).append(pd.Series(y)), df.mcid[0:3]).reindex([idxevt, range(0,5)*df.shape[0]], level=1)

# <codecell>

import os
import multiprocessing as mp
pool = mp.Pool(processes=4)
def tasks(args):
    print('PID = %d, args = %s', (os.getpid(), args))
    return os.getpid(), args

tasks('test')

# <codecell>

result = pool.map(tasks, range(1,9))

# <codecell>

result

# <codecell>

def ar2s_reidx(s):
    from root_numpy import root2rec, root2array
    import os
    import pandas as pd
    df = pd.DataFrame(ar)
    idxevt = np.repeat(df.index, nparts)
    idxprt = np.array([np.arange(0, npart) for npart in nparts]).flatten()
    ar = root2rec('../testdata/genev100_a.root',
              branches=['mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])
    nparts = root2array('../testdata/genev100_a.root',
              branches=['mcnpart']).astype(int)
    print('PID = %d, args = %s' % (os.getpid(), args) )
    return reduce(lambda x, y: pd.Series(x).append(pd.Series(y)), s).reindex([idxevt, idxpart], level=1)

result = pool.map(ar2s_reidx, [df[cname].values for cname in df.columns])

# <codecell>

df.columns

# <codecell>

del df['lv']

# <codecell>

df.save('../testdata/genev100_a.p')

# <codecell>

evtgrps = dftmp.groupby(level=0)

# <codecell>

for e in evtgrps: print(e)

# <codecell>

for evt in [e[1] for e in evtgrps]:
    idxelectron = evt.mcid==11
    print(evt[idxelectron][['E', 'px', 'py', 'pz']])
    print(type(evt[idxelectron][['E', 'px']].values[0]))

# <codecell>

e0 = np.array([5.497, 0.0, 0.0, 5.497])
p0 = np.array([0.938, 0.0, 0.0, 0.0])
g = np.array([1, -1, -1, -1])
def getkin(evt):
    e1 = evt[evt.mcid==11][['E', 'px', 'py', 'pz']].values[0]
    q = e0-e1
    q2 = np.sum(q*g*q)
    w = q+p0
    s = np.sum(w*g*w)
    return pd.Series({'Q2': -q2, 'W': sqrt(s)})
    
s = evtgrps.apply(getkin).reset_index(level=0, drop=True)

# <codecell>

s

# <codecell>

from root_numpy import root2rec, root2array
import pandas as pd
ar = root2rec('../testdata/genev100_a.root',
              branches=['mcnpart', 'mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])
df = pd.DataFrame(ar)
dftmp = df[0:100]

# <codecell>

print(type(dftmp.mcid[0]))
print(type(dftmp.mcid))
print(type(dftmp.mcid.index))
print(type(dftmp.mcid.values))

# <codecell>

# partids = { 'e': 11, 'p': 2212, 'pip': 211, 'pim': -211, 'pi0': 111 }
pids = np.array([11, 2212, 211, -211, 111])

def pidxs_f(mcids):
        return [np.where(mcids==partid)[0][0] for partid in pids]
    
def curry_pidxs(partids):
    def pidxs(mcids):
        return [np.where(mcids==partid)[0][0] for partid in partids]
    return pidxs

pidxs_l = lambda partids: lambda x: [np.where(x==partid)[0][0] for partid in partids]
curry_pidxs(pids)(dftmp.mcid[0])
# dftmp.mcid[0]

# <codecell>

arnd = np.random.random_integers(-212, 2214, 10000000)

# <codecell>

%%timeit -n 10
pidxs_f(arnd)

# <codecell>

%%timeit -n 10
curry_pidxs(pids)(arnd)

# <codecell>

%%timeit -n 10
pidxs_l(arnd)

# <codecell>

%time pidxs_l(pids)(arnd)

# <codecell>

pidxs_l = lambda partids: lambda x: [np.where(x==partid)[0][0] for partid in partids]
pidxs = pidxs_l(np.array([11, 2212, 211, -211, 111]))

# <codecell>

pidxs(dftmp.mcid[0])

# <codecell>

pidxs_l = lambda partids: lambda x: [np.where(x==partid)[0][0] for partid in partids]
pidxs = pidxs_l(np.array([11, 2212, 211, -211, 111]))
arr2d = np.array(list(df.mcid.apply(pidxs).values))
cnames = ['idxE', 'idxP', 'idxPip', 'idxPim', 'idxPi0']
print(df)
for cn, vals in zip(cnames, [arr2d[:,icol] for icol in range(0,5)]):
    df[cn] = pd.Series(vals, name=cn)
print(df)

# <codecell>

[arr2d[:,icol] for icol in range(0,5)]

# <codecell>

"""df.mcid is a series of arrays, where each array contains the particle ids of the
event.  Other particle-level df columns contain arrays with particle property values
in the same order."""
# define curry function that takes as argument desired particle ids
pidxs_l = lambda partids: lambda x: np.array([np.where(x==partid)[0][0] for partid in partids])
# pidxs returns a list of particle indexes
# in this case the order is...
pidxs = pidxs_l(np.array([11, 2212, 211, -211, 111]))
# to correpsond to column labels
cnames = ['idxE', 'idxP', 'idxPip', 'idxPim', 'idxPi0']
# apply pidxs to each the mcid series; result is a 
# pull out the series of mcid arrays as a 2d numpy array, which provides nice slicing
# features (could have been another dataframe, I suppose)
tmp = df.mcid.apply(pidxs)
# arr2d = np.array(list(df.mcid.apply(pidxs).values))
# each row corresponds to an event, each column to the ith particle in the event
# so 
#for cn, vals in zip(cnames, [arr2d[:,icol] for icol in range(0,5)]):
#    df[cn] = pd.Series(vals, name=cn)

# <codecell>

print(tmp)
print(type(tmp))
print(tmp[0])
print(type(tmp[0]))

# <codecell>

tmp[0][:]

# <codecell>

tmp[:,1]

# <codecell>


