# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

%run ../python/oana_setup.py

# <markdowncell>

# # W, Q<sup>2</sup> bin sizing
# 
# * Must include electron fiducial cuts.
# * Check each channel: $p\pi^{+}\pi^{-}$, $p\pi^{-}$, $p\pi^{+}$

# <codecell>

tops = [el1, el2, el3]
hq2ws = [_,_,_]
for i,t in enumerate(tops):
    h10.SetEntryList(t)
    hq2ws[i] = rh1('Q2:W', '(ccpass*mmthreshpass)*(omega.M()>0.7 && omega.M()<0.866)', 'hq2w%d'%(i+1), binning='(170,1.5,3.2,600,0,6)')

# <codecell>

fq2ws = figure(figsize=(15,5), dpi=600)
# [draw(h, 3, 1, i, fig=fq2ws) for i, h in enumerate(hq2ws)]
for i,h in enumerate(hq2ws):
    print('top %d: %d'%(i+1,h.Integral()))
    draw(h, 3, 1, i+1, fig=fq2ws) #figure(figsize=(8,5), dpi=600))
    xlabel('$W\,(GeV)$', fontsize=14)
    ylabel('$Q^{2}\,(GeV^{2})$', fontsize=14)
    X = [3.00,2.64,1.68,1.68,2.25,3.00]
    Y = [0.85,2.89,5.50,1.35,1.18,0.85]
    plot(X,Y,'k--')
    X = [1.68,2.91]
    Y = [1.35,1.35]
    plot(X,Y,'k')
    X = [1.68,2.64]
    Y = [2.89,2.89]
    plot(X,Y,'k')
    x0, x1, dx = 1.68, 2.64, 0.02
    xedges = arange(x0,x1,dx)
    yedges = [1.35,1.5,1.75,2.1,2.89]
    [plot([x,x],[1.35,2.89],'k') for x in xedges]
    [plot([x0,x1],[y,y],'k') for y in yedges]
    xedges = arange(x0,2.281,dx)
    yedges = [4]
    [plot([x,x],[2.89,4],'k') for x in xedges]
    [plot([x0,2.28],[y,y],'k') for y in yedges]
    plot([1.721,1.721],[0,6],'r')

# x0, x1, dx = 1.68, 2.64, 0.02
# X = zip(arange(x0,x1,dx),arange(x0,x1,dx))
# y0, y1, dy = 1.35, 2.89, 0.3
# Y = zip(arange(y0,y1,dy),arange(y0,y1,dy))

# def yi(y0=1.35,y1=2.89,dy=0.3,m=1,ys=[]):
#     ys = ys + [y0]
#     if y0 >= y1: return ys
#     return yi(y0+dy,y1,dy*m,m,ys)

# # find best variable-bin multiplier
# m,d=zip(*[(m,max(yi(1.35,2.89,0.2,m))-2.89) for m in arange(1.3,2,0.001)])
# [x for x in zip(m,d) if x[1] <= min(d)]

# # yi(1.35,5.4,0.15,1.682)
# yi(1.35,5.4,0.2,1.46)

# <codecell>

xedges = arange(1.68,2.281,0.02)
yedges = [1.35,1.5,1.75,2.1,2.89,4]
h = hq2ws[0]
for y0,y1 in zip(yedges[:-1],yedges[1:]):
    h.GetYaxis().SetRangeUser(y0,y1)
    print('Q2=%.3f'%((y0+y1)/2))
    for x0,x1 in zip(xedges[:-1],xedges[1:]):
        h.GetXaxis().SetRangeUser(x0,x1)
        print('W = %.3f .... %d'%((x0+x1)/2,h.Integral()))
h.GetYaxis().SetRange(0,-1)
h.GetXaxis().SetRange(0,-1)

# <markdowncell>

# # $p\pi^{+}\pi^{-}$

# <codecell>

