# import numpy as np
import pandas as pd
import matplotlib.pylab as plt
from rootpy.interactive import wait
# import math
# from ROOT import TGraphErrors

# fig_width_pt = 246.0  # Get this from LaTeX using \showthe\columnwidth
# inches_per_pt = 1.0/72.27               # Convert pt to inch
# golden_mean = (math.sqrt(5) - 1.0) / 2.0         # Aesthetic ratio
# fig_width = fig_width_pt*inches_per_pt  # width in inches
# fig_height = fig_width*golden_mean      # height in inches
# fig_size = [fig_width, fig_height]
# params = {'backend': 'ps',
#           'axes.labelsize': 10,
#           'text.fontsize': 10,
#           'legend.fontsize': 10,
#           'xtick.labelsize': 8,
#           'ytick.labelsize': 8,
#           'text.usetex': True,
#           'figure.figsize': fig_size}
# plt.rcParams.update(params)


def drawxint():
    df = pd.read_csv('out/ana_bd.dat')
    # dftmp = df[['W', 'Q2', 'xsect', 'err', 'fsigchi2']][
    #    (df['fsigchi2'] < 3) & (df['fsigchi2'] > 0) & (df['weightsb'] > 0) & (df['weight3s'] > 0)]

    q2groups = df[(df['fsigchi2'] < 3) & (df['fsigchi2'] > 0.5) & (
        df['fsigstat'] == 'CONVERGED ') & (df['weightsb'] > 0) & (df['weight3s'] > 0) & (df['fsigpar2'] > 0.008) & (df['fsigpar2'] < 0.035)].groupby(df.Q2)
    # xerrs = np.array(48 * [0.0])
    gtitle = '$Q^2 = %.3f$ $GeV^2$'

    for q2, grp in q2groups:
        fig = plt.figure()
        ax1 = fig.add_subplot(111)
        ax1.errorbar(grp.W, grp.xsect, grp.err, ls='none', marker='o', ms=4, color='black')
        ax1.set_ylim(-50, ax1.get_ylim()[1])
        ax1.set_xlim((1.6, 3.0))
        ax1.set_title(gtitle % q2, fontsize=20, y=1.025)
        ax1.set_xlabel('W (GeV)', fontsize=16)
        ax1.set_ylabel('$\sigma$  ($nb^{-1}$)', fontsize=20)
        plt.grid(b=True, which='major', color='black', ls='-')
        plt.grid(b=True, which='minor', color='black', ls='dotted')
        ax1.minorticks_on()
        plt.axhline(0, color='blue', ls='-', linewidth=1.25)

        ax2 = ax1.twinx()
        ax2.set_ylabel('$\~{\chi}^{2}$', color='r', fontsize=20)
        ax2.set_ylim(0, 3)
        ax2.plot(grp.W, grp.fsigchi2, marker='o', ms=4, ls='none', color='r')
        for tickl in ax2.get_yticklabels():
            tickl.set_color('r')

        # ax2 = ax1.twinx()
        # ax2.set_ylabel('$\sigma_{g}$', color='r')
        # ax2.set_ylim(0.008, 0.035)
        # ax2.plot(grp.W, grp.fsigpar2, marker='o', ms=4, ls='none', color='r')
        # for tickl in ax2.get_yticklabels():
        #     tickl.set_color('r')
        # plt.tight_layout()
        plt.show()
        wait()


drawxint()

# g = TGraphErrors(48, dftmp.W.values,
#                  dftmp.xsect.astype('d').values,
#                  xerrs, dftmp.err.astype('d').values)
