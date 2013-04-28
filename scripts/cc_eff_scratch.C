{
  TFile *fin = TFile::Open("out/cc_nphe.root"); //,"update");
  fin.cd("betadistLL"); //expgamma");
  THStack *hs[6];
  THStack *hseff = new THStack("hseff","CC efficiencies");
  float cceff[6][36];
  TH1 *hcceff[6];
  for (int j = 0; j < 6; j++) {
    TString hn = TString::Format("hcceff_s%d",j+1);
    TString ht = TString::Format("CC efficiencies, S%d",j+1);
    hcceff[j] = new TH1F(hn.Data(),hn.Data(),100,0,1);
    hseff->Add(hcceff[j]);
    TString hsn = TString::Format("hsnphe_s%d",j+1);
    hs[j] = (THStack*)gDirectory->Get(hsn.Data());
    for (int i = 0; i < 36; i++) {
      TF1 *f = ((TH1*)hs[j]->GetHists()->At(i))->GetListOfFunctions()->At(1);
      cceff[j][i] = f->Integral(3,35)/f->Integral(0,35);
      hcceff[j]->Fill(cceff[j][i]);
      char* comma = ((i<35)?", ":"");
      printf("%.2f%s",cceff[j][i],comma);
    }
    cout << endl;
  }
  //gDirectory->WriteObject(hseff,hseff->GetName());
}
