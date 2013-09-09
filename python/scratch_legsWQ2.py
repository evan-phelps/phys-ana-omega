%run ../scratch_intQ2.py
# %run /home/ephelps/lhcbstyle.py
import ROOT as r
r.gROOT.ProcessLine('.x /home/ephelps/Dropbox/Notebooks/omega/Miscellany/Linux_OS/styPUB.C')
r.gROOT.SetStyle("PUB")
# import matplotlib as mplt
# mplt.use('TkAgg')
import operator as op
from rootpy.plotting import Hist, HistStack, Canvas
import pandas as pd
ress = pd.DataFrame(doeach(trim=False))
dffits = pd.concat([df.W, df.Q2]+[ress[i] for i in [0,1,2,4,5]], axis=1)
for i in range(84,91): print(dffits.W[i],dffits.Q2[i],[dffits[0][i].GetParameter(j) for j in range(0,4)])
dffits.columns = ['W', 'Q2', 'fitfunc', 'flegs', 'ft', 'h', 'status']
dfQ2s = [dffits[['W','Q2','fitfunc','status']][dffits.Q2==Q2] for Q2 in dffits.Q2.unique()]
# each q2 in q2s contains dffit records for W = 1.79, 1.91, 2.03, 2.15, 2.27
# wselect = [0,6,12,18,24]
wselect = [0,12,24]
q2s = [dffits[(dffits.Q2==q2) & (dffits.W>1.78) & (dffits.W<2.28)].iloc[wselect] for q2 in dffits.Q2.unique()]

for q2_xxxx in op.itemgetter(1,3,5)(q2s):
    for w, h in zip(q2_xxxx.W, q2_xxxx.h):
        h.SetMinimum(-0.1*h.GetMaximum())
        h.GetYaxis().SetTitle('#frac{d#sigma}{d(cos#theta*)} (nb)')
        h.GetXaxis().SetTitle('cos #theta*')
        h.GetXaxis().SetTitleSize(0.05)
        h.GetYaxis().SetTitleSize(0.05)
        h.SetTitle('')
        h.GetListOfFunctions()[0].SetLineColor(r.kBlack)
        h.GetListOfFunctions()[1].SetLineColor(r.kBlue)
        h.GetListOfFunctions()[2].SetLineColor(r.kRed+1)
        h.GetListOfFunctions()[2].SetLineWidth(1)
        h.GetListOfFunctions()[2].SetLineStyle(1)
        h.GetListOfFunctions()[2].SetFillStyle(3008)
        h.GetListOfFunctions()[2].SetFillColor(r.kRed+1)

        h.Draw()
        h.GetListOfFunctions()[2].DrawCopy('FCsame')
        h.GetListOfFunctions()[1].DrawCopy('same')
        h.GetListOfFunctions()[0].DrawCopy('same')

        r.gPad.Modified()
        r.gPad.Update()
        r.gPad.SaveAs('%s.pdf'%h.GetName())



def drawsamples(c=None):
    c = r.TCanvas('c1','c1',2100,1500) if c is None else c
    c.Divide(3,3)
    pnum = 1
    hs = []
    for q2_xxxx in op.itemgetter(1,3,5)(q2s):  # 0:6]:
        for w, h in zip(q2_xxxx.W, q2_xxxx.h):
            c.cd(pnum)
            pnum += 1
            h.SetMinimum(-0.1*h.GetMaximum())
            h.GetYaxis().SetTitle('#frac{d#sigma}{d(cos#theta*)} (nb)')
            h.GetXaxis().SetTitle('cos #theta*')
            h.GetXaxis().SetTitleSize(0.05)
            h.GetYaxis().SetTitleSize(0.05)
            h.SetTitle('')
            h.GetListOfFunctions()[0].SetLineColor(r.kBlack)
            h.GetListOfFunctions()[1].SetLineColor(r.kBlue)
            h.GetListOfFunctions()[2].SetLineColor(r.kRed+1)
            h.GetListOfFunctions()[2].SetLineWidth(1)
            h.GetListOfFunctions()[2].SetLineStyle(1)
            h.GetListOfFunctions()[2].SetFillStyle(3008)
            h.GetListOfFunctions()[2].SetFillColor(r.kRed+1)
            h.Draw()
            h.GetListOfFunctions()[2].DrawCopy('FCsame')
            h.GetListOfFunctions()[1].DrawCopy('same')
            h.GetListOfFunctions()[0].DrawCopy('same')
            hs.append(h)
    return (c, hs)

(c, hs) = drawsamples(r.TCanvas('c1','c1',2100,1500))


q2_1700 = dffits[(dffits.Q2==1.7) & (dffits.W>1.78) & (dffits.W<2.28)]
q2_2580 = dffits[(dffits.Q2==2.58) & (dffits.W>1.78) & (dffits.W<2.28)]
q2_3030 = dffits[(dffits.Q2==3.03) & (dffits.W>1.78) & (dffits.W<2.28)]

c1 = r.TCanvas()
c1.Modified()
c1.Update()
hcoss_1700 = HistStack('costheta_hists_1700')
for w, h in zip(q2_1700.W, q2_1700.h):
    h.SetTitle('W = %.3f' % w)
    h.SetMinimum(-100)
    hcoss_1700.Add(h)
hcoss_1700.Draw('pads')

c2 = r.TCanvas()
c2.Modified()
c2.Update()
hcoss_3030 = HistStack('costheta_hists_3030')
for w, h in zip(q2_3030.W, q2_3030.h):
    h.SetTitle('W = %.3f' % w)
    h.SetMinimum(-100)
    hcoss_3030.Add(h)
hcoss_3030.Draw('pads')


c3 = r.TCanvas()
c3.Modified()
c3.Update()
hcoss_2580 = HistStack('costheta_hists_2580')
for w, h in zip(q2_2580.W, q2_2580.h):
    h.SetTitle('W = %.3f' % w)
    h.SetMinimum(-100)
    hcoss_2580.Add(h)
hcoss_2580.Draw('pads')

# hists = [h0, h1, h2, htA, htM, hchi2] = [Hist(60,1.7,2.9) for i in range(0,6)]
# hrs = [h0r, h1r, h2r] = [Hist(60,1.7,2.9) for i in range(0,3)]
# ress = doall()
# for wbin, (f, status) in enumerate([(res[0], res[5]) for res in ress],1):
#     hchi2.SetBinContent(wbin, f.GetChisquare())
#     for i, h in enumerate([h0,h1,h2,htA,htM]):
#         val, err = f.GetParameter(i), f.GetParError(i)
#         if 'CONV' in status and f.GetChisquare() < 20 and err < 500:
#             h.SetBinContent(wbin,val)
#             h.SetBinError(wbin,err)
#     norm = sum([f.GetParameter(i) for i in range(0,3)])
#     for i, h in enumerate(hrs):
#         val, err = f.GetParameter(i), f.GetParError(i)
#         if 'CONV' in status and f.GetChisquare() < 20 and err < 500:
#             h.SetBinContent(wbin,val/norm)
#             h.SetBinError(wbin,err/norm)

# hstack = HistStack('leghists')
# for h in hists[0:3]+hrs:  # +hists[3:]:
#     h.SetOption('pE')
#     hstack.Add(h)
# hstack.Draw('pads')
# for h in hists[0:3]:
#     h.GetYaxis().SetRangeUser(-100,650)
#     h.GetXaxis().SetRangeUser(1.7,2.5)
# for h in hrs:
#     h.GetYaxis().SetRangeUser(0,1)
#     h.GetXaxis().SetRangeUser(1.7,2.5)
