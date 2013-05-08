import math as m

from ROOT import TF1


class FitSchemeXsectInt():

    def __init__(self, desc):
        self.doptions = ''
        self.sig = TF1('fsig', self.d_sig, 0.4, 2, 5)
        self.bg = TF1('fbg', self.d_pol4, 0.4, 2, 9)
        self.fn = TF1('fbgsig', self.d_pol4gaus, 0.4, 2, 10)
        # adjustable parameters
        # self.bg.range = [0.4, 1.1]
        # self.bg.skip = [[0.74, 0.85], [0.5, 0.6]]
        # self.bg.setparms = []
        # self.bg.fixparms = []
        # self.sig.range = [0.74, 0.85]
        # self.sig.setparms = []
        # self.sig.fixparms = []
        # self.fn.range = [0.4, 1.1]
        # self.fn.skip = [[0.5, 0.6]]
        # self.fn.setparms = []
        # self.fn.fixparms = []

    def stepfactor(self, x, x0, x1):
        return 1 if x < x0 else (0 if x > x1 else 1-1/(x1-x0)*(x-x0))

    def d_sig(self, x, par):
        step = self.stepfactor(x[0], par[3], par[4])
        gaus = par[0]*m.exp(-0.5*pow(((x[0]-par[1])/par[2]), 2))
        return step*gaus

    def d_pol4(self, x, par):
        if par[5] < par[6]:
            if x[0] > par[5] and x[0] < par[6] or (x[0] > 0.5 and x[0] < 0.6):
                TF1.RejectPoint()
        step = self.stepfactor(x[0], par[7], par[8])
        retval = step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                       + par[3]*x[0]**3 + par[4]*x[0]**4)
        return 0 if retval < 0 else retval

    def d_pol4gaus(self, x, par):
        step = self.stepfactor(x[0], par[8], par[9])
        return step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                     + par[3]*x[0]**3 + par[4]*x[0]**4
                     + par[5]*m.exp(-0.5*pow(((x[0]-par[6])/par[7]), 2)))

    def Setup(self, hist):
        print('FitSchemeXsectInt.Setup()')
        self.bg.FixParameter(3, 0)  # pol4 to pol2 by..
        self.bg.FixParameter(4, 0)  # ..fixing p3,p4 to 0
        self.bg.FixParameter(5, 0.74)
        self.bg.FixParameter(6, 0.85)
        self.bg.FixParameter(7, 2)
        self.bg.FixParameter(8, 2.1)
        hist.Fit(self.bg, self.doptions, '', 0.4, 1.1)
        bg = self.bg.Clone('bgtmp')
        bg.FixParameter(5, 0)
        bg.FixParameter(6, 0)
        bg.FixParameter(7, 2)
        bg.FixParameter(8, 2.1)
        h = hist.Clone('htmp')
        h.Add(bg, -1)
        h.Draw()
        self.sig.SetParameter(1, 0.783)
        self.sig.SetParameter(2, 0.02)
        self.sig.FixParameter(3, 2)
        self.sig.FixParameter(4, 2.1)
        h.Fit(self.sig, self.doptions, '', 0.74, 0.823)
        [self.fn.SetParameter(i, v) for i, v in zip(range(0, 5), bg.GetParameters())]
        [self.fn.SetParameter(i, v) for i, v in zip(range(5, 8), self.sig.GetParameters())]
        self.fn.FixParameter(8, 2)
        self.fn.FixParameter(9, 2.1)
        self.fn.FixParameter(6, 0.783)
        self.fn.FixParameter(7, self.sig.GetParameter(2))
        return hist

    def BgSubtract(self, hist):
        print('FitSchemeXsectInt.BgSubtract()')
        return hist
