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
#ifndef H10_h
#include "H10.h"
#endif
#include <TLorentzVector.h>
class H10;

class DH_RunQuality: public DataHandler
{
public:
	TDirectory *fDir;
	DH_RunQuality(std::string name = "unnamed", TDirectory *pDir = NULL);
	virtual ~DH_RunQuality();
	virtual bool Handle(H10 *d);
	virtual void Wrapup(H10 *d);
	virtual void Finalize(H10 *d);
	/* luminosity block-level leaves */
	struct sLB {
		char file_name[256];
		UShort_t run_num;
		UChar_t file_num;
		UInt_t lumblock_num;
		ULong64_t evt_num_first;
		ULong64_t evt_num_last;
		Float_t fc_charge_ltcorr;
		Float_t livetime;
		ULong64_t ntrigs;
		ULong64_t ne;
		ULong64_t np;
		ULong64_t npip;
		ULong64_t npim;
		ULong64_t nevts_Neg1st;
		ULong64_t nevts_Neg1st_2Pos_exc;
		ULong64_t nevts_Neg1st_1Pos1Neg_exc;
		ULong64_t nevts_Neg1st_2Pos1Neg_exc;
	} fLb;
	TH2 *hq2_V_w;
	TH2 *hq2_V_w_elast_exc;
	TH2 *hmmppippim_V_mmp;
protected:
	TLorentzVector *lvE0, *lvE1, *lvP0, *lvP1, *lvPip, *lvPim;
};
#endif // _MON_LUMINOSITY_H_
