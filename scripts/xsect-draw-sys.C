{
  gROOT->SetStyle("Modern");
  TTree tmo;
  TTree ta;
  TTree tb;
  TTree tb_p2;
  TTree tc;
  TTree td;
  TTree td1;
  TTree td105;
  TTree td095;
  TTree td11;
  TTree td09;
  float moedges[] = {0,1.6,1.9,2.2,2.5,2.8,3.1,5.1};
  tmo.ReadFile("xsect_integrated_morand.txt","",'\t');
  ta.ReadFile("xsects/x-int-a.txt","",'\t');
  tb.ReadFile("xsects/x-int-b.txt","",'\t');
  tb_p2.ReadFile("xsects/x-int-b_p2.txt","",'\t');
  tc.ReadFile("xsects/x-int-c.txt","",'\t');
  td.ReadFile("xsects/x-int-d.txt","",'\t');
  td1.ReadFile("xsects/x-int-d_p2.txt","",'\t');
  td105.ReadFile("xsects/x-int-d_p2_f105_refit.txt","",'\t');
  td095.ReadFile("xsects/x-int-d_p2_f095_refit.txt","",'\t');
  td11.ReadFile("xsects/x-int-d_p2_f11_refit.txt","",'\t');
  td09.ReadFile("xsects/x-int-d_p2_f09_refit.txt","",'\t');
  TFile fin("xsects/x-int-a.root");
  TAxis *qax = hq2wmmp->GetZaxis();
  TAxis *wax = hq2wmmp->GetYaxis();
  int qof = qax->GetNbins()+1;
  int wof = wax->GetNbins()+1;
  int gcs[] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1, kCyan+1, 9 };
  for (int iq = 1; iq < qof; iq++) {
    TMultiGraph *mgtmp = new TMultiGraph();
    float qlo = qax->GetBinLowEdge(iq);
    float qhi = qax->GetBinLowEdge(iq+1);
    float qmi = qax->GetBinCenter(iq);

    TString cut = TString::Format("Q2>%f && Q2<%f",qax->GetBinLowEdge(iq),qax->GetBinLowEdge(iq+1));
    tmo.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gmo = new TGraphErrors(tmo.GetSelectedRows(),tmo.GetV1(),tmo.GetV2(),0,tmo.GetV3());
    TString gn = TString::Format("morand_q2_%.3f_f_a",qmi);
    TString gt = TString::Format("Morand, et al., %.3f <= Q^{2} < %.3f",moedges[iq-1],moedges[iq]);
    //TString gt = TString::Format("Q2 = [%.3f,%.3f], a",qlo,qhi);
    gmo->SetName(gn.Data());
    gmo->SetTitle(gt.Data());
    gmo->SetMarkerStyle(27);
    gmo->SetMarkerSize(2);

    TString cut = TString::Format("Q2>%f && Q2<%f",qax->GetBinLowEdge(iq),qax->GetBinLowEdge(iq+1));
    ta.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *ga = new TGraphErrors(ta.GetSelectedRows(),ta.GetV1(),ta.GetV2(),0,ta.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_a",qmi);
    TString gt = TString::Format("full range, function",qlo,qhi);
    //TString gt = TString::Format("Q2 = [%.3f,%.3f], a",qlo,qhi);
    ga->SetName(gn.Data());
    ga->SetTitle(gt.Data());
    ga->SetMarkerStyle(24);
    ga->SetMarkerSize(1);

    tb.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gb = new TGraphErrors(tb.GetSelectedRows(),tb.GetV1(),tb.GetV2(),0,tb.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_b",qmi);
    TString gt = TString::Format("rising GeV, function",qlo,qhi);
    gb->SetName(gn.Data());
    gb->SetTitle(gt.Data());
    gb->SetMarkerStyle(24);
    gb->SetMarkerSize(1);

    tc.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gc = new TGraphErrors(tc.GetSelectedRows(),tc.GetV1(),tc.GetV2(),0,tc.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_c",qmi);
    TString gt = TString::Format("above eta, function");
    gc->SetName(gn.Data());
    gc->SetTitle(gt.Data());
    gc->SetMarkerStyle(24);
    gc->SetMarkerSize(1);

    td.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gd = new TGraphErrors(td.GetSelectedRows(),td.GetV1(),td.GetV2(),0,td.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d",qmi);
    TString gt = TString::Format("rising & above eta, fn");
    gd->SetName(gn.Data());
    gd->SetTitle(gt.Data());
    gd->SetMarkerStyle(30);
    gd->SetMarkerSize(1);

    ta.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *ga2 = new TGraphErrors(ta.GetSelectedRows(),ta.GetV1(),ta.GetV2(),0,ta.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_a",qmi);
    TString gt = TString::Format("full range");
    ga2->SetName(gn.Data());
    ga2->SetTitle(gt.Data());
    ga2->SetMarkerStyle(20);
    ga2->SetMarkerSize(1);

    tb.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gb2 = new TGraphErrors(tb.GetSelectedRows(),tb.GetV1(),tb.GetV2(),0,tb.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_b",qmi);
    TString gt = TString::Format("rising");
    gb2->SetName(gn.Data());
    gb2->SetTitle(gt.Data());
    gb2->SetMarkerStyle(20);
    gb2->SetMarkerSize(1);

    tc.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gc2 = new TGraphErrors(tc.GetSelectedRows(),tc.GetV1(),tc.GetV2(),0,tc.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_c",qmi);
    TString gt = TString::Format("above eta");
    gc2->SetName(gn.Data());
    gc2->SetTitle(gt.Data());
    gc2->SetMarkerStyle(20);
    gc2->SetMarkerSize(1);

    td.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2 = new TGraphErrors(td.GetSelectedRows(),td.GetV1(),td.GetV2(),0,td.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d",qmi);
    TString gt = TString::Format("rising & above eta");
    gd2->SetName(gn.Data());
    gd2->SetTitle(gt.Data());
    gd2->SetMarkerStyle(29);
    gd2->SetMarkerSize(1);

    td1.Draw("W:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gd2_1 = new TGraphErrors(td1.GetSelectedRows(),td1.GetV1(),td1.GetV2(),0,td1.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d1",qmi);
    TString gt = TString::Format("rising & above eta, bg=pol2, fn");
    gd2_1->SetName(gn.Data());
    gd2_1->SetTitle(gt.Data());
    gd2_1->SetMarkerColor(kGreen+1);
    gd2_1->SetLineColor(kGreen+1);
    gd2_1->SetMarkerStyle(24);
    gd2_1->SetMarkerSize(1);

    td11.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_11 = new TGraphErrors(td11.GetSelectedRows(),td11.GetV1(),td11.GetV2(),0,td11.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d11",qmi);
    TString gt = TString::Format("+10\% bg par2");
    gd2_11->SetName(gn.Data());
    gd2_11->SetTitle(gt.Data());
    gd2_11->SetMarkerColor(kRed+1);
    gd2_11->SetLineColor(kRed+1);
    gd2_11->SetMarkerStyle(29);
    gd2_11->SetMarkerSize(1);

    td09.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_09 = new TGraphErrors(td09.GetSelectedRows(),td09.GetV1(),td09.GetV2(),0,td09.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d09",qmi);
    TString gt = TString::Format("-10\% bg par2");
    gd2_09->SetName(gn.Data());
    gd2_09->SetTitle(gt.Data());
    gd2_09->SetMarkerColor(kYellow+1);
    gd2_09->SetLineColor(kYellow+1);
    gd2_09->SetMarkerStyle(29);
    gd2_09->SetMarkerSize(1);

    td105.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_105 = new TGraphErrors(td105.GetSelectedRows(),td105.GetV1(),td105.GetV2(),0,td105.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d11",qmi);
    TString gt = TString::Format("+5\% bg par2");
    gd2_105->SetName(gn.Data());
    gd2_105->SetTitle(gt.Data());
    gd2_105->SetMarkerColor(kRed+1);
    gd2_105->SetLineColor(kRed+1);
    gd2_105->SetMarkerStyle(29);
    gd2_105->SetMarkerSize(1);

    td095.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_095 = new TGraphErrors(td095.GetSelectedRows(),td095.GetV1(),td095.GetV2(),0,td095.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_d09",qmi);
    TString gt = TString::Format("-5\% bg par2");
    gd2_095->SetName(gn.Data());
    gd2_095->SetTitle(gt.Data());
    gd2_095->SetMarkerColor(kYellow+1);
    gd2_095->SetLineColor(kYellow+1);
    gd2_095->SetMarkerStyle(29);
    gd2_095->SetMarkerSize(1);

    tb_p2.Draw("W:xsect:error",cut.Data(),"goff");
    TGraphErrors *gb2_p2 = new TGraphErrors(tb_p2.GetSelectedRows(),tb_p2.GetV1(),tb_p2.GetV2(),0,tb_p2.GetV3());
    TString gn = TString::Format("ep_e1f_q2_%.3f_f_b",qmi);
    TString gt = TString::Format("rising, bg=pol2");
    gb2_p2->SetName(gn.Data());
    gb2_p2->SetTitle(gt.Data());
    gb2_p2->SetMarkerStyle(33);
    gb2_p2->SetMarkerSize(2);

    TCanvas *c = new TCanvas();
    c->cd();
    gPad->Modified();
    gPad->Update();
    if (iq>1) mgtmp->Add((TGraphErrors*)gmo->Clone());
    mgtmp->Add((TGraphErrors*)ga->Clone());
    mgtmp->Add((TGraphErrors*)gb->Clone());
    mgtmp->Add((TGraphErrors*)gc->Clone());
    mgtmp->Add((TGraphErrors*)gd->Clone());
    mgtmp->Add((TGraphErrors*)ga2->Clone());
    mgtmp->Add((TGraphErrors*)gb2->Clone());
    mgtmp->Add((TGraphErrors*)gb2_p2->Clone());
    mgtmp->Add((TGraphErrors*)gc2->Clone());
    mgtmp->Add((TGraphErrors*)gd2->Clone());
    mgtmp->Add((TGraphErrors*)gd2_1->Clone());
    mgtmp->Add((TGraphErrors*)gd2_11->Clone());
    mgtmp->Add((TGraphErrors*)gd2_09->Clone());
    mgtmp->Add((TGraphErrors*)gd2_105->Clone());
    mgtmp->Add((TGraphErrors*)gd2_095->Clone());
    mgtmp->Draw("ap");
    gPad->Modified();
    gPad->Update();
    TString title = TString::Format("%.3f <= Q^{2} < %.3f GeV^{2}",qlo,qhi);
    mgtmp->SetTitle(title.Data());
    mgtmp->GetHistogram()->SetTitle(title.Data());
    //gPad->BuildLegend();
    TLegend *leg = new TLegend(0.6,0.5,0.99,0.93);
    TIter next(mgtmp->GetListOfGraphs());
    TObject *obj;
    while (obj = next()) {
      TGraph *gtmp = (TGraph*)obj;
      leg->AddEntry(gtmp,gtmp->GetTitle(),"p");
    }
    leg->Draw();
    mgtmp->GetHistogram()->SetMinimum(0);
    if (mgtmp->GetHistogram()->GetMaximum() > 5000) mgtmp->GetHistogram()->SetMaximum(5000);
    mgtmp->GetHistogram()->GetXaxis()->SetTitle("W (GeV)");
    mgtmp->GetHistogram()->GetYaxis()->SetTitle("#sigma (nb)");
    mgtmp->GetHistogram()->GetYaxis()->CenterTitle();
    gPad->Modified();
    gPad->Update();
    TString cn = TString::Format("cQ2_%.3f_%.3f.pdf",qlo,qhi);
    c->SaveAs(cn.Data());
  }

  // ************************************************************************
  for (int iw = 1; iw < wof; iw++) {
    TMultiGraph *mgtmp = new TMultiGraph();
    float wlo = wax->GetBinLowEdge(iw);
    float whi = wax->GetBinLowEdge(iw+1);
    float wmi = wax->GetBinCenter(iw);

    TString cut = TString::Format("W>%f && W<%f",wax->GetBinLowEdge(iw),wax->GetBinLowEdge(iw+1));
    tmo.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gmo = new TGraphErrors(tmo.GetSelectedRows(),tmo.GetV1(),tmo.GetV2(),0,tmo.GetV3());
    TString gn = TString::Format("morand_w_%.3f_f_a",qmi);
    TString gt = TString::Format("Morand, et al., #DeltaW > 100 MeV");
    //TString gt = TString::Format("W = [%.3f,%.3f], a",qlo,qhi);
    gmo->SetName(gn.Data());
    gmo->SetTitle(gt.Data());
    gmo->SetMarkerStyle(27);
    gmo->SetMarkerSize(2);

    ta.Draw("Q2:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *ga = new TGraphErrors(ta.GetSelectedRows(),ta.GetV1(),ta.GetV2(),0,ta.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_a",qmi);
    TString gt = TString::Format("full range, function",qlo,qhi);
    //TString gt = TString::Format("W = [%.3f,%.3f], a",qlo,qhi);
    ga->SetName(gn.Data());
    ga->SetTitle(gt.Data());
    ga->SetMarkerStyle(24);
    ga->SetMarkerSize(1);

    tb.Draw("Q2:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gb = new TGraphErrors(tb.GetSelectedRows(),tb.GetV1(),tb.GetV2(),0,tb.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_b",qmi);
    TString gt = TString::Format("rising GeV, function",qlo,qhi);
    gb->SetName(gn.Data());
    gb->SetTitle(gt.Data());
    gb->SetMarkerStyle(24);
    gb->SetMarkerSize(1);

    tc.Draw("Q2:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gc = new TGraphErrors(tc.GetSelectedRows(),tc.GetV1(),tc.GetV2(),0,tc.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_c",qmi);
    TString gt = TString::Format("above eta, function");
    gc->SetName(gn.Data());
    gc->SetTitle(gt.Data());
    gc->SetMarkerStyle(24);
    gc->SetMarkerSize(1);

    td.Draw("Q2:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gd = new TGraphErrors(td.GetSelectedRows(),td.GetV1(),td.GetV2(),0,td.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d",qmi);
    TString gt = TString::Format("rising & above eta, fn");
    gd->SetName(gn.Data());
    gd->SetTitle(gt.Data());
    gd->SetMarkerStyle(30);
    gd->SetMarkerSize(1);

    ta.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *ga2 = new TGraphErrors(ta.GetSelectedRows(),ta.GetV1(),ta.GetV2(),0,ta.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_a",qmi);
    TString gt = TString::Format("full range");
    ga2->SetName(gn.Data());
    ga2->SetTitle(gt.Data());
    ga2->SetMarkerStyle(20);
    ga2->SetMarkerSize(1);

    tb.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gb2 = new TGraphErrors(tb.GetSelectedRows(),tb.GetV1(),tb.GetV2(),0,tb.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_b",qmi);
    TString gt = TString::Format("rising");
    gb2->SetName(gn.Data());
    gb2->SetTitle(gt.Data());
    gb2->SetMarkerStyle(20);
    gb2->SetMarkerSize(1);

    tc.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gc2 = new TGraphErrors(tc.GetSelectedRows(),tc.GetV1(),tc.GetV2(),0,tc.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_c",qmi);
    TString gt = TString::Format("above eta");
    gc2->SetName(gn.Data());
    gc2->SetTitle(gt.Data());
    gc2->SetMarkerStyle(20);
    gc2->SetMarkerSize(1);

    td.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2 = new TGraphErrors(td.GetSelectedRows(),td.GetV1(),td.GetV2(),0,td.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d",qmi);
    TString gt = TString::Format("rising & above eta");
    gd2->SetName(gn.Data());
    gd2->SetTitle(gt.Data());
    gd2->SetMarkerStyle(29);
    gd2->SetMarkerSize(1);

    td1.Draw("Q2:xsectFn:error",cut.Data(),"goff");
    TGraphErrors *gd2_1 = new TGraphErrors(td1.GetSelectedRows(),td1.GetV1(),td1.GetV2(),0,td1.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d1",qmi);
    TString gt = TString::Format("rising & above eta, bg=pol2, fn");
    gd2_1->SetName(gn.Data());
    gd2_1->SetTitle(gt.Data());
    gd2_1->SetMarkerColor(kGreen+1);
    gd2_1->SetLineColor(kGreen+1);
    gd2_1->SetMarkerStyle(24);
    gd2_1->SetMarkerSize(1);

    td11.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_11 = new TGraphErrors(td11.GetSelectedRows(),td11.GetV1(),td11.GetV2(),0,td11.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d11",qmi);
    TString gt = TString::Format("+10\% bg par2");
    gd2_11->SetName(gn.Data());
    gd2_11->SetTitle(gt.Data());
    gd2_11->SetMarkerColor(kRed+1);
    gd2_11->SetLineColor(kRed+1);
    gd2_11->SetMarkerStyle(29);
    gd2_11->SetMarkerSize(1);

    td09.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_09 = new TGraphErrors(td09.GetSelectedRows(),td09.GetV1(),td09.GetV2(),0,td09.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d09",qmi);
    TString gt = TString::Format("-10\% bg par2");
    gd2_09->SetName(gn.Data());
    gd2_09->SetTitle(gt.Data());
    gd2_09->SetMarkerColor(kYellow+1);
    gd2_09->SetLineColor(kYellow+1);
    gd2_09->SetMarkerStyle(29);
    gd2_09->SetMarkerSize(1);

    td105.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_105 = new TGraphErrors(td105.GetSelectedRows(),td105.GetV1(),td105.GetV2(),0,td105.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d11",qmi);
    TString gt = TString::Format("+5\% bg par2");
    gd2_105->SetName(gn.Data());
    gd2_105->SetTitle(gt.Data());
    gd2_105->SetMarkerColor(kRed+1);
    gd2_105->SetLineColor(kRed+1);
    gd2_105->SetMarkerStyle(29);
    gd2_105->SetMarkerSize(1);

    td095.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gd2_095 = new TGraphErrors(td095.GetSelectedRows(),td095.GetV1(),td095.GetV2(),0,td095.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_d09",qmi);
    TString gt = TString::Format("-5\% bg par2");
    gd2_095->SetName(gn.Data());
    gd2_095->SetTitle(gt.Data());
    gd2_095->SetMarkerColor(kYellow+1);
    gd2_095->SetLineColor(kYellow+1);
    gd2_095->SetMarkerStyle(29);
    gd2_095->SetMarkerSize(1);

    tb_p2.Draw("Q2:xsect:error",cut.Data(),"goff");
    TGraphErrors *gb2_p2 = new TGraphErrors(tb_p2.GetSelectedRows(),tb_p2.GetV1(),tb_p2.GetV2(),0,tb_p2.GetV3());
    TString gn = TString::Format("ep_e1f_w_%.3f_f_b",qmi);
    TString gt = TString::Format("rising, bg=pol2");
    gb2_p2->SetName(gn.Data());
    gb2_p2->SetTitle(gt.Data());
    gb2_p2->SetMarkerStyle(33);
    gb2_p2->SetMarkerSize(2);

    TCanvas *c = new TCanvas();
    c->cd();
    gPad->Modified();
    gPad->Update();
    if (gmo->GetN()>0) mgtmp->Add((TGraphErrors*)gmo->Clone());
    mgtmp->Add((TGraphErrors*)ga->Clone());
    mgtmp->Add((TGraphErrors*)gb->Clone());
    mgtmp->Add((TGraphErrors*)gc->Clone());
    mgtmp->Add((TGraphErrors*)gd->Clone());
    mgtmp->Add((TGraphErrors*)ga2->Clone());
    mgtmp->Add((TGraphErrors*)gb2->Clone());
    mgtmp->Add((TGraphErrors*)gb2_p2->Clone());
    mgtmp->Add((TGraphErrors*)gc2->Clone());
    mgtmp->Add((TGraphErrors*)gd2->Clone());
    mgtmp->Add((TGraphErrors*)gd2_1->Clone());
    mgtmp->Add((TGraphErrors*)gd2_11->Clone());
    mgtmp->Add((TGraphErrors*)gd2_09->Clone());
    mgtmp->Add((TGraphErrors*)gd2_105->Clone());
    mgtmp->Add((TGraphErrors*)gd2_095->Clone());
    mgtmp->Draw("ap");
    gPad->Modified();
    gPad->Update();
    TString title = TString::Format("%.3f GeV <= W < %.3f GeV",wlo,whi);
    mgtmp->SetTitle(title.Data());
    mgtmp->GetHistogram()->SetTitle(title.Data());
    //gPad->BuildLegend();
    TLegend *leg = new TLegend(0.75,0.6,0.99,0.93);
    TIter next(mgtmp->GetListOfGraphs());
    TObject *obj;
    while (obj = next()) {
      TGraph *gtmp = (TGraph*)obj;
      leg->AddEntry(gtmp,gtmp->GetTitle(),"p");
    }
    leg->Draw();
    mgtmp->GetHistogram()->SetMinimum(0);
    //if (mgtmp->GetHistogram()->GetMaximum() > 5000) mgtmp->GetHistogram()->SetMaximum(5000);
    mgtmp->GetHistogram()->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mgtmp->GetHistogram()->GetYaxis()->SetTitle("#sigma (nb)");
    mgtmp->GetHistogram()->GetYaxis()->CenterTitle();
    gPad->Modified();
    gPad->Update();
    TString cn = TString::Format("cW_%.3f_%.3f.pdf",wlo,whi);
    c->SaveAs(cn.Data());
  }
}
