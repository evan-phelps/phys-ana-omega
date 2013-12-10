#ifndef DH_W_SKIM_H_
#define DH_W_SKIM_H_
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#ifndef H10_h
#include "H10.h"
#endif
class H10;

class DH_W_Skim : public DataHandler
{
    public:
        DH_W_Skim(std::string name = "DH_W_Skim", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_W_Skim()
        {
            fDir->cd();
        }
        virtual void Setup(H10 *d)
        {
        }
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* d)
        {
            return d->W > 1.6 && d->W < 3.2;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_W_SKIM_H_
