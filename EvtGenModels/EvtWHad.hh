//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package. If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//
// Module: EvtWHad.hh
//
// Description: Routine to calculate W -> (n pi) + (m K) current
//			according to [Kuhn, Was, Acta.Phys.Polon B39 (2008) 147]
//
// Modification history:
//	A V Luchinsky  	20 Jan, 2013	Module created
//
//------------------------------------------------------------------------

#ifndef EvtWHad_HH
#define EvtWHad_HH

#include "EvtGenBase/EvtComplex.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"

#include <vector>

class EvtWHad {

public:

    EvtWHad();

    EvtVector4C WCurrent(const EvtVector4R& q1) const;

    EvtVector4C WCurrent(const EvtVector4R& q1, const EvtVector4R& q2) const;

    EvtVector4C WCurrent(const EvtVector4R& q1, const EvtVector4R& q2, const EvtVector4R& q3) const;

    EvtVector4C WCurrent(const EvtVector4R& q1, const EvtVector4R& q2, const EvtVector4R& q3,
			 const EvtVector4R& q4, const EvtVector4R& q5) const;

    EvtVector4C WCurrent_KKP(const EvtVector4R& pKplus, const EvtVector4R& pKminus,
			     const EvtVector4R& pPiPlus) const;

    EvtVector4C WCurrent_KPP(const EvtVector4R& pKplus, const EvtVector4R& pPiPlus,
			     const EvtVector4R& pPiMinus) const;

    EvtVector4C WCurrent_KSK(const EvtVector4R& pKS, const EvtVector4R& pKplus) const;

protected:

    EvtVector4C JB(const EvtVector4R& q1, const EvtVector4R& q2, const EvtVector4R& q3, 
		   const EvtVector4R& q4, const EvtVector4R& q5) const;

    EvtComplex BWa(const EvtVector4R& q) const;

    EvtComplex BWf(const EvtVector4R& q) const;

    EvtComplex BWr(const EvtVector4R& q) const;

    EvtComplex BWKK(double s, int i) const;

    double pi3G(double Q2) const;

    EvtComplex pcm(double s) const;

private:

    std::vector<double> mRho_, gamma0_, cK_;

};

#endif
