import ROOT as r
from ROOT.Math import crystalball_function as cbfunc
import rootpy as rpy
from rootpy import asrootpy
from rootpy.io import root_open
import numpy as np
from array import array
import math

MASS_P = 0.93827203

def bgfun(x, p):
	pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
	#print(p[3], x[0], p[4])
	drop = 1 - math.erf((x[0]-p[3])/p[4])
	#if x[0] > 0.7 and x[0] < 0.85: r.TF1.RejectPoint()
	if x[0] > 0.7 and x[0] < 0.825: r.TF1.RejectPoint()
	return pol2*drop/2

def drop(x, p):
	return 1 - math.erf((x[0]-p[0])/p[1])

# def gausdrop(x, p):
# 	gaus = p[0]*math.exp(-0.5*pow(((x[0]-p[1])/p[2]), 2))
# 	return gaus*drop(x, [p[3],p[4]])/2

def gausdrop(x, p):
	gaus = p[0]*cbfunc(-1*x[0], p[1], p[2], p[3], p[4])
	return gaus*drop(x, [p[5],p[6]])/2

# def fitfun(x, p):
# 	pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
# 	gaus = p[5]*math.exp(-0.5*pow(((x[0]-p[6])/p[7]), 2))
# 	return (gaus+pol2)*drop(x, [p[3],p[4]])/2

def fitfun(x, p):
	pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
	gaus = gausdrop(x[0], p[5:]+[p[3],p[4]])
	return (gaus+pol2)*drop(x, [p[3],p[4]])/2

