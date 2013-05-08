import math as m

from ROOT import TF1


class FitSchemeXsectInt():

    def __init__(self, desc):
        self.doptions = ''
        self.sig = TF1('fsig', self.d_sig, 0.4, 2, 5)
        self.bg = TF1('fbg', self.d_pol4, 0.4, 2, 9)
        self.fn = TF1('fbgsig', self.d_pol4gaus, 0.4, 2, 10)
        # adjustable parameters
        self.bg.range = [0.4, 1.1]
        self.bg.skip = [[0.74, 0.85], [0.5, 0.6]]
        self.bg.setparms = {}
        self.bg.fixparms = {5: 2, 6: 2.1, 3: 0, 4: 0}   # pol2
        self.sig.range = [0.74, 0.85]
        self.sig.setparms = {1: 0.783, 2: 0.020}
        self.sig.fixparms = {3: 2, 4: 2.1}
        self.fn.range = [0.4, 1.1]
        self.fn.skip = [[0.5, 0.6]]
        self.fn.setparms = {}
        self.fn.fixparms = {8: 2, 9: 2.1}

    def stepfactor(self, x, x0, x1):
        return 1 if x < x0 else (0 if x > x1 else 1-1/(x1-x0)*(x-x0))

    def d_sig(self, x, par):
        step = self.stepfactor(x[0], par[3], par[4])
        gaus = par[0]*m.exp(-0.5*pow(((x[0]-par[1])/par[2]), 2))
        return step*gaus

    def d_pol4(self, x, par):
        for skip in self.bg.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0], par[5], par[6])
        retval = step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                       + par[3]*x[0]**3 + par[4]*x[0]**4)
        return 0 if retval < 0 else retval

    def d_pol4gaus(self, x, par):
        for skip in self.fn.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0], par[8], par[9])
        return step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                     + par[3]*x[0]**3 + par[4]*x[0]**4
                     + par[5]*m.exp(-0.5*pow(((x[0]-par[6])/par[7]), 2)))

    def Setup(self, hist):
        print('FitSchemeXsectInt.Setup()')
        for k, v in self.bg.setparms.items():
            self.bg.SetParameter(k, v)
        for k, v in self.bg.fixparms.items():
            self.bg.FixParameter(k, v)
        hist.Fit(self.bg, self.doptions, '',
                 self.bg.range[0], self.bg.range[1])
        bg = self.bg.Clone('bgtmp')
        for k, v in self.bg.fixparms.items():
            bg.FixParameter(k, v)
        h = hist.Clone('htmp')
        h.Add(bg, -1)
        h.Draw()
        for k, v in self.sig.setparms.items():
            self.sig.SetParameter(k, v)
        for k, v in self.sig.fixparms.items():
            self.sig.FixParameter(k, v)
        h.Fit(self.sig, self.doptions, '',
              self.sig.range[0], self.sig.range[1])
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(0, 5), bg.GetParameters())]
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(5, 8), self.sig.GetParameters())]
        for k, v in self.fn.fixparms.items():
            self.fn.FixParameter(k, v)
        return hist

    def BgSubtract(self, hist):
        print('FitSchemeXsectInt.BgSubtract()')
        return hist
