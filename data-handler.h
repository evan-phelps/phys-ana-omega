#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_
#include <string>
#include "TDirectory.h"
#include "data.h"
class DataHandler
{
public:
	DataHandler(std::string name) { fName = name; }
	virtual ~DataHandler() { };
	/* Handle event */
	virtual bool Handle(Data *d) = 0;
	/* Wrapup event if event passes all filters/handlers */
	virtual void Wrapup(Data *d) = 0;
	/* Finalize after all events processed */
	virtual void Finalize(Data *d) = 0;
protected:
	std::string fName;
	
};
#endif // _DATA_HANDLER_H_
