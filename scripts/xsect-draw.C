{
  gROOT->SetStyle("Modern");
  TTree t;
  TTree t2;
  t.ReadFile("xsect_integrated_morand.txt","",'\t');
  t2.ReadFile("xsect-integrated-me.txt","",'\t');
  //t2.ReadFile("xsect_integrated_me_50.txt","",'\t');
  TFile fin("h3maker-hn.root");
  TAxis *qax = hq2wmmp->GetZaxis();
  int qof = qax->GetNbins()+1;
  TMultiGraph *me50 = new TMultiGraph();
  TMultiGraph *mo = new TMultiGraph();
  int gcs[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  for (int iq = 1; iq < qof; iq++) {
    float qlo = qax->GetBinLowEdge(iq);
    float qhi = qax->GetBinLowEdge(iq+1);
    float qmi = qax->GetBinCenter(iq);
    TString cut = TString::Format("Q2>%f && Q2<%f",qax->GetBinLowEdge(iq),qax->GetBinLowEdge(iq+1));
    t2.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *g = new TGraphErrors(t2.GetSelectedRows(),t2.GetV1(),t2.GetV2(),0,t2.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f",qmi);
    TString gt = TString::Format("Q2 = [%.3f,%.3f]",qlo,qhi);
    g->SetName(gn.Data());
    g->SetTitle(gt.Data());
    g->SetMarkerColor(kBlue+1);
    g->SetMarkerStyle(20);

    t.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *g2 = new TGraphErrors(t.GetSelectedRows(),t.GetV1(),t.GetV2(),0,t.GetV3());
    TString gn2 = TString::Format("lv_e16_q2_%.3f",qmi);
    TString gt2 = TString::Format("Q2 = [%.3f,%.3f]",qlo,qhi);
    g2->SetName(gn2.Data());
    g2->SetTitle(gt2.Data());
    g2->SetMarkerStyle(29);
    g2->SetMarkerSize(2);

    TCanvas *c = new TCanvas();
    c->cd();
    gPad->Modified();
    gPad->Update();
    g->DrawClone("ap");
    g2->DrawClone("p");
    //gSystem->Sleep(1000);
    //gPad->BuildLegend();

    gt = TString::Format("Q2 = [%.3f,%.3f], e1f-phelps",qlo,qhi);
    g->SetTitle(gt.Data());
    gt2 = TString::Format("Q2 = [%.3f,%.3f], e16-morand",qlo,qhi);
    g2->SetTitle(gt2.Data());
    gPad->Modified();
    gPad->Update();

    g->SetMarkerColor(gcs[iq-1]);
    g->SetLineColor(gcs[iq-1]);
    me50->Add(g);
    g2->SetMarkerColor(gcs[iq-1]);
    g2->SetLineColor(gcs[iq-1]);
    mo->Add(g2);
    TString cn = TString::Format("cQ2_%.3f_%.3f.png",qlo,qhi);
    c->SaveAs(cn.Data());
  }
  TCanvas *c8 = new TCanvas();
  c8->cd();
  gPad->Modified();
  gPad->Update();
  me50->Draw("ap");
  mo->Draw("p");
  gPad->BuildLegend();
  c8->SaveAs("cAll.png");
  printf("see h3maker.root for fits!\n");
}
