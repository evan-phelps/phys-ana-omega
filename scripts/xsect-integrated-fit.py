#!/bin/python2

"""Provides class, configuration functions, and main function meant to make
systematic studies of integrated background subtraction easier.  To add a new
fitting scheme, create a new FS* function that sets the adjustable parameters
of FitSchemeXsectInt and add corresponding scheme to 'fss' map in main().
"""

# TODO: histogram cosmetics
#       fix histogram titles; add axis titles,
#       give descriptive names for legend,
#       move legend to left where there's more space.
# TODO: simulation-inspired background function
# TODO: add radiative tail to signal function
# TODO: allow class to accumulate cross-sections
#       W, Q^2, cross-sections, error, and parameters see fit() of
#       xsect-integrated-fit.C. remember to propagate errors!
# README: background subtraction error propagation
#         average relative error of sideband yields
#         and apply to integral of background function under signal
# TODO: add background-subtracted histogram
#       for each scheme to canvas and legend
# TODO: add ability to change functional forms
#       number of parameters etc.`
# TODO: pull reusables to module
#       e.g., MASS_P, goodcolors, common regexps, etc.
# TODO: ParticleConstants for python
# TODO: maybe add resolution convolution
# TODO: option to create histograms
#       rather than pulling from file (ala h3maker.h)
# TODO: change re.search to re.findall
#       and get wlow, whigh, q2low, q2high in one expression

import math as m
import re

import ROOT as R
from ROOT import TFile
from ROOT import TF1
from ROOT import TCanvas
from ROOT import TLegend

MASS_P = 0.93827203
RE_FLOAT = '[-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?'
goodcolors = [R.kRed+1,
              R.kGreen+1,
              R.kBlue,
              R.kYellow+1,
              R.kMagenta+1,
              R.kCyan+1,
              9]


