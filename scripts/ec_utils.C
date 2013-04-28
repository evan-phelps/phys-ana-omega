#include <TMath.h>

using namespace TMath;

void GetUVW(double xyz[3], double uvw[3]) {
  enum { X, Y, Z };
  enum { U, V, W };
  Float_t phi, ec_phi, ec_the, tgrho, sinrho, cosrho;
  Float_t ylow, yhi, xi, yi, zi;
  ec_the = 0.4363323;
  ylow = -182.974;
  yhi = 189.956;
  tgrho = 1.95325;
  sinrho = 0.8901256;
  cosrho = 0.455715;
  //----------------
  phi = ATan2(xyz[Y], xyz[X]) * 57.29578;
  if (phi < 0.)
    phi += 360.;
  phi = phi + 30.;
  if (phi > 360.)
    phi -= 360.;
  ec_phi = 1.0471975 * int(phi / 60.);
  //----------------
  float rot11 = cos(ec_the) * cos(ec_phi);
  float rot12 = -sin(ec_phi);
  float rot13 = sin(ec_the) * cos(ec_phi);
  float rot21 = cos(ec_the) * sin(ec_phi);
  float rot22 = cos(ec_phi);
  float rot23 = sin(ec_the) * sin(ec_phi);
  float rot31 = -sin(ec_the);
  float rot32 = 0.;
  float rot33 = cos(ec_the);
  //----------------
  yi = xyz[X] * rot11 + xyz[Y] * rot21 + xyz[Z] * rot31;
  xi = xyz[X] * rot12 + xyz[Y] * rot22 + xyz[Z] * rot32;
  zi = xyz[X] * rot13 + xyz[Y] * rot23 + xyz[Z] * rot33;
  zi -= 510.32;
  //----------------
  uvw[U] = (yi - ylow) / sinrho;
  uvw[V] = (yhi - ylow) / tgrho - xi + (yhi - yi) / tgrho;
  uvw[W] = ((yhi-ylow)/tgrho+xi+(yhi-yi)/tgrho)/2./cosrho;
}