def fit_mmp(h, W, Q2, fout=None):
	'''
	Fit histogram with (gauss+pol2)*(erf cutoff).
	Return tuple of W, Q2, histogram, function, fit parameters, chi2, FCstatu.
	+ Cutoff parameters estimated from phase space edge from W.
	+ Background parameters estimated by fitting with signal, skipping signal region.
	+ Signal parameters estimated by subtracting initial background and fitting in
	  vicinity of the omega mass mean.
	+ Full histogram fitted with combined function.
	'''
	result = []
	
	W = sum(W)/2 if isinstance(W, list) else W
	wlow, whigh = W-0.010, W+0.010  # W bins are 20 MeV
	# ######### calculate MMp phase space edge range
	mmpax = h.GetXaxis()
	x0 = math.sqrt(wlow**2+MASS_P**2-2*wlow*MASS_P)
	x1 = math.sqrt(whigh**2+MASS_P**2-2*whigh*MASS_P)
	# ######### setup fit functions
	# fgaus = r.TF1('fgaus', gausdrop, 0.6, 0.95, 5) #0.4, 1.2, 5)
	fgaus = r.TF1('fgaus', gausdrop, 0.6, 0.95, 7)
	# fgaus.SetParameters(r.Double(20), r.Double(0.783), r.Double(0.20),
	#                     r.Double((x1+x0)*0.5), r.Double(x1-x0))
	fgaus.SetParameters(r.Double(10), r.Double(3), r.Double(0.20), r.Double(0.783),
	                    r.Double((x1+x0)*0.5), r.Double(x1-x0))
	fgaus.SetParLimits(0, 10, 1e7)
	fgaus.SetParLimits(1, 1, 10)
	fgaus.SetParLimits(2, 1, 10)
	fgaus.SetParLimits(3, 0.011, 0.035)
	fgaus.SetParLimits(4, 0.773, 0.793)
	fgaus.SetParLimits(5,x0+(x1-x0)/4,x1-(x1-x0)/4)
	fgaus.SetParLimits(6,0.15*(x1-x0), 2*(x1-x0))
	# fgaus.SetParLimits(0, 0.01, 1e4)
	# fgaus.SetParLimits(1, 0.773, 0.793)
	# fgaus.SetParLimits(2, 0.011, 0.035)
	# fgaus.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
	# fgaus.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
	fbg = r.TF1('fbg', bgfun, 0.4, 1.2, 5)
	fbg.SetParameters(r.Double(0), r.Double(3000), r.Double(1000),
	                  r.Double((x1+x0)*0.5), r.Double(x1-x0))
	if x0 > 0.95:
		fbg.FixParameter(3, x1)
		fbg.FixParameter(4, x1-x0)
	else:
		fbg.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
		fbg.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
	fbg.SetParLimits(2, 1, 1e6)
	fbg.SetParLimits(1, 1, 1e9)
	# ######### fit background for parameter estimates
	h.Fit(fbg, '', 'goff', 0.6, 0.95) #0.4, 1.2)
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
			# fsigbg = r.TF1('fsigbg', fitfun, 0.4, 1.2, 8)
			fsigbg = r.TF1('fsigbg', fitfun, 0.4, 1.2, 10)
			#fsigbg.SetParameters(r.Double(0), r.Double(0), r.Double(0.02), r.Double((x1+x0)*0.5), r.Double(x1-x0), r.Double(20), r.Double(0.783), r.Double(0.022))
			p0, p1, p2 = fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2)
			# fsigbg.SetParameters(fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2),
			#                      fbg.GetParameter(3), fbg.GetParameter(4),
			#                      fgaus.GetParameter(0), fgaus.GetParameter(1), fgaus.GetParameter(2))
			fsigbg.SetParameters(fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2),
			                     fbg.GetParameter(3), fbg.GetParameter(4),
			                     fgaus.GetParameter(0), fgaus.GetParameter(1), fgaus.GetParameter(2), fgaus.GetParameter(3), fguas.GetParameter(4))
			fsigbg.SetParLimits(0, 0.85*p0, 1.15*p0)
			fsigbg.SetParLimits(1, 0.85*p1, 1.15*p1)
			fsigbg.SetParLimits(2, 0.85*p2, 1.15*p2)
			#fsigbg.SetParLimits(2, 0.0001, 1e9)
			fsigbg.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
			fsigbg.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
			#fsigbg.SetParLimits(3,0.65,2)
			#fsigbg.SetParLimits(4,0.03, 0.2)
			fsigbg.SetParLimits(5, 0.1*sigint, 2*sigint)
			fsigbg.SetParLimits(9, 0.773, 0.793)
			fsigbg.SetParLimits(8, 0.011, 0.035)
			# fsigbg.SetParLimits(5, 0.1*sigint, 2*sigint)
			# fsigbg.SetParLimits(6, 0.773, 0.793)
			# fsigbg.SetParLimits(7, 0.011, 0.035)
			# ######### fit signal and background
			h.Fit(fsigbg, '', 'goff', 0.6, 0.95) #0.4, 1.2)
			if True:  #r.gMinuit.fCstatu.startswith('CONV'):
				q = fsigbg.GetChisquare()/fsigbg.GetNDF() if fsigbg.GetNDF() > 0 else -1
				result += [W,Q2]
				result.append(h),
				result.append(tuple([fbg.GetParameter(ipar) for ipar in range(0,5)]))
				tfuncs = h.GetListOfFunctions()
				tfunc = None if len(tfuncs)==0 else tfuncs[0]
				result.append(tfunc)
				result.append(tuple([fsigbg.GetParameter(ipar) for ipar in range(0,8)]))
				result.append(q)
				result.append(r.gMinuit.fCstatu)
				if fout is not None: fout.WriteObject(h, h.GetName())
	return tuple(result) if result is not None and len(result)>0 else None

def get_mask(h, vlow=None, vhigh=None, elow=None, ehigh=None):
	_h = h.Clone()
	if vhigh is not None or vlow is not None:
		for ibin in range(0, _h.GetNcells()):
			val = _h.GetBinContent(ibin)
			if (vlow is not None and val <= vlow) or \
			   (vhigh is not None and val >= vhigh or \
				np.isnan(val)):
				_h.SetBinContent(ibin, 0)
			else:
				_h.SetBinContent(ibin, 1)
	if ehigh is not None or elow is not None:
		_h.Sumw2()
		for ibin in range(0, _h.GetNcells()):
			val = _h.GetBinContent(ibin)
			err = _h.GetBinError(ibin)
			if (elow is not None and (val == 0 or err/val <= elow)) or \
			   (ehigh is not None and (val == 0 or err/val >= ehigh) or \
				np.isnan(val)):
				_h.SetBinContent(ibin, 0)
			else:
				_h.SetBinContent(ibin, 1)
	return _h

