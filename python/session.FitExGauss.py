import ROOT as r
# from epfuncs import d_pol2
from epfuncs import d_gausexp
from epfuncs import f_gausexp
from epfuncs import RejectWrapper
from epxsectutils import vgflux
import re


def wq2fromtitle(hist):
    RE_FLOAT = '[-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?'
    re_string = '(?<=W = \()'+RE_FLOAT
    wlow = float(re.search(re_string, hist.GetTitle()).group(0))
    re_string = '(?<=,)'+RE_FLOAT+'(?<=\))?'
    whigh = float(re.search(re_string, hist.GetTitle()).group(0))
    re_string = '(?<=Q\^2 = \()'+RE_FLOAT
    q2low = float(re.search(re_string, hist.GetTitle()).group(0))
    re_string = '(?<=,)'+RE_FLOAT+'(?<=\))?'
    q2high = float(re.findall(re_string, hist.GetTitle())[1][0])
    return ((wlow+whigh)/2, (q2low+q2high)/2, whigh-wlow, q2high-q2low)


f = r.TFile('~/projects/phys-ana-omega/h3maker-hn.root')

hs0 = f.Get('hs1')
h = hs0.GetHists()[10]
(w, q2, dw, dq2) = wq2fromtitle(h)
hacc = f.Get('hq2w_eff_acc')
heff = f.Get('hq2w_eff_cc')
gidx = hacc.FindBin(w, q2)

acc = hacc.GetBinContent(gidx)
eff = heff.GetBinContent(gidx)
flx = vgflux(w, q2)
brf = 0.891
lum = 19.844
dmmp = 0.01

etarange = (0.5, 0.6)
omegarange = (0.7, 0.9)
fbgskip = [etarange, omegarange]
bgwrapper = RejectWrapper(r.TF1('fbg', 'pol2', 0.4, 1.2), fbgskip)
fbgrej = bgwrapper.newtf1
fbg = bgwrapper.tf1
# fbgw = RejectWrapper(r.TF1('fbg', 'pol2', 0.4, 1.2), fbgskip)
# fbg = r.TF1('fbg', 'pol2', 0.4, 1.2)
# fbg = r.TF1('fbg', d_pol2, 0.4, 1.2, 3)
# fbg.rejranges = fbgskip
# h.Fit(fbg, '', '', 0.56, 0.74)
h.Fit(fbgrej, 'N0', '', 0.4, 1.05)
h.GetListOfFunctions().Add(fbg.Clone('fbg'))
h.GetListOfFunctions()[0].SetRange(0.4, 1.2)
hsig = h.Clone('hsig')
hsig.Add(h.GetListOfFunctions()[0], -1)
fs = f_gausexp()
hsig.Fit(fs, '', '', *omegarange)


# define signal+background function
def d_fsigbg(v, par):
    p0, p1, p2, p3, p4, p5, p6 = par[0], par[1], par[2], par[3], par[4], par[5], par[6]
    parsig = [p0, p1, p2, p3]
    return d_gausexp(v, parsig)+p4+p5*v[0]+p6*v[0]**2

fsigbg = RejectWrapper(r.TF1('fsigbg', d_fsigbg, 0.4, 1.2, 7), [etarange]).newtf1
# fsigbg = r.TF1('fsigbg', d_fsigbg, 0.4, 1.2, 7)
# fsigbg.rejranges = [etarange]
fsigbg.lims = {}
fsigbg.lims.update(fs.lims)

# get start parameters for signal+background function
par = [fs.GetParameter(i) for i in range(0, 4)]
for i in range(0, 3):
    par.append(fbgrej.GetParameter(i))
for i in range(0, 7):
    fsigbg.SetParameter(i, par[i])
    low8, hiw8 = 0.5, 1.5
    if par[i] < 0:
        low8, hiw8 = hiw8, low8
    parlo, parhi = low8*par[i], hiw8*par[i]
    if i in fsigbg.lims:
        parlo = max(parlo, fsigbg.lims[i][0])
        parhi = min(parhi, fsigbg.lims[i][1])
    fsigbg.SetParLimits(i, parlo, parhi)
    fsigbg.lims[i] = (parlo, parhi)

h.SetMinimum(0)
h.Fit(fsigbg, '', '', 0.4, 1.05)

for i in range(0, 4):
    fs.SetParameter(i, fsigbg.GetParameter(i))
for i in range(4, 7):
    fbg.SetParameter(i-4, fsigbg.GetParameter(i))

fs.Draw('same')
fbg.Draw('same')
hsig = h.Clone('hsig')
hsig.Add(fbg, -1)
hsig.GetListOfFunctions().Clear()
hsig.SetMarkerColor(r.kBlue)
hsig.Draw('same')

fnames = ['branching fraction', 'luminosity', 'virtual photon flux',
          'acceptance', 'cc efficiency', 'W bin width', 'Q2 bin width',
          'MMp bin width']
factors = [brf, lum, flx, acc, eff, dw, dq2, dmmp]
for (fname, factor) in zip(fnames, factors):
    print(fname, factor)
