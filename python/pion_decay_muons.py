# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from root_numpy import root2rec, root2array

np.set_printoptions(precision=4, suppress=True)
plt.rc('text', usetex=True)

# <codecell>

treerecs = root2rec('../testdata/genev102.gsim.gpp.10.ana.root', treename='h10', branches=['mcid', 'pidpart', 'id', 'mcp'])
df = pd.DataFrame(treerecs)

# <codecell>

smcid = df.mcid.apply(lambda x: pd.Series(x)).stack()
spartid = df.pidpart.apply(lambda x: pd.Series(x)).stack()
sid = df.id.apply(lambda x: pd.Series(x)).stack()
sp = df.mcp.apply(lambda x: pd.Series(x)).stack()

# <codecell>

hmc = hist(smcid, bins=np.arange(-2500.5, 2500.5,1))
hpart = hist(spartid, bins=np.arange(-2500.5, 2500.5,1))
hrecon = hist(sid, bins=np.arange(-2500.5, 2500.5,1))

# <codecell>

ds = [dict(zip((h[1][1:]+h[1][:-1])/2,h[0])) for h in [hmc, hpart, hrecon]]

# <codecell>

countmcid = smcid.value_counts()
countpartid = spartid.value_counts()
countevntid = sid.value_counts()

# <codecell>

pdgids = {0:'unknown', -211:'pi-', 211:'pi+', 111:'pi0', 2212:'p+', 11:'e-', 22:'photon',
          2112:'n', 321:'k+', -321:'k-', 45:'deuteron?', 47:'alpha?', 49:'he3?'}
geantids = {0:'unknown', 1:'photon', 3:'e-', 8:'pi+', 9:'pi-', 11:'k+', 12:'k-',
            13:'n', 14:'p+', 15:'p-', 45:'deuteron'}

# <codecell>

idmaps = [pdgids, geantids, pdgids]
labels = ['thrown','gsim','recon']
for l, s, m in zip(labels, [countmcid, countpartid, countevntid], idmaps):
    s.index = pd.Index([m[k] for k in s.index])
# for l, s, m in zip(labels, [countmcid, countpartid, countevntid], idmaps):
#     s.index = pd.MultiIndex.from_tuples([(l, m[k]) for k in s.index])

# <codecell>

df = pd.DataFrame({'thrown':countmcid, 'gsim':countpartid, 'recon':countevntid})

# <codecell>

df.sort_index(by='thrown', ascending=True).reindex_axis(['thrown','gsim','recon'], axis=1).describe

# <codecell>

hp = hist(sp, bins=np.arange(0, 4, 0.01))

# <codecell>

def lifetime(p):
    E = sqrt(p**2+0.1396**2)
    b = p/E
    g = sqrt(1/(1-b**2))
    return 26*g

plt.figure(figsize=(14,6))
hlifetime = hist(sp[smcid==211].apply(lifetime), bins=arange(0,700,4), lw=0)  # , log=True)
plt.xlabel(r'$\pi^{+}$ mean lifetime (ns)', fontsize=16)
plt.tick_params(labelsize=18)
plt.savefig('pion_meanlifetime.png')

# <codecell>

def lfdistance(p):
    E = sqrt(p**2+0.1396**2)
    b = p/E
    g = sqrt(1/(1-b**2))
    return 26*g*b*0.3

plt.figure(figsize=(14,6))
hdist = hist(sp[smcid==211].apply(lfdistance), bins=arange(0,200,1), lw=0)
plt.xlabel(r'$\pi^{+}$ mean path length (m)', fontsize=16)
plt.tick_params(labelsize=18)
plt.axvline(4, color='r', lw=2)
plt.savefig('pion_meandistance.png')

# <codecell>


