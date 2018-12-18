//--------------------------------------------------------------------------
//
// Copyright Information: See EvtGen/COPYRIGHT
//
// Module: EvtBLLNuL.hh
//
// Description: The header file for the model "BLLNUL" which simulates
//              the rare four-leptonic B-decays
//              B^-(p) -> ell^+(k_1) ell^-(k_2) neu (k_3) ell^-(k_4)
//
// Modification history:
// 
// Anna Danilina (anna.danilina@cern.ch) and 
// Nikolai Nikitin (Nikolai.Nikitine@cern.ch) Nov 2018  Module created
//
//------------------------------------------------------------------------

#ifndef EVTBLLNUL_HH
#define EVTBLLNUL_HH

#include "EvtGenBase/EvtDecayAmp.hh"
#include "EvtGenModels/EvtBLLNuLAmp.hh"

#include <string>

class EvtParticle;
class EvtbTosllMSFF;  // Form factor class

class EvtBLLNuL: public EvtDecayAmp {

public:

    EvtBLLNuL();
    virtual ~EvtBLLNuL();

    virtual std::string getName();
    virtual EvtDecayBase* clone();

    virtual void init();
    virtual void initProbMax();
    virtual void decay(EvtParticle *p);

private:
  
    EvtBLLNuLAmp calcAmp_;

};

#endif
