import ROOT as r
from ROOT import TMinuit
from rootpy.io import root_open as ropen
import pandas as pd
from functools import reduce
# import matplotlib
# matplotlib.use('gtkagg')
# import matplotlib.pylab as mplt
cs = ['r', 'y', 'g', 'b', 'm']

r.gSystem.Load('libMathMore.so')
gMin = r.gMinuit

def get_d_legs(legs):
    lms = legs

    def d_legs(v, par):
        retval = reduce(lambda x, y: x + y, [par[i] * r.Math.assoc_legendre(
            L, M, v[0]) for i, (L, M) in enumerate(lms)])
        return retval
    return d_legs


def get_f_legs(legs, par):
    retf = r.TF1('flegs', get_d_legs(legs), -1, 1, len(legs))
    for i in range(0, len(legs)):
        retf.SetParameter(i, par[i])
    return retf


def get_d_legs_tu(legs):
    lms = legs
    iexp0 = [i+len(legs) for i in range(0, 4)]

    def d_legs_tu(v, par):
        retval = reduce(lambda x, y: x + y, [par[i] * r.Math.assoc_legendre(
            L, M, v[0]) for i, (L, M) in enumerate(lms)])
        retval = retval + par[iexp0[0]] * r.TMath.Exp(par[iexp0[1]] * v[0])
        retval = retval + par[iexp0[2]] * r.TMath.Exp(-par[iexp0[3]] * v[0])
        return retval
    return d_legs_tu


def get_f_legs_tu(order=7, ignore=[], tamp=200, uamp=200):
    nlegs = order - len(ignore) + 1
    npars = nlegs + 4  # legendre 0...leg_order plus 4 for the 2 expos
    legs = [(i, 0) for i in range(0, order+1) if (i, 0) not in ignore]
    retf = r.TF1('flegstu', get_d_legs_tu(legs), -1, 1, npars)
    if tamp == 0:
        retf.FixParameter(nlegs+0, tamp)
        retf.FixParameter(nlegs+1, 0)
    else:
        retf.SetParLimits(nlegs+0, 0, tamp)
        retf.SetParLimits(nlegs+1, 5, 15)
        retf.SetParameter(nlegs+0, 0)
        # retf.SetParameter(nlegs+1, 7.3)
        retf.FixParameter(nlegs+1, 7.3)
    if uamp == 0:
        retf.FixParameter(nlegs+2, uamp)
        retf.FixParameter(nlegs+3, 0)
    else:
        retf.SetParLimits(nlegs+2, 0, uamp)
        retf.SetParLimits(nlegs+3, 1, 50)
    # for ipar in range(0, nlegs):
    #     retf.SetParLimits(ipar, 0, 1000)
    #     retf.SetParName(ipar, 'L_%d_%d' % legs[ipar])
    retf.SetParLimits(0, 0, 1000)
    for ipar in range(1, nlegs):
        retf.SetParLimits(1, -1000, 1000)
    retf.legs = legs
    return retf


def fitlegs(h, leg_order=3, ignore=[], tamp=0, uamp=0, trim=True):
    f = get_f_legs_tu(leg_order, ignore, tamp, uamp)
    p0, p1 = r.Double(-1), r.Double(-1)
    # for ipar in range(0, f.GetNpar()):
    #     f.GetParLimits(ipar, p0, p1)
    #     print(ipar, p0, p1)
    # h.SetMinimum(1)
    (rlo, rhi) = (-0.875, 0.875) if trim else (-1, 1)

    #correction for drop in thrown event yield at forward angle
    #lastbc = h.GetBinContent(h.GetNbinsX())
    #if lastbc > 0: h.SetBinContent(h.GetNbinsX(), 1.15*lastbc)

    h.Fit(f, 'QN0', 'goff', rlo, rhi)
    f.SetRange(-1,1)
    h.GetListOfFunctions().Add(f)
    status = gMin.fCstatu
    fl = get_f_legs(f.legs, f.GetParameters())
    fl.SetLineColor(r.kBlue+1)
    fl.SetLineStyle(2)
    fl.SetLineWidth(3)
    #fl.Draw("same")
    h.GetListOfFunctions().Add(fl)

    ft = r.TF1('ft', '[0]*TMath::Exp([1]*x)', -1, 1)
    ft.SetParameters(f.GetParameter(len(f.legs)), f.GetParameter(len(f.legs)+1))
    ft.SetLineColor(r.kGreen+1)
    ft.SetLineStyle(2)
    #ft.Draw('same')
    h.GetListOfFunctions().Add(ft)

    fu = None
    if uamp > 0:
        fu = r.TF1('fu', '[0]*TMath::Exp(-[1]*x)', -1, 1)
        fu.SetParameters(f.GetParameter(len(f.legs)+2), f.GetParameter(len(f.legs)+3))
        fu.SetLineColor(r.kYellow+1)
        fu.SetLineStyle(2)
        #fu.Draw('same')
    return (f, fl, ft, fu, h, status)


fin = ropen('top1/ana_bd.root')
df = pd.read_csv('top1/ana_bd.dat')


def intQ2(wmid=1890):
    wstr = '_%d_' % wmid
    hs = [fin.Get(hn) for hn in df.hcost[df.hcost.apply(lambda x: wstr in x)]]
    dQ2 = df[df.hcost.apply(lambda x: wstr in x)][['Q2lo', 'Q2hi']]
    dQ2 = dQ2.Q2hi - dQ2.Q2lo

    # unscale, add, scale
    for i, h in enumerate(hs): h.Scale(dQ2.iloc[i])
    htot = hs[0].Clone(wstr)
    for i in range(1, len(hs)): htot.Add(hs[i])
    for i, h in enumerate(hs): h.Scale(1/dQ2.iloc[i])
    htot.Scale(1/dQ2.sum())
    htot.SetTitle('W = %.3f GeV' % (wmid/1000.0))
    #htot.Draw()
    return htot


def fitH(h, leg_order=2, ignore=[], tamp=0, uamp=0, trim=True):
    res = fitlegs(h, leg_order, ignore, tamp, uamp, trim)
    # print(res[0].GetChisquare()/res[0].GetNDF())
    return res


def doall(trim=True):
    chi2s = []
    for W in [round(w*1000) for w in df.W.unique()]:
        htot = intQ2(W)
        #texpA = htot.Interpolate(0)
        #texpA = 1.1*texpA if texpA > 0 else 10
        texpA = 2
        res = fitH(htot, trim=trim, tamp=texpA)
        chi2s.append(res)
    return chi2s


def doeach(trim=True):
    ress = []
    for w, h in zip(df.W, [fin.Get(hn) for hn in df.hcost]):
        texpA = 5 if w > 2 else 0
        res = fitH(h, trim=trim, tamp=texpA)
        ress.append(res)
    return ress


res = fitH(intQ2(1890))
