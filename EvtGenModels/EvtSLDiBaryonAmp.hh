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
//    John B     Oct 2018      Code simplification
//
//------------------------------------------------------------------------

#ifndef EVTSLDIBARYONAMP_HH
#define EVTSLDIBARYONAMP_HH

#include "EvtGenBase/EvtAmp.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenModels/EvtBToDiBaryonlnupQCDFF.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector4C.hh"

#include <vector>

class EvtParticle;

class EvtSLDiBaryonAmp {

public:

    EvtSLDiBaryonAmp(const EvtBToDiBaryonlnupQCDFF&);

    void CalcAmp(EvtParticle *parent, EvtAmp& amp) const;

protected:

    int checkDibaryonParity(const EvtId& id1, const EvtId& id2,
			    const int J1, const int J2) const;

    int getBaryonParity(const EvtId& id) const;

    std::vector<EvtVector4C> getHadronicCurrents(const EvtDiracSpinor& u, const EvtDiracSpinor& v,
						 const EvtVector4R& p, const EvtVector4R& gMtmTerms,
						 const EvtVector4R& fMtmTerms) const;

private:

    EvtBToDiBaryonlnupQCDFF ffModel_;
    
};

#endif


