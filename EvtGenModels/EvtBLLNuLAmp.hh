//--------------------------------------------------------------------------
//
//
// Module: EvtB2MuMuENuAmp.hh
//
// Description: Header file for the amplitude calculation for the "BLLNUL" 
//              model which generates rare four-leptonic B-decays
//              B^-(p) -> ell^+(k_1) ell^-(k_2) neu (k_3) ell^-(k_4)
//
// Modification history:
// 
// Anna Danilina (anna.danilina@cern.ch) and 
// Nikolai Nikitin (Nikolai.Nikitine@cern.ch) Nov 2018  Module created
// John B                                               Code optimisations 
//
//------------------------------------------------------------------------

#ifndef EVTBLLNUL_AMP_HH
#define EVTBLLNUL_AMP_HH

#include "EvtGenBase/EvtAmp.hh"
#include "EvtGenBase/EvtComplex.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtVector4R.hh"

#include <vector>

class EvtParticle;

class EvtBLLNuLAmp {

public:

    EvtBLLNuLAmp(double Vub = 4.09e-3);
    EvtBLLNuLAmp(double qSqMin, double kSqMin, bool symmetry, double Vub = 4.09e-3);

    virtual ~EvtBLLNuLAmp();

    void CalcAmp(EvtParticle *parent, EvtAmp& amp) const;
    void setParameters(double qSqMin, double kSqMin, bool symmetry);

    // Resonance poles
    class ResPole {
    public:
	ResPole(double mass, double width, double coupling);
	virtual ~ResPole() {;}

	EvtComplex propagator(double qSq, int numForm = 0) const;

	double getMass() const {return m0_;}
	double getMassSq() const {return m0Sq_;}
	double getWidth() const {return w0_;}
	double getCoupling() const {return c_;}

    private:
	double m0_; // pole mass
	double m0Sq_;
	double w0_; // width
	double c_; // coupling constant
	EvtComplex I_;
	EvtComplex Imw_;

    };

protected:
    
    EvtTensor4C getHadronTensor(const EvtVector4R& q, const EvtVector4R& k,
                                const double qSq, const double kSq, const double MB,
                                const int sign) const;

    std::vector<EvtComplex> getVMDTerms(double qSq, double kSq, double MB) const;

    EvtComplex getBStarTerm(double qSq, double kSq, double MB) const;

    double FF_B2Bstar(double qSq) const;
    
    double FF_V(double kSq) const;
    
    double FF_A1(double kSq) const;
    
    double FF_A2(double kSq) const;

private:

    // Kinematic cut-offs
    double qSqMin_;
    double kSqMin_;

    // If we have identical charged lepton flavours
    bool symmetry_;

    // B+, B- Ids
    EvtId BpId_, BnId_;

    // Form factor constants
    double coupling_, sqrt2_;
    double fBu_;

    // Resonance poles
    EvtBLLNuLAmp::ResPole* Bstar_;
    EvtBLLNuLAmp::ResPole* Upsilon_;

    std::vector<EvtBLLNuLAmp::ResPole*> resPoles_;
    int nPoles_;

    // Complex number constants
    EvtComplex zero_, unitI_;

};

inline void EvtBLLNuLAmp::setParameters(double qSqMin, double kSqMin, bool symmetry)
{
    qSqMin_ = qSqMin;
    kSqMin_ = kSqMin;
    symmetry_ = symmetry;
}

#endif
