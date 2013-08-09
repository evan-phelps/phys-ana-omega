%run ../scratch_intQ2.py
from rootpy.plotting import Hist, HistStack
import pandas as pd
ress = pd.DataFrame(doeach())
dffits = pd.concat([df.W, df.Q2]+[ress[i] for i in [0,1,2,4,5]], axis=1)
for i in range(84,91): print(dffits.W[i],dffits.Q2[i],[dffits[0][i].GetParameter(j) for j in range(0,4)])
dffits.columns = ['W', 'Q2', 'fitfunc', 'flegs', 'ft', 'h', 'status']
dfQ2s = [dffits[['W','Q2','fitfunc','status']][dffits.Q2==Q2] for Q2 in dffits.Q2.unique()]
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
