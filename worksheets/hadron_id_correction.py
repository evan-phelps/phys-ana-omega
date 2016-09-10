# -*- coding: utf-8 -*-
"""
Created on Sat Feb 28 20:36:16 2015

@author: ephelps
"""

from RunData import RunData
from util_phys import golden_runs
from rootpy.plotting import Hist2D

def get_peaks(sector, paddle, from_tree=False):
    X_runs, Y_dts = [], []
    bad_runs = [37688]
    h2 = Hist2D(500, 0, 5, 400, -10, 10)
    h2corr = Hist2D(500, 0, 5, 400, -10, 10)
    for run in [g for g in golden_runs if g not in bad_runs]:
        with RunData(run) as rd:
            X_runs.append(run)
            Yhs = rd.get_xpeak_proton_pid(sector, paddle, from_tree)
            h2.Add(Yhs[1])
            h2corr.Add(Yhs[2])
            Y_dts.append(Yhs[0])
    return (zip(X_runs, Y_dts), h2, h2corr)

'''sample ipython session
from RunData import RunData
runfile('/home/ephelps/copy/gmail/phys-ana-omega/worksheets/hadron_id_correction.py', wdir='/home/ephelps/copy/gmail/phys-ana-omega/worksheets')
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import rootpy.plotting.root2matplotlib as rplt

(XY, h2, h2corr) = get_peaks(6, 18, True)
X,Y = zip(*XY)
plt.plot(X,Y,'ko')
h2b = rplt.hist2d(h2, vmax=130)
h2corrb = rplt.hist2d(h2corr, vmax=130)
'''

    
#def get_peaks(sectors, paddles, runs=golden_runs):
#    if type(sectors) is int:
#        sectors = [sectors]
#    if type(paddles) is int:
#        paddles = [paddles]
#    sector_paddle_s = dict([((sect, pd), ([],[])) for sect in sectors \
#                                                  for pd in paddles])
#    bad_runs = [37688]
#    for run in [g for g in runs if g not in bad_runs]:
#        with RunData(run) as rd:
#            for sector in sectors:
#                for paddle in paddles:
#                    dt = rd.get_xpeak_proton_pid(sector, paddle)
#                    sector_paddle_s[(sector, paddle)][0].append(run)
#                    sector_paddle_s[(sector, paddle)][1].append(dt)
#    return sector_paddle_s
