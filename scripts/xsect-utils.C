namespace epXsect {
  static const float PI = 3.14159265358979312;
  static const float E1F_E0 = 5.499;
  static const float FSC = 0.00729735253;
  static const float A = FSC;
  static const float NA = 6.02214129E23;
  static const float QE = 1.60217646E-19;
  static const float MP = 0.93827203;

  inline double nu(double w, double q2) {
    return (w*w-MP*MP+q2)/(2*MP);
  }

  inline double epsilon(double w, double q2) {
    double n = nu(w,q2);
    double e0 = E1F_E0;
    double e1 = e0-n;
    double epsInv = 1+2*(q2+n*n)/(4*e0*e1-q2);
    return 1.0/epsInv;
  }

  inline double vgflux(double w, double q2, double e0 = E1F_E0) {
    double eps = epsilon(w,q2);
    return A*w*(w*w-MP*MP)/(4*PI*e0*e0*MP*MP*q2*(1-eps));
  }

  inline double xb(double w, double q2) {
    return q2/(2*MP*nu(w,q2));
  }
}
