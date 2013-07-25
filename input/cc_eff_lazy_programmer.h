
const float g_cceff[6][36] = {

  0.69, 0.63, 0.62, 0.53, 0.79, 0.82, 0.86, 0.90, 0.94, 0.93, 0.95, 0.91, 0.97, 0.93, 0.97, 0.96, 0.98, 0.98, 0.99, 0.97, 0.98, 0.97, 0.98, 0.96, 0.98, 0.97, 0.99, 0.97, 0.98, 0.98, 0.96, 0.98, 0.95, 0.94, 0.53, 0.52,

  0.73, 0.70, 0.82, 0.69, 0.82, 0.93, 0.67, 0.82, 0.74, 0.80, 0.86, 0.97, 0.84, 0.96, 0.92, 0.94, 0.91, 0.99, 0.93, 0.98, 0.89, 0.96, 0.96, 0.97, 0.97, 0.97, 0.97, 0.96, 0.98, 0.97, 0.96, 0.96, 0.96, 0.93, 0.65, 0.67,

  0.76, 0.79, 0.81, 0.82, 0.81, 0.90, 0.88, 0.95, 0.94, 0.97, 0.94, 0.93, 0.89, 0.95, 0.93, 0.97, 0.98, 0.96, 0.97, 0.96, 0.98, 0.95, 0.97, 0.98, 0.97, 0.98, 0.97, 0.98, 0.98, 0.99, 0.98, 0.99, 0.97, 0.97, 0.61, 0.57,

  0.77, 0.66, 0.43, 0.36, 0.85, 0.75, 0.81, 0.89, 0.82, 0.89, 0.67, 0.90, 0.89, 0.92, 0.93, 0.93, 0.93, 0.95, 0.92, 0.93, 0.85, 0.84, 0.95, 0.88, 0.93, 0.89, 0.92, 0.94, 0.98, 0.96, 0.96, 0.92, 0.92, 0.90, 0.51, 0.46,

  0.48, 0.70, 0.64, 0.47, 0.94, 0.88, 0.86, 0.92, 0.96, 0.93, 0.95, 0.98, 0.95, 0.99, 0.95, 0.98, 0.93, 0.98, 0.97, 0.98, 0.97, 0.99, 0.93, 0.99, 0.97, 0.99, 0.97, 0.97, 0.98, 0.98, 0.98, 0.99, 0.94, 0.95, 0.51, 0.46,

0.69, 0.77, 0.78, 0.81, 0.87, 0.88, 0.93, 0.90, 0.90, 0.93, 0.79, 0.96, 0.95, 0.96, 0.98, 0.97, 0.98, 0.99, 0.99, 0.99, 0.97, 0.99, 0.98, 0.98, 0.99, 0.95, 0.97, 0.95, 0.99, 0.98, 0.99, 0.98, 0.95, 0.94, 0.37, 0.48

};

double cceff(int sector, int pmt) {
  return g_cceff[sector-1][pmt-1];
}

float ccw8(int sector, int segm, int hit) {
  float w8 = 1;
  if (hit != 0) {
    int ipmt = segm*2 - (hit==1?0:1) - 1;
    w8 = g_cceff[sector-1][ipmt];
  }
  return w8;
}

