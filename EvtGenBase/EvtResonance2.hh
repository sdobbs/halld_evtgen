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
// Module: EvtGen/EvtResonance2.hh
//
// Description:resonance-defining class
//
// Modification history:
//
//    lange   Nov 21, 2000         Module created
//
//------------------------------------------------------------------------

#ifndef EVTRESONANCE2_HH
#define EVTRESONANCE2_HH

#include "EvtGenBase/EvtVector4R.hh"

class EvtResonance2 {
public:

    //operator
    EvtResonance2& operator = (const EvtResonance2 &);

    // Constructor
    EvtResonance2(const EvtVector4R& p4_p, const EvtVector4R& p4_d1,
		  const EvtVector4R& p4_d2, double ampl = 1.0, double theta = 0.0, 
		  double gamma = 0.0, double bwm = 0.0, int spin = 0, bool invmass_angdenom = false,
		  double  barrier1 = 1.5, double  barrier2 = 5.0);

    //accessors
    //return 4-momenta of the particles involved
    inline const EvtVector4R& p4_p() const { return _p4_p; }
    inline const EvtVector4R& p4_d1() const { return _p4_d1; }
    inline const EvtVector4R& p4_d2() const { return _p4_d2; }

    //return amplitude
    inline double amplitude() const { return _ampl; }

    //return theta
    inline double theta() const { return _theta; }

    //return gamma
    inline double gamma() const { return _gamma; }

    //return bwm
    inline double bwm() const { return _bwm; }

    //return spin
    inline int spin() const { return _spin; }

    //calculate amplitude for this resonance
    EvtComplex resAmpl() const;

private:

    EvtVector4R _p4_p, _p4_d1, _p4_d2;
    double _ampl, _theta, _gamma, _bwm, _barrier1, _barrier2;
    int _spin;
    bool _invmass_angdenom;

};

#endif