def get_errs2d(h2):
	h2e = h2.Clone('%s_relerrs'%h2.GetName())
	h2e.Reset()
	nbinsX, nbinsY = h2e.GetXaxis().GetNbins(), h2e.GetYaxis().GetNbins()
	for iy in range(1,nbinsY+1):
		for ix in range(1,nbinsX+1):
			val = h2.GetBinContent(ix,iy)
			err = h2.GetBinError(ix,iy)
			rerr = 0 if val==0 else err/val
			h2e.SetBinContent(ix,iy,rerr)
	return h2e

def get_vhists_from_sparse(h4):
	hacc = rpy.plotting.Hist(1000,0,1,title='acc')
	herr = rpy.plotting.Hist(1000,0,1,title='err')
	for ibin in range(0,h4.GetNbins()):
		bincoord = array('i', [0,0,0,0])
		val = h4.GetBinContent(ibin, bincoord)
		err = 0 if val==0 else h4.GetBinError(ibin)/val
		hacc.Fill(val)
		herr.Fill(err)
	return (hacc,herr)

def get_vhists(h):
	hacc = rpy.plotting.Hist(1000,0,1,title='acc')
	herr = rpy.plotting.Hist(1000,0,1,title='err')
	for ibin in range(0,h.GetNcells()):
		val = h.GetBinContent(ibin)
		err = 0 if val==0 else h.GetBinError(ibin)/val
		hacc.Fill(val)
		herr.Fill(err)
	return (hacc,herr)

def get_adjusted_edges(ax, x):
	_x = x if isinstance(x, list) else [x,x]
	bin0, bin1 = ax.FindBin(_x[0]), ax.FindBin(_x[1])
	if bin1 > bin0:
		bin1 = ax.FindBin(_x[1]-0.000001)
	_x[0], _x[1] = ax.GetBinLowEdge(bin0), ax.GetBinUpEdge(bin1)
	xmid = sum(_x)/2.0
	xwidth = _x[1]-_x[0]
	return (_x, (bin0, bin1), xmid, xwidth)


MMP_RANGES = [(0.68,0.745), (0.755,0.825), (0.835,0.9)]

