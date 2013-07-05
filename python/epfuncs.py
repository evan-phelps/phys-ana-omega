"""Contains functions to be used for modeling and fitting, specifically with
ROOT.  Function naming conventions:

    + "f_*": return TF1 objects for fitting.

    + "d_*": function used by TF1.  arguments bound to Double_t*, Double_t*
    for coordinate and parameter vectors.

"""
import operator as op

import ROOT
from ROOT import TMath as m
from ROOT import TF1

from epxsectutils import MOMEGA, WOMEGA

def d_gausexp(v, par):
    """Convolution of gauss and exponential using analytic expression.
    parameters:
        par[0] (mag):    magnitude
        par[1] (gmean):  gauss mean
        par[2] (gsigma): gauss sqrt(variance)
        par[3] (ehl):  exponential decay half-life.
    """
    x, mag, gmean, gsigma, ehl = v[0], par[0], par[1], par[2], par[3]
    assert mag > 0
    assert gsigma > 0
    assert ehl > 0
    erate = m.Log(2)/ehl
    erfcarg = (gmean + erate*gsigma**2 - x)/(gsigma*m.Sqrt(2))
    factor = erate/2 * m.Exp(erate/2 * (2*gmean + erate*gsigma**2 - 2*x))
    return mag * factor * m.Erfc(erfcarg)


def f_gausexp(fn='fgexp', funcrange=(0.4, 2), limsmag=(1, 100000),
              limsmean=(0.773, 0.793), limssigma=(0.010, 0.030),
              limsehl=(0.01, 0.2)):
    """Wraps creation of TF1 corresponding to d_gausexp.  Default values and
    parameter limits are meant to be suitable for e1f omega mass range, but
    note that the limsmag[1] parameter can change dramatically depending on
    binning choices.
    """
    f = TF1('fgexp', d_gausexp, funcrange[0], funcrange[1], 4)
    f.SetParLimits(0, limsmag[0], limsmag[1])
    f.SetParLimits(1, limsmean[0], limsmean[1])
    f.SetParLimits(2, limssigma[0], limssigma[1])
    f.SetParLimits(3, limsehl[0], limsehl[1])
    f.SetParameters(limsmag[1], limsmean[0], limssigma[0], limsehl[0])
    f.SetNpx(500)
    # add some convenience properties to this instance of TF1
    f.lims = {0: limsmag, 1: limsmean, 2: limssigma, 3: limsehl}
    return f


def d_pol2(v, par):
    return par[0] + par[1]*v[0] + par[2]*v[0]**2


def d_bw(v, par):
    mmp = v[0]
    mag = par[0]
    mean = par[1]
    gamma = par[2]
    return mag*m.BreitWigner(mmp, mean, gamma)


def d_bwexpgaus(v, par):
    """Convolution of Breit-Wigner with parameters fixed to omega properties,
    gauss and exponential.
        par[0]:  amplitude
        par[1]:  bw mean
        par[2]:  gauss sqrt(variance)
        par[3]:  exponential decay half-life
    """
    convrange = [v[0]-6*par[2], v[0]+6*par[2]+4*par[3]]
    nsteps = 100
    convstepsize = (convrange[1]-convrange[0])/nsteps
    summ = 0
    for x in [convrange[0] + step*convstepsize for step in range(0, nsteps+1)]:
        bwpars = [ROOT.Double(1), par[1], WOMEGA]
        egpars = [ROOT.Double(1), ROOT.Double(x), ROOT.Double(par[2]), ROOT.Double(par[3])]
        gval = d_gausexp([ROOT.Double(v[0])], egpars)
        summ += gval*d_bw([ROOT.Double(x)], bwpars)
    return par[0]*convstepsize*summ


def f_bwexpgaus(fn='fbwexgaus', funcrange=(0.4, 2), limsmag=(1, 100000),
                limsmass=(MOMEGA-0.03, MOMEGA+0.01), limssigma=(0.005, 0.030),
                limsehl=(0.01, 0.2)):
    """Wraps creation of TF1 corresponding to d_bwgausexp.  Default values and
    parameter limits are meant to be suitable for e1f omega mass range, but
    note that the limsmag[1] parameter can change dramatically depending on
    binning choices.
    """
    f = TF1('fbwexpgaus', d_bwexpgaus, funcrange[0], funcrange[1], 4)
    f.SetParLimits(0, limsmag[0], limsmag[1])
    f.SetParLimits(1, limsmass[0], limsmass[1])
    f.SetParLimits(2, limssigma[0], limssigma[1])
    f.SetParLimits(3, limsehl[0], limsehl[1])
    f.SetParameters(limsmag[1], MOMEGA, limssigma[0], limsehl[0])
    f.SetNpx(500)
    # add some convenience properties to this instance of TF1
    f.lims = {0: limsmag, 1: limsmass, 2: limssigma, 3: limsehl}
    return f


def d_bwexpgaus_pol4(v, par):
    bwegpars = [ROOT.Double(par[i]) for i in range(0, 4)]
    pol4pars = (ROOT.Double(par[i]) for i in range(4, 9))
    sig = d_bwexpgaus(v, bwegpars)
    bg = reduce(op.add, [pol4par*v[0]**j for (j, pol4par) in enumerate(pol4pars)])
    return sig + bg


def f_bwexpgaus_pol4(fn='fbwexgaus', funcrange=(0.4, 2), limsmag=(1, 100000),
                     limsmass=(MOMEGA-0.01, MOMEGA+0.01), limssigma=(0.005, 0.030),
                     limsehl=(0.01, 0.2)):
    """Wraps creation of TF1 corresponding to d_bwgausexp_pol4.  Default values
    and parameter limits are meant to be suitable for e1f omega mass range, but
    note that the limsmag[1] parameter can change dramatically depending on
    binning choices.
    """
    f = TF1('fbwexpgauspol4', d_bwexpgaus_pol4, funcrange[0], funcrange[1], 9)
    f.SetParLimits(0, limsmag[0], limsmag[1])
    f.SetParLimits(1, limsmass[0], limsmass[1])
    f.SetParLimits(2, limssigma[0], limssigma[1])
    f.SetParLimits(3, limsehl[0], limsehl[1])
    f.SetParameters(limsmag[1], MOMEGA, limssigma[0], limsehl[0])
    f.SetNpx(500)
    # add some convenience properties to this instance of TF1
    f.lims = {0: limsmag, 1: limssigma, 2: limsehl}
    return f


class RejectWrapper:
    """Wraps a TF1 with point rejection logic according to provided skip
    ranges."""
    def __init__(self, tf1, rejs, fname=None):
        self.rejranges = rejs
        self.tf1 = tf1
        if fname is None:
            fname = '%s_rej' % self.tf1.GetName()
        lo, hi, nparms = ROOT.Double(0), ROOT.Double(0), self.tf1.GetNpar()
        self.tf1.GetRange(lo, hi)
        self.newtf1 = TF1(fname, self.d_tf1, lo, hi, nparms)

    def d_tf1(self, v, par):
        for i, p in enumerate(par):
            self.tf1.SetParameter(i, par[i])
        val = self.tf1.Eval(v[0])
        for (lo, hi) in self.rejranges:
            if v[0] > lo and v[0] < hi:
                TF1.RejectPoint()
        return val
