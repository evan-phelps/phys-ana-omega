#ifndef _PROCESS_CHAIN_H_
#define _PROCESS_CHAIN_H_
#include <vector>

#include "DataHandler.h"
//#ifndef H10_h
//#include "H10.h"
//#endif
class H10;

class HandlerChain
{
    public:
        HandlerChain() { };
        virtual ~HandlerChain();
        virtual void Add(DataHandler *dh);
        virtual void Setup(H10 *d);
        virtual bool Process(H10 *d);
        virtual void Finalize(H10 *d);
    protected:
        std::vector< DataHandler* > _handlers;
};
#endif                           // _PROCESS_CHAIN_H_
