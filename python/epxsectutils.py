
PI = 3.14159265358979312
E1F_E0 = 5.499
FSC = 0.00729735253
A = FSC
NA = 6.02214129E23
QE = 1.60217646E-19
MP = 0.93827203
MOMEGA = 0.78265
WOMEGA = 0.00849


def nu(w, q2):
    return (w*w-MP*MP+q2)/(2*MP)


def epsilon(w, q2):
    n = nu(w, q2)
    e0 = E1F_E0
    e1 = e0-n
    epsInv = 1+2*(q2+n*n)/(4*e0*e1-q2)
    return 1.0/epsInv


def vgflux(w, q2, e0=E1F_E0):
    eps = epsilon(w, q2)
    return A*w*(w*w-MP*MP)/(4*PI*e0*e0*MP*MP*q2*(1-eps))


def xb(w, q2):
    return q2/(2*MP*nu(w, q2))
