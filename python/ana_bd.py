# TODO: apply acceptance
# TODO: apply hole corrections
# TODO: calculate integrated cross-sections and add to output
# TODO: calculate differential cross-sections and add to output
import sys
sys.path.append('/opt/root/lib')
import new

import ROOT as r
from rootpy.io import root_open as ropen
from rootpy import asrootpy
from rootpy.plotting import Canvas
from rootpy.interactive import wait
from epfuncs import RejectWrapper

# set style for canvases
r.gROOT.SetStyle('Modern')
r.gStyle.SetTitleYOffset(1.25)
r.gStyle.SetTitleXOffset(1.1)
r.gStyle.SetOptStat(0)
r.gStyle.SetOptFit(0)

# particle properties
mpi0 = 0.1349766
mpi = 0.139570
mrho0 = 0.77549
mphi = 1.019445
mk = 0.493677
mw, ww = 0.78265, 0.00849

# fit and cut information
fitrange = (0.6, 0.95)
etarange = (0.5, 0.6)
omegarange = (mw-3*0.02, mw+3*0.02)
fbgskip = [etarange, omegarange]
cutsigma = 3

# input data
ifacc = '/home/ephelps/projects/phys-ana-omega/input/acc-per-part.root'
ifexp = '/home/ephelps/projects/phys-ana-omega/h6maker-hn-el1.root'
ihacc = 'hacc'
ihthr = 'hthrown'
ihexp = 'hbd_yield'
idims = new.module('idims_namespace')
idims.__dict__.update({'w': 0, 'q2': 1, 't': 2, 'ct': 3, 'phi': 4, 'mmp': 5})

# canvas and histogram information
hstitle = 'W = [%.3f,%.3f), Q^{2} = [%.3f,%.3f)'
hmmpTitle = 'MM_p, W = [%.3f,%.3f), Q^{2} = [%.3f,%.3f)'
hmmpXtitle = 'MM_{p} (GeV)'
hmmpYtitle = 'yield'
tchi2str = '#tilde{#chi}^{2} = #scale[0.65]{#frac{%.2f}{%d}}' \
           + ' = #scale[0.75]{%.2f}'
tchi2posX = 0.07
tchi2posY = 0.75

#output data
oroot = 'out/ana_bd.root'
orecords = 'out/ana_bd.dat'
keyorder = ['Wbin', 'Q2bin', 'W', 'Wlo', 'Whi', 'Q2', 'Q2lo', 'Q2hi',
            'fbgstat', 'fsigstat', 'fsigchi2', 'weight3s', 'weightsb']

# get histograms for simulation, acceptance, and experiment
fin_t = ropen(ifacc)
fin_e = ropen(ifexp)
h4_t, h4_a = fin_t.Get(ihthr), fin_t.Get(ihacc)
h6_e = fin_e.Get(ihexp)
# open output files
fout = ropen(oroot, 'recreate')
foutrecs = open(orecords, 'w')
# write data header
for i, k in enumerate(keyorder):
    if i > 0:
        foutrecs.write(', ')
    foutrecs.write(k)
foutrecs.write('\n')


# TODO: add cutoff at phase-space edge
def d_voigt(v, par):
    return par[0]*r.TMath.Voigt(v[0]-par[1], par[2], par[3])


# TODO: add cutoff at phase-space edge
def d_bg(v, par):
    ret = 0
    for n in range(0, 6):
        ret += par[n]*v[0]**n
    return ret if ret > 0 else 0


def fitbg(h, skip=fbgskip):
    # get function with fbgskip regions neglected in fit
    bgwrapper = RejectWrapper(r.TF1('fbg', d_bg, fitrange[0], fitrange[1], 6), fbgskip)
    fbgrej = bgwrapper.newtf1
    fbg = bgwrapper.tf1
    [f.SetNpx(1000) for f in [fbg, fbgrej]]
    h.Fit(fbgrej, 'QN0', '', *fitrange)
    fbg.SetLineColor(r.kBlack)
    fbg.SetLineStyle(2)
    h.GetListOfFunctions().Add(fbg)
    return fbg


# TODO: improve parameter limits
def fitsig(h):
    fvoigt = r.TF1('fsig', d_voigt, fitrange[0], fitrange[1], 4)
    fvoigt.SetNpx(1000)
    fvoigt.SetParameters(250000, mw, 0.017, ww)
    fvoigt.SetParLimits(1, mw-0.00060, mw+0.00060)
    fvoigt.SetParLimits(3, ww-0.00040, ww+0.00040)
    h.Fit(fvoigt, 'QN0')
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
        vals = ibins2vals(ibinW, ibinQ2)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    h.SetName('hmmp_%d_%d' % (1000*wval, 1000*q2val))
    h.SetTitle(hmmpTitle % (wlo, whi, q2lo, q2hi))
    h.GetXaxis().SetTitle(hmmpXtitle)
    h.GetYaxis().SetTitle(hmmpYtitle)

    return h


