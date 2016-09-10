# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

from rootpy.io import root_open
from rootpy.tree import Tree

# <codecell>

with root_open('/home/ephelps/downloads/set2.root') as fin:
    for e in fin.t20:
        print(e)
        break

# <codecell>

fin = root_open('/home/ephelps/downloads/set2.root')

# <codecell>

t20 = fin.t20

# <codecell>

print(t20.GetEntries())

# <codecell>

for i, e in enumerate(t20.branches[0]):
    print(e.iev)
    break
print(i)

# <codecell>

t20.Print()

# <codecell>


# <codecell>

t20.branches[0]

# <codecell>


