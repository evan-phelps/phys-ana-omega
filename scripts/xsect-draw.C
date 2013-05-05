{
  gROOT->SetStyle("Modern");
  TTree t;
  TTree t2;
  TTree t3;
  t.ReadFile("xsect_integrated_morand.txt","",'\t');
  t2.ReadFile("xsect-integrated-me.txt","",'\t');
  t3.ReadFile("xsect_integrated_me_50.txt","",'\t');
  TFile fin("h3maker-hn.root");
  TAxis *qax = hq2wmmp->GetZaxis();
  int qof = qax->GetNbins()+1;
  TMultiGraph *me = new TMultiGraph();
  TMultiGraph *me50 = new TMultiGraph();
  TMultiGraph *mo = new TMultiGraph();
  int gcs[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  for (int iq = 1; iq < qof; iq++) {
    TMultiGraph *mgtmp = new TMultiGraph();
    float qlo = qax->GetBinLowEdge(iq);
    float qhi = qax->GetBinLowEdge(iq+1);
    float qmi = qax->GetBinCenter(iq);
    TString cut = TString::Format("Q2>%f && Q2<%f",qax->GetBinLowEdge(iq),qax->GetBinLowEdge(iq+1));
    t2.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *g = new TGraphErrors(t2.GetSelectedRows(),t2.GetV1(),t2.GetV2(),0,t2.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f",qmi);
    TString gt = TString::Format("Q2 = [%.3f,%.3f], from fn",qlo,qhi);
    g->SetName(gn.Data());
    g->SetTitle(gt.Data());
    g->SetMarkerColor(kGreen+1);
    g->SetLineColor(kGreen+1);
    g->SetMarkerStyle(20);

    t2.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gB = new TGraphErrors(t2.GetSelectedRows(),t2.GetV1(),t2.GetV2(),0,t2.GetV3());
    gn = TString::Format("ep_e1f_q2_%.3f_y",qmi);
    gt = TString::Format("Q2 = [%.3f,%.3f], from yield",qlo,qhi);
    gB->SetName(gn.Data());
    gB->SetTitle(gt.Data());
    gB->SetMarkerColor(kGreen+1);
    gB->SetLineColor(kGreen+1);
    gB->SetMarkerStyle(21);

    t3.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *g3 = new TGraphErrors(t3.GetSelectedRows(),t3.GetV1(),t3.GetV2(),0,t3.GetV3());
    TString gn3 = TString::Format("ep_step_e1f_q2_%.3f",qmi);
    TString gt3 = TString::Format("Q2 = [%.3f,%.3f]",qlo,qhi);
    g3->SetName(gn3.Data());
    g3->SetTitle(gt3.Data());
    g3->SetMarkerColor(kYellow+1);
    g3->SetLineColor(kYellow+1);
    g3->SetMarkerStyle(21);

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
    mgtmp->Add((TGraphErrors*)g->Clone());
    mgtmp->Add((TGraphErrors*)gB->Clone());
    mgtmp->Add((TGraphErrors*)g3->Clone());
    if (iq>1) mgtmp->Add((TGraphErrors*)g2->Clone());
    mgtmp->Draw("ap");
    gPad->Modified();
    gPad->Update();
    mgtmp->GetHistogram()->SetMinimum(0);
    if (mgtmp->GetHistogram()->GetMaximum() > 5000) mgtmp->GetHistogram()->SetMaximum(5000);
    //g->DrawClone("ap");
    //g2->DrawClone("p");
    //g3->DrawClone("p");
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
    g3->SetMarkerColor(gcs[iq-1]);
    g3->SetLineColor(gcs[iq-1]);
    me50->Add(g);
    me->Add(g3);
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
