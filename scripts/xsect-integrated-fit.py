import math as m

from ROOT import TF1
from ROOT import TFile


class FitSchemeXsectInt:

    def __init__(self, desc):
        # adjustable parameters
        self.drawrange = [0.4, 2]
        self.bg.nparms = 7
        self.bg.range = [0.4, 1.1]
        self.bg.skip = [[0.74, 0.85], [0.5, 0.6]]
        self.bg.setparms = {}
        self.bg.fixparms = {5: 2, 6: 2.1, 3: 0, 4: 0}
        self.sig.nparms = 5
        self.sig.range = [0.74, 0.85]
        self.sig.setparms = {1: 0.783, 2: 0.020}
        self.sig.fixparms = {3: 2, 4: 2.1}
        self.fn.nparms = 10
        self.fn.range = [0.4, 1.1]
        self.fn.skip = [[0.5, 0.6]]
        self.fn.setparms = {}
        self.fn.fixparms = {3: 0, 4: 0, 8: 2, 9: 2.1}
        self.doptions = 'N'
        self.bg = TF1('fbg', self.d_pol4,
                      self.drawrange[0], self.drawrange[1],
                      self.bg.nparms)
        self.sig = TF1('fsig', self.d_sig,
                       self.drawrange[0], self.drawrange[1],
                       self.sig.nparms)
        self.fn = TF1('fbgsig', self.d_pol4gaus,
                      self.drawrange[0], self.drawrange[1],
                      self.fn.nparms)

    def stepfactor(self, x, x0, x1):
        return 1 if x < x0 else (0 if x > x1 else 1-1/(x1-x0)*(x-x0))

    def d_sig(self, x, par):
        step = self.stepfactor(x[0], par[3], par[4])
        gaus = par[0]*m.exp(-0.5*pow(((x[0]-par[1])/par[2]), 2))
        return step*gaus

    def d_pol4(self, x, par):

        # reject points in ranges not modeled by bg function
        for skip in self.bg.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0], par[5], par[6])
        retval = step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                       + par[3]*x[0]**3 + par[4]*x[0]**4)
        return 0 if retval < 0 else retval

    def d_pol4gaus(self, x, par):

        # reject points in ranges not modeled by function
        for skip in self.fn.skip:
            if x[0] > skip[0] and x[0] < skip[1]:
                TF1.RejectPoint()
                break
        step = self.stepfactor(x[0], par[8], par[9])
        return step*(par[0] + par[1]*x[0] + par[2]*x[0]**2
                     + par[3]*x[0]**3 + par[4]*x[0]**4
                     + par[5]*m.exp(-0.5*pow(((x[0]-par[6])/par[7]), 2)))

    def Setup(self, hist):

        # estimate background subtraction
        for pidx, pval in self.bg.setparms.items():
            self.bg.SetParameter(pidx, pval)
        for pidx, pval in self.bg.fixparms.items():
            self.bg.FixParameter(pidx, pval)
        hist.Fit(self.bg, self.doptions, '',
                 self.bg.range[0], self.bg.range[1])
        bg = self.bg.Clone('bgtmp')
        for pidx, pval in self.bg.fixparms.items():
            bg.FixParameter(pidx, pval)
        h = hist.Clone('htmp')
        h.Add(bg, -1)

        # estimate signal function parameters
        for pidx, pval in self.sig.setparms.items():
            self.sig.SetParameter(pidx, pval)
        for pidx, pval in self.sig.fixparms.items():
            self.sig.FixParameter(pidx, pval)
        h.Fit(self.sig, self.doptions, '',
              self.sig.range[0], self.sig.range[1])

        # set initial parameters of bg+sig function
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(0, 5), bg.GetParameters())]
        [self.fn.SetParameter(i, v) for i, v
            in zip(range(5, 8), self.sig.GetParameters())]
        for pidx, pval in self.fn.fixparms.items():
            self.fn.FixParameter(pidx, pval)
        return hist

    def Fit(self, hist):
        hist.Fit(self.fn, self.doptions, '',
                 self.fn.range[0], self.fn.range[1])

    def Decorate(self, hist):
        fns = [self.bg.Clone('fbg'), self.sig.Clone('fsig'),
               self.fn.Clone('fbgsig')]
        for fn in fns:
            fn.SetRange(self.drawrange[0], self.drawrange[1])
            hist.GetListOfFunctions().Add(fn)


def main():
    fs = FitSchemeXsectInt('a desc')
    fin = TFile('/home/ephelps/analysis/sandbox/h3maker-hn.root')
    h = fin.Get('hs0').GetHists()[12]
    fs.Setup(h)
    fs.Fit(h)
    fs.Decorate(h)
    h.Draw()

if __name__ == '__main__':
    main()
