from ROOT import TMath as m
from ROOT import TF1


def d_gausexp(v, par):
    """convolution of gauss and exponential using analytic expression.
    parameters:
        par[0] (mag):    magnitude
        par[1] (gmean):  gauss mean
        par[2] (gsigma): gauss sqrt(variance)
        par[3] (ehl):  exponential decay half-life
    """
    x, mag, gmean, gsigma, ehl = v[0], par[0], par[1], par[2], par[3]
    assert mag > 0
    assert gsigma > 0
    assert ehl > 0
    erate = m.Log(2)/ehl
    erfcarg = (gmean + erate*gsigma**2 - x)/(gsigma*m.Sqrt(2))
    factor = erate/2 * m.Exp(erate/2 * (2*gmean + erate*gsigma**2 - 2*x))
    return mag * factor * m.Erfc(erfcarg)


def f_gausexp(fn='fgexp', rLo=0.4, rHi=2, magLo=0, magHi=100000,
              meanLo=0.773, meanHi=0.793, sigmaLo=0.010, sigmaHi=0.030,
              ehlLo=0.01, ehlHi=0.2):
    """Wraps creation of TF1 corresponding to d_gausexp.  Default values and
    parameter limits are meant to be suitable for e1f omega mass range, but
    note that the magHi parameter can change dramatically depending on binning
    choices.
    """
    f = TF1('fgexp', d_gausexp, rLo, rHi, 4)
    f.SetParLimits(0, magLo, magHi)
    f.SetParLimits(1, meanLo, meanHi)
    f.SetParLimits(2, sigmaLo, sigmaHi)
    f.SetParLimits(3, ehlLo, ehlHi)
    f.SetParameters(magHi, meanLo, sigmaLo, ehlLo)
    return f