class ExpData:

	def __init__(self, fn, h6dir='h6_eid_efid_nphe_hfid_pcor_badsc_mmp',
		     h6y='hbd_yield', h6w='hbd_nphe_eff', h6treff='hbd_tr_eff'):
		print('Setting up binned data for %s:%s'%(fn,h6dir))
		self.h4es = []
		self._mmpranges_ = MMP_RANGES
		self.fin_n = fn
		self.h6dir_n = h6dir
		self.h6y_n = h6y
		self.h6w_n = h6w
		self.h6tr_n = h6treff;
		self.h2es = {}
		with root_open(fn) as fin:
			self.h6w = fin['%s/%s'%(h6dir, h6w)]
			self.h6treff = fin['%s/%s'%(h6dir, h6treff)]
			self.h6y = fin['%s/%s'%(h6dir, h6y)]
			for i in range(0,self.h6w.GetNbins()):
				self.h6w.SetBinError(i, 0)
			self.h6y.Divide(self.h6w)
			if self.h6treff is not None and self.h6treff.GetEntries()>0:
				print('found h6treff')
				for i in range(0,self.h6treff.GetNbins()):
					self.h6treff.SetBinError(i, 0)
				self.h6y.Divide(self.h6treff)
			h4_dims = [dimW, dimQ2, dimCosTheta, dimPhi] = [0, 1, 3, 4]
			for i in [0,1,5]: #range(0,6):
				self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
				self.h6y.GetAxis(i).SetBit(r.TAxis.kAxisRange)
			# print("WARNING -- t cut in place!!!! ************")
			# axT = self.h6y.GetAxis(2)
			# axT.SetRange(0, axT.FindBin(2.7))
			# end t cut
			axMMP = self.h6y.GetAxis(5)
			for i,(mmp0,mmp1) in enumerate(self._mmpranges_,1):
			    b0, b1 = axMMP.FindBin(mmp0), axMMP.FindBin(mmp1)
			    axMMP.SetRange(b0,b1)
			    h4e = self.h6y.ProjectionND(4, array('i', h4_dims))
			    h4e.SetName('%s_%d'%(h4e.GetName(),i))
			    self.h4es.append(h4e)

	def __del__(self):
		print('**************\nCleaning up...\n--------------\n%s\n**************'%str(self))

	def __repr__(self):
		str_data_loc = 'data file:\t%s\ndirectory:\t%s'%(self.fin_n, self.h6dir_n)
		str_data_loc += 'h6 yields:\t%s\nh6 weights:\t%s'%(self.h6y_n, self.h6w_n)
		return str_data_loc

	def __str__(self):
		str_data_loc = 'data file:\t%s\ndirectory:\t%s'%(self.fin_n, self.h6dir_n)
		str_data_loc += 'h6 yields:\t%s\nh6 weights:\t%s'%(self.h6y_n, self.h6w_n)
		return str_data_loc

	def get_hmmp(self, W, Q2):
		axW = self.h6y.GetAxis(0)
		axQ2 = self.h6y.GetAxis(1)
		_W, (Wbin0, Wbin1), _, _ = get_adjusted_edges(axW, W)
		_Q2, (Q2bin0, Q2bin1), _, _ = get_adjusted_edges(axQ2, Q2)
		for i in [0,1,5]: #range(0,6):
			self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
			self.h6y.GetAxis(i).SetBit(r.TAxis.kAxisRange)
		axW.SetRange(Wbin0,Wbin1)
		axQ2.SetRange(Q2bin0,Q2bin1)
		hmmp = self.h6y.Projection(5)
		hmmp.SetDirectory(0)
		for i in [0,1,5]: #range(0,6):
			self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
			self.h6y.GetAxis(i).SetBit(r.TAxis.kAxisRange)
		return asrootpy(hmmp)

	def load_h2s(self, W, Q2):
		h2s, hmmps, bgw8, WQ2_str = [], [], [], None
		fitres = _, _, hmmp, bgparms, f, parms, q, stat = fit_mmp(self.get_hmmp(W, Q2), W, Q2)
		ints = [0,0,0]
		mmp0, mmp1 = self._mmpranges_[0]
		mmp_b0, mmp_b1 = hmmp.FindBin(mmp0), hmmp.FindBin(mmp1)
		ints[0] = hmmp.Integral(mmp_b0, mmp_b1)
		mmp0, mmp1 = self._mmpranges_[2]
		mmp_b0, mmp_b1 = hmmp.FindBin(mmp0), hmmp.FindBin(mmp1)
		ints[2] = hmmp.Integral(mmp_b0, mmp_b1)
		mmp0, mmp1 = self._mmpranges_[1]
		mmp_b0, mmp_b1 = hmmp.FindBin(mmp0), hmmp.FindBin(mmp1)
		X = [hmmp.GetBinCenter(mmp_bin) for mmp_bin in range(mmp_b0, mmp_b1+1)]
		ints[1] = sum([bgfun([x], bgparms) for x in X])
		#for (mmp0, mmp1),h in zip(self._mmpranges_,[hmmp, hbg, hmmp):
		#	h.GetXaxis().SetRange(h.FindBin(mmp0), h.FindBin(mmp1))
		#	ints.append(hbg.Integral())
		bgw8 = ints[1]/(ints[0]+ints[2]) if ints[0]+ints[2]>0 else 0
		for i,h4 in enumerate(self.h4es,1):
			axW = h4.GetAxis(0)
			axQ2 = h4.GetAxis(1)
			_W, (Wbin0, Wbin1), _, _ = get_adjusted_edges(axW, W)
			_Q2, (Q2bin0, Q2bin1), _, _ = get_adjusted_edges(axQ2, Q2)

			#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
			#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
			Wmid = 1000*(sum(_W)/2.0)+0.5
			Q2mid = 1000*(sum(_Q2)/2.0)+0.5
			Wwidth = 1000*(_W[1]-_W[0])+0.5
			Q2width = 1000*(_Q2[1]-_Q2[0])+0.5

			if WQ2_str is None:
				WQ2_str = '%dx%d_%dx%d'%(Wmid, Wwidth, Q2mid, Q2width)

			hname = 'h2_%d_%s'%(i, WQ2_str)

			axW.SetRange(Wbin0, Wbin1)
			axQ2.SetRange(Q2bin0, Q2bin1)

			h2 = asrootpy(h4.Projection(2,3))
			h2.Sumw2()
			h2.SetName(hname)
			h2.SetDirectory(0)
			h2s.append(h2)
		h2 = h2s[1].Clone('%s_bgsubd'%h2s[1].GetName())
		h2.Add(h2s[0], -1*bgw8)
		h2.Add(h2s[2], -1*bgw8)
		#for hmmp in hmmps:
		#	hmmp.SetName('%s_%s'%(hmmp.GetName(),h2s[1].GetName()))
		#print('adding key %s'%WQ2_str)
		self.h2es[WQ2_str] = [h2, fitres, ints, h2s]
		
	def get_h2s(self, W, Q2):
		h4 = self.h4es[0]
		axW = h4.GetAxis(0)
		axQ2 = h4.GetAxis(1)
		_W, (Wbin0, Wbin1), _, _ = get_adjusted_edges(axW, W)
		_Q2, (Q2bin0, Q2bin1), _, _ = get_adjusted_edges(axQ2, Q2)

		#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
		#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
		Wmid = 1000*(sum(_W)/2.0)+0.5
		Q2mid = 1000*(sum(_Q2)/2.0)+0.5
		Wwidth = 1000*(_W[1]-_W[0])+0.5
		Q2width = 1000*(_Q2[1]-_Q2[0])+0.5

		WQ2_str = '%dx%d_%dx%d'%(Wmid, Wwidth, Q2mid, Q2width)
		if WQ2_str not in self.h2es:
			self.load_h2s(W, Q2)
		return self.h2es[WQ2_str]


