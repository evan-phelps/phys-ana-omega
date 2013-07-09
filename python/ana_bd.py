import sys
sys.path.append('/opt/root/lib')

import ROOT as r
from rootpy.io import root_open as ropen
from rootpy import asrootpy
from rootpy.plotting import Canvas
from rootpy.interactive import wait
from epfuncs import RejectWrapper
import new

r.gROOT.SetStyle('Modern')
r.gStyle.SetTitleYOffset(1.25)
r.gStyle.SetTitleXOffset(1.1)
r.gStyle.SetOptStat(0)
r.gStyle.SetOptFit(0)

mpi0, mpi, mrho0, mphi, mk, mw = 0.1349766, 0.139570, 0.77549, 1.019445, 0.493677, 0.78265
records = []

etarange = (0.5, 0.6)
omegarange = (mw-3*0.02, mw+3*0.02)
fitrange = (0.6, 0.95)
fbgskip = [etarange, omegarange]

fin_t = ropen('/home/ephelps/projects/phys-ana-omega/input/acc-per-part.root')
fin_e = ropen('/home/ephelps/projects/phys-ana-omega/h6maker-hn-el1.root')
idims = new.module('idims_namespace')
idims.__dict__.update({'w': 0, 'q2': 1, 't': 2, 'ct': 3, 'phi': 4, 'mmp': 5})
h4_t, h4_a = fin_t.Get('hthrown'), fin_t.Get('hacc')
h6_e = fin_e.Get('hbd_yield')


def d_voigt(v, par):
    return par[0]*r.TMath.Voigt(v[0]-par[1], par[2], par[3])


def fitbg(h, fstr='pol8', skip=fbgskip):
    # get function with fbgskip regions neglected in fit
    bgwrapper = RejectWrapper(r.TF1('fbg', fstr, *fitrange), fbgskip)
    fbgrej = bgwrapper.newtf1
    fbg = bgwrapper.tf1
    [f.SetNpx(1000) for f in [fbg, fbgrej]]
    h.Fit(fbgrej, 'N0', '', *fitrange)
    fbg.SetLineColor(r.kBlack)
    fbg.SetLineStyle(2)
    h.GetListOfFunctions().Add(fbg)
    return fbg


def fitsig(h):
    fvoigt = r.TF1('fsig', d_voigt, 0.6, 0.95, 4)
    fvoigt.SetNpx(1000)
    fvoigt.SetParameters(250000, 0.78265, 0.017, 0.0849)
    fvoigt.SetParLimits(1, mw-0.00060, mw+0.00060)
    fvoigt.SetParLimits(3, 0.00849-0.00040, 0.00849+0.00040)
    h.Fit(fvoigt, 'N0')
    h.GetListOfFunctions().Add(fvoigt)
    return fvoigt


def ibins2vals(ibinW, ibinQ2):
    mW = h6_e.GetAxis(idims.w).GetBinCenter(ibinW)
    lW = h6_e.GetAxis(idims.w).GetBinLowEdge(ibinW)
    hW = h6_e.GetAxis(idims.w).GetBinLowEdge(ibinW+1)
    mQ2 = h6_e.GetAxis(idims.q2).GetBinCenter(ibinQ2)
    lQ2 = h6_e.GetAxis(idims.q2).GetBinLowEdge(ibinQ2)
    hQ2 = h6_e.GetAxis(idims.q2).GetBinLowEdge(ibinQ2+1)
    return ((mW, lW, hW), (mQ2, lQ2, hQ2))


def vals2ibins(valW, valQ2):
    ibinW = h6_e.GetAxis(idims.w).FindBin(valW)
    ibinQ2 = h6_e.GetAxis(idims.q2).FindBin(valQ2)
    return (ibinW, ibinQ2)