class FitSchemeXsectInt:
    """Primary functions include Setup(), Fit(), and Decorate().

        * Setup() determines parameters of main fit;
        * Fit(), well, fits; and
        * Decorate() adds background (bg), signal (sig), and combined (fn)
          functions, along with any other "decorations" or visual modification
          to the histogram.
    """
    def __init__(self, desc, name='', fnsetup=None):
        self.g = R.TMinuit()    # leave reference active for access to gMinuit
        self.name = name
        # ##########################################################
        # ######## default values of adjustable parameters #########
        # ##########################################################
        self.prefit = None
        self.wrange, self.q2range = [0, 0], [0, 0]
        self.drawrange = [0.4, 2]
        self.bgNparms = 5
        self.sigNparms = 3
        self.fnNparms = 8
        self.endAtEdge = False
        self.bg = TF1('fbg_%s' % self.name, self.d_pol4,
                      self.drawrange[0], self.drawrange[1],
                      self.bgNparms)
        self.sig = TF1('fsig_%s' % self.name, self.d_sig,
                       self.drawrange[0], self.drawrange[1],
                       self.sigNparms)
        self.fn = TF1('fbgsig_%s' % self.name, self.d_pol4gaus,
                      self.drawrange[0], self.drawrange[1],
                      self.fnNparms)
        # fit ranges
        self.bg.range = [0.4, 1.1]
        self.sig.range = [0.74, 0.85]
        self.fn.range = [0.4, 1.1]
        # initial parameters
        self.bg.setparms = {}
        self.sig.setparms = {1: 0.783, 2: 0.020}
        self.fn.setparms = {}
        # fixed parameters
        self.bg.fixparms = {5: 2, 6: 2.1, 3: 0, 4: 0}
        self.sig.fixparms = {3: 2, 4: 2.1}
        self.fn.fixparms = {3: 0, 4: 0, 8: 2, 9: 2.1}
        # ranges to skip during fitting
        self.bg.skip = [[0.74, 0.85], [0.5, 0.6]]
        self.sig.skip = []
        self.fn.skip = [[0.5, 0.6]]
        # x values of modified step, i.e., x0/x1 of stepfactor()
        self.edgerange = [2, 2.1]
        # draw options during fitting
        self.doptions = 'QN'
        # ################## end default values ####################
        self.goptions = ''
        self.desc = desc
        self.converged = True

        # override default parameters
        if fnsetup is not None:
            fnsetup(self)

    # #######################################################################
    # #################### DEFAULT FIT FUNCTIONS ############################
    # #######################################################################
    def stepfactor(self, x):
        x0, x1 = self.edgerange[0], self.edgerange[1]
        return 1 if x < x0 else (0 if x > x1 else 1-1/(x1-x0)*(x-x0))

    def d_sig(self, x, par):
        step = self.stepfactor(x[0])
        gaus = par[0]*m.exp(-0.5*pow(((x[0]-par[1])/par[2]), 2))
        return step*gaus

    def d_pol4(self, x, par):

        # reject points in ranges not modeled by bg function
        for skip in self.bg.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0])
        retval = step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                       + par[3]*x[0]**3 + par[4]*x[0]**4)
        return 0 if retval < 0 else retval

    def d_pol4gaus(self, x, par):

        # reject points in ranges not modeled by function
        for skip in self.fn.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0])
        return step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                     + par[3]*x[0]**3 + par[4]*x[0]**4
                     + par[5]*m.exp(-0.5*pow(((x[0]-par[6])/par[7]), 2)))
    # ################## END DEFAULT FIT FUNCTIONS ##########################

    def Setup(self, hist, wrange=None, q2range=None):
        print('****************************** \n'
              + self.desc + '\n******************************')
        # attempt to set modified step function (phase-space edge) parameters
        # and populate W and Q2 ranges
        wlow, whigh = 0, 0
        if wrange is not None:
            wlow, whigh = wrange[0], wrange[1]
        else:
            # try to get wrange from histogram title
            # assuming histograms of h3maker
            # look for '(W = <number>' without returning '(W = '
            re_string = '(?<=W = \()'+RE_FLOAT
            wlow = float(re.search(re_string, hist.GetTitle()).group(0))
            # look for ',<number)' without returning ',' or ')'
            re_string = '(?<=,)'+RE_FLOAT+'(?<=\))?'
            whigh = float(re.search(re_string, hist.GetTitle()).group(0))
        # fix stepfactor parameters according to wrange
        if whigh > 0:
            x0 = m.sqrt(wlow**2+MASS_P**2-2*wlow*MASS_P)
            x1 = m.sqrt(whigh**2+MASS_P**2-2*whigh*MASS_P)
            self.edgerange = [x0, x1]
        self.wrange[0], self.wrange[1] = wlow, whigh

        q2low, q2high = 0, 0
        if q2range is not None:
            q2low, q2high = q2range[0], q2range[1]
        else:
            # try to get wrange from histogram title
            # assuming histograms of h3maker
            # look for '(W = <number>' without returning '(W = '
            re_string = '(?<=Q\^2 = \()'+RE_FLOAT
            q2low = float(re.search(re_string, hist.GetTitle()).group(0))
            # look for ',<number)' without returning ',' or ')'
            re_string = '(?<=,)'+RE_FLOAT+'(?<=\))?'
            q2high = float(re.findall(re_string, hist.GetTitle())[1][0])
        self.q2range[0], self.q2range[1] = q2low, q2high

        # estimate background subtraction
        for pidx, pval in self.bg.setparms.items():
            self.bg.SetParameter(pidx, pval)
        for pidx, pval in self.bg.fixparms.items():
            self.bg.FixParameter(pidx, pval)
        x1 = self.bg.range[1]
        if self.endAtEdge:
            x1 = x1 if x1 < self.edgerange[0] else self.edgerange[0]
        hist.Fit(self.bg, self.doptions, self.goptions,
                 self.bg.range[0], x1)
        bg = self.bg.Clone('bgtmp')
        for pidx, pval in self.bg.fixparms.items():
            bg.FixParameter(pidx, pval)
        h = hist.Clone('htmp')
        h.Add(bg, -1)

        # estimate signal function parameters
        for pidx, pval in self.sig.setparms.items():
            self.sig.SetParameter(pidx, pval)
        for pidx, pval in self.sig.fixparms.items():
            self.sig.FixParameter(pidx, pval)
        x1 = self.sig.range[1]
        if self.endAtEdge:
            x1 = x1 if x1 < self.edgerange[0] else self.edgerange[0]
        h.Fit(self.sig, self.doptions, self.goptions,
              self.sig.range[0], x1)

        # set initial parameters of bg+sig function
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(0, self.bgNparms), bg.GetParameters())]
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(self.bgNparms, self.fnNparms),
                   self.sig.GetParameters())]
        for pidx, pval in self.fn.fixparms.items():
            self.fn.FixParameter(pidx, pval)
        return hist

    def Fit(self, hist):
        x1 = self.bg.range[1]
        if self.endAtEdge:
            x1 = x1 if x1 < self.edgerange[0] else self.edgerange[0]
        if self.prefit is not None:
            self.prefit(self)
        self.converged = R.gMinuit.fCstatu.startswith('CONV')
        hist.Fit(self.fn, self.doptions, self.goptions,
                 self.fn.range[0], x1)
        for pidx in range(0, self.bgNparms):
            self.bg.SetParameter(pidx, self.fn.GetParameter(pidx))
        for pidx in range(0, self.sigNparms):
            self.sig.SetParameter(pidx,
                                  self.fn.GetParameter(pidx+self.bgNparms))

    def Decorate(self, hist):
        fns = [self.bg.Clone('fbg'), self.sig.Clone('fsig'),
               self.fn.Clone('fbgsig')]
        for fn in fns:
            fn.SetRange(self.drawrange[0], self.drawrange[1])
            hist.GetListOfFunctions().Add(fn)


