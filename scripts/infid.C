gROOT->ProcessLine(".L eff.C");

bool infid(double p, double theta, double phi, int id = 11, double tness = 1) {
  return Fid::Instance()->InFid(p,theta,phi,id,tness);
}

bool infideff(double p, double theta, double phi, double thresh = 0.25, int id = 11, double tness = 1) {
  bool retval = false;
  if (Fid::Instance()->InFid(p,theta,phi,id,tness)) {
    if (eff(p,theta,phi,id)>thresh) retval = true;
  }
  return retval;
}
