#ifndef PARTICLECONSTANTS_H_
#define PARTICLECONSTANTS_H_
#include <map>
using namespace std;
namespace ParticleConstants
{
static const float PI = 3.14159265358979312;
static const float D2R = PI/180.0;
static const float E1F_E0 = 5.499;//7;
static const float SOL = 29.9792458;
//misc. constants
static const float FSC = 0.00729735253;
static const float NA = 6.02214129E23;
static const float QE = 1.60217646E-19;
//particle codes, usually PDG codes, but always those used in BOS
static const int PROTON = 2212;
static const int NEUTRON = 2112;
static const int PIP = 211;
static const int PIM = -211;
static const int PI0 = 111;
static const int KP = 321;
static const int KM = -321;
static const int PHOTON = 22;
static const int ELECTRON = 11;
//PDG particle masses in GeV/c2
static const float MASS_P = 0.93827203;
static const float MASS_N = 0.93956556;
static const float MASS_E = 0.000511;
static const float MASS_PIP = 0.13957018;
static const float MASS_PIM = 0.13957018;
static const float MASS_PI0 = 0.1349766;
static const float MASS_KP = 0.493677;
static const float MASS_KM = 0.493677;
static const float MASS_G = 0.0;
static const float MASS_OMEGA = 0.78265;
typedef map<int,double> partMap;
static partMap PARTMAP;
inline float GetPdgMass(int pdgId) {
	if (PARTMAP.empty()) {
		PARTMAP[PROTON] = MASS_P;
		PARTMAP[NEUTRON] = MASS_N;
		PARTMAP[ELECTRON] = MASS_E;
		PARTMAP[PIP] = MASS_PIP;
		PARTMAP[PIM] = MASS_PIM;
		PARTMAP[PI0] = MASS_PI0;
		PARTMAP[KP] = MASS_KP;
		PARTMAP[KM] = MASS_KM;
		PARTMAP[PHOTON] = MASS_G;
	}
	return PARTMAP[pdgId];
}
}
#endif /* PARTICLECONSTANTS_H_ */