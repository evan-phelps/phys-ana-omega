class Fitter:

    def stepfactor(self,x,x0,x1):
        if x < x0: return 1
        elif x > x1: return 0
        else return 1 - 1/(x1-x0) * (x-x0)

    def d_sig(self,x,par):
        return stepfactor(x[0],par[3],par[4])*par[0]*exp(-0.5*pow(((x[0]-par[1])/par[2]),2))

    def d_pol4(self,x,par):
        if par[5] < par[6]:
            if x[0] > par[5] and x[0] < par[6] or (x[0] > 0.5 && x[0] < 0.6): TF1.RejectPoint()
        retval = (par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*x[0]*x[0]*x[0] + par[4]*x[0]*x[0]*x[0]*x[0])*stepfactor(x[0],par[7],par[8])
        if retval < 0: return 0
        return retval

