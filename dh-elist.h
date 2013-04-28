#ifndef DHELIST_H
#define DHELIST_H
#include "data-handler.h"
#include <iostream>
#include <stdexcept>
#include "TDirectory.h"
#include <string>
#include "TFile.h"
#include "TEntryList.h"
#include "Data.h"
class DhElist: public DataHandler
{
public:
	TDirectory *fDir;
	TEntryList *_elist;
	Long64_t _evtnum;
	DhElist(std::string name = "elist", TDirectory *pDir = NULL);
	virtual ~DhElist();
	virtual bool Handle(Data *d);
	virtual void Wrapup(Data *d);
	virtual void Finalize(Data *d);
protected:
};

DhElist::DhElist(std::string name, TDirectory *pDir) : DataHandler(name)
{
	if (pDir == NULL) {
		std::string emsg = name;
		emsg += ": no parent directory, and couldn't create file!";
		std::string newfilename = name += ".root";
		fDir = new TFile(newfilename.c_str(),"create");
		if (fDir == NULL) throw new std::runtime_error(emsg.c_str());
	} else {
		int nexisting = 0;
		std::string emsg = name;
		emsg += ": more than five folders!";
		while ( (fDir = pDir->mkdir(name.c_str())) == 0 ) {
			if (++nexisting>5) {
				throw new std::runtime_error(emsg.c_str());
			}
		}
	}
	fDir->cd();
	_elist = new TEntryList("elist","entrylist",fLumEvents);
}

DhElist::~DhElist()
{
	delete _elist;
	delete fDir;
}

bool DhElist::Handle(Data *data)
{
	h10exp *d = dynamic_cast<h10exp*>(data);
	fLb.ntrigs++;
	bool passed = true;
	bool newrun = (fRun_last != d->run);
	bool newfile = (newrun || (fAnum_last != d->file_anum));
	//if (newfile && !newrun) fLb.gap = (d->file_anum > fAnum_last+1);
	bool newblock = (newrun || (fQl_last < d->q_l));
	bool somethingswrong = (!newfile && (fQl_last > d->q_l));
	/* Assume:
	 * 	+ events are ordered in time and, therefore, by file Anumber and run;
	 *	+ q_l always changes if file changes (verified for e1f data, where
	 * 		leading events of each file have q_l = 0). */
	/* Reject events of unkown luminosity. In e1-f case, these
		result from missing A00 files, making the first non-zero q_l
		in A01 the first known reference for accumulated charge. */
	if ( newrun ) {
		/* set flag for first file in run */
		firstfile = true;
		/* reset lum block data */
		fLb.run = -1;
		fLb.fileAnum = -1;
		fLb.q1 = -1;
		fLb.q2 = -1;
		fLb.dq = 0;
		fLb.qlo_run = -1;
		fLb.qhi_run = -1;
		/* does first file, A00, exist */
		if(d->file_anum != 0) {
			a00exists = false;
		} else {
			a00exists = true;
		}
	} else if(newfile) firstfile = false;
	if (newblock) {
		/* reject unknown-luminosity event */
		if ( firstfile && !a00exists && (d->q_l == 0) ) passed = false;
		else {
			if (!newrun) {
				hLb->Fill(d->run, d->q_l-fQl_last);
				/* ****************** FILL PREVIOUS BLOCK *********************** */
				fillPreviousBlock(d);
			}
			/* ***************** HANDLE CURRENT BLOCK *********************** */
			fLb.run = d->run;
			fLb.fileAnum = d->file_anum;
			//fLb.block++;
			fLb.evt1 = d->evntid;
			fLb.evt2 = -1;
			fLb.q1 = d->q_l;
			fLb.q2 = -1;
			fLb.dq = -1;
			nevtsLbPassed = 0;
			fLb.ntrigs = 0;
			fQl_last = d->q_l;
			fAnum_last = d->file_anum;
			fRun_last = d->run;
			if (somethingswrong) {
				std::cerr << "WARNING! Drop in accumulated charge." << std::endl;
			}
		}
	}
	return passed;
}

void DhElist::Wrapup(Data *d)
{
	nevtsLbPassed++;
	_elist->Enter(++_evtnum);
}

void DhElist::Finalize(Data *data)
{
	h10exp *d = dynamic_cast<h10exp*>(data);
	/* process last block */
	fillPreviousBlock(d);
	fDir->cd();
	hLb->SetDirectory(fDir);
	hLb->Write();
	fLumBlocks->Write();
	fLumEvents->Write();
	_elist->Write();
}

void DhElist::fillPreviousBlock(Data *data)
{
	h10exp *d = dynamic_cast<h10exp*>(data);
	//std::cout << d->q_l << " - " << fQl_last << " = " << d->q_l-fQl_last << std::endl;
	fLb.block++;
	fLb.evt2 = d->evntid - 1;
	fLb.q2 = d->q_l;
	fLb.dq = fLb.q2 - fLb.q1;
	fLb.npassed = nevtsLbPassed;
	fLumBlocks->Fill();
	for (int i = 0; i < nevtsLbPassed; i++) fLumEvents->Fill();
}
#endif // DHELIST_H
