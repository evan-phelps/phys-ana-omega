#ifndef DHEID_H
#define DHEID_H
#include "data-handler.h" // Base class: DataHandler
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TH1.h"
#include "eid.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhEid : public DataHandler
{
public:
	TH1I *hevtsum;
	Eid *eid;
	static const Int_t NUM_EVTCUTS = 13;
	enum { EVT_NULL, EVT_TRIG, EVT_GPART1, EVT_STAT1, EVT_Q1,
	       EVT_SC1, EVT_DC1, EVT_EC1, EVT_CC1,
	       EVT_DCSTAT1, EVT_ECLOW1, EVT_SF, EVT_EI, EVT_BOS11
	     };
	TDirectory *fDir;
	DhEid(std::string name = "eid", TDirectory *pDir = NULL, string infile = "input/eid.parms") : DataHandler(name) {
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
				if (++nexisting>5) throw new std::runtime_error(emsg.c_str());
			}
		}
		eid = Eid::Instance(infile);
		fDir->cd();
		hevtsum = new TH1I("hevtsum","Event Statistics",NUM_EVTCUTS,0.5,NUM_EVTCUTS+0.5);
		hevtsum->GetXaxis()->SetBinLabel(EVT_TRIG,"Trigger");
		hevtsum->GetXaxis()->SetBinLabel(EVT_GPART1,"gpart>1");
		hevtsum->GetXaxis()->SetBinLabel(EVT_STAT1,"stat>0");
		hevtsum->GetXaxis()->SetBinLabel(EVT_Q1,"q=-1");
		hevtsum->GetXaxis()->SetBinLabel(EVT_SC1,"SC");
		hevtsum->GetXaxis()->SetBinLabel(EVT_DC1,"DC");
		hevtsum->GetXaxis()->SetBinLabel(EVT_EC1,"EC");
		hevtsum->GetXaxis()->SetBinLabel(EVT_ECLOW1,"EC Theshold");
		hevtsum->GetXaxis()->SetBinLabel(EVT_CC1,"CC");
		hevtsum->GetXaxis()->SetBinLabel(EVT_DCSTAT1,"Time-based");
		hevtsum->GetXaxis()->SetBinLabel(EVT_SF,"SF");
		hevtsum->GetXaxis()->SetBinLabel(EVT_EI,"EC Inner");
		hevtsum->GetXaxis()->SetBinLabel(EVT_BOS11,"EVNT.id=11");
	}
	virtual ~DhEid() {
		delete hevtsum;
		delete fDir;
	}
public:
	virtual void Finalize(Data* data) {
		fDir->cd();
		hevtsum->SetDirectory(fDir);
		hevtsum->Write();
	}
	virtual bool Handle(Data* data) {
		T_ *d = dynamic_cast<T_*>(data);
		bool passed = IsE(d);
		return passed;
	}
	virtual void Wrapup(Data* d) {
	}
	bool IsE(Data* data) {
		T_ *d = dynamic_cast<T_*>(data);
		Bool_t retval = kFALSE;
		hevtsum->Fill(EVT_TRIG);
		if (d->id[0]==ELECTRON) hevtsum->Fill(EVT_BOS11);
		if (d->gpart>1) {
			hevtsum->Fill(EVT_GPART1);
			if (d->stat[0]>0) {
				hevtsum->Fill(EVT_STAT1);
				if (d->q[0]==-1) {
					hevtsum->Fill(EVT_Q1);
					if (d->sc[0]>0) {
						hevtsum->Fill(EVT_SC1);
						if (d->dc[0]>0) {
							hevtsum->Fill(EVT_DC1);
							if (d->ec[0]>0) {
								hevtsum->Fill(EVT_EC1);
								if (d->cc[0]>0 || d->is_sim) {
									hevtsum->Fill(EVT_CC1);
									if (d->dc_stat[d->dc[0]-1]>0) {
										hevtsum->Fill(EVT_DCSTAT1);
										if (eid->PassThreshold(d->p[0])) {
											hevtsum->Fill(EVT_ECLOW1);
											Int_t sector = d->sc_sect[d->sc[0]-1];
											Float_t mom = d->p[0];
											Float_t sf = d->etot[d->ec[0]-1]/d->p[0];
											if (eid->PassSF(sector,mom,sf)) {
												hevtsum->Fill(EVT_SF);
												Float_t ei = d->ec_ei[d->ec[0]-1];
												if (eid->PassEi(ei)) {
													hevtsum->Fill(EVT_EI);
													if (!eid->Pass(sector,mom,sf)) {
														hevtsum->Fill(NUM_EVTCUTS+1);
													}
													retval = kTRUE;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return retval;
	}
};
#endif // DHEID_H
