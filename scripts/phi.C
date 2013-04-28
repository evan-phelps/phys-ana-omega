const double r2d = 57.2957795130823229;
const double d2r = 1/r2d;

double phideg(double phir) {
  double ret = phir*r2d;
  if (ret < -30) ret += 360;
  return ret;
}

double phirad(double phir) {
  double r30 = 30*d2r;
  double r360 = 360*d2r;
  double ret = phir < -r30 ? phir+r360 : phir;
  return ret;
}
