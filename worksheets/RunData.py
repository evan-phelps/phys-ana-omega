# -*- coding: utf-8 -*-
"""
Created on Sat Feb 28 12:35:08 2015

@author: ephelps
"""

from rootpy.io import root_open
from rootpy import asrootpy
#import matplotlib.pyplot as plt

MASS_P = 0.93827203
SOL = 29.9792458

class RunData(object):
    '''docstring'''
    hpid_p_templ = 'hdt_V_p_pos_p_s%d_p%d'
    
    def __init__(self, run_number, w_dir='~/data/batch/e1f_skim'):
        self.run = run_number
        self.fin = root_open('%s/%d.root'%(w_dir, run_number))
    
    def hist_proton_pid(self, sector, paddle, from_tree=False, cor=0):
        htmp = None
        if from_tree:
            #root [3] c->SetAlias("bifp","sqrt(p*p/(p*p+0.93827203*0.93827203))");
            #root [4] c->SetAlias("dtifp","sc_t[sc-1]-sc_r[sc-1]/(29.9792458*bifp)-tr_time");
            #root [5] c->Draw("dtifp:p>>h2(550, 0, 5.5, 200, -10, 10)", "q>0 && sc>0 && ec>0 && sc_sect[sc-1]==6 && sc_pd[sc-1]==25", "colz")
            bifp = 'sqrt(p*p/(p*p+%f*%f))'%(MASS_P, MASS_P)
            dtifp = 'sc_t[sc-1]-sc_r[sc-1]/(%f*%s)-tr_time-%d'%(SOL, bifp, cor)
            h10 = asrootpy(self.fin.h10clone.h10)
            htmp = h10.draw('p:%s>>htmp(500,0,5,400,-10,10)'%dtifp,
                            'q>0 && sc>0 && dc>0 && sc_sect[sc-1]==%d && sc_pd[sc-1]==%d'%(sector, paddle), 'goff')
            htmp = asrootpy(htmp)
        else:
            htmp = asrootpy(self.fin.scpid[self.hpid_p_templ%(sector, paddle)])
        return htmp.clone()
    
    def get_xpeak_proton_pid(self, sector, paddle, from_tree=False):
        h2 = self.hist_proton_pid(sector, paddle, from_tree)
        h2corr = h2
        h = h2.projection_y()
        xpeak = 0
        h.Smooth(2)
        if h.ShowPeaks(2, '', 0.5) > 0:
            pm = h.GetListOfFunctions().FindObject('TPolyMarker')
            xpeak = max([pm.GetX()[i] for i in range(0, pm.GetN()) \
                                      if  pm.GetX()[i] > -3 \
                                      and pm.GetX()[i] < 3]) #, key=lambda x: pm.GetY()[i])
            h2corr = self.hist_proton_pid(sector, paddle, from_tree, round(xpeak))
        return (xpeak, h2.clone('h_r%d_s%d_p%d'%(self.run, sector, paddle)),
                h2corr.clone('h_r%d_s%d_p%d_cor'%(self.run, sector, paddle)))

    def __enter__(self):
        #print('opening run %d'%self.run)
        return self
        
    def __exit__(self, type, value, traceback):
        #print('closing run %d'%self.run)
        self.fin.close()
        return isinstance(value, TypeError)
    
    def __del__(self):
        #print('deleting RunData(%d)'%self.run)
        del self.fin
