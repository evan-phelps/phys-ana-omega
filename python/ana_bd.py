# TODO: calculate differential cross-sections and add to output

# TODO: propagate integral errors to error on weight3s
import sys
#sys.path.append('/opt/root/lib')
import new
from math import sqrt

import ROOT as r
from rootpy.io import root_open as ropen
from rootpy import asrootpy
from rootpy.plotting import Canvas
from rootpy.plotting import Hist
from rootpy.interactive import wait
from epfuncs import RejectWrapper

from epxsectutils import vgflux

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
ifexp = '/home/ephelps/projects/phys-ana-omega/h6maker-hn-el1-CCcor.root'  # -el1_now8.root'
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
h2costphiTitle = '%s, W = [%.3f,%.3f), Q^{2} = [%.3f,%.3f)'
h2costphiXtitle = '#phi^{*} (radians)'
h2costphiYtitle = 'cos(#theta^{*})'

#output data
oroot = 'out/ana_bd.root'
orecords = 'out/ana_bd.dat'
keyorder = ['Wbin', 'Q2bin', 'W', 'Wlo', 'Whi', 'Q2', 'Q2lo', 'Q2hi',
            'weight3s', 'weightsb', 'fbgstat', 'fbgchi2', 'fsigstat', 'fsigchi2',
            'fbgpar0', 'fbgpar1', 'fbgpar2', 'fbgpar3', 'fbgpar4', 'fbgpar5',
            'fsigpar0', 'fsigpar1', 'fsigpar2', 'fsigpar3', 'hsigint3s']

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
        foutrecs.write(',')
    foutrecs.write(k)
foutrecs.write(',xsect,err,hcost\n')


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


def fitsig(h):
    fvoigt = r.TF1('fsig', d_voigt, fitrange[0], fitrange[1], 4)
    fvoigt.SetNpx(1000)
    mag = h.Integral('width')
    fvoigt.SetParameters(mag, mw, 0.017, ww)
    fvoigt.SetParLimits(0, 0.2*mag, 3*mag)
    fvoigt.SetParLimits(1, mw-0.00060, mw+0.00060)
    fvoigt.SetParLimits(2, 0.008, 0.035)
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
    h = h6_e.Projection(idims.mmp, 'e')
    h6_e.GetAxis(idims.w).SetRange(0, -1)
    h6_e.GetAxis(idims.q2).SetRange(0, -1)

    if vals is None:
        vals = ibins2vals(ibinW, ibinQ2)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    h.SetName('hmmp_%d_%d' % (round(1000*wval), round(1000*q2val)))
    h.SetTitle(hmmpTitle % (wlo, whi, q2lo, q2hi))
    h.GetXaxis().SetTitle(hmmpXtitle)
    h.GetYaxis().SetTitle(hmmpYtitle)

    return h


