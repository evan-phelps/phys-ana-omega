import ROOT as r
import rootpy as rpy
from rootpy import asrootpy
from rootpy.io import root_open
from array import array
import math

MASS_P = 0.93827203

		
def bgfun(x, p):
	pol2 = p[0]+p[1]*x[0]+p[2]*x[0]*x[0]
	#print(p[3], x[0], p[4])
	drop = 1 - math.erf((x[0]-p[3])/p[4])
#	 if (x[0] > 0.5 and x[0] < 0.6) or (x[0] > 0.7 and x[0] < 0.92): r.TF1.RejectPoint()
#	if x[0] > 0.7 and x[0] < 0.89: r.TF1.RejectPoint()
	if x[0] > 0.7 and x[0] < 0.85: r.TF1.RejectPoint()
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

def fit_mmp(h, W, Q2, fout=None):
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
	# ######### get W,Q2 bin information
	#W, Q2 = h.GetName().split('_')[1:]
	#W = float(W)/1000.0
	#Q2 = float(Q2)/1000.0
	
	wlow, whigh = W-0.010, W+0.010  # W bins are 20 MeV
	# ######### calculate MMp phase space edge range
	mmpax = h.GetXaxis()
	x0 = math.sqrt(wlow**2+MASS_P**2-2*wlow*MASS_P)
	x1 = math.sqrt(whigh**2+MASS_P**2-2*whigh*MASS_P)
	# ######### setup fit functions
	fgaus = r.TF1('fgaus', gausdrop, 0.6, 0.95, 5) #0.4, 1.2, 5)
	fgaus.SetParameters(r.Double(20), r.Double(0.783), r.Double(0.20), r.Double((x1+x0)*0.5), r.Double(x1-x0))
	fgaus.SetParLimits(0, 0.01, 1e4)
	fgaus.SetParLimits(1, 0.773, 0.793)
	fgaus.SetParLimits(2, 0.011, 0.035)
	fgaus.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
	fgaus.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
	fbg = r.TF1('fbg', bgfun, 0.4, 1.2, 5)
	fbg.SetParameters(r.Double(0), r.Double(3000), r.Double(1000), r.Double((x1+x0)*0.5), r.Double(x1-x0))
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
			fsigbg = r.TF1('fsigbg', fitfun, 0.4, 1.2, 8)
#			 fsigbg.SetParameters(r.Double(0), r.Double(0), r.Double(0.02), r.Double((x1+x0)*0.5), r.Double(x1-x0), r.Double(20), r.Double(0.783), r.Double(0.022))
			p0, p1, p2 = fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2)
			fsigbg.SetParameters(fbg.GetParameter(0), fbg.GetParameter(1), fbg.GetParameter(2),
								 fbg.GetParameter(3), fbg.GetParameter(4),
								 fgaus.GetParameter(0), fgaus.GetParameter(1), fgaus.GetParameter(2))
			fsigbg.SetParLimits(0, 0.85*p0, 1.15*p0)
			fsigbg.SetParLimits(1, 0.85*p1, 1.15*p1)
			fsigbg.SetParLimits(2, 0.85*p2, 1.15*p2)
#			 fsigbg.SetParLimits(2, 0.0001, 1e9)
			fsigbg.SetParLimits(3,x0+(x1-x0)/4,x1-(x1-x0)/4)
			fsigbg.SetParLimits(4,0.15*(x1-x0), 2*(x1-x0))
			#fsigbg.SetParLimits(3,0.65,2)
			#fsigbg.SetParLimits(4,0.03, 0.2)
			fsigbg.SetParLimits(5, 0.1*sigint, 2*sigint)
			fsigbg.SetParLimits(6, 0.773, 0.793)
			fsigbg.SetParLimits(7, 0.011, 0.035)
			# ######### fit signal and background
			h.Fit(fsigbg, '', 'goff', 0.6, 0.95) #0.4, 1.2)
			if True:  #r.gMinuit.fCstatu.startswith('CONV'):
				q = fsigbg.GetChisquare()/fsigbg.GetNDF() if fsigbg.GetNDF() > 0 else -1
				result += [W,Q2]
				result.append(h),
				result.append(tuple([fbg.GetParameter(ipar) for ipar in range(0,5)]))
				result.append(h.GetListOfFunctions()[0])
				result.append(tuple([fsigbg.GetParameter(ipar) for ipar in range(0,8)]))
				result.append(q)
				result.append(r.gMinuit.fCstatu)
				if fout is not None: fout.WriteObject(h, h.GetName())
	return tuple(result) if result is not None else None

