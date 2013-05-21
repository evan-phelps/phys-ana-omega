import ROOT as r
from epfuncs import d_gausexp
from epfuncs import f_gausexp

f = r.TFile('/data/ephelps.bak/analysis/sandbox/h3maker-hn.root')
hs0 = f.Get('hs0')
h = hs0.GetHists()[10]

fbg = r.TF1('fbg', 'pol2', 0.4, 1.2)
h.Fit(fbg, '', '', 0.56, 0.74)
h.GetListOfFunctions()[0].SetRange(0.4, 1.2)
hsig = h.Clone('hsig')
hsig.Add(h.GetListOfFunctions()[0], -1)
fs = f_gausexp()
hsig.Fit(fs, '', '', 0.7, 0.9)

# define signal+background function
def d_fsigbg(v, par):
    p0, p1, p2, p3, p4, p5, p6 = par[0], par[1], par[2], par[3], par[4], par[5], par[6]
    parsig = [p0, p1, p2, p3]
    parbg = [p4, p5, p6]
    return d_gausexp(v, parsig)+p4+p5*v[0]+p6*v[0]**2

fsigbg = r.TF1('fsigbg', d_fsigbg, 0.4, 1.2, 7)
fsigbg.lims = {}
fsigbg.lims.update(fs.lims)

# get start parameters for signal+background function
par = [fs.GetParameter(i) for i in range(0, 4)]
for i in range(0, 3): par.append(fbg.GetParameter(i))
for i in range(0, 7):
    fsigbg.SetParameter(i, par[i])
    parlo, parhi = 0.75*par[i], 1.5*par[i]
    if par[i] < 0: parlo, parhi = parhi, parlo
    # if i in fsigbg.lims:
    #     parlo = max(parlo, fsigbg.lims[i][0])
    #     parhi = min(parhi, fsigbg.lims[i][1])
    fsigbg.SetParLimits(i, parlo, parhi)

h.Fit(fsigbg, '', '', 0.7, 1)

for i in range(0, 4): fs.SetParameter(i, fsigbg.GetParameter(i))
for i in range(4, 7): fbg.SetParameter(i, fsigbg.GetParameter(i))

fs.Draw('same')
fbg.Draw('same')
hsig = h.Clone('hsig')
hsig.Add(fbg, -1)
hsig.GetListOfFunctions().Clear()
hsig.SetMarkerColor(r.kBlue)
hsig.Draw('same')
