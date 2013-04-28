#define t3pi_cxx
#include "t3pi.h"
#include <TH3.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TStopwatch.h>

void t3pi::Loop()
{  
  fout = new TFile("_fid_h10t3pi.root","recreate");
  const char* TNAME = "t3pi";
  const char* HTNAME = "h3%s_pVthetaVphi_s%d";
  const int NTOPROC = -1; //100000;
  const char* PNAMES[NPARTS] = {"e", "p", "pip", "pim"};
  const int STARTWITH = E;

  const int BINS_P[NPARTS] = {550, 400, 400, 400};
  const int BINS_THETA[NPARTS] = {320, 400, 560, 560};
  const int BINS_PHI = 240;
  const float L_P[NPARTS] = {0, 0, 0, 0};
  const float H_P[NPARTS] = {5.5, 4, 4, 4};
  const float L_THETA[NPARTS] = {0, 0, 0, 0};
  const float H_THETA[NPARTS] = {80, 100, 140, 140};
  const float L_PHI = -30;
  const float H_PHI = 30;
  for (int ip = STARTWITH; ip < NPARTS; ip++) {
    for (int j = 0; j < SECTORS; j++) {
      TString hn = TString::Format(HTNAME,PNAMES[ip],j+1);
      h3[ip][j] = new TH3D(hn.Data(),hn.Data(),BINS_PHI,L_PHI,H_PHI,BINS_THETA[ip],L_THETA[ip],H_THETA[ip],BINS_P[ip],L_P[ip],H_P[ip]);
      h3[ip][j]->SetDirectory(fout->GetDirectory(""));
    }
  }
   
  TStopwatch swmain;
  TStopwatch swgroup;
  const Int_t BLOCKSIZE = 1000000;

  Long64_t nentries = NTOPROC;
  printf("NTOPROC = %lld\n",nentries);
  if (nentries<=0) nentries = fChain->GetEntries(); //Fast();
  printf("nentries = %lld\n",nentries);
  Long64_t eventnum = 0;
  const float L_PHI_RAD = L_PHI*TMath::DegToRad();
  const float H_PHI_RAD = H_PHI*TMath::DegToRad();
  const float R2D = TMath::RadToDeg();
  const float D2R = TMath::DegToRad();
  const float PI = TMath::Pi();

  Fid *fid = Fid::Instance();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if ( (++eventnum) % BLOCKSIZE == 0 ) {
      Float_t gtime = swgroup.RealTime();
      Float_t ttime = swmain.RealTime();
      Double_t percentProcessed = (((Double_t)eventnum)/(Double_t)nentries)*100;
      Float_t remaining = (100.0/percentProcessed*ttime-ttime)/60.0;
      printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(BLOCKSIZE/gtime),remaining);
      swgroup.Start();
      swmain.Start(kFALSE);
    }

    //    if (h10idx_e==0 && h10idx_p>0) {
    if (h10idx_e==0 && h10idx_p>0 && omega->M()>0.733 && omega->M()<0.833 && ((h10idx_pip>0 && mmppip->M()>0.280) || (h10idx_pim>0 && mmppim->M()>0.280))) {
      int h10idx[] = {h10idx_e, h10idx_p, h10idx_pip, h10idx_pim};
      bool einfid = true;
      for (int iip = STARTWITH; iip<NPARTS; iip++) {
	//	if (iip==E || (h10idx[iip] > 0 && omega->M()>0.450)) {
	Double_t p = lv[iip]->P();
	Double_t phi = R2D*lv[iip]->Phi();
	Double_t theta = R2D*lv[iip]->Theta();
	if (phi<L_PHI) phi+=360;
	//determine sector
	Double_t sectphi = phi + 30;
	Int_t iSector = sectphi/60; //relies on truncation
	iSector = iSector < 6 ? iSector : 0;
	sectphi -= 60*iSector+30;
	if (iip==E) einfid = fid->InFid(p,D2R*theta,D2R*phi,iSector+1,11);
	//fill non-electrons only if electron fiducial cut passed
	if (iip==E || einfid) h3[iip][iSector]->Fill(sectphi,theta,p);
	//}
      }
    }
  }
  fid->Release();
  fout->Write();
}
