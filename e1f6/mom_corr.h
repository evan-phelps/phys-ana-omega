#ifndef MOMCORR
#define MOMCORR

#include <TLorentzVector.h>

//#define NSect 6
//#define ThetaC_n 144
//#define MomC_T_n 48

/* Size of Vectors with parameters for phi dependence of Momentum Correction
   phi between -30 and 30 deg */
//#define Npar 4

inline Int_t GetSector(Double_t phi)
{
	//phi between -180 and 180
	//phi2 between 0 and 360
	Double_t phi2 = phi;
	if (phi<0) phi2 = phi2+360;
	Int_t sect = 1 + ((int)phi2+30)/60;
	if (sect == 7) sect = 1;
	return sect;
}


/* CLASS DEFINITION */
class MomCorr_e1f
{
public:
	MomCorr_e1f(char* ddir /*= "MomCorr"*/);
	Double_t ThetaC_min, ThetaC_max, ThetaC_wid;
	Double_t MomC_T_min, MomC_T_max, MomC_T_wid;
	static const Int_t MomC_T_n = 48;
	static const Int_t ThetaC_n = 144;
	static const Int_t NSect = 6;
	static const Int_t Npar = 4;
	
private:
	char* datadir;
	/* Vectors with parameters for phi dependence of Theta Correction
	   phi between -30 and 30 deg */
	Double_t c0_theta[ThetaC_n][NSect];
	Double_t c1_theta[ThetaC_n][NSect];
	Double_t c2_theta[ThetaC_n][NSect];

	/* Vectors with parameters for phi dependence of Electron Momentum Correction
	   phi between -30 and 30 deg */
	Double_t c0_mom[MomC_T_n][NSect][Npar];
	Double_t c1_mom[MomC_T_n][NSect][Npar];

	/* Vectors with parameters for phi dependence of pi+ Momentum Correction
	   phi between -30 and 30 deg */
	Double_t d0_mom[MomC_T_n][NSect][Npar];
	Double_t d1_mom[MomC_T_n][NSect][Npar];

	/* REad angle correction parameters */
	void read_theta_par();
	/* Read momentum correction parameters for electrons */
	void read_mom_par();
	/* Read momentum correction parameters for pi+ */
	void read_mom_pip_par();

	/* Angle correction */
	Double_t theta_corr(Double_t , Double_t , Int_t );

	/* momentum correction for electrons */
	Double_t mom_corr(Double_t , Double_t , Double_t , Int_t );

	/* momentum correction for hadrons */
	Double_t mom_corr_pip(Double_t , Double_t , Double_t , Int_t);



public:
	TLorentzVector PcorN(TLorentzVector Pin, Int_t charge, Int_t ipart);


};
#endif /* MOMCORR */
