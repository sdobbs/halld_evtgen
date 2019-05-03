//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtGen/EvtBToDiBaryonlnupQCD.hh
//
// Description: Class to handle semileptonic B -> Baryon Anti-baryon l nu decays 
// using the using form factor predictions from pQCD counting rules. Taken
// from arXiv:1107.0801
// 
//
// Modification history:
//
//    Mark Smith     July 17, 2017     Module created
//    John B         Oct 2018          Code simplification
//
//------------------------------------------------------------------------

#ifndef EVTBTODIBARYONLNUPQCD_HH
#define EVTBTODIBARYONLNUPQCD_HH

#include "EvtGenBase/EvtDecayAmp.hh"
#include <string>

class EvtParticle;
class EvtBToDiBaryonlnupQCDFF;
class EvtSLDiBaryonAmp;

class EvtBToDiBaryonlnupQCD: public EvtDecayAmp  {

public:

    EvtBToDiBaryonlnupQCD();
    virtual ~EvtBToDiBaryonlnupQCD();

    std::string getName();
    EvtDecayBase* clone();

    void decay(EvtParticle *p);
    void initProbMax();
    void init();

private:

    EvtBToDiBaryonlnupQCDFF* ffModel_;
    EvtSLDiBaryonAmp* calcAmp_;

};

#endif

