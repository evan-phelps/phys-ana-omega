SetAliases(TChain *c)
{
    //constants
    const char *sol = TString::Format("%e",29.9792458).Data();
    const char *mpip = TString::Format("%e",0.13957018).Data();
    const char *mp = TString::Format("%e",0.93827203).Data();
    c->SetAlias("c",sol);
    c->SetAlias("mp",mp);
    c->SetAlias("mpip",mpip);
    c->SetAlias("mpim",mpip);
    //Delta-t for particle identification wrt electron time
    c->SetAlias("etime","sc_t[sc[0]-1]-sc_r[sc[0]-1]/c");
    c->SetAlias("bifp","sqrt(p*p/(p*p+mp*mp))");
    c->SetAlias("dtifpE","sc_t[sc-1]-sc_r[sc-1]/(c*bifp)-etime");
    c->SetAlias("bifpip","sqrt(p*p/(p*p+mpip*mpip))");
    c->SetAlias("dtifpipE","sc_t[sc-1]-sc_r[sc-1]/(c*bifpip)-etime");
    c->SetAlias("bifpim","sqrt(p*p/(p*p+mpim*mpim))");
    c->SetAlias("dtifpimE","sc_t[sc-1]-sc_r[sc-1]/(c*bifpim)-etime");
    //Delta-t for particle identification wrt bunch time
    c->SetAlias("dtifp","sc_t[sc-1]-sc_r[sc-1]/(c*bifp)-tr_time");
    c->SetAlias("dtifpip","sc_t[sc-1]-sc_r[sc-1]/(c*bifpip)-tr_time");
    c->SetAlias("dtifpim","sc_t[sc-1]-sc_r[sc-1]/(c*bifpim)-tr_time");
    //EC sampling fraction
    c->SetAlias("sf","etot[ec-1]/p");

}
