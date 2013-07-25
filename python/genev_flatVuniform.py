# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # GENEV Angular Distributions:  "flat" versus "uniform"

# <codecell>

import numpy as np
import matplotlib.pyplot as plt

# <codecell>

rands = np.random.random_sample(100000)

# <codecell>

plt.rc('text', usetex=True)
fig = plt.figure(1, figsize(10,8))
ax1 = plt.subplot(221)
plt.title('"flat"')
ax1.axis([0.0, pi, 0.0, 1.0])
ax1.set_autoscalex_on(False)
ax1.set_autoscaley_on(True)
hflat_angle = plt.hist(pi*rands, bins=100, range=(0, pi))
plt.xlabel(r"$\theta$ (radians)")
ax2 = plt.subplot(222)
plt.title('"flat"')
hflat_cos = plt.hist(np.cos(pi*rands), bins=100, log=True)
plt.xlabel(r"cos($\theta$)")
ax3 = plt.subplot(223)
plt.title('"uniform"')
ax3.axis([0.0, pi, 0.0, 1.0])
ax3.set_autoscalex_on(False)
ax3.set_autoscaley_on(True)
hunif_angle = plt.hist(np.arccos(-2*rands+1), bins=100, range=(0, pi))
plt.xlabel(r"$\theta$ (radians)")
ax4 = plt.subplot(224)
plt.title('"uniform"')
hunif_cos = plt.hist(-2*rands+1, bins=100)
plt.xlabel(r"cos($\theta$)")
plt.tight_layout()

# <codecell>


