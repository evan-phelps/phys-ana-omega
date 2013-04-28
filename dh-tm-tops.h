#ifndef DHTMTOPS_H
#define DHTMTOPS_H

#include "data-handler.h" // Base class: DataHandler

class DhTmTops : public DataHandler
{
public:
	DhTmTops()
	{
	}
	virtual ~DhTmTops()
	{
	}

public:
	virtual void Finalize(Data* d)
	{
	}
	virtual bool Handle(Data* d)
	{
	}
	virtual void Wrapup(Data* d)
	{
	}
};

#endif // DHTMTOPS_H
