#ifndef fid_ecsc_h
#define fid_ecsc_h

#include "h10t3pi_sel.h"
#include "rootutils.C"
#include "ec_utils.C"
#include "fid.h"
#include <TH3.h>
#include <TH2.h>

class FidEcSc : public h10t3pi_sel {
 public:
  static const int NSECT = 6;
  static const int NCOOR = 3;
  static const int NPART = 4;
  static const int SCNBX = 250;
  static const int SCNBY = 250;
  static const int ECNBX = 225;
  static const int ECNBY = 275;
  static const float SCLOX, SCHIX, SCLOY, SCHIY, ECLOX, ECHIX, ECLOY, ECHIY;
  static const char* SCOOR[NCOOR];
  static const char* SPART[NPART];
  static const char* HECTMPL;
  static const char* HECXYTMPL;
  static const char* HTECTMPL;
  static const char* HTTMPL;
  static const char* HSCTMPL;
  static const char* H3NAME;
  static const char* H3TITLE;
  static const int BINS_PHI = 120;
  static const int BINS_P[NPART];
  static const int BINS_THETA[NPART];
  static const float L_P[NPART];
  static const float H_P[NPART];
  static const float L_THETA[NPART];
  static const float H_THETA[NPART];
  static const float L_PHI;
  static const float H_PHI;
  static const float L_PHI_RAD;
  static const float H_PHI_RAD;
  static const float R2D;
  static const float D2R;
  static const float PI;

  TH3 *h3[NSECT][NPART][2];
  TH2 *hpVec[NSECT][NPART][NCOOR][2];
  TH2 *hecXY[NPART][2];
  TH2 *hscXY[NSECT][NPART][2];
  Fid *fid;
 FidEcSc() : h10t3pi_sel() {
    printf("in SelEc::SelEc()\n");
    fid = Fid::Instance();
  }

  ~FidEcSc() {
    printf("in SelEc::SelEc()\n");
    for (int ipart = 0; ipart < NPART; ipart++) {
      delete hecXY[ipart][0];
      delete hecXY[ipart][1];
    }
    for (int isect = 0; isect < NSECT; isect++) {
      for (int ipart = 0; ipart < NPART; ipart++) {
	delete hscXY[isect][ipart][0];
	delete hscXY[isect][ipart][1];
	delete h3[isect][ipart][0];
	delete h3[isect][ipart][1];
	for (int icoor = 0; icoor < NCOOR; icoor++) {
	  delete hpVec[isect][ipart][icoor][0];
	  delete hpVec[isect][ipart][icoor][1];
	}
      }
    }
    fid->Release();
  }

  void Setup() {
    printf("in SelEc::Setup()\n");
    for (int ipart = 0; ipart < NPART; ipart++) {
      TString hecn = TString::Format(HECXYTMPL,SPART[ipart]);
      TString ht = TString::Format(HTECTMPL,SPART[ipart]);
      hecXY[ipart][0] = new TH2D(hecn.Data(),ht.Data(),1000,-500,500,1000,-500,500);
      hecn.Append("_bad");
      ht.Append(", bad");
      hecXY[ipart][1] = new TH2D(hecn.Data(),ht.Data(),1000,-500,500,1000,-500,500);
    }

    for (int isect = 0; isect < NSECT; isect++) {
      for (int ipart = 0; ipart < NPART; ipart++) {
	TString h3n = TString::Format(H3NAME,SPART[ipart],isect+1);
	TString h3t = TString::Format(H3TITLE,SPART[ipart],isect+1);
	//printf("creating... ");
	//printf("%s -- %s\n",h3n.Data(),h3t.Data());
	h3[isect][ipart][0] = new TH3D(h3n.Data(),h3t.Data(),BINS_PHI,L_PHI,H_PHI,BINS_THETA[ipart],L_THETA[ipart],H_THETA[ipart],BINS_P[ipart],L_P[ipart],H_P[ipart]);
	h3n.Append("_bad");
	h3t.Append(", bad paddles");
	h3[isect][ipart][1] = new TH3D(h3n.Data(),h3t.Data(),BINS_PHI,L_PHI,H_PHI,BINS_THETA[ipart],L_THETA[ipart],H_THETA[ipart],BINS_P[ipart],L_P[ipart],H_P[ipart]);
	TString hscn = TString::Format(HSCTMPL,isect+1,SPART[ipart]);
	TString hsct = TString::Format(HTTMPL,isect+1);
	hscXY[isect][ipart][0] = new TH2D(hscn.Data(),hsct.Data(),SCNBX,SCLOX,SCHIX,SCNBY,SCLOY,SCHIY);
	hscn.Append("_bad");
	hsct.Append(", bad");
	hscXY[isect][ipart][1] = new TH2D(hscn.Data(),hsct.Data(),SCNBX,SCLOX,SCHIX,SCNBY,SCLOY,SCHIY);
	for (int icoor = 0; icoor < NCOOR; icoor++) {
	  TString hecn = TString::Format(HECTMPL,isect+1,SPART[ipart],SCOOR[icoor]);
	  TString hect = TString::Format(HTTMPL,isect+1);
	  hpVec[isect][ipart][icoor][0] = new TH2D(hecn.Data(),hect.Data(),ECNBX,ECLOX,ECHIX,ECNBY,ECLOY,ECHIY);
	  hecn.Append("_bad");
	  hect.Append(", bad");
	  hpVec[isect][ipart][icoor][1] = new TH2D(hecn.Data(),hect.Data(),ECNBX,ECLOX,ECHIX,ECNBY,ECLOY,ECHIY);
	} 
      } 
    }
  }

