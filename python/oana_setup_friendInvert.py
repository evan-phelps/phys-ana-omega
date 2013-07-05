import ROOT as r
from ROOT import TFile, THnSparseF, gSystem, gROOT, TChain, TEntryList
from rootpy.io import root_open as ropen, DoesNotExist
from rootpy.plotting import Hist, Hist2D
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt
import matplotlib.pyplot as plt
import numpy as np
from epfuncs import f_bwexpgaus
from epfuncs import f_bwexpgaus_pol4
from epfuncs import RejectWrapper
from epxsectutils import vgflux

gSystem.AddIncludePath("-I/home/ephelps/projects/phys-ana-omega")
gROOT.ProcessLine(".include /home/ephelps/projects/phys-ana-omega")

h10 = TChain("h10clone/h10")
tpilf = TChain("3pi-tree/t3pi")
tpipf = TChain("3pi-tree/t3pi_physfrm")
lum = TChain("accuq/lumevents")
run = TChain("lumevent")
tkin = TChain("tkin")

lb = TChain("accuq/lumblocks")
runb = TChain("lumblock")

chains = [h10, tpilf, tpipf, lum, lb]
for c in chains:
    c.Add("/data/e1f/skim/3????.root")
run.Add("/data/e1f/skim/3xxxx_run.root")
runb.Add("/data/e1f/skim/3xxxx_run.root")
tkin.Add("/data/e1f/skim/friend-tkin.root")

run.AddFriend(lum, "l")
runb.AddFriend(lb, "l")

tpipf.AddFriend(tpilf, "lf")
tkin.AddFriend(tpipf, "pf")
tkin.AddFriend(run, "r")
tkin.AddFriend(h10, "h10")

print("/data/e1f/skim/3????.root")
print("/data/e1f/skim/3xxxx_run.root")
print("run.lum(=l)")
print("runb.lb(=l)")
print("tpipf.tpilf(=lf)")
print("tkin (top level)")
print("tkin.tpipf(=pf)")
print("tkin.run(=r)")

gROOT.ProcessLine(".L /home/ephelps/projects/phys-ana-omega/acc.cpp+")
print("loaded acc.cpp")
gROOT.ProcessLine(".L /home/ephelps/projects/phys-ana-omega/fid.cpp+")
print("loaded fid.cpp")
gROOT.ProcessLine(".L /home/ephelps/projects/phys-ana-omega/particle-constants.h")
print("loaded particle-constants.h")
gROOT.ProcessLine('Fid::Instance("/home/ephelps/projects/phys-ana-omega/input/fid.parms")')
print("loaded Fid instance")
gROOT.ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/scripts/infid.C")
print("loaded infid.C")
gROOT.ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/scripts/eff.C")
print("loaded eff.cpp")
print("recompiling h10t3pi_sel.C...")
gROOT.ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/h10t3pi_sel.C+")
print("... done.")
gROOT.ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/input/cc_eff_lazy_programmer.h")

tkin.SetAlias("fidpass", "(infid(e.p,e.theta,e.phi) && (h10idx_p<0 || infid(p.p,p.theta,p.phi,2212)) && (h10idx_pip<0 || infid(pip.p,pip.theta,pip.phi,211)) && (h10idx_pim<0 || infid(pim.p,pim.theta,pim.phi,-211)))")
tkin.SetAlias("fidpasse", "(h10idx_e==0 && infid(e.p,e.theta,e.phi))")
tkin.SetAlias("fidpassp", "(h10idx_p>0 && infid(p.p,p.theta,p.phi,2212))")
tkin.SetAlias("fidpasspip", "(h10idx_pip>0 && infid(pip.p,pip.theta,pip.phi,211))")
tkin.SetAlias("fidpasspim", "(h10idx_pim>0 && infid(pim.p,pim.theta,pim.phi,-211))")
tkin.SetAlias("top1pass", "(h10idx_pip>0 && h10idx_pim<0 && h10idx_e==0 && h10idx_p>0)")
tkin.SetAlias("top2pass", "(h10idx_pip<0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)")
tkin.SetAlias("top3pass", "(h10idx_pip>0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)")
tkin.SetAlias("mmthreshpass", "(h10idx_p>0 && h10idx_e==0 && ((h10idx_pip<0 || mmppip.M()>0.275) && (h10idx_pim<0 || mmppim.M()>0.275)) && omega.M()>0.415)")
tkin.SetAlias("mmpi0pass", "(h10idx_p>0 && h10idx_e==0 && h10idx_pip>0 && h10idx_pim>0 && pi0.M()>0.049 && pi0.M()<0.183)")
tkin.SetAlias("ccpass", "(cc[h10idx_e]>0 && nphe[cc[h10idx_e]-1]>25)")
tkin.SetAlias("ccsect", "cc_sect[cc[h10idx_e]-1]")
tkin.SetAlias("ccseg", "(cc_segm[cc[h10idx_e]-1]%1000)/10")
tkin.SetAlias("cchit", "(cc_segm[cc[h10idx_e]-1]/1000 - 1)")

print("Fid::Instance() loaded\t from /home/ephelps/projects/phys-ana-omega/fid.cpp\t with parameters from /home/ephelps/analysis/omega/input/fid.parms")
print("infid() loaded\t from /home/ephelps/projects/phys-ana-sandbox/sim/infid.C")
print("h10 aliases: c, mpip, bifpip, dtifpip, sf\tfidpass, fidpass[e,p,pip,pim]\ttop[1,2,3]pass, mmthreshpass, mmpi0pass\tccpass")


def draw(hist, ncols=1, nrows=1, cell=1, fig=None, figsize=(10, 5)):
    if fig is None:
        fig = plt.figure(figsize=figsize, dpi=100, facecolor='white')
    else:
        plt.figure(fig.number)
    subplot = fig.add_subplot(nrows, ncols, cell)
    subplot.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))
    if isinstance(hist, r.TH2):
        rplt.hist2d(hist, axes=subplot)
    else:
        rplt.errorbar([hist], xerr=False, emptybins=False)
    # plt.show()
    return fig


def rh1(drawstr, cutstr='', hn='htemp', N=None, binning=None, t=tkin):
    drawstr = '%s>>%s' % (drawstr, hn)
    if binning is not None:
        drawstr += str(binning)
    t.Draw(drawstr, cutstr, 'goff', N if N is not None else h10.GetEntriesFast())
    return asrootpy(gROOT.FindObject(hn))


def cuts(*cs):
    return ' && '.join(['%s>%f && %s<%f' % (c[0], c[1], c[0], c[2]) for c in cs])
