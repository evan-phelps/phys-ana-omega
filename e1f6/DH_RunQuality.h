#ifndef _MON_LUMINOSITY_H_
#define _MON_LUMINOSITY_H_

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#include <iostream>
#include <stdexcept>
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TEntryList.h"

class H10;

class DH_RunQuality: public DataHandler
{
public:
	TDirectory *fDir;
	TTree *fLumBlocks, *fLumEvents;
	TEntryList *_elist;
	Long64_t _evtnum;
	TH2 *hLb;
	DH_RunQuality(std::string name = "unnamed", TDirectory *pDir = NULL);
	virtual ~DH_RunQuality();
	virtual bool Handle(H10 *d);
	virtual void Wrapup(H10 *d);
	virtual void Finalize(H10 *d);
protected:
	void fillPreviousBlock(H10 *d);
	/* number of event-level entries in block to fill */
	int nevtsLbPassed;
	bool a00exists;
	bool firstfile;
	/* values of last event */
	int fRun_last;
	int fAnum_last;
	float fQl_last;
	/* luminosity block-level leaves */
	struct {
		//bool gap;
		int run, fileAnum, block, ntrigs;
		int evt1, evt2, npassed;
		float q1, q2, dq, qlo_run, qhi_run;
	} fLb;
	/* luminosity event-level leaves:
	 * 		just reuse lbRun, lbDq, and lbNum from block-level */
	/* variables for histograms */
	float f_hLb_x0, f_hLb_x1, f_hLb_y0, f_hLb_y1;
	int f_hLb_xn, f_hLb_yn;
	std::string f_hLb_name, f_hLb_title, f_hLb_xtitle, f_hLb_ytitle;
};
#endif // _MON_LUMINOSITY_H_
