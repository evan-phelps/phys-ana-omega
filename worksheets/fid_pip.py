# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

%run ../python/oana_setup.py
import rootpy as rp

# <markdowncell>

# # open paddle-to-$\theta/p$ mapping, $\theta:p$ histograms
# 
# If htp_pip.root has not been created, scroll down to "generate $\theta:p$ histogram file" section.

# <codecell>

fin_htp = ropen('htp_pip.root','read')
fin_pdl = ropen('/home/ephelps/Dropbox/h3_pdlVtVp_pip.root','read')
def drawpdl(sector, paddle, options='same'):
    h = fin_pdl['htvp_%d_pdl%d_mean'%(sector,paddle)]
    #h.Draw(options)
    f = h.GetListOfFunctions()[0]
    f.SetLineWidth(2)
    f.SetLineColor(r.kBlack)
    f.Draw(options)
    return f

# <markdowncell>

# # investigate

# <codecell>

fin_htp.htp_pip_3.Draw("colz")

# <codecell>

#2, 11, 24, 25, 27, 28
f311 = drawpdl(3,11)
f324 = drawpdl(3,24)
f324 = drawpdl(3,25)
f327 = drawpdl(3,27)
f324 = drawpdl(3,28)

# <markdowncell>

# # sector 5 examples
# 
# Some of the $\theta:p$ structure parallel to SC paddles comes from other sub-detectors with elements running in the same direction (like EC).  So, The general approach should perhaps be to look at each sub-detector in its natural coordinate system, perform cuts, and THEN look at the $\theta:p$ distributions of experiment and simulation to see diferences.  Let's take, for example, sector 5.
# 
# ## EC UVW

# <codecell>

fin_uvw = ropen('uvw.root')

# <codecell>

huvwS = [fin_uvw.uvw['hsf_V_%s_%d'%(coord, sector)] for sector in range(1,7) for coord in ('u','v','w')]

# <codecell>

fig = figure(figsize=(16,12))
lines = [[70, 400], [362], [395]]*6 # [[20, 400], [375], [410]]*6
for i, h in enumerate(huvwS):
    draw(asrootpy(h.ProjectionX()), nrows=6, ncols=3, cell=i+1, fig=fig) #fhs[0][3].markercolor='r')
    for l in lines[i]: axvline(l)

# <markdowncell>

# The U coordinate has the most structure, because those wires run perpendicular to SC paddles -- a bad SC paddle will lower the yield at a specific U.  I'll cut along U before moving on to SC paddles, but to better see where to cut some kind of normalization will be useful.  First, look at the normalized distributions.

# <codecell>

huvwS_2d = [huvwS[(i-1)*3:i*3] for i in range(1,7)]

# <codecell>

huvwS_2d_b = list(np.empty((3,6), dtype=object))
for sector, hs in enumerate(huvwS_2d):
    for coord, h in enumerate(hs):
        huvwS_2d_b[coord][sector] = h.Clone('%s_b'%h.GetName())

# <codecell>

colorcycle = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
hs = [map(lambda x: asrootpy(x.ProjectionX()),huvwS_2d_b[coord]) for coord in range(0,3)]
for coord, h in enumerate(hs):
    for i in range(0,6):
        h[i].markercolor='k' #colorcycle[i]

# <codecell>

fig = figure(figsize=(20,6))
lines = [[20, 400], [375], [410]]*6
for coord in range(0,3):
    draw(hs[coord], ncols=3, cell=coord+1, fig=fig)

# <codecell>

hs[0][4].Draw()
fpol2 = r.TF1('fpol2', 'pol2', 0, 450)
fpol2.SetParameters(56823.5, -304.585, 0.436)
fpol2.Draw('same')

# <markdowncell>

# # cleanup

# <codecell>

[f.close() for f in [fin_htp, fin_pdl]]

# <markdowncell>

# # generate $\theta:p$ histogram file

# <codecell>

import rootpy as rp

h10.SetEntryList(el1)

fout = ropen('htp_pip.root','recreate')
theta_max = 2.4*180/pi
htp_pipS = [rh1('TMath::RadToDeg()*acos(cz[h10idx_pip]):p[h10idx_pip]', 'sc[h10idx_pip]>0 && sc_sect[sc[h10idx_pip]-1]==%d'%isect, 'htp_pip_%d'%isect, binning='(400,0,4,240,0,%.0f)'%theta_max) for isect in range(1,7)]
hs_tp_pip = rp.plotting.HistStack('hs_tp_pip', '#theta versus p, #pi^{+}')
for h in htp_pipS:
    h.SetOption('col')
    h.SetDrawOption('col')
    hs_tp_pip.Add(h)

#hs_tp_pip.Draw('pads')
fout.Write()
fout.Close()

# <codecell>

import rootpy as rp

h10.SetEntryList(el1)

fout = ropen('htp_p.root','recreate')
theta_max = 2.4*180/pi
htp_pS = [rh1('TMath::RadToDeg()*acos(cz[h10idx_p]):p[h10idx_p]', 'sc[h10idx_p]>0 && sc_sect[sc[h10idx_p]-1]==%d'%isect, 'htp_p_%d'%isect, binning='(400,0,4,240,0,%.0f)'%theta_max) for isect in range(1,7)]
hs_tp_p = rp.plotting.HistStack('hs_tp_p', '#theta versus p, proton')
for h in htp_pS:
    h.SetOption('col')
    h.SetDrawOption('col')
    hs_tp_p.Add(h)

#hs_tp_pip.Draw('pads')
fout.Write()
fout.Close()

# <markdowncell>

# # misc

# <codecell>

import rootpy as rp

h10.SetEntryList(el1)
def getpdl_fromH10(sector, paddle):
    theta_max = 2.4*180/pi
    h = rh1('TMath::RadToDeg()*acos(cz[h10idx_pip]):p[h10idx_pip]',
            'sc[h10idx_pip]>0 && sc_sect[sc[h10idx_pip]-1]==%d && \
             sc_pd[sc[h10idx_pip]-1]==%d'%(sector, paddle),
            'htp_pip_%d_%d'%(sector, paddle),
            binning='(400,0,4,240,0,%.0f)'%theta_max)
    return h

# <codecell>

h339 = getpdl_fromH10(3, 39)
h340 = getpdl_fromH10(3, 40)
h341 = getpdl_fromH10(3, 41)

# <codecell>

h339.Draw('colz')

# <codecell>

h340.Draw('colsame')

# <codecell>

hsum = h339.Clone('hsum')
hsum.Add(h340)
hsum.Add(h341)
hsum.Draw('colz')

# <codecell>

ctmp = rp.plotting.Canvas()

# <codecell>

hpdls_s3 = rh1('dc_xsc:dc_ysc', 'sc[h10idx_pip]>0 && \
                sc_sect[sc[h10idx_pip]-1]==%d'%3, N=100000)

# <codecell>

h340.Draw('colsame')

# <codecell>


