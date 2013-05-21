{
  gSystem->AddIncludePath("-I/home/ephelps/projects/phys-ana-omega");
  gROOT->ProcessLine(".include /home/ephelps/projects/phys-ana-omega");

  TChain *h10 = new TChain("h10clone/h10");
  TChain *tpilf = new TChain("3pi-tree/t3pi");
  TChain *tpipf = new TChain("3pi-tree/t3pi_physfrm");
  TChain *lum = new TChain("accuq/lumevents");
  TChain *run = new TChain("lumevent");
  TChain *tkin = new TChain("tkin");

  TChain *lb = new TChain("accuq/lumblocks");
  TChain *runb = new TChain("lumblock");

  TChain *chains[] = {h10,tpilf,tpipf,lum,lb};
  for (int ichain = 0; ichain < 5; ichain++) chains[ichain]->Add("/data/e1f/skim/3????.root");
  run->Add("/data/e1f/skim/3xxxx_run.root");
  runb->Add("/data/e1f/skim/3xxxx_run.root");
  tkin->Add("/data/e1f/skim/friend-tkin.root");

  run->AddFriend(lum,"l");
  runb->AddFriend(lb,"l");

  tpipf->AddFriend(tpilf,"lf");
  h10->AddFriend(tpipf,"pf");
  h10->AddFriend(run,"r");
  h10->AddFriend(tkin,"k");

  TFile felist("/data/e1f/skim/elists.root"); //,"update");
  TEntryList *elf = (TEntryList*)felist->Get("fid_cc");
  TEntryList *el1 = (TEntryList*)felist->Get("t1_mm_cc_fid");
  TEntryList *el2 = (TEntryList*)felist->Get("t2_mm_cc_fid");
  TEntryList *el3 = (TEntryList*)felist->Get("t3_mm_cc_fid");
  //h10->SetEntryList(elist);

  printf("/data/e1f/skim/3????.root\n");
  printf("/data/e1f/skim/3xxxx_run.root\n");
  printf("run->lum(=l)\n");
  printf("runb->lb(=l)\n");
  printf("tpipf->tpilf(=lf)\n");
  printf("h10->tpipf(=t)\n");
  printf("h10->run(=r)\n");
  printf("h10->tkin(=k)\n");
  printf("elist_fid_mm_[123]=el[123]\n");

  gROOT->ProcessLine(".L /home/ephelps/projects/phys-ana-omega/acc.cpp+");
  printf("loaded acc.cpp\n");
  gROOT->ProcessLine(".L /home/ephelps/projects/phys-ana-omega/fid.cpp+");
  printf("loaded fid.cpp\n");
  gROOT->ProcessLine(".L /home/ephelps/projects/phys-ana-omega/particle-constants.h");
  printf("loaded particle-constants.h\n");
  Fid::Instance("/home/ephelps/projects/phys-ana-omega/input/fid.parms");
  printf("loaded Fid instance\n");
  gROOT->ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/scripts/infid.C");
  printf("loaded infid.C\n");
  gROOT->ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/scripts/eff.C");
  printf("loaded eff.cpp\n");
  printf("recompiling h10t3pi_sel.C...\n");
  gROOT->ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/h10t3pi_sel.C++");
  printf("... done.\n");
  gROOT->ProcessLine(".L  /home/ephelps/projects/phys-ana-omega/input/cc_eff_lazy_programmer.h");
  using namespace ParticleConstants;

  const char *sol = TString::Format("%e",SOL).Data();
  const char *mpip = TString::Format("%e",MASS_PIP).Data();
  const char *mp = TString::Format("%e",MASS_P).Data();
  h10->SetAlias("c",sol);
  h10->SetAlias("mp",mp);
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
  h10->SetAlias("ccsect","cc_sect[cc[h10idx_e]-1]");
  h10->SetAlias("ccseg","(cc_segm[cc[h10idx_e]-1]%1000)/10");
  h10->SetAlias("cchit","(cc_segm[cc[h10idx_e]-1]/1000 - 1)");

  printf("Fid::Instance() loaded\n\t from /home/ephelps/projects/phys-ana-omega/fid.cpp\n\t with parameters from /home/ephelps/analysis/omega/input/fid.parms\n");
  printf("infid() loaded\n\t from /home/ephelps/projects/phys-ana-sandbox/sim/infid.C\n");
  printf("h10 aliases: c, mpip, bifpip, dtifpip, sf\n\tfidpass, fidpass[e,p,pip,pim]\n\ttop[1,2,3]pass, mmthreshpass, mmpi0pass\n\tccpass\n");
}