def h2costphis(ibinW, ibinQ2, iedges, vals=None):
    h6_e.GetAxis(idims.w).SetRange(ibinW, ibinW)
    h6_e.GetAxis(idims.q2).SetRange(ibinQ2, ibinQ2)
    h6_e.GetAxis(idims.mmp).SetRange(iedges[0], iedges[1])
    hexpsb = h6_e.Projection(idims.ct, idims.phi, 'e')

    if vals is None:
        vals = ibins2vals(ibinW, ibinQ2)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    hexpsb.SetName('h2costphi_sb_%d_%d' % (round(1000*wval), round(1000*q2val)))
    hexpsb.SetTitle(h2costphiTitle % ('sideband', wlo, whi, q2lo, q2hi))
    hexpsb.GetXaxis().SetTitle(h2costphiXtitle)
    hexpsb.GetYaxis().SetTitle(h2costphiYtitle)

    h6_e.GetAxis(idims.mmp).SetRange(iedges[2], iedges[3])
    # "potential memory leak" warning will be issued here, but it's not
    hexpsb2 = h6_e.Projection(idims.ct, idims.phi, 'e')
    hexpsb2.SetName('h2costphi_sb2_%d_%d' % (round(1000*wval), round(1000*q2val)))
    hexpsb.Add(hexpsb2)

    h6_e.GetAxis(idims.mmp).SetRange(iedges[1], iedges[2])
    hexpsig = h6_e.Projection(idims.ct, idims.phi, 'e')
    hexpsig.SetName('h2costphi_sig_%d_%d' % (round(1000*wval), round(1000*q2val)))
    hexpsig.SetTitle(h2costphiTitle % ('signal', wlo, whi, q2lo, q2hi))
    hexpsig.GetXaxis().SetTitle(h2costphiXtitle)
    hexpsig.GetYaxis().SetTitle(h2costphiYtitle)

    h2sims = []
    h2simsnames = ['thrown', 'acceptance']
    for i, h4 in enumerate([h4_t, h4_a]):
        h4.GetAxis(idims.w).SetRange(ibinW, ibinW)
        h4.GetAxis(idims.q2).SetRange(ibinQ2, ibinQ2)
        h2 = h4.Projection(2, 3, 'e')
        h2sims.append(h2)

        h4.GetAxis(idims.w).SetRange(0, -1)
        h4.GetAxis(idims.q2).SetRange(0, -1)

        h2.SetName('h2costphi_%s_%d_%d' % (h2simsnames[i], round(1000*wval), round(1000*q2val)))
        h2.SetTitle(h2costphiTitle % (h2simsnames[i], wlo, whi, q2lo, q2hi))
        h2.GetXaxis().SetTitle(h2costphiXtitle)
        h2.GetYaxis().SetTitle(h2costphiYtitle)

    h6_e.GetAxis(idims.w).SetRange(0, -1)
    h6_e.GetAxis(idims.q2).SetRange(0, -1)
    h6_e.GetAxis(idims.mmp).SetRange(0, -1)

    return (hexpsig, hexpsb, h2sims[0], h2sims[1])


def fitbin(wbin, q2bin):
    # get bin center and edges
    vals = ibins2vals(wbin, q2bin)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    record = {'W': wval, 'Wlo': wlo, 'Whi': whi, 'Wbin': wbin,
              'Q2': q2val, 'Q2lo': q2lo, 'Q2hi': q2hi, 'Q2bin': q2bin}

    # get MMp mass distribution for current W/Q2 bin
    h = asrootpy(hmmp(wbin, q2bin, vals))

    # fit the background
    fbg = fitbg(h)
    record['fbgstat'] = r.gMinuit.fCstatu
    for ipar in range(0, fbg.GetNpar()):
        record['fbgpar%d' % ipar] = fbg.GetParameter(ipar)

    # subtract background fit function
    hsig = asrootpy(h.Clone('%s_hsig' % h.GetName()))
    hsig.Add(fbg, -1)

    # fit signal
    fsig = fitsig(hsig)
    record['fsigstat'] = r.gMinuit.fCstatu
    for ipar in range(0, fsig.GetNpar()):
        record['fsigpar%d' % ipar] = fsig.GetParameter(ipar)

    # get signal boundaries and integral correction factor
    gm = fsig.GetParameter(1)
    gs = fsig.GetParameter(2)
    mmplo, mmphi = gm-cutsigma*gs, gm+cutsigma*gs
    mmpbinlo, mmpbinhi = h.FindBin(mmplo), h.FindBin(mmphi)
    mmplo, mmphi = h.GetBinLowEdge(mmpbinlo), h.GetBinLowEdge(mmpbinhi+1)
    intfull = fsig.Integral(*fitrange)
    int3s = fsig.Integral(mmplo, mmphi)
    hsigint = hsig.Integral(mmpbinlo, mmpbinhi)
    weight3s = int3s/intfull if intfull > 0 else -1
    record['hsigint3s'] = hsigint
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
    cmmp = Canvas(name='bgsigfit_%d_%d' % (round(1000*wval), round(1000*q2val)),
                  title='MMp Fits, W, Q2 = %.3f GeV,%.3f GeV2' % (wval, q2val))
    hs = r.THStack('hs_%d_%d' % (round(1000*wval), round(1000*q2val)),
                   hstitle % (wlo, whi, q2lo, q2hi))
    hsig.SetColor('blue')
    hsig.GetListOfFunctions().FindObject('fsig').SetLineColor(r.kBlue)
    hs.Add(hsig)
    hs.Add(h)
    hs.Draw('nostack')
    r.gPad.Update()
    hs.GetHistogram().GetXaxis().SetTitle(h.GetXaxis().GetTitle())
    hs.GetHistogram().GetYaxis().SetTitle('yield')
    chi2sig, ndfsig = fsig.GetChisquare(), fsig.GetNDF()
    rchi2sig = chi2sig/ndfsig if ndfsig > 0 else -1
    record['fsigchi2'] = rchi2sig
    chi2bg, ndfbg = fbg.GetChisquare(), fbg.GetNDF()
    rchi2bg = chi2bg/ndfbg if ndfbg > 0 else -1
    record['fbgchi2'] = rchi2bg
    ylo, yhi = r.gPad.GetUymin(), r.gPad.GetUymax()
    xlo, xhi = r.gPad.GetUxmin(), r.gPad.GetUxmax()
    for l in [r.TLine(mmplo, ylo, mmplo, yhi), r.TLine(mmphi, ylo, mmphi, yhi)]:
        hsig.GetListOfFunctions().Add(l)
        l.SetLineColor(r.kBlue)
    yscale = yhi-ylo
    xscale = xhi-xlo
    tchi2 = r.TLatex(xlo+tchi2posX*xscale, ylo+tchi2posY*yscale, tchi2str % (chi2sig, ndfsig, rchi2sig))
    tchi2.Draw()
    r.gPad.Update()
    cmmp.SaveAs('out/%s.pdf' % cmmp.GetName())

    # output
    for (k, v) in record.items():
        if isinstance(v, float):
            record[k] = '%.5f' % v if k.startswith('fsig') else '%.3f' % v
        if isinstance(v, basestring):
            record[k] = '\"%s\"' % v
    for i, k in enumerate(keyorder):
        if i > 0:
            foutrecs.write(',')
        foutrecs.write(str(record[k]))

    wait()
    return (iedges, vedges, [weightsb, weight3s])


