import matplotlib.pyplot as plt
import numpy as np
from numpy import array
import ROOT as r
from ROOT import TFile, THnSparseF, gSystem, gROOT, TChain, TEntryList
from rootpy.io import root_open as ropen, DoesNotExist
from rootpy.plotting import Hist, Hist2D
from rootpy import asrootpy
import rootpy.plotting.root2matplotlib as rplt

from epfuncs import f_bwexpgaus
from epfuncs import f_bwexpgaus_pol4
from epfuncs import RejectWrapper
from epxsectutils import vgflux

dir_skim = '/hdd500/home/e16e1fjobssimtof/e1f/skim'
#dir_src = '/home/ephelps/dropbox/barracuda_copy-gmail/phys-ana-omega'
dir_src = '/home/ephelps/projects/phys-ana-omega'

gSystem.AddIncludePath("-I%s"%dir_src)
gROOT.ProcessLine(".include %s"%dir_src)

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
    c.Add("%s/skims.root"%dir_skim) #3????.root")
run.Add("%s/3xxxx_run.root"%dir_skim)
runb.Add("%s/3xxxx_run.root"%dir_skim)
tkin.Add("%s/friend-tkin.root"%dir_skim)

run.AddFriend(lum, "l")
runb.AddFriend(lb, "l")

tpipf.AddFriend(tpilf, "lf")
h10.AddFriend(tpipf, "pf")
h10.AddFriend(run, "r")
h10.AddFriend(tkin, "k")

felist = TFile("%s/elists.root"%dir_skim)
elf = felist.Get("fid_cc")
el1 = felist.Get("el1")
el2 = felist.Get("el2")
el3 = felist.Get("el3")
el1_himm = felist.Get("el1_himm")

print("%s/3????.root"%dir_skim)
print("%s/3xxxx_run.root"%dir_skim)
print("run.lum(=l)")
print("runb.lb(=l)")
print("tpipf.tpilf(=lf)")
print("h10.tpipf(=pf)")
print("h10.run(=r)")
print("h10.tkin(=k)")
print("elist_fid_mm_[123]=el[123]")

gROOT.ProcessLine(".L %s/acc.cpp+"%dir_src)
print("loaded acc.cpp")
gROOT.ProcessLine(".L %s/fid.cpp+"%dir_src)
print("loaded fid.cpp")
gROOT.ProcessLine(".L %s/particle-constants.h"%dir_src)
print("loaded particle-constants.h")
gROOT.ProcessLine('Fid::Instance("%s/input/fid.parms")'%dir_src)
print("loaded Fid instance")
gROOT.ProcessLine(".L  %s/scripts/infid.C"%dir_src)
print("loaded infid.C")
gROOT.ProcessLine(".L %s/scripts/eff.C"%dir_src)
print("loaded eff.cpp")
print("recompiling h10t3pi_sel.C...")
gROOT.ProcessLine(".L  %s/scripts/h10t3pi_sel.C+"%dir_src)
print("... done.")
gROOT.ProcessLine(".L  %s/input/cc_eff_lazy_programmer.h"%dir_src)

h10.SetAlias("fidpass", "(infid(k.e.p,k.e.theta,k.e.phi) && (h10idx_p<0 || infid(k.p.p,k.p.theta,k.p.phi,2212)) && (h10idx_pip<0 || infid(k.pip.p,k.pip.theta,k.pip.phi,211)) && (h10idx_pim<0 || infid(k.pim.p,k.pim.theta,k.pim.phi,-211)))")
h10.SetAlias("fidpasse", "(h10idx_e==0 && infid(k.e.p,k.e.theta,k.e.phi))")
h10.SetAlias("fidpassp", "(h10idx_p>0 && infid(k.p.p,k.p.theta,k.p.phi,2212))")
h10.SetAlias("fidpasspip", "(h10idx_pip>0 && infid(k.pip.p,k.pip.theta,k.pip.phi,211))")
h10.SetAlias("fidpasspim", "(h10idx_pim>0 && infid(k.pim.p,k.pim.theta,k.pim.phi,-211))")
h10.SetAlias("top1pass", "(h10idx_pip>0 && h10idx_pim<0 && h10idx_e==0 && h10idx_p>0)")
h10.SetAlias("top2pass", "(h10idx_pip<0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)")
h10.SetAlias("top3pass", "(h10idx_pip>0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)")
h10.SetAlias("mmthreshpass", "(h10idx_p>0 && h10idx_e==0 && ((h10idx_pip<0 || mmppip.M()>0.275) && (h10idx_pim<0 || mmppim.M()>0.275)) && omega.M()>0.415)")
h10.SetAlias("mmpi0pass", "(h10idx_p>0 && h10idx_e==0 && h10idx_pip>0 && h10idx_pim>0 && pi0.M()>0.049 && pi0.M()<0.183)")
h10.SetAlias("ccpass", "(cc[h10idx_e]>0 && nphe[cc[h10idx_e]-1]>25)")
h10.SetAlias("ccsect", "cc_sect[cc[h10idx_e]-1]")
h10.SetAlias("ccseg", "(cc_segm[cc[h10idx_e]-1]%1000)/10")
h10.SetAlias("cchit", "(cc_segm[cc[h10idx_e]-1]/1000 - 1)")

print("Fid::Instance() loaded\t from %s/fid.cpp\t with parameters from %s/input/fid.parms"%(dir_src,dir_src))
print("infid() loaded\t from %s/infid.C"%dir_src)
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


def rh1(drawstr, cutstr='', hn='htemp', N=None, binning=None):
    drawstr = '%s>>%s' % (drawstr, hn)
    if binning is not None:
        drawstr += str(binning)
    h10.Draw(drawstr, cutstr, 'goff', N if N is not None else h10.GetEntriesFast())
    return asrootpy(gROOT.FindObject(hn))


def cuts(*cs):
    return ' && '.join(['%s>%f && %s<%f' % (c[0], c[1], c[0], c[2]) for c in cs])
