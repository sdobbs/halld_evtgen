//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtGen/EvtSLDiBaryonAmp.hh
//
// Description:
//
// Modification history:
//
//    Mark Smith July 18, 2017 Created
//
//------------------------------------------------------------------------

#ifndef EVTSLDIBARYONAMP_HH
#define EVTSLDIBARYONAMP_HH

#include "EvtGenBase/EvtAmp.hh"
#include <vector>

class EvtParticle;
class EvtBToDiBaryonlnupQCDFF;

class EvtSLDiBaryonAmp {

public:

    EvtSLDiBaryonAmp(EvtBToDiBaryonlnupQCDFF*);

    virtual ~EvtSLDiBaryonAmp() {;}
    
    void CalcAmp(EvtParticle *parent, EvtAmp& amp) const;

private:

    EvtBToDiBaryonlnupQCDFF* ffModel;

};

#endif