def applythresholds(h4a, thresh=0):
    haccdist0s = Hist(100, 0, 0.1)
    herrdist0s = Hist(100, 0, 1)
    for ibin in range(0, h4a.GetNbins()):
        bc = h4a.GetBinContent(ibin)
        haccdist0s.Fill(bc)
        if bc <= thresh:
            h4a.SetBinContent(ibin, 0)
            h4a.SetBinError(ibin, 0)
        else:
            herrdist0s.Fill(h4_a.GetBinError(ibin)/bc)
    haccdist0s.GetListOfFunctions().Add(r.TLine(thresh, 0, thresh, 1.05*haccdist0s.GetMaximum()))
    return (haccdist0s, herrdist0s)


def applyacc(wbin, q2bin, iedges, vedges, weights):
    # TODO: clone 2d hists to keep snapshots between steps of acceptance
    # corrections and zero-filling
    vals = ibins2vals(wbin, q2bin)
    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])
    h2s = (hexpsig, hexpsb, h2thr, h2acc) = h2costphis(wbin, q2bin, iedges)
    h2rec_acor = h2thr.Clone('h2r_acor')
    h2rec_acor.Multiply(h2acc)
    inth2r = h2rec_acor.Integral()  # integral of reconstructed simulated events
    h2rec_acor.Divide(h2acc)        # thrown events with acceptance holes
    h2thr.Add(h2rec_acor, -1)       # thrown events IN acceptance holes
    [h2thr.SetBinError(i, j, sqrt(h2thr.GetBinContent(i, j))) for i in range(1, h2thr.GetNbinsX()) for j in range(1, h2thr.GetNbinsY())]
    hexpsb.Scale(weights[0])  # weightsb
    hexpsig.Add(hexpsb, -1)
    inth2e = hexpsig.Integral()/weights[1]  # integral of signal region with 3-sigma cut correction
    hexpsig.Divide(h2acc)
    h2thr.Scale(inth2e/inth2r)
    hexpsig.Add(h2thr)

    cmmp = Canvas(name='hcostphi_%d_%d' % (round(1000*wval), round(1000*q2val)),
                  title='Angular Distributions, W, Q2 = %.3f GeV,%.3f GeV2' % (wval, q2val))
    cmmp.Divide(2, 2)
    for ipad, h2 in enumerate(h2s):
        cmmp.cd(ipad+1)
        r.gPad.Update()
        h2.Draw('colz')

    wait()
    return hexpsig


