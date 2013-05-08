#include <iostream>
#include "stdio.h"
#include <stdexcept>
#include "TROOT.h"
#include "TRint.h"
#include "TChain.h"
#include "TTree.h"
#include "h10exp.h"
#include "h10sim.h"
#include "h10looper.h"
#include "fid.h"
#include "data-handler.h"
#include "handler-chain.h"
#include "dh-calc-luminosity.h"
#include "dh-skim-charge.h"
#include "dh-skim-charged-parts.h"
#include "dh-eid.h"
#include "dh-pcor.h"
#include "dh-fid.h"
#include "dh-tm-h10clone.h"
#include "dh-tm-3pi.h"
#include "dh-3pi-hists.h"
#include "dh-3pi-to-phys-frm.h"
#include "dh-top1.h"
enum { _DATA_H10EXP, _DATA_H10SIM, _DATA_3PI };
int main(int argc, char **argv)
{
	printf("Bazinga!\n");
	try {
		/* get data */
		std::cout << "argc = " << argc << endl;
		if (argc == 1) {
			//_mychain->Add("/home/ephelps/analysis/root_37808_pass1.a20.root/h10");
			/* open cint session */
			int argc_root = 0;
			Fid *fid = Fid::Instance();
			fid->Print();
			TRint *app = new TRint("Rint",&argc_root,argv,NULL,0);
			app->Run();
		} else {
			TChain *_mychain = new TChain();
			int dtype = _DATA_H10SIM;
			for (int i = 1; i < argc; i++) {
				_mychain->Add(argv[i]);
				std::cout << i << ": " << argv[i] << std::endl;
			}
			if (_mychain->GetBranch("rf_time2")) dtype = _DATA_H10EXP;
			else if(_mychain->GetBranch("w4")) dtype = _DATA_3PI;
			else dtype = _DATA_H10SIM;
			TFile fout("out.root","recreate");
			h10looper *_mylooper = 0;
			Data *data = 0;
			if (dtype == _DATA_H10EXP) {
				data = new h10exp();
				//TFile fin_el("/data/e1f/exp-el.root");
				TEntryList *elist = 0; //(TEntryList*)fin_el.Get("q2_18_27_pippim");
				h10looper *_mylooper = new h10looper(data, _mychain);
				_mylooper->Add(new DhCalcLuminosity("accuq",&fout));
				_mylooper->Add(new DhSkimCharge<h10exp>("skimq",&fout));
				_mylooper->Add(new DhEid<h10exp>("skime",&fout,"/home/ephelps/analysis/omega/input/eid.exp.out"));
				_mylooper->Add(new DhPcor("pcor",&fout));
				_mylooper->Add(new DhSkimChargedParts<h10exp>("skim1or2pi",&fout));
				_mylooper->Add(new DhTmH10clone<h10exp>(_mylooper,"h10clone",&fout));
				_mylooper->Add(new DhTm3pi<h10exp>("3pi-tree",&fout));
				//_mylooper->Add(new DhFid<h10exp>("fid",&fout,"/home/ephelps/analysis/omega/input/fid.parms"));
				/* process data */
				_mylooper->Loop(-1,kFALSE,elist);
			} else if (dtype == _DATA_H10SIM) {
				data = new h10sim();
				_mylooper = new h10looper(data, _mychain);
				_mylooper->Add(new DhTm3pi<h10sim,true>("3pi-tree-thrown-all",&fout));
				_mylooper->Add(new DhSkimCharge<h10sim>("skimq",&fout));
				_mylooper->Add(new DhEid<h10sim>("skime",&fout,"/home/ephelps/analysis/omega/input/eid.mc.out"));
				_mylooper->Add(new DhSkimChargedParts<h10sim>("skim1or2pi",&fout));
				_mylooper->Add(new DhTmH10clone<h10sim>(_mylooper,"h10clone",&fout));
				_mylooper->Add(new DhTm3pi<h10sim,true>("3pi-tree-thrown",&fout));
				_mylooper->Add(new DhTm3pi<h10sim,false>("3pi-tree-recon",&fout));
				//_mylooper->Add(new DhFid<h10exp>("fid",&fout,"/home/ephelps/analysis/omega/input/fid.parms"));
				/* process data */
				_mylooper->Loop(-1,kFALSE);
			} else if (dtype == _DATA_3PI) {
				data = new Evt();
				_mylooper = new h10looper(data, _mychain);
				//_mylooper->Add(new Dh3piToPhysFrm(_mylooper,"3pi-cm",&fout));
				//_mylooper->Add(new Dh3piHists("3pi-hists",&fout));
				_mylooper->Add(new DhTop1("top1",&fout));
				_mylooper->Loop(-1,kFALSE);
			}
			if (_mychain) delete _mychain;
			if (_mylooper) delete _mylooper;
			if (data) delete data;
			/* clean up */
		}
	} catch(exception *e) {
		std::cout << e->what() << std::endl;
		delete e;
		return 88;
	}
	printf("!agnizaB\n");
	return 0;
}
