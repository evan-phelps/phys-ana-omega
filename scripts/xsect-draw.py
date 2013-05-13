import ROOT as R
from ROOT import TCanvas
from ROOT import TTree
from ROOT import TGraphErrors
from ROOT import TMultiGraph
import numpy

goodcolors = [R.kRed+1,
              R.kGreen+1,
              R.kBlue,
              R.kYellow+1,
              R.kMagenta+1,
              R.kCyan+1,
              9, 46]
goodmarkers = list(range(20, 27))
goodmarkers.append(32)

t = TTree()
t.ReadFile('xsects.txt', '', '\t')
gs = {}
q2s = [{}, {}, {}, {}, {}, {}, {}, {}]
gnames = ['fn pol2', 'fn pol4', 'fn pol2 -5%', 'fn pol2 +5%',
          'bc pol2', 'bc pol4', 'bc pol2 -5%', 'bc pol2 +5%']
for r in t:
    xss = (r.xs1, r.xs2, r.xs3, r.xs4, r.xs1b, r.xs2b, r.xs3b, r.xs4b)
    ers = (r.e1, r.e2, r.e3, r.e4, r.e1b, r.e2b, r.e3b, r.e4b)
    for i, (xs, er) in enumerate(zip(xss, ers)):
        if r.Q2 not in q2s[i]:
            q2s[i][r.Q2] = [[], [], [], []]
        q2s[i][r.Q2][0].append(r.W)
        q2s[i][r.Q2][1].append(xs)
        q2s[i][r.Q2][2].append(0)
        q2s[i][r.Q2][3].append(er)

for i in range(0, 5):
    for k, v in q2s[i].items():
        if k not in gs:
            gs[k] = TMultiGraph()
        ge = TGraphErrors(len(v[0]), numpy.asarray(v[0]), numpy.asarray(v[1]),
                          numpy.asarray(v[2]), numpy.asarray(v[3]))
        ge.SetMarkerColor(goodcolors[i % 4])
        ge.SetLineColor(goodcolors[i % 4])
        ge.SetMarkerStyle(goodmarkers[i])
        ge.SetName(gnames[i])
        ge.SetTitle(gnames[i])
        gs[k].Add(ge)
        gs[k].SetName('Q^{2} = %.3f GeV' % k)
        if i == 4:
            ge.SetMarkerStyle(29)
            ge.SetMarkerSize(2)
        gs[k].SetMinimum(0)

cvss = []
for k, mg in gs.items():
    c = TCanvas()
    cvss.append(c)
    mg.Draw('ap')
    c.Modified()
    c.Update()
    mg.GetHistogram().SetTitle(mg.GetName())
    mg.GetHistogram().GetXaxis().SetTitle('W (GeV)')
    mg.GetHistogram().GetYaxis().SetTitle('#sigma_{h} (nb)')
    mg.GetHistogram().GetXaxis().SetTitleSize(0.0375)
    mg.GetHistogram().GetYaxis().SetTitleSize(0.0375)
    mg.GetHistogram().GetXaxis().SetTitleOffset(1.2)
    mg.GetHistogram().GetYaxis().SetTitleOffset(1.1)
    R.gPad.BuildLegend(0.8, 0.6, 0.99, 0.92)
    c.Modified()
    c.Update()
