{
  
  TChain *h10 = new TChain("h10clone/h10");
  TChain *tpilf = new TChain("3pi-tree-recon/t3pi");
  TChain *tpipf = new TChain("3pi-tree-recon/t3pi_physfrm");
  TChain *tkin = new TChain("3pi-tree-recon/tkin");

  TChain *chains[] = {h10,tpilf,tpipf,tkin};
  for (int ichain = 0; ichain < 4; ichain++) chains[ichain]->Add("/home/ephelps/w/data/1901*.root");

  tpipf->AddFriend(tpilf,"lf");
  h10->AddFriend(tpipf,"pf");
  //h10->AddFriend(tkin,"k");
  tpipf->AddFriend(tkin,"k");

  gROOT->ProcessLine(".include /home/ephelps/analysis/omega");
  gROOT->ProcessLine(".include /home/ephelps/analysis/omega/scripts");
  gROOT->ProcessLine(".L ~/analysis/omega/acc.cpp++");
  gROOT->ProcessLine(".L ~/analysis/omega/fid.cpp++");
  gROOT->ProcessLine(".L ~/analysis/omega/particle-constants.h");
  Fid::Instance("/home/ephelps/analysis/omega/input/fid.parms");
  gROOT->ProcessLine(".L  ~/analysis/omega/scripts/infid.C");
  gROOT->ProcessLine(".L  ~/analysis/omega/scripts/eff.C");
  gROOT->ProcessLine(".L  ~/analysis/omega/h10t3pi_sel.C++");
  using namespace ParticleConstants;

  const char *sol = TString::Format("%e",SOL).Data();
  const char *mpip = TString::Format("%e",MASS_PIP).Data();
  h10->SetAlias("c",sol);
  h10->SetAlias("mpip",mpip);
  h10->SetAlias("bifpip","sqrt(p*p/(p*p+mpip*mpip))");
  h10->SetAlias("dtifpip","sc_t[sc-1]-sc_r[sc-1]/(c*bifpip)-tr_time");
  h10->SetAlias("sf","etot[ec-1]/p");

  h10->SetAlias("fidpass","(infid(k.e.p,k.e.theta,k.e.phi) && (h10idx_p<0 || infid(k.p.p,k.p.theta,k.p.phi,2212)) && (h10idx_pip<0 || infid(k.pip.p,k.pip.theta,k.pip.phi,211)) && (h10idx_pim<0 || infid(k.pim.p,k.pim.theta,k.pim.phi,-211)))");
  h10->SetAlias("fidpasse","(h10idx_e==0 && infid(k.e.p,k.e.theta,k.e.phi))");
  h10->SetAlias("fidpassp","(h10idx_p>0 && infid(k.p.p,k.p.theta,k.p.phi,2212))");
  h10->SetAlias("fidpasspip","(h10idx_pip>0 && infid(k.pip.p,k.pip.theta,k.pip.phi,211))");
  h10->SetAlias("fidpasspim","(h10idx_pim>0 && infid(k.pim.p,k.pim.theta,k.pim.phi,-211))");
  h10->SetAlias("top1pass","(h10idx_pip>0 && h10idx_pim<0 && h10idx_e==0 && h10idx_p>0)");
  h10->SetAlias("top2pass","(h10idx_pip<0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)");
  h10->SetAlias("top3pass","(h10idx_pip>0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)");
  h10->SetAlias("mmthreshpass","(h10idx_p>0 && h10idx_e==0 && ((h10idx_pip<0 || mmppip.M()>0.275) && (h10idx_pim<0 || mmppim.M()>0.275)) && omega.M()>0.415)");
  h10->SetAlias("mmpi0pass","(h10idx_p>0 && h10idx_e==0 && h10idx_pip>0 && h10idx_pim>0 && pi0.M()>0.049 && pi0.M()<0.183)");
  h10->SetAlias("ccpass","(cc[h10idx_e]>0 && nphe[cc[h10idx_e]-1]>25)");

  tpipf->SetAlias("fidpass","(infid(k.e.p,k.e.theta,k.e.phi) && (h10idx_p<0 || infid(k.p.p,k.p.theta,k.p.phi,2212)) && (h10idx_pip<0 || infid(k.pip.p,k.pip.theta,k.pip.phi,211)) && (h10idx_pim<0 || infid(k.pim.p,k.pim.theta,k.pim.phi,-211)))");
  tpipf->SetAlias("fidpasse","(h10idx_e==0 && infid(k.e.p,k.e.theta,k.e.phi))");
  tpipf->SetAlias("fidpassp","(h10idx_p>0 && infid(k.p.p,k.p.theta,k.p.phi,2212))");
  tpipf->SetAlias("fidpasspip","(h10idx_pip>0 && infid(k.pip.p,k.pip.theta,k.pip.phi,211))");
  tpipf->SetAlias("fidpasspim","(h10idx_pim>0 && infid(k.pim.p,k.pim.theta,k.pim.phi,-211))");
  tpipf->SetAlias("top1pass","(h10idx_pip>0 && h10idx_pim<0 && h10idx_e==0 && h10idx_p>0)");
  tpipf->SetAlias("top2pass","(h10idx_pip<0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)");
  tpipf->SetAlias("top3pass","(h10idx_pip>0 && h10idx_pim>0 && h10idx_e==0 && h10idx_p>0)");
  tpipf->SetAlias("mmthreshpass","(h10idx_p>0 && h10idx_e==0 && ((h10idx_pip<0 || mmppip.M()>0.275) && (h10idx_pim<0 || mmppim.M()>0.275)) && omega.M()>0.415)");
  tpipf->SetAlias("mmpi0pass","(h10idx_p>0 && h10idx_e==0 && h10idx_pip>0 && h10idx_pim>0 && pi0.M()>0.049 && pi0.M()<0.183)");

  printf("Fid::Instance() loaded\n\t from ~/analysis/omega/fid.cpp\n\t with parameters from /home/ephelps/analysis/omega/input/fid.parms\n");
  printf("infid() loaded\n\t from ~/analysis/sandbox/sim/infid.C\n");
  printf("h10 aliases: c, mpip, bifpip, dtifpip, sf\n\tfidpass, fidpass[e,p,pip,pim]\n\ttop[1,2,3]pass, mmthreshpass, mmpi0pass\n\tccpass\n");
}