class SimData:

	def __init__(self):
		self.h6s = []
		self.h4s = []
		self.fns = []
		self._mmpranges_ = MMP_RANGES

	def __del__(self):
		print('**************\nCleaning up...\n--------------\n%s\n**************'%str(self))

	def __repr__(self):
		str_data_loc = 'simulated data files:\n%s'%('\n\t'.join(self.fns))
		return str_data_loc

	def __str__(self):
		str_data_loc = 'simulated data files:\n\t%s'%('\n\t'.join(self.fns))
		return str_data_loc

	def add_sim(self, fn, h6tdir='h6thrown', h6rdir='h6recon',
	            h6tname='hbd_yield', h6rname='hbd_yield', h6rfn=None,
	            shapename='hbd_shape_norm_Q2'):
		with root_open(fn) as fin:
			h6t = fin['%s/%s'%(h6tdir,h6tname)]
			h6r = None
			if h6rfn is None:
				h6r = fin['%s/%s'%(h6rdir,h6rname)]
			else:
				with root_open(h6rfn) as h6rfin:
					h6r = h6rfin['%s/%s'%(h6rdir,h6rname)]
			# h6t_w8s = fin['%s/%s'%(h6tdir,shapename)]
			# h6r_w8s = fin['%s/%s'%(h6rdir,shapename)]
			# for i in range(0, h6t_w8s.GetNbins()):
			# 	h6t_w8s.SetBinError(i,0)
			# for i in range(0, h6r_w8s.GetNbins()):
			# 	h6r_w8s.SetBinError(i,0)
			# h6t.Divide(h6t_w8s)
			# h6r.Divide(h6r_w8s)
			dims = array('i', [0,1,3,4])
			for iax in [0,1,5]: #range(0,6):
				for h6 in [h6r]: #[h6t, h6r]:
					ax = h6.GetAxis(iax)
					nbins = ax.GetNbins()
					ax.SetRange(1, nbins)
					ax.SetBit(r.TAxis.kAxisRange)
			# print("WARNING -- t cut in place!!!! ************")
			# axT = h6r.GetAxis(2)
			# axT.SetRange(0, axT.FindBin(2.7))
			# end t cut
			axMMp = h6r.GetAxis(5)
			mmp_b0 = axMMp.FindBin(self._mmpranges_[1][0])
			mmp_b1 = axMMp.FindBin(self._mmpranges_[1][1])
			axMMp.SetRange(mmp_b0, mmp_b1)
			h4t = h6t.Projection(4, dims)
			h4r = h6r.Projection(4, dims)
			h4t.SetName('h4t_%d'%(len(self.h4s)+1))
			h4r.SetName('h4r_%d'%(len(self.h4s)+1))
			#h4a = h4r.Clone('h4a_%d'%(len(self.h4s)+1))
			#h4a.Divide(h4t)
			#self.h4s.append((h4t, h4r, h4a))
			
			self.h4s.append((h4t, h4r))
		self.fns.append(fn)

	def get_acc2d(self, W, Q2, mask=None, lo_acc=None):
