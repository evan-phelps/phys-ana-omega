#define H10_cxx
#include "H10.h"
#include <iostream>
#include <math.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void H10::Loop(Long64_t ntoproc/* = -1*/, Bool_t fastcount/* = kTRUE*/, TEntryList *elist/* = 0 */)
{
    if (fChain == 0) return;
    if (ntoproc == -1)
    {
        if (fastcount) ntoproc = fChain->GetEntriesFast();
        else ntoproc = elist ? elist->GetN() : fChain->GetEntries();
    }
    std::cout << "ntoproc = " << ntoproc << std::endl;
    _ntoprocess = ntoproc;
    //Long64_t nbytes = 0, nb = 0;
    _swmain.Start();
    _swgroup.Start();
    for (Long64_t jentry_el=0; jentry_el<ntoproc; jentry_el++)
    {
        jentry = elist ? elist->GetEntry(jentry_el) : jentry_el;
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        PrintProgress(jentry);
        //data->CheapPop(ientry);
        E0 = nu = Q2 = s = W = MMp = MMppip = MMppim = MMppippim = cosTheta = phi = t = t0 = t1 = 0;
        np = npip = npim = 0;
        lvE1.SetXYZM(0,0,0,0);
        lvP1.SetXYZM(0,0,0,0);
        lvPip.SetXYZM(0,0,0,0);
        lvPim.SetXYZM(0,0,0,0);
        lvW.SetXYZM(0,0,0,0);
        lvq.SetXYZM(0,0,0,0);
        lvMMp.SetXYZM(0,0,0,0);
        lvMMppip.SetXYZM(0,0,0,0);
        lvMMppim.SetXYZM(0,0,0,0);
        lvMMppippim.SetXYZM(0,0,0,0);

        GetEntry(jentry);

        E0 = beamEnergy;
        //nu = E0-p[0];
        //Q2 = -(nu*nu-p[0]*p[0]-E0*E0+2*E0*p[0]*cz[0]);
        //s = -Q2+2*MASS_P*nu+MASS_P*MASS_P;
        //W = s >= 0 ? sqrt(s) : -sqrt(-s);

        /* Very inefficient to do this for all events.  Could refactor
        by moving lv's to DataHandler and allow subclasses to flag for
        population and check for already-populated state before
        proceeding.  For now, I'm just populating the lv's here.
        More simply, it could be pulled into separate data handler
        to be added into the chain of data handlers when needed! */
        lvE1.SetXYZM(p[0]*cx[0], p[0]*cy[0], p[0]*cz[0], MASS_E);
        nu = lvE1[3]-lvE0[3];
        lvq = lvE0-lvE1;
        lvW = lvq+lvP0;
        Q2 = -lvq.M2();
        s = lvW.M2();
        W = lvW.M();

        int idx[] = {0,0,0};    // h10 idx of proton, pip, pim
        if (gpart>1) {
            for (int ipart = 1; ipart < gpart; ipart++) {
                switch(id[ipart]) {
                    case 2212:
                        np++;
                        if (idx[0]==0) idx[0]=ipart;
                        break;
                    case 211:
                        npip++;
                        if (idx[1]==0) idx[1]=ipart;
                        break;
                    case -211:
                        npim++;
                        if (idx[2]==0) idx[2]=ipart;
                        break;
                    default:
                        break;
                }
            }
            if (idx[0]>0) {
                lvP1.SetXYZM(p[idx[0]]*cx[idx[0]], p[idx[0]]*cy[idx[0]], p[idx[0]]*cz[idx[0]], MASS_P);
                t = (lvP1-lvP0).M2();
                lvMMp = lvW-lvP1;
                MMp = lvMMp.M();
                if (idx[1]>0) {
                    lvPip.SetXYZM(p[0]*cx[idx[1]], p[idx[1]]*cy[idx[1]], p[idx[1]]*cz[idx[1]], MASS_PIP);
                    lvMMppip = lvMMp-lvPip;
                    MMppip = lvMMppip.M();
                    if (idx[2]>0) {
                        lvMMppippim = lvMMppip-lvPim;
                        MMppippim = lvMMppippim.M();
                    }
                }
                if (idx[2]>0) {
                    lvPim.SetXYZM(p[idx[2]]*cx[idx[2]], p[idx[2]]*cy[idx[2]], p[idx[2]]*cz[idx[2]], MASS_PIM);
                    lvMMppim = lvMMp-lvPim;
                    MMppim = lvMMppim.M();
                }
                //4-rotate into boosted frame and get CM variables
                TVector3 uz = lvq.Vect().Unit();
                TVector3 ux = (lvE0.Vect().Cross(lvE1.Vect())).Unit();
                ux.Rotate(-PI/2,uz);
                TRotation _3rot;
                _3rot.SetZAxis(uz,ux).Invert();
                TLorentzRotation _4rot(_3rot);
                TVector3 _boost(-1*lvW.BoostVector());
                _4rot *= _boost;
                TLorentzVector _mmp = lvMMp;
                _mmp.Transform(_4rot);
                cosTheta = _mmp.CosTheta();
                phi = _mmp.Phi();
                //printf("(%.2f, %.2f)\n", phi, cosTheta);
                t = (lvP1-lvP0).M2();
                TLorentzVector _p0 = lvP0;
                TLorentzVector _p1 = lvP1;
                _p0.Transform(_4rot);
                _p1.Transform(_4rot);
                t0 = pow(_p0.E()-_p1.E(),2)-pow(_p0.P()-_p1.P(),2);
                t1 = t-t0;
            }
        }
        fHandlerChain->Process(this);
    }

    fHandlerChain->Finalize(this);
    Float_t ttime = _swmain.RealTime();
    Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
    printf("Total: (%.2f) %lld/%.2f = %i events/sec\n",percentProcessed,eventnum,ttime,(Int_t)(eventnum/ttime));
}
