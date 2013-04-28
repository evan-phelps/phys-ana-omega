#include <TString.h>
#include <TDirectory.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>

void refit(int sector, int paddle, TDirectory *dir) {
  TString hnhi = TString::Format("htvp_%d_pdl%d_hi",sector,paddle);
  TString hnlo = TString::Format("htvp_%d_pdl%d_lo",sector,paddle);
  TH1 *hbphi = (TH1*)dir->Get(hnhi.Data());
  TF1 *ftvphi = (TF1*)hbphi->GetListOfFunctions()->FindObject("ftp");
  TString fnhi = TString::Format("ftp_s%d_p%d_hi",sector,paddle);
  TH1 *hbplo = (TH1*)dir->Get(hnlo.Data());
  TF1 *ftvplo = (TF1*)hbplo->GetListOfFunctions()->FindObject("ftp");
  printf("***old parms***\n");
  printf("%.2f, %.2f, %.2f\n",ftvplo->GetParameter(0),ftvplo->GetParameter(1),ftvplo->GetParameter(2));
  printf("%.2f, %.2f, %.2f\n",ftvphi->GetParameter(0),ftvphi->GetParameter(1),ftvphi->GetParameter(2));
  printf("***************\n");
  ftvphi->FixParameter(2,-0.5);
  ftvplo->FixParameter(2,-0.5);
  hbphi->Fit(ftvphi);
  hbplo->Fit(ftvplo);
  printf("***new parms***\n");
  printf("%.2f, %.2f, %.2f\n",ftvplo->GetParameter(0),ftvplo->GetParameter(1),ftvplo->GetParameter(2));
  printf("%.2f, %.2f, %.2f\n",ftvphi->GetParameter(0),ftvphi->GetParameter(1),ftvphi->GetParameter(2));
  printf("***************\n");
}

void drawPaddleFits(int sector, int paddle, TDirectory *dir, TH1 *hh = NULL, bool fonly = false) {
  TString hnhi = TString::Format("htvp_%d_pdl%d_hi",sector,paddle);
  TString hnlo = TString::Format("htvp_%d_pdl%d_lo",sector,paddle);
  TH1 *hbphi = (TH1*)dir->Get(hnhi.Data());
  TString fnhi = TString::Format("ftp_s%d_p%d_hi",sector,paddle);
  TF1 *ftvphi = (TF1*)hbphi->GetListOfFunctions()->FindObject("ftp")->Clone(fnhi.Data());
  TH1 *hbplo = (TH1*)dir->Get(hnlo.Data());
  TString fnlo = TString::Format("ftp_s%d_p%d_lo",sector,paddle);
  TF1 *ftvplo = (TF1*)hbplo->GetListOfFunctions()->FindObject("ftp")->Clone(fnhi.Data());;
  if (fonly) {
    ftvphi->Draw("same");
    ftvplo->Draw("same");
  } else {
    hbplo->Draw("same");
    hbphi->Draw("same");
  }
  printf("%.2f, %.2f, %.2f\n",ftvplo->GetParameter(0),ftvplo->GetParameter(1),ftvplo->GetParameter(2));
  printf("%.2f, %.2f, %.2f\n",ftvphi->GetParameter(0),ftvphi->GetParameter(1),ftvphi->GetParameter(2));
  if (hh != NULL) {
    hh->GetListOfFunctions()->Add(ftvplo);
    hh->GetListOfFunctions()->Add(ftvphi);
  }
}

void printParms(int sector, int paddle) {
  TF1 *flo = (TF1*)gDirectory->FindObject("ftp_s%d_p%d_lo");
  TF1 *fhi = (TF1*)gDirectory->FindObject("ftp_s%d_p%d_hi");
  printf("%.2f, %.2f, %.2f\n",flo->GetParameter(0),flo->GetParameter(1),flo->GetParameter(2));
}

