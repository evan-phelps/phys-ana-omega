#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_
#include <string>
#include "TDirectory.h"

class H10;

class DataHandler
{
protected:
	std::string fName;
public:
	DataHandler(std::string name) { fName = name; }
	virtual ~DataHandler() { };
	/* Handle event */
	virtual bool Handle(H10 *d) = 0;
	/* Wrapup event if event passes all filters/handlers */
	virtual void Wrapup(H10 *d) = 0;
	/* Finalize after all events processed */
	virtual void Finalize(H10 *d) = 0;
	std::string GetName() { return fName; }
};
#endif // _DATA_HANDLER_H_