def testme(wbin=15, q2bin=5):
    sigsbparms = fitbin(wbin, q2bin)
    if sigsbparms:
        (haccdists, herrdists) = applythresholds(h4_a, 0.01)
        haccdists.Draw()
        wait()
        # virtual photon flux factors and cc-cut efficiency factors
        # already applied in h6maker.h!
        h2e = applyacc(wbin, q2bin, *sigsbparms)

        vals = ibins2vals(wbin, q2bin)
        (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
        (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])

        d2wq2 = (whi-wlo)*(q2hi-q2lo)
        branch = 0.891
        lum = 19.844*(1e6)
        h2e.Scale(1/(d2wq2*branch*lum))
        h2e.Scale(1/vgflux(wval, q2val))
        hcost = h2e.ProjectionY(str(h2e.GetName().replace('h2costphi', 'hcost')), 0, -1, 'e')
        hcost.Scale(1, 'width')
        h2e.Scale(1, 'width')
        hcost.SetDirectory(fout)
        errh2e = r.Double(0)
        inth2e = h2e.IntegralAndError(1, h2e.GetNbinsX(), 1, h2e.GetNbinsY(), errh2e, 'width')
        foutrecs.write(',%.0f,%.0f,%s\n' % (inth2e, errh2e, hcost.GetName()))
    foutrecs.close()
    fout.Write()
    fout.Close()


if __name__ == "__main__":
    r.gROOT.SetBatch(r.kTRUE)
    hq2w_e = h6_e.Projection(idims.q2, idims.w, 'e')
    hq2w_t = h4_t.Projection(1, 0, 'e')
    (haccdists, herrdists) = applythresholds(h4_a, 0.01)
    wstart, q2start = 1, 1  # vals2ibins(1.8, 1.7)
    for wbin in range(wstart, h6_e.GetAxis(idims.w).GetNbins()+1):
        for q2bin in range(q2start, h6_e.GetAxis(idims.q2).GetNbins()+1):
            if hq2w_e.GetBinContent(wbin, q2bin) > 100 and hq2w_t.GetBinContent(wbin, q2bin) > 100:
                sigsbparms = fitbin(wbin, q2bin)
                if sigsbparms:
                    (haccdists, herrdists) = applythresholds(h4_a, 0.01)
                    haccdists.Draw()
                    wait()
                    # cc-cut efficiency factors already applied in h6maker.h!
                    h2e = applyacc(wbin, q2bin, *sigsbparms)

                    vals = ibins2vals(wbin, q2bin)
                    (wval, wlo, whi) = (vals[0][0], vals[0][1], vals[0][2])
                    (q2val, q2lo, q2hi) = (vals[1][0], vals[1][1], vals[1][2])

                    d2wq2 = (whi-wlo)*(q2hi-q2lo)
                    branch = 0.891
                    lum = 19.844*(1e6)
                    h2e.Scale(1/(d2wq2*branch*lum))
                    h2e.Scale(1/vgflux(wval, q2val))
                    hcost = h2e.ProjectionY(str(h2e.GetName().replace('h2costphi', 'hcost')), 0, -1, 'e')
                    hcost.Scale(1, 'width')
                    h2e.Scale(1, 'width')
                    hcost.SetDirectory(fout)
                    errh2e = r.Double(0)
                    inth2e = h2e.IntegralAndError(1, h2e.GetNbinsX(), 1, h2e.GetNbinsY(), errh2e, 'width')
                    # foutrecs.write(',%.0f,%.0f\n' % (inth2e, errh2e))
                    foutrecs.write(',%.0f,%.0f,%s\n' % (inth2e, errh2e, hcost.GetName()))
    foutrecs.close()
    fout.Write()
    fout.Close()
