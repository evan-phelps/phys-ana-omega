#ifndef DHKINTREE_H
#define DHKINTREE_H
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

class DH_KinTree : public DataHandler
{
    public:
        TTree *tkin;
        DH_KinTree(std::string name = "KinTree", TDirectory *pDir = NULL, H10 *h10looper = NULL) : DataHandler(name, pDir, h10looper)
        {
            fDir->cd();
            tkin = NULL;
        }
        virtual ~DH_KinTree()
        {
            if (tkin) delete tkin;
        }
    public:
        virtual void Finalize(H10* d)
        {
            fDir->cd();

            if (tkin) {
                tkin->BuildIndex("run", "evntid");
                tkin->Write();
            }
        }
        virtual bool Handle(H10* d)
        {
            //T_ *d = dynamic_cast<T_*>(data);
            bool passed = true;

            if (tkin == NULL)
            {
                tkin = new TTree("tkin", "tkin");

                tkin->Branch("run", &(d->run));
                tkin->Branch("evntid", &(d->evntid));
                tkin->Branch("esector", &(d->esector));

                tkin->Branch("np", &(d->np));
                tkin->Branch("npip", &(d->npip));
                tkin->Branch("npim", &(d->npim));
                tkin->Branch("nother", &(d->nother));
                tkin->Branch("n0", &(d->n0));
                tkin->Branch("npos", &(d->npos));
                tkin->Branch("nneg", &(d->nneg));
                tkin->Branch("nneu", &(d->nneu));
                
                tkin->Branch("E0", &(d->E0));
                tkin->Branch("nu", &(d->nu));
                tkin->Branch("Q2", &(d->Q2));
                tkin->Branch("W", &(d->W));
                tkin->Branch("s", &(d->s));
                tkin->Branch("t", &(d->t));
                tkin->Branch("t0", &(d->t0));
                tkin->Branch("t1", &(d->t1));
                tkin->Branch("MMp", &(d->MMp));
                tkin->Branch("MMppip", &(d->MMppip));
                tkin->Branch("MMppim", &(d->MMppim));
                tkin->Branch("MMppippim", &(d->MMppippim));
                tkin->Branch("cosTheta", &(d->cosTheta));
                tkin->Branch("phi", &(d->phi));
                
                tkin->Branch("lvE0", &(d->lvE0));
                tkin->Branch("lvP0", &(d->lvP0));
                tkin->Branch("lvE1", &(d->lvE1));
                tkin->Branch("lvP1", &(d->lvP1));
                tkin->Branch("lvPip", &(d->lvPip));
                tkin->Branch("lvPim", &(d->lvPim));
                tkin->Branch("lvW", &(d->lvW));
                tkin->Branch("lvq", &(d->lvq));
                tkin->Branch("lvMMp", &(d->lvMMp));
                tkin->Branch("lvMMppip", &(d->lvMMppip));
                tkin->Branch("lvMMppim", &(d->lvMMppim));
                tkin->Branch("lvMMppippim", &(d->lvMMppippim));

            }
            tkin->Fill();

            return passed;
        }
        virtual void Wrapup(H10* d)
        {
        }
};
#endif                           // DHKINTREE_H
