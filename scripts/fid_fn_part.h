#include "h10t3pi_sel.h"
#include <TH3.h>

class FidFnPart : public h10t3pi_sel {
 public:
  TH3 *h_e_mtp;
 FidFnPart() : h10t3pi_sel() {
    printf("in FidFnPart::FidFnPart()\n");
  }
  ~FidFnPart() {
    printf("in FidFnPart::FidFnPart()\n");
    delete h_e_mtp;
  }
  void Setup() {
    printf("in FidFnPart::Setup()\n");
    double d2r = 1/57.2957795130823229;
    TString hn = "h_e_mtp";
    TString ht = "e^{-} vertex kinematics";
    //0:phi, 1:theta, 2:momentum
    int nbins[] = {360, 90, 192};
    int low[] = {-30*d2r, 0, 0.682};
    int high[] = {330*d2r, 90*d2r, 5.482};
    h_e_mtp = new TH3(hn.Data(),ht.Data(),
		      nbins[0], low[0], high[0],
		      nbins[1], low[1], high[1],
		      nbins[2], low[2], high[2]);
  }
  void Process() {
    double d2r = 1/57.2957795130823229;
    double phi = e1.Phi();
    double theta = e1.Theta();
    double mom = e1.P();
    if (phi < -30*d2r) phi += 360*d2r;
    h_e_mtp->Fill(phi,theta,mom);
  }
  void Finalize() {
    printf("in FidFnPart::Finalize()\n");
  }
};
