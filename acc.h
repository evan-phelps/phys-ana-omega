#ifndef ACC_H
#define ACC_H
#include <string>
#include <TFile.h>
#include <TH3.h>
#include <THnSparse.h>
using namespace std;
class Acc
{
public:
	enum {E, P, PIP, PIM};
	TFile *fIn;
	THnSparse *fHacc; //effective acceptance in final kinematics
	TH3 *fHeff[4]; //per-particle efficiencies
	THnSparse *fHf; //fiducial volume thrown events
                        //(thrown values of reconstructed particles)
	THnSparse *fHo; //"hole" thrown events
	Acc(string infile = "/home/ephelps/dropbox/barracuda_copy-gmail/phys-ana-omega/input/acc-per-part.root");
	//Acc(string infile);
	virtual ~Acc();
	/* effective acceptance factor in final kinematics */
	virtual float GetAcc(float w, float q2, float ct, float phi, float &err);
	/* per particle acceptance with fiducial volume */
	virtual float GetEff(float p, float theta, float phi, int pid, float &err);
	/* compensation for "holes" in acceptance. */
	virtual float GetFiller(float w, float q2, float ct, float phi, float &err);
	int IdxPart(int pid);
};
#endif // ACC_H
