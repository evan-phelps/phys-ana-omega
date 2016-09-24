#define H10_cxx
#include "H10.h"
#include <iostream>
#include <math.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

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

    fHandlerChain->Setup(this);

    _swmain.Start();
    _swgroup.Start();
    for (Long64_t jentry_el=0; jentry_el<ntoproc; jentry_el++)
    {
        jentry = elist ? elist->GetEntry(jentry_el) : jentry_el;
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;

        PrintProgress(jentry);

        GetEntry(jentry);

        if ( gpart > 20 ) continue;
        // if ( !(q[0]==-1 && p[0]<beamEnergy) ) continue;
        //since q[0] must be -1, let's count a negative particle
        //nneg = 1;

        //data->CheapPop(ientry);
        E0 = nu = Q2 = s = W = MMp = MMppip = MMppim = MMppippim = cosTheta = phi = t = t0 = t1 = 0;
        np = npip = npim = nother = n0 = 0;
        npos = nneu = nneg = 0;

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

        CalcLVs();

        // if ( !(np>0 && W>=1.6 && W<=3.2 && MMp>=0.4 && MMp<=1.2) ) continue;

        fHandlerChain->Process(this);
    }

    fHandlerChain->Finalize(this);
    Float_t ttime = _swmain.RealTime();
    Float_t percentProcessed = (Float_t)eventnum/_ntoprocess*100;
    printf("Total: (%.2f) %lld/%.2f = %i events/sec\n",percentProcessed,eventnum,ttime,(Int_t)(eventnum/ttime));
}

void H10::CalcLVs() {
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
    double _phi = lvE1.Phi();        //returns -pi to pi
    if (_phi <= -PI/6) _phi += 2*PI;  //sector 1 is between -30 and 30 degrees
    _phi += PI/6;                    //shift so that sector 1 relates to 0 to 60
    esector = _phi/(PI/3)+1;          //divide by 60, truncate to int add one
    if (esector == 7) esector = 1;    //identify 360 with 0
    int idx[] = {0,0,0};    // h10 idx of proton, pip, pim
    //Since I, above, assumed that the first particle is an electron
    // let's set nneg=1 to start
    nneg = 1;
    if (gpart>1) {
    //We'll check all gpart particles, which permits bad status in particles beyond npart.
        for (int ipart = 1; ipart < gpart; ipart++) {
            if (q[ipart] < 0) nneg++;
            else if (q[ipart] > 0) npos++;
            else nneu++;

            switch(id[ipart]) {
                case PROTON:
                    np++;
                    //only first proton is used
                    if (idx[0]==0) idx[0]=ipart;
                    break;
                case PIP:
                    npip++;
                    //only first pi+ is used
                    if (idx[1]==0) idx[1]=ipart;
                    break;
                case PIM:
                    npim++;
                    //only first pi- is used
                    if (idx[2]==0) idx[2]=ipart;
                    break;
                case 0:
                    n0++;
                    break;
                default:
                    nother++;
                    break;
            }
        }
        if (idx[0]>0) {
        //Set everything that requires proton
            lvP1.SetXYZM(p[idx[0]]*cx[idx[0]], p[idx[0]]*cy[idx[0]], p[idx[0]]*cz[idx[0]], MASS_P);
            t = (lvP1-lvP0).M2();
            lvMMp = lvW-lvP1;
            MMp = lvMMp.M();
            if (idx[2]>0) {
            //Set everything that requires pi-
                lvPim.SetXYZM(p[idx[2]]*cx[idx[2]], p[idx[2]]*cy[idx[2]], p[idx[2]]*cz[idx[2]], MASS_PIM);
                lvMMppim = lvMMp-lvPim;
                MMppim = lvMMppim.M();
            }
            if (idx[1]>0) {
            //Set everything that requires pi+
                lvPip.SetXYZM(p[0]*cx[idx[1]], p[idx[1]]*cy[idx[1]], p[idx[1]]*cz[idx[1]], MASS_PIP);
                lvMMppip = lvMMp-lvPip;
                MMppip = lvMMppip.M();
                if (idx[2]>0) {
                    lvMMppippim = lvMMppip-lvPim;
                    MMppippim = lvMMppippim.M();
                }
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
}
using namespace TMath;

void H10::GetUVW(double xyz[3], double uvw[3]) {
  enum { X, Y, Z };
  enum { u, v, w };
  Float_t _phi, ec_phi, ec_the, tgrho, sinrho, cosrho;
  Float_t ylow, yhi, xi, yi, zi;
  ec_the = 0.4363323;
  ylow = -182.974;
  yhi = 189.956;
  tgrho = 1.95325;
  sinrho = 0.8901256;
  cosrho = 0.455715;
  //----------------
  _phi = ATan2(xyz[Y], xyz[X]) * 57.29578;
  if (_phi < 0.)
    _phi += 360.;
  _phi = _phi + 30.;
  if (_phi > 360.)
    _phi -= 360.;
  ec_phi = 1.0471975 * int(_phi / 60.);
  //----------------
  float rot11 = cos(ec_the) * cos(ec_phi);
  float rot12 = -sin(ec_phi);
  float rot13 = sin(ec_the) * cos(ec_phi);
  float rot21 = cos(ec_the) * sin(ec_phi);
  float rot22 = cos(ec_phi);
  float rot23 = sin(ec_the) * sin(ec_phi);
  float rot31 = -sin(ec_the);
  float rot32 = 0.;
  float rot33 = cos(ec_the);
  //----------------
  yi = xyz[X] * rot11 + xyz[Y] * rot21 + xyz[Z] * rot31;
  xi = xyz[X] * rot12 + xyz[Y] * rot22 + xyz[Z] * rot32;
  zi = xyz[X] * rot13 + xyz[Y] * rot23 + xyz[Z] * rot33;
  zi -= 510.32;
  //----------------
  uvw[u] = (yi - ylow) / sinrho;
  uvw[v] = (yhi - ylow) / tgrho - xi + (yhi - yi) / tgrho;
  uvw[w] = ((yhi-ylow)/tgrho+xi+(yhi-yi)/tgrho)/2./cosrho;
}