def hmmp(ibinW, ibinQ2, vals=None):
    h6_e.GetAxis(idims.w).SetRange(ibinW, ibinW)
    h6_e.GetAxis(idims.q2).SetRange(ibinQ2, ibinQ2)
    h = h6_e.Projection(idims.mmp)
    h6_e.GetAxis(idims.w).SetRange(0, -1)
    h6_e.GetAxis(idims.q2).SetRange(0, -1)

    if vals is None:
        vals = ibins2vals(wbin, q2bin)
    (wval, wlo, whi, q2val, q2lo, q2hi) = (vals[0][0], vals[0][1], vals[0][2], vals[1][0], vals[1][1], vals[1][2])
    h.SetName('hmmp_%d_%d' % (1000*wval, 1000*q2val))
    h.SetTitle('MM_p, W = [%.3f,%.3f), Q^{2} = [%.3f,%.3f)' % (wlo, whi, q2lo, q2hi))
    h.GetXaxis().SetTitle('MM_{p} (GeV)')
    h.GetYaxis().SetTitle('#epsilon_{CC}-/#Gamma_{v}-corrected yield')

    return h


# pick a W/Q2 bin; these will be function arguments ultimately
wbin, q2bin = 15, 5
# get bin center and edges
vals = ibins2vals(wbin, q2bin)
(wval, wlo, whi, q2val, q2lo, q2hi) = (vals[0][0], vals[0][1], vals[0][2], vals[1][0], vals[1][1], vals[1][2])
record = {'W': wval, 'Wlo': wlo, 'Whi': whi, 'Wbin': wbin,
          'Q2': q2val, 'Q2lo': q2lo, 'Q2hi': q2hi, 'Q2bin': q2bin}

# get MMp mass distribution for current W/Q2 bin
h = asrootpy(hmmp(wbin, q2bin, vals))
# fit the background
fbg = fitbg(h, 'pol3')
record['fbgstat'] = r.gMinuit.fCstatu
for ipar in range(0, fbg.GetNpar()):
    record['p%d' % ipar] = fbg.GetParameter(ipar)

# subtract background fit function
hsig = asrootpy(h.Clone('%s_hsig' % h.GetName()))
hsig.GetYaxis().SetTitle('#epsilon_{CC}-/#Gamma_{v}-corrected, bg-subtracted yield')
hsig.Add(fbg, -1)

# fit signal
fsig = fitsig(hsig)

# get signal boundaries and correction factor
gm = fsig.GetParameter(1)
gs = fsig.GetParameter(2)
mmplo, mmphi = gm-3*gs, gm+3*gs
mmpbinlo, mmpbinhi = h.FindBin(mmplo), h.FindBin(mmphi)
mmplo, mmphi = h.GetBinLowEdge(mmpbinlo), h.GetBinLowEdge(mmpbinhi+1)
intfull = fsig.Integral(*fitrange)
int3s = fsig.Integral(mmplo, mmphi)
weight3s = int3s/intfull
print(weight3s)

# define signal/sideband edges and sideband weight factor
vedges = (fitrange[0], mmplo, mmphi, fitrange[1])
iedges = (1, mmpbinlo, mmpbinhi, h.GetNbinsX())
intsig = fbg.Integral(vedges[1], vedges[2])
intsb1 = h.Integral(iedges[0], iedges[1]-1, 'width')
intsb2 = h.Integral(iedges[2]+1, iedges[3], 'width')
weightsb = intsig/(intsb1+intsb2)
print(weightsb)

# draw canvas
cmmp = Canvas(name='bgsigfit', title='BG/Signal Fits, W, Q2 = %.3f GeV,%.3f GeV2' % (wval, q2val))
hs = r.THStack('hs', '#omega^{0} mass distribution')
hsig.SetColor('blue')
hsig.GetListOfFunctions().FindObject('fsig').SetLineColor(r.kBlue)
hs.Add(hsig)
hs.Add(h)
hs.Draw('nostack')
r.gPad.Update()
hs.GetHistogram().GetXaxis().SetTitle(h.GetXaxis().GetTitle())
hs.GetHistogram().GetYaxis().SetTitle('yield')
rchi2 = fsig.GetChisquare()/fsig.GetNDF()
ylo, yhi = r.gPad.GetUymin(), r.gPad.GetUymax()
for l in [r.TLine(mmplo, ylo, mmplo, yhi), r.TLine(mmphi, ylo, mmphi, yhi)]:
    hsig.GetListOfFunctions().Add(l)
    l.SetLineColor(r.kBlue)
yrange = yhi-ylo
tchi2 = r.TLatex(0.86, 0.8*yrange, '#tilde{#chi}^{2} = %.2f' % (rchi2))
tchi2.Draw()
r.gPad.Update()

# output
for (k, v) in record.items():
    print(k, v)

wait()