class ExpData:

	def __init__(self, fn, h6dir='h6_eid_efid_nphe_hfid_pcor_badsc_mmp',
		     h6y='hbd_yield', h6w='hbd_nphe_eff'):
		print('Setting up binned data for %s:%s'%(fn,h6dir))
		self.h4es = []
		self._mmpranges_ = [(0.6,0.6999), (0.7,0.8499), (0.85,0.9499)]
		self.fin_n = fn
		self.h6dir_n = h6dir
		self.h6y_n = h6y
		self.h6w_n = h6w
		self.h2es = {}
		with root_open(fn) as fin:
			self.h6w = fin['%s/%s'%(h6dir, h6w)]
			self.h6y = fin['%s/%s'%(h6dir, h6y)]
			self.h6y.Divide(self.h6w)
			h4_dims = [dimW, dimQ2, dimCosTheta, dimPhi] = [0, 1, 3, 4]
			for i in range(0,6):
				self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
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

	def _gethsubd_(self, h, niter=5):
		s = r.TSpectrum()
		hbg = asrootpy(s.Background(h, niter))
		hbg.set_name('%s_bg'%h.get_name())
		hbgsubd = h.clone('%s_bgsubd'%h.get_name())
		hbgsubd.add(hbg, -1)
		return (hbgsubd, hbg, h)

	def get_hmmp(self, W, Q2):
		_W = W if isinstance(W, list) else [W,W]
		_Q2 = Q2 if isinstance(Q2, list) else [Q2,Q2]
		axW, axQ2 = self.h6y.GetAxis(0), self.h6y.GetAxis(1)
		Wbin0, Wbin1 = axW.FindBin(_W[0]), axW.FindBin(_W[1])
		Q2bin0, Q2bin1 = axQ2.FindBin(_Q2[0]), axQ2.FindBin(_Q2[1])
		if Wbin1 > Wbin0:
		    Wbin1 = axW.FindBin(_W[1]-0.000001)
		if Q2bin1 > Q2bin0:
		    Q2bin1 = axQ2.FindBin(_Q2[1]-0.000001)
		_W[0], _W[1] = axW.GetBinLowEdge(Wbin0), axW.GetBinUpEdge(Wbin1)
		_Q2[0], _Q2[1] = axQ2.GetBinLowEdge(Q2bin0), axQ2.GetBinUpEdge(Q2bin1)
		for i in range(0,6):
			self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
		axW.SetRange(Wbin0,Wbin1)
		axQ2.SetRange(Q2bin0,Q2bin1)
		hmmp = self.h6y.Projection(5)
		hmmp.SetDirectory(0)
		for i in range(0,6):
			self.h6y.GetAxis(i).SetRange(1, self.h6y.GetAxis(i).GetNbins())
		return asrootpy(hmmp)

	def load_h2s(self, W, Q2, bg_niter=5):
		h2s, hmmps, bgw8, WQ2_str = [], [], [], None
		#hmmps = list(self._gethsubd_(self.get_hmmp(W, Q2), niter=bg_niter))
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
		bgw8 = ints[1]/(ints[0]+ints[2])
		for i,h4 in enumerate(self.h4es,1):
			_W = W if isinstance(W, list) else [W,W]
			_Q2 = Q2 if isinstance(Q2, list) else [Q2,Q2]
			axW, axQ2 = h4.GetAxis(0), h4.GetAxis(1)
			Wbin0, Wbin1 = axW.FindBin(_W[0]), axW.FindBin(_W[1])
			Q2bin0, Q2bin1 = axQ2.FindBin(_Q2[0]), axQ2.FindBin(_Q2[1])
			if Wbin1 > Wbin0:
			    Wbin1 = axW.FindBin(_W[1]-0.000001)
			if Q2bin1 > Q2bin0:
			    Q2bin1 = axQ2.FindBin(_Q2[1]-0.000001)
			_W[0], _W[1] = axW.GetBinLowEdge(Wbin0), axW.GetBinUpEdge(Wbin1)
			_Q2[0], _Q2[1] = axQ2.GetBinLowEdge(Q2bin0), axQ2.GetBinUpEdge(Q2bin1)
			#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
			#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
			Wmid = 1000*(sum(_W)/2.0)+0.5
			Q2mid = 1000*(sum(_Q2)/2.0)+0.5
			Wwidth = 1000*(_W[1]-_W[0])+0.5
			Q2width = 1000*(_Q2[1]-_Q2[0])+0.5

			if WQ2_str is None:
				WQ2_str = '%dx%d_%dx%d_iter%d'%(Wmid, Wwidth, Q2mid, Q2width, bg_niter)

			hname = 'h2_%d_%s'%(i, WQ2_str)

			axW.SetRange(Wbin0, Wbin1)
			axQ2.SetRange(Q2bin0, Q2bin1)

			h2 = asrootpy(h4.Projection(2,3))
			h2.SetName(hname)
			h2.SetDirectory(0)
			h2s.append(h2)
		h2 = h2s[1].Clone('%s_bgsubd'%h2s[1].GetName())
		h2.Add(h2s[0], -1*bgw8)
		h2.Add(h2s[2], -1*bgw8)
		#for hmmp in hmmps:
		#	hmmp.SetName('%s_%s'%(hmmp.GetName(),h2s[1].GetName()))
		print('adding key %s'%WQ2_str)
		self.h2es[WQ2_str] = [h2, fitres, ints, h2s]
		
	def get_h2s(self, W, Q2, bg_niter=5):
		_W = W if isinstance(W, list) else [W,W]
		_Q2 = Q2 if isinstance(Q2, list) else [Q2,Q2]
		h4 = self.h4es[0]
		axW, axQ2 = h4.GetAxis(0), h4.GetAxis(1)
		Wbin0, Wbin1 = axW.FindBin(_W[0]), axW.FindBin(_W[1])
		Q2bin0, Q2bin1 = axQ2.FindBin(_Q2[0]), axQ2.FindBin(_Q2[1])
		if Wbin1 > Wbin0:
		    Wbin1 = axW.FindBin(_W[1]-0.000001)
		if Q2bin1 > Q2bin0:
		    Q2bin1 = axQ2.FindBin(_Q2[1]-0.000001)
		_W[0], _W[1] = axW.GetBinLowEdge(Wbin0), axW.GetBinUpEdge(Wbin1)
		_Q2[0], _Q2[1] = axQ2.GetBinLowEdge(Q2bin0), axQ2.GetBinUpEdge(Q2bin1)
		#print('W: %.3f-%.3f, %d-%d'%(_W[0],_W[1],Wbin0,Wbin1))
		#print('Q2: %.3f-%.3f, %d-%d'%(_Q2[0],_Q2[1],Q2bin0,Q2bin1))
		Wmid = 1000*(sum(_W)/2.0)+0.5
		Q2mid = 1000*(sum(_Q2)/2.0)+0.5
		Wwidth = 1000*(_W[1]-_W[0])+0.5
		Q2width = 1000*(_Q2[1]-_Q2[0])+0.5

		WQ2_str = '%dx%d_%dx%d_iter%d'%(Wmid, Wwidth, Q2mid, Q2width, bg_niter)
		if WQ2_str not in self.h2es:
			self.load_h2s(W, Q2, bg_niter)
		return self.h2es[WQ2_str]


if __name__ == '__main__':
	e1f_binned = ExpData('/home/ephelps/projects/phys-ana-omega/e1f6/test/e1f_exp_h6_top1.root')
	print(e1f_binned)
	h2s = e1f_binned.get_h2s(2.01, [1.35,1.55])
	h2s[0].Print()

