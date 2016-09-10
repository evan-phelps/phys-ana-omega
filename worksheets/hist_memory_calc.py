# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <markdowncell>

# # histograms
# 
# Parsed from DataHandler sources.  Currently need about 2 GB of memory for histograms assuming each handler only appears once.  During a full skim run, they definitely appear more than once.

# <codecell>

NSECTS, NPMTS, NPDLS, NPBINS = 6, 36, 48, 43
# hnphe_V_pmtS = MakeHists(NSECTS, "hnphe_V_pmt_s%d", "sector %d", NPMTS, 0.5, NPMTS+0.5, 175, -0.5, 349.5);
nbins = NSECTS*(2+NPMTS)*(2+175)
# hsf_V_p = MakeHists(NSECTS, "hsf_V_p_%d", "stat>0, nphe>40, etot>0.14, ei>0.07, sector %d", 250, 0.0, 5.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+250)*(2+100)
# heo_V_ei = MakeHists(NSECTS, "heo_V_ei_%d", "stat>0, nphe>40, sf>0.26, p>0.64, sector %d", 300, 0.0, 0.3, 300, 0.0, 0.3);
nbins += NSECTS*(2+300)*(2+300)
# hsf_V_p_anti = MakeHists(NSECTS, "hsf_V_p_anti_%d", "NOT stat>0, nphe>40, etot>0.14, ei>0.07, sector %d", 250, 0.0, 5.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+250)*(2+100)
# heo_V_ei_anti = MakeHists(NSECTS, "heo_V_ei_anti_%d", "NOT stat>0, nphe>40, sf>0.26, p>0.64, sector %d", 300, 0.0, 0.3, 300, 0.0, 0.3);
nbins += NSECTS*(2+300)*(2+300)
# hsf_V_p_clean = MakeHists(NSECTS, "hsf_V_p_clean_%d", "stat>0, nphe>40, etot>0.14, ei>0.07, sector %d", 250, 0.0, 5.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+250)*(2+100)
# heo_V_ei_clean = MakeHists(NSECTS, "heo_V_ei_clean_%d", "stat>0, nphe>40, sf>0.26, p>0.64, sector %d", 300, 0.0, 0.3, 300, 0.0, 0.3);
nbins += NSECTS*(2+300)*(2+300)
# hsf_V_p_clean_anti = MakeHists(NSECTS, "hsf_V_p_clean_anti_%d", "NOT stat>0, nphe>40, etot>0.14, ei>0.07, sector %d", 250, 0.0, 5.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+250)*(2+100)
# heo_V_ei_clean_anti = MakeHists(NSECTS, "heo_V_ei_clean_anti_%d", "NOT stat>0, nphe>40, sf>0.26, p>0.64, sector %d", 300, 0.0, 0.3, 300, 0.0, 0.3);
nbins += NSECTS*(2+300)*(2+300)
# hsf_V_u = MakeHists(NSECTS, "hsf_V_u_%d", "SF vs. EC U, sector %d", 450, 0.0, 450.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+450)*(2+100)
# hsf_V_v = MakeHists(NSECTS, "hsf_V_v_%d", "SF vs. EC V, sector %d", 450, 0.0, 450.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+450)*(2+100)
# hsf_V_w = MakeHists(NSECTS, "hsf_V_w_%d", "SF vs. EC W, sector %d", 450, 0.0, 450.0, 100, 0.0, 0.5);
nbins += NSECTS*(2+450)*(2+100)
# hangpS[ipbin] = MakeHists(NSECTS, name.Data(), title.Data(), 60, 0, 60, 240, -30, 30);
nbins += NSECTS*(2+NPBINS)*(2+60)*(2+240)
# heidS = MakeHists(NSECTS, "heid_%d", "electron id summary, sector %d", nconditions, -0.5, nconditions-0.5);
nbins += NSECTS*(2+12)
# heidindS = MakeHists(NSECTS, "heidind_%d", "electron id summary, independent, sector %d", nconditions, -0.5, nconditions-0.5);
nbins += NSECTS*(2+12)
# hq2_V_wS = MakeHists(NSECTS, "hq2_V_w_%d", "Q^{2} vs. W, Sector %d", 340, 0.0, 3.4, 800, 0.0, 8.0);
nbins += NSECTS*(2+340)*(2+800)
# htprime_V_wS = MakeHists(NSECTS, "htprime_V_w_%d", "-t^{#prime} vs. W, Sector %d", 340, 0.0, 3.4, 900, -1.0, 8.0);
nbins += NSECTS*(2+340)*(2+900)
# hcosTheta_V_phiS = MakeHists(NSECTS, "hcosTheta_V_phi_%d", "cos(#theta) vs. #phi, Sector %d", 720, -PI, PI, 200, -1, 1);
nbins += NSECTS*(2+720)*(2+200)
# hmmppip_V_mmpS = MakeHists(NSECTS, "hmmppip_V_mmp_%d", "MMppip vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
nbins += NSECTS*(2+220)*(2+300)
# hmmppim_V_mmpS = MakeHists(NSECTS, "hmmppim_V_mmp_%d", "MMppim vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
nbins += NSECTS*(2+220)*(2+300)
# hmmppippim_V_mmpS = MakeHists(NSECTS, "hmmppippim_V_mmp_%d", "MMppippim vs. MMp, Sector %d", 220, 0.0, 2.2, 300, -1, 2);
nbins += NSECTS*(2+220)*(2+300)
# hq2_V_wS = MakeHists(NSECTS, "hq2_V_w_%d", "Q^{2} vs. W, Sector %d", 340, 0.0, 3.4, 800, 0.0, 8.0);
nbins += NSECTS*(2+340)*(2+800)
# hq2_V_w_elast_excS = MakeHists(NSECTS, "hq2_V_w_elast_exc_%d", "Q^{2} vs. W, elastic, Sector %d", 100, 0.5, 1.5, 600, 0.0, 6.0);
nbins += NSECTS*(2+100)*(2+600)
# hm_V_pdlS = MakeHists(NSECTS, "hm_V_pdl_s%d", "hadron mass spectrum per paddle, sector %d", 48, 0.5, 48.5, 450, -0.4, 1.4);
nbins += NSECTS*(2+48)*(2+450)
# hetime_V_pdlS = MakeHists(NSECTS, "hetime_V_pdl_s%d", "electron time per paddle, sector %d", 24, 0.5, 24.5, 40, -1, 1);
nbins += NSECTS*(2+24)*(2+40)
# hdt_V_p_pos_pS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
nbins += NSECTS*(NPDLS)*(2+500)*(2+400)
# hdt_V_p_pos_pipS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
nbins += NSECTS*(NPDLS)*(2+500)*(2+400)
# hdt_V_p_neg_pimS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
nbins += NSECTS*(NPDLS)*(2+500)*(2+400)
# hdt_V_p_neg_eS[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 400, -10, 10);
nbins += NSECTS*(NPDLS)*(2+500)*(2+400)
# hdt_V_p_neg_e_part1S[isect] = MakeHists(NPDLS, name.Data(), title.Data(), 500, 0, 5, 40, -1, 1);
nbins += NSECTS*(NPDLS)*(2+500)*(2+40)
print('%.3f GB'%(nbins*8.0/1073741824))

# <codecell>