h10.SetEntryList(el1)
x0, x1, dx = 1.68, 2.64, 0.1  # 0.02
xedges = arange(x0,x1,dx)
yedges = [1.35,1.5,1.75,2.1,2.89]
wvals = (xedges[1:]+xedges[:-1])/2
wbin = lambda x: int(len(wvals)*(x-xedges[0])/(xedges[-1]-xedges[0]))
print(wvals)

# <codecell>

# hs_mmp_Q2_1500_1750 = [Hist(160,0.4,1.2,name='hmmp_W_%d_Q2_1625'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
# hs_t1_Q2_1500_1750 = [Hist(60,0,6,name='ht1_W_%d_Q2_1625'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
# hs_ct_Q2_1500_1750 = [Hist(50,-1,1,name='hct_W_%d_Q2_1625'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
# hs_phi_Q2_1500_1750 = [Hist(60,-math.pi,math.pi,name='hphi_W_%d_Q2_1625'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
# fout = TFile('tmp_wq2_1','create')
hs_mmp = [Hist(160,0.4,1.2,name='hmmp_W_%d'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
hs_t1 = [Hist(60,0,6,name='ht1_W_%d'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
hs_ct = [Hist(50,-1,1,name='hct_W_%d'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]
hs_phi = [Hist(60,-math.pi,math.pi,name='hphi_W_%d'%int(1000*wval),title='W = %.3f GeV'%wval) for wval in wvals]

# <codecell>

# h10.Draw('>>elist', 'top3pass && mmthreshpass && ccpass && fidpass && mmpi0pass', 'entrylist')
el = el1  # r.gROOT.FindObject('elist')
for ientry in range(0,el.GetN()):
    tkin.GetEntry(el.GetEntry(ientry))
    # if tkin.Q2 < 1.5 or tkin.Q2 > 1.75 or tkin.W < 1.68 or tkin.W > 2.64: continue
    if tkin.W < 1.68 or tkin.W > 2.58: continue
    # hs_mmp_Q2_1500_1750[wbin(tkin.W)].Fill(tkin.mmp)
    hs_mmp[wbin(tkin.W)].Fill(tkin.mmp)
    if tkin.mmp < 0.7 and tkin.mmp > 0.866: continue
    hs_t1[wbin(tkin.W)].Fill(-tkin.t1)
    hs_ct[wbin(tkin.W)].Fill(tkin.ct)
    hs_phi[wbin(tkin.W)].Fill(tkin.phi)
#         hs_t1_Q2_1500_1750[wbin(tkin.W)].Fill(-tkin.t1)
#         hs_ct_Q2_1500_1750[wbin(tkin.W)].Fill(tkin.ct)
#         hs_phi_Q2_1500_1750[wbin(tkin.W)].Fill(tkin.phi)

# <codecell>

ncols, nrows = 3, 3
# ncols, nrows = 6, 8
fmmp = figure(figsize=(18,18)) # 24))
for i,h in enumerate(hs_mmp): #_Q2_1500_1750):
    draw(h, ncols, nrows, i+1, fig=fmmp)
    text(0.1, 0.85, '$W = %.3f GeV$'%wvals[i], fontsize=16, transform=fmmp.axes[i].transAxes)
ft1 = figure(figsize=(18,24))
for i,h in enumerate(hs_t1): #_Q2_1500_1750):
    draw(h, ncols, nrows, i+1, fig=ft1)
    text(0.1, 0.85, '$W = %.3f GeV$'%wvals[i], fontsize=16, transform=ft1.axes[i].transAxes)
fct = figure(figsize=(18,24))
for i,h in enumerate(hs_ct): #_Q2_1500_1750):
    draw(h, ncols, nrows, i+1, fig=fct)
    text(0.1, 0.85, '$W = %.3f GeV$'%wvals[i], fontsize=16, transform=fct.axes[i].transAxes)
fphi = figure(figsize=(18,24))
for i,h in enumerate(hs_phi): #_Q2_1500_1750):
    draw(h, ncols, nrows, i+1, fig=fphi)
    text(0.1, 0.85, '$W = %.3f GeV$'%wvals[i], fontsize=16, transform=fphi.axes[i].transAxes)

# <codecell>

h10.SetEntryList(el3)
h10.Draw('mmp>>hmmp3(160,0.4,1.2)')
h10.SetEntryList(el)
h10.Draw('mmp>>hmmp3b(160,0.4,1.2)','1.458','same')

# <codecell>

4300/2950.

# <codecell>

import os

MASS_P = 0.93827203
tops = {1:(el1, 'simulation_shaping_top1.root'), 2:(el3, 'simulation_shaping_top3.root')}
top = 1    # default to topology 1
h10.SetEntryList(el1)

def create_hists(fn=tops[top][1], el=tops[top][0]):
    global h10
    h10.SetEntryList(el)
    ht1q2w = rh1('-t1:Q2:W', hn='ht1q2w', cutstr='1/vgflux', binning='(32,1.6,3.2, 9,1.0,5.5, 80,0,8)')
    hmq2w = rh1('mmp:Q2:W', hn='hmq2w', cutstr='1/vgflux', binning='(32,1.6,3.2, 9,1.0,5.5, 80, 0.4, 1.2)')
    hq2w = rh1('Q2:W', hn='hq2w', cutstr='1/vgflux', binning='(320,1.6,3.2, 80,1,5.5)')
    #with ropen('simulation_shaping.root', 'recreate') as fout:
    with ropen(fn, 'recreate') as fout:
        [h.Clone().Write(h.GetName()) for h in [ht1q2w, hmq2w, hq2w]]
    return (ht1q2w, hmq2w, hq2w)

def get_hists(fn=tops[top][1], el=tops[top][0]):
    global h10
    h10.SetEntryList(el)
    ht1q2w, hmq2w, hq2w = _,_,_
    if (os.path.exists(fn)):
        with ropen(fn, 'read') as fin:
            ht1q2w, hmq2w, hq2w = [asrootpy(h.Clone()) for h in [fin.ht1q2w, fin.hmq2w, fin.hq2w]]
            [h.SetDirectory(0) for h in [ht1q2w, hmq2w, hq2w]]
            fin.Close()
    else:
        ht1q2w, hmq2w, hq2w = create_hists(fn, el)
    return (ht1q2w, hmq2w, hq2w)

ht1q2w, hmq2w, hq2w = get_hists()

def get_projs(h3=hmq2w, hntmpl='mass_%d_%d', zintrange=(0.750, 0.793), minint=1000):
    wax, q2ax, zax = h3.GetXaxis(), h3.GetYaxis(), h3.GetZaxis()
    zlobin, zhibin = zax.FindBin(zintrange[0]), zax.FindBin(zintrange[1])+1
    hms = [asrootpy(h3.ProjectionZ(hntmpl%(1000*wax.GetBinCenter(wbin), 1000*q2ax.GetBinCenter(q2bin)),
                                    wbin, wbin+1, q2bin, q2bin+1))
            for q2bin in range(1, q2ax.GetNbins())
            for wbin in range(1, wax.GetNbins())
            if h3.Integral(wbin, wbin+1, q2bin, q2bin+1, zlobin, zhibin) > minint]
        
    zax.SetRange(0,-1)
    return hms

def bgfun(x, p):
    pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
    #print(p[3], x[0], p[4])
    drop = 1 - math.erf((x[0]-p[3])/p[4])
    if (x[0] > 0.5 and x[0] < 0.6) or (x[0] > 0.7 and x[0] < 0.92): r.TF1.RejectPoint()
    return pol2*drop/2

def drop(x, p):
    return 1 - math.erf((x[0]-p[0])/p[1])

def gausdrop(x, p):
    gaus = p[0]*math.exp(-0.5*pow(((x[0]-p[1])/p[2]), 2))
    return gaus*drop(x, [p[3],p[4]])/2

def fitfun(x, p):
    pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
    gaus = p[5]*math.exp(-0.5*pow(((x[0]-p[6])/p[7]), 2))
    return (gaus+pol2)*drop(x, [p[3],p[4]])/2

# <codecell>

def fit_mmp(h, W, dW, Q2=None, dQ2=None, fout=None):
    '''
    Fit histogram with (gauss+pol2)*(erf cutoff).
    Return tuple of W, Q2, histogram, function, fit parameters, chi2, FCstatu.
    + Cutoff parameters estimated from phase space edge from W.
    + Background parameters estimated by fitting with signal, skipping signal region.
    + Signal parameters estimated by subtracting initial background and fitting in
      vicinity of the omega mass mean.
    + Full histogram fitted with combined function.
    TODO:  add option to provide initial parameter estimates.
    '''
    result = []
    wlow, whigh = W-dW, W+dW  # W bins are 50 MeV
    # ######### calculate MMp phase space edge range
    mmpax = h.GetXaxis()
    x0 = math.sqrt(wlow**2+MASS_P**2-2*wlow*MASS_P)
    x1 = math.sqrt(whigh**2+MASS_P**2-2*whigh*MASS_P)
    # ######### setup fit functions
    fgaus = r.TF1('fgaus', gausdrop, 0.4, 1.2, 5)
    fgaus.SetParameters(r.Double(20), r.Double(0.783), r.Double(0.20), r.Double((x1+x0)*0.5), r.Double(x1-x0))
    fgaus.SetParLimits(0, 10, 1e9)
    fgaus.SetParLimits(1, 0.773, 0.793)
    fgaus.SetParLimits(2, 0.011, 0.035)
    fgaus.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
    fgaus.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
    fbg = r.TF1('fbg', bgfun, 0.4, 1.2, 5)
    fbg.SetParameters(r.Double(0), r.Double(0), r.Double(1.1e3), r.Double((x1+x0)*0.5), r.Double(x1-x0))
    fbg.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
    fbg.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
    fbg.SetParLimits(2, 1e3, 1e9)
    # ######### fit background for parameter estimates
    h.Fit(fbg, '', 'goff', 0.4, 1.2)
    if True:  #r.gMinuit.fCstatu.startswith('CONV'):
        # ######### fit signal for parameter estimates
        wsig = h.Clone()
        wsig.Add(fbg, -1)
        sigint = wsig.Integral(mmpax.FindBin(0.76), mmpax.FindBin(0.80))
        sigint = sigint if sigint > 0 else h.Integral(mmpax.FindBin(0.76), mmpax.FindBin(0.80))
        fgaus.SetParLimits(0, 0.1*sigint, 2*sigint)
        wsig.Fit(fgaus, '', '', 0.76, 0.80)
        if True:  #r.gMinuit.fCstatu.startswith('CONV'):
            # ######### create combined signal-background function and set parameters
            fsigbg = r.TF1('fsigbg', fitfun, 0.4, 1.2, 8)
            fsigbg.SetParameters(r.Double(0), r.Double(0), r.Double(1.1e3), r.Double((x1+x0)*0.5), r.Double(x1-x0), r.Double(3e3), r.Double(0.783), r.Double(0.022))
            fsigbg.SetParameters(fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2),
                                 fbg.GetParameter(3), fbg.GetParameter(4),
                                 fgaus.GetParameter(0), fgaus.GetParameter(1), fgaus.GetParameter(2))
            fsigbg.SetParLimits(2, 1e3, 1e9)
            fsigbg.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
            fsigbg.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
            #fsigbg.SetParLimits(3,0.65,2)
            #fsigbg.SetParLimits(4,0.03, 0.2)
            fsigbg.SetParLimits(5, 0.1*sigint, 2*sigint)
            fsigbg.SetParLimits(6, 0.773, 0.793)
            fsigbg.SetParLimits(7, 0.011, 0.035)
            # ######### fit signal and background
            h.Fit(fsigbg, '', 'goff', 0.4, 1.2)
            if True:  #r.gMinuit.fCstatu.startswith('CONV'):
                q = fsigbg.GetChisquare()/fsigbg.GetNDF() if fsigbg.GetNDF() > 0 else -1
                result += [W,Q2]
                result.append(h)
                result.append(h.GetListOfFunctions()[0])
                result.append(tuple([fsigbg.GetParameter(ipar) for ipar in range(0,8)]))
                result.append(q)
                result.append(r.gMinuit.fCstatu)
                if fout is not None: fout.WriteObject(h, h.GetName())
    return tuple(result) if result is not None else None

# <codecell>

res = fit_mmp(hs_mmp[1], 1.83, 0.05)

# <codecell>

res

# <markdowncell>

# # old

# <codecell>

h10.SetEntryList(el3)

# <codecell>

hmass_el3 = rh1('p:sqrt(m)-0.01', '(ccpass*mmthreshpass)*(q>0)', 'hpVmass_el3', binning='(300, 0.001, 1.500001, 400, 0, 4)')

# <codecell>

draw(hmass_el3)

# <codecell>

hmass_el3.Draw('colz')

# <codecell>


# <codecell>

h10.GetEntries('h10idx_pip==0 && h10idx_pim==0')

# <codecell>

h10.SetEntryList(0)
h10.Draw('>>elf', 'ccpass && fidpass', 'entrylist')
elf = r.gROOT.FindObject('elf')
h10.SetEntryList(elf)
h10.GetEntries('h10idx_pip==0 && h10idx_pim==0')

# <codecell>

h10.SetEntryList(elf)
#h10.SetEntryList(el1)
fig=figure(figsize=(8,5))
hel1 = rh1('mmppip.M():mmp.M()', 'top1pass', 'hel1', binning='(160,0.4,1.2,200,0,1)')
draw(hel1, fig=fig)
xlabel('$MM_p\,(GeV)$', fontsize=14)
ylabel('$MM_p\pi^{+}\,(GeV)$', fontsize=14)
#h10.SetEntryList(el2)
fig=figure(figsize=(8,5))
hel2 = rh1('mmppim.M():mmp.M()', 'top2pass', 'hel2', binning='(160,0.4,1.2,200,0,1)')
draw(hel2, fig=fig)
xlabel('$MM_p\,(GeV)$', fontsize=14)
ylabel('$MM_p\pi^{-}\,(GeV)$', fontsize=14)
#h10.SetEntryList(el3)
fig=figure(figsize=(8,5))
hel3 = rh1('mmppippim.M():mmp.M()', 'top3pass', 'hel3', binning='(160,0.4,1.2,160,0,0.8)')
draw(hel3, fig=fig)
ylabel('$MM_p\pi^{+}\pi^{-}\,(GeV)$', fontsize=14)
xlabel('$MM_p\,(GeV)$', fontsize=14)

# <codecell>

draw(hel1)
draw(hel2)
draw(hel3)
ylim(0.0, 0.8)

# <codecell>

h10.SetEntryList(el3)
hel3 = rh1('mmppippim.M():mmp.M()', 'top3pass', 'hel3', binning='(160,0.4,1.2,160,0,0.8)')
hel3b = rh1('mmppippim.M():mmp.M()', 'mmpi0pass && pi0.M()>0.075', 'hel3b', binning='(160,0.4,1.2,160,0,0.8)')

# <codecell>

hel3b_sb = rh1('mmp.M()', 'mmpi0pass && ((pi0.M()<0.075 && pi0.M()>0.021) || (pi0.M()>0.183 && pi0.M()<0.237))', 'hel3sb', binning='(160,0.4,1.2)')
hel3x = asrootpy(hel3b.ProjectionX())

# <codecell>

hel3x.Add(hel3b_sb, -1)

# <codecell>

golden_mean = (sqrt(5)-1.0)/2.0
fig = figure(figsize=(14,14*golden_mean))
draw(hel3, fig=fig)
#scatter(list(hel3x.x()), [y*0.6/max(hel3x) for y in hel3x])
fill_between(list(hel3x.x()), [y*0.7/max(hel3x) for y in hel3x], alpha=0.5, color='gray')
#scatter(list(hel3b_sb.x()), [y*0.6/max(hel3x) for y in hel3b_sb])
# axhline(0.237, color='k')
# axhline(0.183, color='k')
# axhline(0.075, color='k')
# axhline(0.021, color='k')

# <codecell>

h10.SetEntryList(el3)

# <codecell>

hq2w_el3_omega = rh1('Q2:W', 'mmpi0pass && pi0.M()>0.075 && mmp.M()>0.733 && mmp.M()<0.833', 'hel3b', binning='(320,1.6,3.2, 80,1,5.5)')

# <codecell>

hSectQ2W_el3_omega_e = rh1('sc_sect[sc[h10idx_e]-1]:Q2:W', 'mmpi0pass && pi0.M()>0.075 && mmp.M()>0.733 && mmp.M()<0.833', 'hel3_by_esect', binning='(320,1.6,3.2, 80,1,5.5, 6, 0.5, 6.5)')

# <codecell>

hSectQ2W_el3_omega_p = rh1('dc_sect[dc[h10idx_p]-1]:Q2:W', 'mmpi0pass && pi0.M()>0.075 && mmp.M()>0.733 && mmp.M()<0.833', 'hel3_by_psect', binning='(320,1.6,3.2, 80,1,5.5, 6, 0.5, 6.5)')

# <codecell>

hq2w_per_esect = []
hsects = hSectQ2W_el3_omega_p
for isect in range(0,6):
    hsects.GetZaxis().SetRange(isect+1,isect+1)
    h = asrootpy(hsects.Project3D('yx').Rebin2D(4,2, '%s_s%d'%(hsects.GetName(), isect+1)))
    h.SetOption('colz')
    h.SetDrawOption('colz')
    hq2w_per_esect.append(h)

# <codecell>

import rootpy as rp
hs = rp.plotting.HistStack(hists=hq2w_per_esect)
hs.Draw('pads')

# <codecell>

hq2w_per_esect[0].GetYaxis().FindBin(2.4)
type(hq2w_per_esect[0])

# <codecell>

hs1 = rp.plotting.HistStack(hists=[asrootpy(h.ProjectionX('%s_%d'%(h.GetName(),i),10,13)) for i, h in enumerate(hq2w_per_esect)])
hs1.Draw('pads')

# <codecell>

for h in hs1:
    h.GetListOfFunctions().Clear()
    yhi = h.GetMaximum()*1.15
    l1, l2 = r.TLine(1.8, 0, 1.8, yhi), r.TLine(1.88, 0, 1.88, yhi)
    l1.SetLineColor(2)
    l2.SetLineColor(2)
    h.GetListOfFunctions().Add(l1)
    h.GetListOfFunctions().Add(l2)

# <codecell>

draw(hq2w_el3_omega)
hq2w_el3_omega.GetYaxis().SetRangeUser(2.1,2.4)
draw(asrootpy(hq2w_el3_omega.ProjectionX()))
hq2w_el3_omega.GetYaxis().SetRange(0,-1)
print(len(hq2w_el3_omega))

# <codecell>

h10.SetEntryList(0)
hq2w_omega_region = rh1('Q2:W', 'mmp.M()>0.733 && mmp.M()<0.833', 'hq2w_omega_mass', binning='(320,1.6,3.2, 80,1,5.5)')

# <codecell>

draw(hq2w_omega_region)

# <codecell>

import math as m
import numpy as np

# <codecell>

x0, w = 4, 1
X = np.linspace(0,10)
Y = [(1-m.erf((x-x0)/w))/2 for x in X]
plot(X,Y)

# <codecell>


