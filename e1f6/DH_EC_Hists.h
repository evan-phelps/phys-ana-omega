#ifndef DH_EC_HISTS_H_
#define DH_EC_HISTS_H_
#include <stdio.h>
#include <stdexcept>
#include <string>

#ifndef _DATA_HANDLER_H_
#include "DataHandler.h"
#endif

#ifndef H10_h
#include "H10.h"
#endif
class H10;

#ifndef H10CONSTANTS_H_
#include "H10Constants.h"
#endif

using namespace H10Constants;

class DH_EC_Hists : public DataHandler
{
    public:
        DH_EC_Hists(std::string name = "EC_Hists", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
        }
        virtual ~DH_EC_Hists()
        {
        }
    public:
        virtual void Finalize(H10* d)
        {
            fDir->cd();
        }
        virtual bool Handle(H10* data)
        {
            bool passed = true;
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DH_EC_HISTS_H_
