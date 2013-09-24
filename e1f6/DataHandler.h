#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_
#include <iostream>
#include <stdexcept>
#include <string>
#include "TFile.h"
#include "TDirectory.h"

class H10;

class DataHandler
{
protected:
	std::string fName;
	TDirectory *fDir;
	H10 *_h10looper;
public:
	DataHandler(std::string name, TDirectory *pDir, H10 *h10looper = NULL) {
		fName = name;
		fDir = pDir;
		_h10looper = h10looper;
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
				} else {
					name += nexisting;
				}
			}
		}
	};
	virtual ~DataHandler() {
		delete fDir;
	};
	void SetH10Looper(H10 *h10looper) { _h10looper = h10looper; }
	/* Handle event */
	virtual bool Handle(H10 *d) = 0;
	/* Wrapup event if event passes all filters/handlers */
	virtual void Wrapup(H10 *d) = 0;
	/* Finalize after all events processed */
	virtual void Finalize(H10 *d) = 0;
	std::string GetName() { return fName; }
};
#endif // _DATA_HANDLER_H_
