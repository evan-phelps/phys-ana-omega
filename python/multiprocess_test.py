# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

from multiprocessing import Process
import os
import numpy as np
from root_numpy import root2rec, root2array
import pandas as pd

ar = root2rec('../testdata/genev100_a.root',
          branches=['mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])
nparts = root2array('../testdata/genev100_a.root',
          branches=['mcnpart']).astype(int)
df = pd.DataFrame(ar)

idxevt = np.repeat(df.index, nparts)
idxprt = np.array([np.arange(0, npart) for npart in nparts]).flatten()

# <codecell>

def test(s):
    print('PID = %d, args = %s' % (os.getpid(), s) )
    return reduce(lambda x, y: pd.Series(x).append(pd.Series(y)), s).reindex([idxevt, idxprt], level=1)

ss = [df[cname].values for cname in df.columns]
ps = [Process(target=test, args=(s,)) for s in ss]
for p in ps:
    p.start()

# <codecell>

for p in ps:
    p.join()

# <markdowncell>

# # Only with __main__

# <codecell>

import multiprocessing as mp
import os
import numpy

def task(args):
    print 'PID =', os.getpid(), ', args =', args
    return os.getpid(), args

task('test')

# <codecell>

pool = mp.Pool(processes=4)

# <codecell>

r = pool.map(task, range(1,9))

# <codecell>

r

# <codecell>

from root_numpy import root2rec, root2array
import os
import pandas as pd
ar = root2rec('../testdata/genev100_a.root',
          branches=['mcid', 'mcm', 'mcp', 'mctheta', 'mcphi'])
nparts = root2array('../testdata/genev100_a.root',
          branches=['mcnpart']).astype(int)
df = pd.DataFrame(ar)
idxevt = np.repeat(df.index, nparts)
idxprt = np.array([np.arange(0, npart) for npart in nparts]).flatten()

# <codecell>

def test(s):
    print('PID = %d, args = %s' % (os.getpid(), args) )
    return reduce(lambda x, y: pd.Series(x).append(pd.Series(y)), s).reindex([idxevt, idxpart], level=1)

rtest = pool.map(test, [df[cname].values for cname in df.columns])

# <codecell>