def FSpol2trunc(fs):
    fs.endAtEdge = True
    fs.drawrange = [0.4, 2]
    fs.bg.nparms = 7
    fs.bg.range = [0.4, 1.1]
    fs.bg.skip = [[0.74, 0.85], [0.5, 0.6]]
    fs.bg.setparms = {}
    fs.bg.fixparms = {3: 0, 4: 0}
    fs.sig.nparms = 5
    fs.sig.range = [0.74, 0.85]
    fs.sig.setparms = {1: 0.783, 2: 0.020}
    fs.sig.fixparms = {3: 2, 4: 2.1}
    fs.fn.nparms = 10
    fs.fn.range = [0.4, 1.1]
    fs.fn.skip = [[0.5, 0.6]]
    fs.fn.setparms = {}
    fs.fn.fixparms = {3: 0, 4: 0, 8: 2, 9: 2.1}


def FSpol2trunc5(fs):
    FSpol2trunc(fs)
    fs.prefit = lambda s: s.fn.FixParameter(2, 1.05*s.fn.GetParameter(2))


def FSpol2truncM5(fs):
    FSpol2trunc(fs)
    fs.prefit = lambda s: s.fn.FixParameter(2, 0.95*s.fn.GetParameter(2))


def FSpol4trunc(fs):
    # use 2nd order polynomial
    FSpol2trunc(fs)
    # remove restriction on 3rd and 4th order
    del fs.bg.fixparms[3]
    del fs.bg.fixparms[4]
    del fs.fn.fixparms[3]
    del fs.fn.fixparms[4]


def FSpol4full(fs):
    # use truncated 4th order polynomial
    FSpol4trunc(fs)
    #fs.doptions = ''
    # open range
    fs.bg.range = [0.4, 2]
    fs.fn.range = [0.4, 2]
    fs.endAtEdge = False


def main():
    R.gROOT.SetBatch(True)
    fss = {'pol2trunc':
           FitSchemeXsectInt('2nd order polynomial background, Gauss signal.\
                             fit range = 0.4-1.1, skipping eta peak', 'p2t',
                             FSpol2trunc),
           'pol4trunc':
           FitSchemeXsectInt('4th order polynomial background, Gauss signal.\
                             fit range = 0.4-1.1, skipping eta peak', 'p4t',
                             FSpol4trunc),
           'pol4full':
           FitSchemeXsectInt('4th order polynomial background, Gauss signal.\
                             fit range = 0.4-2, skipping eta peak', 'p4f',
                             FSpol4full),
           'pol2truncM5':
           FitSchemeXsectInt('2th order polynomial background, Gauss signal.\
                             fit range = 0.4-1.1, skipping eta peak, -5\% p2',
                             'p2tm5', FSpol2truncM5),
           'pol2trunc5':
           FitSchemeXsectInt('2th order polynomial background, Gauss signal.\
                             fit range = 0.4-1.1, skipping eta peak, +5\% p2',
                             'p2t5', FSpol2trunc5)}
    fin = TFile('/home/ephelps/analysis/sandbox/h3maker-hn.root')
    for h in fin.Get('hs0').GetHists():
        c = TCanvas('cpreview', 'preview')
        c.cd()
        h.Draw()
        leg = TLegend(0.75, 0.6, 0.99, 0.93)
        h.GetListOfFunctions().Add(leg)
        for i, (k, v) in enumerate(fss.items()):
            v.Setup(h)
            v.Fit(h)
            v.bg.SetLineColor(goodcolors[i])
            v.sig.SetLineColor(goodcolors[i])
            v.sig.SetLineStyle(2)
            v.fn.SetLineColor(goodcolors[i])
            v.fn.SetLineStyle(3)
            v.bg.SetNpx(500)
            v.sig.SetNpx(500)
            v.fn.SetNpx(500)
            h.GetListOfFunctions().Add(v.bg)
            h.GetListOfFunctions().Add(v.sig)
            h.GetListOfFunctions().Add(v.fn)
            # range only needs to be set once, but since range is accessible here,
            h.GetXaxis().SetRangeUser(v.drawrange[0], v.edgerange[1]+0.1)
            lbl = k
            lbl = k if v.converged else k+' (X)'
            leg.AddEntry(v.bg, lbl, 'l')
        h.GetYaxis().SetRangeUser(0, 1.1*h.GetMaximum())
        c.Modified()
        c.Update()
        c.SaveAs('xsect/%s.pdf' % h.GetName())

if __name__ == '__main__':
    main()