#		_W = W if isinstance(W, list) else [W,W]
#		_Q2 = Q2 if isinstance(Q2, list) else [Q2,Q2]
#		h4tt = self.h4s[0][0]
#		axW, axQ2 = h4tt.GetAxis(0), h4tt.GetAxis(1)
#		Wbin0, Wbin1 = axW.FindBin(_W[0]), axW.FindBin(_W[1])
#		Q2bin0, Q2bin1 = axQ2.FindBin(_Q2[0]), axQ2.FindBin(_Q2[1])
#		if Wbin1 > Wbin0:
#			Wbin1 = axW.FindBin(_W[1]-0.000001)
#		if Q2bin1 > Q2bin0:
#			Q2bin1 = axQ2.FindBin(_Q2[1]-0.000001)
#		_W[0], _W[1] = axW.GetBinLowEdge(Wbin0), axW.GetBinUpEdge(Wbin1)
#		_Q2[0], _Q2[1] = axQ2.GetBinLowEdge(Q2bin0), axQ2.GetBinUpEdge(Q2bin1)
#		#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
#		#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
#		Wmid = 1000*(sum(_W)/2.0)+0.5
#		Q2mid = 1000*(sum(_Q2)/2.0)+0.5
#		Wwidth = 1000*(_W[1]-_W[0])+0.5
#		Q2width = 1000*(_Q2[1]-_Q2[0])+0.5
		Wmid, Q2mid, Wwidth, Q2width = None, None, None, None

		h2tr = []
		for i,(h4t,h4r) in enumerate(self.h4s,1):
			axW = h4t.GetAxis(0)
			axQ2 = h4t.GetAxis(1)
			_W, (Wbin0, Wbin1), _, _ = get_adjusted_edges(axW, W)
			_Q2, (Q2bin0, Q2bin1), _, _ = get_adjusted_edges(axQ2, Q2)

			#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
			#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
			Wmid = 1000*(sum(_W)/2.0)+0.5
			Q2mid = 1000*(sum(_Q2)/2.0)+0.5
			Wwidth = 1000*(_W[1]-_W[0])+0.5
			Q2width = 1000*(_Q2[1]-_Q2[0])+0.5
			h4t_axW, h4r_axW = h4t.GetAxis(0), h4r.GetAxis(0)
			h4t_axQ2, h4r_axQ2 = h4t.GetAxis(1), h4r.GetAxis(1)

			h4t_axW.SetRange(Wbin0, Wbin1)
			h4t_axQ2.SetRange(Q2bin0, Q2bin1)
			h4r_axW.SetRange(Wbin0, Wbin1)
			h4r_axQ2.SetRange(Q2bin0, Q2bin1)

			h2t = asrootpy(h4t.Projection(2,3))
			h2t.SetName('h2t%d_%dx%d_%dx%d'%(i, Wmid, Wwidth, Q2mid, Q2width))
			h2r = asrootpy(h4r.Projection(2,3))
			h2r.SetName('h2r%d_%dx%d_%dx%d'%(i, Wmid, Wwidth, Q2mid, Q2width))
			h2a = h2r.Clone('h2a%d_%dx%d_%dx%d'%(i, Wmid, Wwidth, Q2mid, Q2width))
			h2a.Divide(h2t)

