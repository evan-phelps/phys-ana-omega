#ifndef DHTMH10CLONE_H
#define DHTMH10CLONE_H
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <math.h>

#include "TTree.h"

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

class DH_CloneH10 : public DataHandler
{
    public:
        TTree *tH10;
        int treenumber;
        DH_CloneH10(std::string name = "CloneH10", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            treenumber = -1;
            tH10 = NULL;
            fDir->cd();
        }
        virtual ~DH_CloneH10()
        {
            delete tH10;
        }
    public:
        virtual void Finalize(H10* d)
        {
            fDir->cd();
            if (tH10) tH10->Write();
        }
        virtual bool Handle(H10* data)
        {
            //T_ *d = dynamic_cast<T_*>(data);
            bool passed = true;
            if (treenumber != _h10looper->fChain->GetTreeNumber())
            {
                if (treenumber == -1)
                {
                    fDir->cd();
                    tH10 = (TTree*)_h10looper->fChain->GetTree()->CloneTree(0);
                    tH10->SetDirectory(fDir);
                }
                _h10looper->GetEntry(_h10looper->jentry);
                                 //,kTRUE);
                _h10looper->fChain->CopyAddresses(tH10);
                treenumber = _h10looper->fChain->GetTreeNumber();
            }
            tH10->Fill();
            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DHTM10CLONE_H
