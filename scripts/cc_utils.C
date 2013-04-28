double cctheta(double x_sc, double y_sc, double z_sc, double cx_sc, double cy_sc, double cz_sc){

  const float R2D = 5.72957795130823229e+01;
	//Define CC plane equation: Ax + By + Cz + D = 0
	double A = -0.000785;
	double B = 0;
	double C = -0.00168;
	double D = 1;
	double magS = sqrt(A*A + B*B + C*C);

	//length of line perpendicular to CC plane from hit in SC plane
	double h = 0;

	//cosine of angle between h & t vector
	double cAlpha = 0;

	//magnitude of vector t
	double t = 0;

	h = ((A*x_sc + B*y_sc + C*z_sc) + D)/magS;

	cAlpha = (cx_sc*x_sc + cy_sc*y_sc + cz_sc*z_sc)/magS;

	t = h/cAlpha;

	double x_cc = x_sc - t*cx_sc;
	double y_cc = y_sc - t*cy_sc;
	double z_cc = z_sc - t*cz_sc;

	return acos(z_cc/sqrt(x_cc*x_cc + y_cc*y_cc + z_cc*z_cc))*R2D;
}

double ccphi(double x_sc, double y_sc, double z_sc, double cx_sc, double cy_sc, double cz_sc){
  const float R2D = 5.72957795130823229e+01;
	//Define CC plane equation: Ax + By + Cz + D = 0
	double A = -0.000785;
	double B = 0;
	double C = -0.00168;
	double D = 1;
	double magS = sqrt(A*A + B*B + C*C);

	//length of line perpendicular to CC plane from hit in SC plane
	double h = 0;

	//cosine of angle between h & t vector
	double cAlpha = 0;

	//magnitude of vector t
	double t = 0;

	h = ((A*x_sc + B*y_sc + C*z_sc) + D)/magS;

	cAlpha = (cx_sc*x_sc + cy_sc*y_sc + cz_sc*z_sc)/magS;

	t = h/cAlpha;

	double x_cc = x_sc - t*cx_sc;
	double y_cc = y_sc - t*cy_sc;
	double z_cc = z_sc - t*cz_sc;

	return atan2(y_cc, x_cc)*R2D;
}
