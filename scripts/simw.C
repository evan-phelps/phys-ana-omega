TF1 *dsimw_fsim = 0;
TF1 *dsimw_fexp = 0;
TF1 *dsimw_simw = 0;
TF1 *dsimw_simw_gt4 = 0;
Double_t dsimw(Double_t *x, Double_t *par)
{
  Double_t retval = 1;
  if (!dsimw_fsim) {
    dsimw_fsim = new TF1("dsimw_fsim","expo(0)+pol1(2)",0,10);
    dsimw_fsim->SetParameters(par[4],par[5],par[6],par[7]);
  }
  if (!dsimw_fexp) {
    dsimw_fexp = new TF1("dsimw_fexp","expo(0)+pol1(2)",0,10);
    dsimw_fexp->SetParameters(par[0],par[1],par[2],par[3]);
  }
  if (!dsimw_simw_gt4) {
    dsimw_simw_gt4 = new TF1("dsimw_fexp","expo",0,10);
    dsimw_simw_gt4->SetParameters(1.95,-4.91489e-1);
  }
  if (x[0]<4) {
    Double_t vsim = dsimw_fsim->Eval(x[0]);
    Double_t vexp = dsimw_fexp->Eval(x[0]);
    if (vsim>0 && vexp>0) retval = vexp/vsim;
    else if (vexp==0) retval = vsim;
  } else {
    retval = dsimw_simw_gt4->Eval(x[0]);
  }
  return retval;
}

TF1 *dsimw_fsimw() {
  if (!dsimw_simw) {
    dsimw_simw = new TF1("dsimw_simw",&dsimw,0,8,8);
    dsimw_simw->SetParameters(-4.55,-3.99e-1,-3.689e-3,4.6016e-4,-3.835,-3.04248,1.277e-3,-2.458e-4);
  }
  return dsimw_simw;
}

Double_t simevtw(Double_t t1) {
  return dsimw_fsimw()->Eval(t1);
}