  void Process() {
    if (w4->M()<1.6) return;
    TLorentzVector *lv[NPART] = { e1, p1, pip, pim };
    int idx[NPART] = { h10idx_e, h10idx_p, h10idx_pip, h10idx_pim };
    for (int ipart = 0; ipart < NPART; ipart++) {
      if (idx[ipart]<0) continue;
      else {
	int dcidx = dc[idx[ipart]]-1;
	int ecidx = ec[idx[ipart]]-1;
	int scidx = sc[idx[ipart]]-1;
	if (scidx>=0 && dcidx>=0) {
	  int isect = dc_sect[dcidx]-1;
	  int iscsect = sc_sect[scidx]-1;
	  int scpaddle = sc_pd[scidx];
	  bool goodpdl = fid->mPw[iscsect][scpaddle]; //true; //fid->InFid(lv[ipart]->P(),lv[ipart]->Theta(),lv[ipart]->Phi(),isect+1,id[idx[ipart]]);
	  Double_t _p = lv[ipart]->P();
	  Double_t _phi = R2D*lv[ipart]->Phi();
	  Double_t _theta = R2D*lv[ipart]->Theta();
	  if (_phi<L_PHI) _phi+=360;
	  //determine sector
	  Double_t sectphi = _phi + 30;
	  Int_t iSector = sectphi/60; //relies on truncation
	  iSector = iSector < 6 ? iSector : 0;
	  sectphi -= 60*iSector+30;
	  if (goodpdl) {
	    hscXY[isect][ipart][0]->Fill(dc_xsc[dcidx],dc_ysc[dcidx]);
	    h3[iSector][ipart][0]->Fill(sectphi,_theta,_p);
	  } else {
	    hscXY[isect][ipart][1]->Fill(dc_xsc[dcidx],dc_ysc[dcidx]);
	    h3[iSector][ipart][1]->Fill(sectphi,_theta,_p);
	  }
	  if (ecidx>=0) {
	    double xyz[] = { (double)ech_x[ecidx], (double)ech_y[ecidx], (double)ech_z[ecidx] };
	    double eccoord[NCOOR];
	    GetUVW(xyz,eccoord);
	    if(goodpdl) hecXY[ipart][0]->Fill(xyz[0],xyz[1]);
	    else hecXY[ipart][1]->Fill(xyz[0],xyz[1]);
	    for (int icoor = 0; icoor < NCOOR; icoor++) {
	      if(goodpdl) hpVec[isect][ipart][icoor][0]->Fill(eccoord[icoor],p[idx[ipart]]);
	      else hpVec[isect][ipart][icoor][1]->Fill(eccoord[icoor],p[idx[ipart]]);
	    }
	  }
	}
      }
    }
  }

  void Finalize() {
    printf("in SelEc::Finalize()\n");
  }

  void SetDirectory(TDirectory *dir) {
    for (int i = 0; i < 2; i++) {
      for (int isect = 0; isect < NSECT; isect++) {
	for (int ipart = 0; ipart < NPART; ipart++) {
	  hecXY[ipart][i]->SetDirectory(dir);
	  hscXY[isect][ipart][i]->SetDirectory(dir);
	  h3[isect][ipart][i]->SetDirectory(dir);
	  for (int icoor = 0; icoor < NCOOR; icoor++) {
	    hpVec[isect][ipart][icoor][i]->SetDirectory(dir);
	  }
	}
      }
    }
  }
};

#endif // #ifdef fid_ecsc_h

const float FidEcSc::SCLOX = 0;
const float FidEcSc::SCHIX = 500;
const float FidEcSc::SCLOY = -250;
const float FidEcSc::SCHIY = 250;
const float FidEcSc::ECLOX = 0;
const float FidEcSc::ECHIX = 450;
const float FidEcSc::ECLOY = 0;
const float FidEcSc::ECHIY = 5.5;
const char* FidEcSc::SCOOR[FidEcSc::NCOOR] = { "u", "v", "w" };
const char* FidEcSc::SPART[FidEcSc::NPART] = { "e", "p", "pip", "pim" };
const char* FidEcSc::HECTMPL = "h_%d_%s_pVec%s";
const char* FidEcSc::HTTMPL = "sector %d";
const char* FidEcSc::HSCTMPL = "h_%d_%s_scXY";
const char* FidEcSc::HECXYTMPL = "h_%s_ecXY";
const char* FidEcSc::HTECTMPL = "EC Y vs. X, %s";
const char* FidEcSc::H3NAME = "h3%s_pVthetaVphi_s%d";
const char* FidEcSc::H3TITLE = "p:#theta:#phi, %s, s%d";
const int FidEcSc::BINS_P[FidEcSc::NPART] = {225, 200, 200, 200};
const int FidEcSc::BINS_THETA[FidEcSc::NPART] = {160, 200, 280, 280};
const float FidEcSc::L_P[FidEcSc::NPART] = {0, 0, 0, 0};
const float FidEcSc::H_P[FidEcSc::NPART] = {5.5, 4, 4, 4};
const float FidEcSc::L_THETA[FidEcSc::NPART] = {0, 0, 0, 0};
const float FidEcSc::H_THETA[FidEcSc::NPART] = {80, 100, 140, 140};
const float FidEcSc::L_PHI = -30;
const float FidEcSc::H_PHI = 30;
const float FidEcSc::L_PHI_RAD = FidEcSc::L_PHI*TMath::DegToRad();
const float FidEcSc::H_PHI_RAD = FidEcSc::H_PHI*TMath::DegToRad();
const float FidEcSc::R2D = TMath::RadToDeg();
const float FidEcSc::D2R = TMath::DegToRad();
const float FidEcSc::PI = TMath::Pi();
