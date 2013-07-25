import pandas as pd
from rootpy.io import root_open as ropen
import ROOT as r
from functools import reduce

r.gSystem.Load('libMathMore.so')

fin = ropen('out/ana_bd.root')
df = pd.read_csv('out/ana_bd.dat')
df1bin = df[(df.Wbin == 15) & (df.Q2bin == 5)]
hs = dict(zip(zip(df.Wbin, df.Q2bin), [fin.Get(hn) for hn in df.hcost]))
h = hs[(15, 5)]


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


def get_f_legs_tu(order=7, ignore=[]):
    nlegs = order - len(ignore) + 1
    npars = nlegs + 4  # legendre 0...leg_order plus 4 for the 2 expos
    legs = [(i, 0) for i in range(0, order+1) if (i, 0) not in ignore]
    retf = r.TF1('flegstu', get_d_legs_tu(legs), -1, 1, npars)
    retf.SetParLimits(nlegs+0, 0, 500)
    retf.SetParLimits(nlegs+1, 2, 8)
    retf.SetParLimits(nlegs+2, 0, 500)
    retf.SetParLimits(nlegs+3, 2, 8)
    for ipar in range(0, nlegs):
        retf.SetParLimits(ipar, 0, 10000)
        retf.SetParName(ipar, 'L_%d_%d' % legs[ipar])
    retf.legs = legs
    return retf

leg_order = 8  # 2
ignore = [(1, 0), (2, 0), (3, 0), (4, 0), (6, 0), (8, 0)]  # []
f = get_f_legs_tu(leg_order, ignore)
p0, p1 = r.Double(-1), r.Double(-1)
for ipar in range(0, f.GetNpar()):
    f.GetParLimits(ipar, p0, p1)
    print(ipar, p0, p1)
h.SetMinimum(1)
h.Fit(f, '', '', -0.875, 0.875)

fl = get_f_legs(f.legs, f.GetParameters())
fl.SetLineColor(r.kBlue+1)
fl.SetLineStyle(2)
fl.SetLineWidth(3)
fl.Draw("same")

ft = r.TF1('ft', '[0]*TMath::Exp([1]*x)', -1, 1)
ft.SetParameters(f.GetParameter(len(f.legs)), f.GetParameter(len(f.legs)+1))
ft.SetLineColor(r.kGreen+1)
ft.SetLineStyle(2)
ft.Draw('same')

fu = r.TF1('fu', '[0]*TMath::Exp(-[1]*x)', -1, 1)
fu.SetParameters(f.GetParameter(len(f.legs)+2), f.GetParameter(len(f.legs)+3))
fu.SetLineColor(r.kYellow+1)
fu.SetLineStyle(2)
fu.Draw('same')