def fitbin(wbin, q2bin):
    # get bin center and edges
    vals = ibins2vals(wbin, q2bin)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    record = {'W': wval, 'Wlo': wlo, 'Whi': whi, 'Wbin': wbin,
              'Q2': q2val, 'Q2lo': q2lo, 'Q2hi': q2hi, 'Q2bin': q2bin}

    # get MMp mass distribution for current W/Q2 bin
    h = asrootpy(hmmp(wbin, q2bin, vals))
    if h.Integral() < 10000:
        return

    # fit the background
    fbg = fitbg(h)
    record['fbgstat'] = r.gMinuit.fCstatu
    for ipar in range(0, fbg.GetNpar()):
        record['p%d' % ipar] = fbg.GetParameter(ipar)

    # subtract background fit function
    hsig = asrootpy(h.Clone('%s_hsig' % h.GetName()))
    hsig.Add(fbg, -1)

    # fit signal
    fsig = fitsig(hsig)
    record['fsigstat'] = r.gMinuit.fCstatu
    for ipar in range(0, fsig.GetNpar()):
        record['p%d' % ipar] = fbg.GetParameter(ipar)

    # get signal boundaries and integral correction factor
    gm = fsig.GetParameter(1)
    gs = fsig.GetParameter(2)
    mmplo, mmphi = gm-cutsigma*gs, gm+cutsigma*gs
    mmpbinlo, mmpbinhi = h.FindBin(mmplo), h.FindBin(mmphi)
    mmplo, mmphi = h.GetBinLowEdge(mmpbinlo), h.GetBinLowEdge(mmpbinhi+1)
    intfull = fsig.Integral(*fitrange)
    int3s = fsig.Integral(mmplo, mmphi)
    weight3s = int3s/intfull if intfull > 0 else -1
    record['weight3s'] = weight3s

    # define signal/sideband edges and sideband weight factor
    vedges = (fitrange[0], mmplo, mmphi, fitrange[1])
    iedges = (1, mmpbinlo, mmpbinhi, h.GetNbinsX())
    intsig = fbg.Integral(vedges[1], vedges[2])
    intsb1 = h.Integral(iedges[0], iedges[1]-1, 'width')
    intsb2 = h.Integral(iedges[2]+1, iedges[3], 'width')
    weightsb = intsig/(intsb1+intsb2) if intsb1+intsb2 > 0 else -1
    record['weightsb'] = weightsb

    # draw canvas
    cmmp = Canvas(name='bgsigfit_%d_%d' % (1000*wval, 1000*q2val),
                  title='MMp Fits, W, Q2 = %.3f GeV,%.3f GeV2' % (wval, q2val))
    hs = r.THStack('hs_%d_%d' % (1000*wval, 1000*q2val),
                   hstitle % (wlo, whi, q2lo, q2hi))
    hsig.SetColor('blue')
    hsig.GetListOfFunctions().FindObject('fsig').SetLineColor(r.kBlue)
    hs.Add(hsig)
    hs.Add(h)
    hs.Draw('nostack')
    r.gPad.Update()
    hs.GetHistogram().GetXaxis().SetTitle(h.GetXaxis().GetTitle())
    hs.GetHistogram().GetYaxis().SetTitle('yield')
    chi2, ndf = fsig.GetChisquare(), fsig.GetNDF()
    rchi2 = chi2/ndf if ndf > 0 else -1
    record['fsigchi2'] = rchi2
    ylo, yhi = r.gPad.GetUymin(), r.gPad.GetUymax()
    xlo, xhi = r.gPad.GetUxmin(), r.gPad.GetUxmax()
    for l in [r.TLine(mmplo, ylo, mmplo, yhi), r.TLine(mmphi, ylo, mmphi, yhi)]:
        hsig.GetListOfFunctions().Add(l)
        l.SetLineColor(r.kBlue)
    yscale = yhi-ylo
    xscale = xhi-xlo
    tchi2 = r.TLatex(xlo+tchi2posX*xscale, ylo+tchi2posY*yscale, tchi2str % (chi2, ndf, rchi2))
    tchi2.Draw()
    r.gPad.Update()
    cmmp.SaveAs('out/%s.pdf' % cmmp.GetName())

    # output
    # TODO: add fit parameters to record output
    for (k, v) in record.items():
        if isinstance(v, float):
            record[k] = '%.3f' % v
        if isinstance(v, basestring):
            record[k] = '\"%s\"' % v
    for i, k in enumerate(keyorder):
        if i > 0:
            foutrecs.write(', ')
        foutrecs.write(str(record[k]))
    foutrecs.write('\n')

    wait()


def testme(wbin=15, q2bin=5):
    fitbin(wbin, q2bin)
    foutrecs.close()
    fout.Write()
    fout.Close()


if __name__ == "__main__":
    r.gROOT.SetBatch(r.kTRUE)
    for wbin in range(1, h6_e.GetAxis(idims.w).GetNbins()+1):
        for q2bin in range(1, h6_e.GetAxis(idims.q2).GetNbins()+1):
            fitbin(wbin, q2bin)

    foutrecs.close()
    fout.Write()
    fout.Close()
