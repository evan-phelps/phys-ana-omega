#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_
#include <iostream>
#include <stdexcept>
#include <string>
#include "TFile.h"
#include "TDirectory.h"
#include "TObjArray.h"
#include "TString.h"
#include "TH2.h"

class H10;

class DataHandler
{
    protected:
        static const Int_t NSECTS = 6;
        std::string fName;
        TDirectory *fDir;
        H10 *_h10looper;
    public:
        DataHandler(std::string name, TDirectory *pDir, H10 *h10looper = NULL)
        {
            fName = name;
            fDir = pDir;
            _h10looper = h10looper;
            if (pDir == NULL)
            {
                std::string emsg = name;
                emsg += ": no parent directory, and couldn't create file!";
                std::string newfilename = name += ".root";
                fDir = new TFile(newfilename.c_str(),"create");
                if (fDir == NULL) throw new std::runtime_error(emsg.c_str());
            }
            else
            {
                int nexisting = 0;
                std::string emsg = name;
                emsg += ": more than five folders!";
                while ( (fDir = pDir->mkdir(name.c_str())) == 0 )
                {
                    if (++nexisting>5)
                    {
                        throw new std::runtime_error(emsg.c_str());
                    }
                    else
                    {
                        name += nexisting;
                    }
                }
            }
        };
        virtual ~DataHandler()
        {
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
        vector<TH2*> MakeHists(Int_t N, const char* nametmpl, const char* titletmpl,
            Int_t nbinsx, Float_t xlo, Float_t xhi,
            Int_t nbinsy, Float_t ylo, Float_t yhi)
        {
            vector<TH2*> ret(N);
            for (int n = 1; n <= N; n++)
            {
                TString name = TString::Format(nametmpl,n);
                TString title = TString::Format(titletmpl,n);
                ret.push_back(new TH2D(name.Data(), title.Data(), nbinsx, xlo, xhi, nbinsy, ylo, yhi));
            }
            return ret;
        }
        static void WriteObj(TObject *o) { o->Write(); };

        Bool_t IsElastic()
        {
            return kFALSE;
        }
};
#endif                           // _DATA_HANDLER_H_
