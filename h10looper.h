#ifndef h10_h
#define h10_h
#include <iostream>
#include "TRegexp.h"
#include "TChain.h"
#include "TEntryList.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "handler-chain.h"
#include "data.h"
class h10looper
{
public :
	int _cachesize;
	Long64_t jentry;
	h10looper();
	h10looper(Data *d, TTree *tree=0, int cachesize = -1);
	virtual ~h10looper();
	TTree *fChain;   //!pointer to the analyzed TTree or TChain
	TRegexp *fRegExp_run, *fRegExp_Anum;
	int fTreeNumber;
	Int_t fCurrent; //!current Tree number in a TChain
	Data *data;
	HandlerChain *fHandlerChain;
	virtual void Add(DataHandler *dhandler) {
		fHandlerChain->Add(dhandler);
	}
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual void     Loop(Long64_t ntoproc = -1, Bool_t fastcount = kTRUE, TEntryList *elist = 0);
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
protected:
	Long64_t _ntoprocess;
	TStopwatch _swmain;
	TStopwatch _swgroup;
	static const Int_t blocksize = 100000;
	Long64_t eventnum;
	void PrintProgress(Long64_t entry) {
		if ( ++eventnum % blocksize == 0 ) {
			Float_t gtime = _swgroup.RealTime();
			Float_t ttime = _swmain.RealTime();
			Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
			Float_t remaining = (100/percentProcessed*ttime-ttime)/60;
			printf("(%.2f) %lld/%.2f = %i events/sec | block = %i events/sec ... %.1f min remaining\n",percentProcessed,eventnum,ttime,((Int_t)(eventnum/ttime)),(Int_t)(blocksize/gtime),remaining);
			_swgroup.Start();
			_swmain.Start(kFALSE);
		}
	}

};
#endif
