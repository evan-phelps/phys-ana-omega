#ifndef _PROCESS_CHAIN_H_
#define _PROCESS_CHAIN_H_
#include <vector>
#include "data-handler.h"
#include "data.h"

class HandlerChain
{
public:
	HandlerChain() { };
	virtual ~HandlerChain();
	virtual void Add(DataHandler *dh);
	virtual bool Process(Data *d);
	virtual void Finalize(Data *d);
protected:
	std::vector< DataHandler* > _handlers;

};
#endif // _PROCESS_CHAIN_H_
