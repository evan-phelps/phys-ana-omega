#include "HandlerChain.h"

void HandlerChain::Add(DataHandler *dh)
{
    _handlers.push_back(dh);
}


bool HandlerChain::Process(H10 *d)
{
    bool passed = true;
    unsigned int hnum = 0;
    for (hnum = 0; hnum < _handlers.size(); hnum++)
    {
        DataHandler *h = _handlers[hnum];
        passed = h->Handle(d);
        if (!passed) break;
    }
    if (passed)
    {
        while (hnum-- > 0)
        {
            DataHandler *h = _handlers[hnum];
            h->Wrapup(d);
        }
    }
    return passed;
}


void HandlerChain::Finalize(H10 *d)
{
    unsigned int hnum = 0;
    for (hnum = 0; hnum < _handlers.size(); hnum++)
    {
        DataHandler *h = _handlers[hnum];
        h->Finalize(d);
    }
}


HandlerChain::~HandlerChain()
{
    _handlers.clear();
}
