#ifndef SKIMCHARGE_H
#define SKIMCHARGE_H
#include <stdexcept>
#include "data-handler.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "particle-constants.h"
using namespace ParticleConstants;
template <class T_>
class DhSkimCharge : public DataHandler
{
public:
	TDirectory *fDir;
	TH1I *hevtsum;
	static const Int_t NUM_EVTCUTS = 8;
	enum {
	    EVT_NULL, EVT, EVT_1ST_NEG, EVT_LT8, EVT_1ST_E, EVT_1POS,
	    EVT_2POS_EX, EVT_1POS1NEG_EX, EVT_2POS1NEG_EX
	};
	DhSkimCharge(std::string name = "unnamed", TDirectory *pDir = NULL) : DataHandler(name) {
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
		hevtsum = new TH1I("hevtsum","Event Statistics",NUM_EVTCUTS,0.5,NUM_EVTCUTS+0.5);
		hevtsum->GetXaxis()->SetBinLabel(EVT,"Total");
		hevtsum->GetXaxis()->SetBinLabel(EVT_1ST_NEG,"X^{-} 1^{st}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_LT8,"gpart < 8");
		hevtsum->GetXaxis()->SetBinLabel(EVT_1ST_E,"e^{-} 1^{st}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_1POS,"e^{-} + X^{+} + X");
		hevtsum->GetXaxis()->SetBinLabel(EVT_2POS_EX,"e^{-} + 2X^{+}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_1POS1NEG_EX,"e^{-} + X^{+} + X^{-}");
		hevtsum->GetXaxis()->SetBinLabel(EVT_2POS1NEG_EX,"e^{-} + 2X^{+} + X^{-}");
	}
	virtual ~DhSkimCharge() {
		delete hevtsum;
		delete fDir;
	}
public:
	virtual void Finalize(Data *data) {
		fDir->cd();
		hevtsum->SetDirectory(fDir);
		hevtsum->Write();
	}
	virtual bool Handle(Data *data) {
		T_ *d = dynamic_cast<T_*>(data);
		bool passed = false;
		hevtsum->Fill(EVT);
		if ( d->q[0] == -1 ) {
			hevtsum->Fill(EVT_1ST_NEG);
			if ( d->gpart <= 8 ) {
				hevtsum->Fill(EVT_LT8);
				if ( d->id[0] == ELECTRON ) {
					hevtsum->Fill(EVT_1ST_E);
					Int_t numPos = 0;
					Int_t numNeg = 0;
					Int_t num0 = 0;
					for (Int_t gpnum = 1; gpnum < d->gpart; gpnum++) {
						if ( d->q[gpnum] == 1 ) numPos++;
						else if ( d->q[gpnum] == -1 ) numNeg++;
						else if ( d->q[gpnum] == 0 ) num0++;
					}
					if ( numPos > 0 ) hevtsum->Fill(EVT_1POS);
					Int_t binNum = 0;
					if ( numPos == 2 && numNeg == 0 ) binNum = hevtsum->Fill(EVT_2POS_EX);
					if ( numPos == 1 && numNeg == 1 ) binNum = hevtsum->Fill(EVT_1POS1NEG_EX);
					if ( numPos == 2 && numNeg == 1 ) binNum = hevtsum->Fill(EVT_2POS1NEG_EX);
					if ( binNum > 0 ) {
						passed = true;
					}
				}
			}
		}
		return passed;
	}
	virtual void Wrapup(Data *d) {
	}
};
#endif // SKIMCHARGE_H