#			print(i, self.fns[i-1])
#			print(Wbin0, Wbin1)
#			print(Q2bin0, Q2bin1)
#			print(h4t.GetNbins(),h2t.GetEntries())
#			print(h4r.GetNbins(),h2r.GetEntries())
			h2tr.append((h2t,h2r,h2a,get_mask(h2a,ehigh=mask,vlow=lo_acc),None))
			#h2tr.append((h2t,h2r,h2a,get_mask(h2a,vlow=mask),None))
		h2t_all = asrootpy(h2tr[0][0].Clone('h2t_%dx%d_%dx%d'%(Wmid, Wwidth, Q2mid, Q2width)))
		h2t_all.Reset()
		h2r_all = asrootpy(h2tr[0][1].Clone('h2r_%dx%d_%dx%d'%(Wmid, Wwidth, Q2mid, Q2width)))
		h2r_all.Reset()
		nbinsX, nbinsY = h2t_all.GetXaxis().GetNbins(), h2t_all.GetYaxis().GetNbins()
		for iy in range(1,nbinsY+1):
			for ix in range(1,nbinsX+1):
				tvals, tw8s = [], []
				rvals, rw8s = [], []
				for (h2t, h2r, _, _, _) in h2tr:
					if h2t.GetEntries()==0:
						break
					for h2, (vals, w8s) in zip([h2t,h2r],[(tvals,tw8s),(rvals,rw8s)]):
						v = h2.GetBinContent(ix,iy)
						e = h2.GetBinError(ix,iy)
						w8 = 1/(e*e) if e>0 else 0
						vals.append(v)
						w8s.append(w8)
	#						 if (iy==3 and ix==1):
	#							 print('%s:\t%.3f\t%.3f\t%.3f'%(h2.GetName(),v,e,w8))
				tw8s = None if sum(tw8s)==0 else tw8s
				rw8s = None if sum(rw8s)==0 else rw8s
				tw8ed_mean, tsum_of_w8s = np.ma.average(tvals, weights=tw8s, returned=True)
				rw8ed_mean, rsum_of_w8s = np.ma.average(rvals, weights=rw8s, returned=True)
				h2t_all.SetBinContent(ix,iy,tw8ed_mean)
				h2r_all.SetBinContent(ix,iy,rw8ed_mean)
				terr = 0 if tsum_of_w8s==0 else 1/math.sqrt(tsum_of_w8s)
				rerr = 0 if rsum_of_w8s==0 else 1/math.sqrt(rsum_of_w8s)
				h2t_all.SetBinError(ix,iy,terr)
				h2r_all.SetBinError(ix,iy,rerr)
	#				 if (iy==3 and ix==1):
	#					 print(tvals)
	#					 print(tw8s)
	#					 print(rvals)
	#					 print(rw8s)
		h2a_all = asrootpy(h2r_all.Clone('h2a_%dx%d_%dx%d'%(Wmid, Wwidth, Q2mid, Q2width)))
		h2a_all.Divide(h2t_all)
		hmask = get_mask(h2a_all,ehigh=mask,vlow=lo_acc)
		#hmask = get_mask(h2a_all,vlow=mask)
		h2a_all_masked = h2a_all.Clone('%s_masked'%h2a_all.GetName())
		h2a_all_masked.Divide(hmask)
		return [(h2t_all,h2r_all,h2a_all,hmask,h2a_all_masked)]+h2tr


if __name__ == '__main__':
	W, Q2 = 1.8, [2.75, 3.25]
	if False:
		e1f_binned = ExpData('/home/ephelps/projects/phys-ana-omega/e1f6/test/e16_exp_h6_top1.root')
		print(e1f_binned)
		#W, Q2 = 2.01, [1.35, 1.55]
		h2s = e1f_binned.get_h2s(W, Q2)
		for h2 in h2s:
			h2s[0].Print()

	e1f_sim = SimData()
	dirin = '/home/ephelps/projects/phys-ana-omega/e1f6/test'
	fns = ['e16_sim_skim_top1_ho.root', 'e16_sim_skim_top1_321_ho.root', 'e16_sim_skim_top1_326_ho.root']
	#fns = ['e1f_sim_skim_top1_ho.root', 'e1f_sim_skim_top1_321_ho.root', 'e1f_sim_skim_top1_326_ho.root']
	#fns = ['e16_sim_skim_top1_321_ho.root', 'e16_sim_skim_top1_ho.root', 'e16_sim_skim_top1_326_ho.root']
	#fns = ['e1f_sim_skim_top1_321_ho.root', 'e1f_sim_skim_top1_ho.root', 'e1f_sim_skim_top1_326_ho.root']
	for fn in fns:
		e1f_sim.add_sim('%s/%s'%(dirin,fn))
	h2ass = e1f_sim.get_acc2d(W, Q2, 0.15)
	for h2as in h2ass:
		for h2a in h2as:
			print(h2a)
