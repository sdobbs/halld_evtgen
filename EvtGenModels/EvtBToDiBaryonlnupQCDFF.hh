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
// Module: EvtGen/EvtBToDiBaryonlnupQCDFF.hh.hh
//
// Description: Module for computation of B->ppbar form factors according 
// to pQCD counting rules, see arXiv:1107.0801.  
//
// Modification history:
//
//    Mark Smith     July 17, 2017     Module created
//    John B         Oct 2018          Added FormFactors class
//
//------------------------------------------------------------------------

#ifndef EVTBTODIBARYONLNUPQCDFF_HH
#define EVTBTODIBARYONLNUPQCDFF_HH

class EvtParticle;
#include <vector>

class EvtBToDiBaryonlnupQCDFF {

public:

    class FormFactors {
    public:
	FormFactors() {;}

	virtual ~FormFactors() {;}

	double F1, F2, F3, F4, F5;
	double G1, G2, G3, G4, G5;

    };

    EvtBToDiBaryonlnupQCDFF();

    EvtBToDiBaryonlnupQCDFF(std::vector<double>& DParameters);

    void getDiracFF(EvtParticle* parent, double dibaryonMass,
		    EvtBToDiBaryonlnupQCDFF::FormFactors& FF) const;
    
    void getRaritaFF(EvtParticle* parent, double dibaryonMass,
		     EvtBToDiBaryonlnupQCDFF::FormFactors& FF) const;

    void getFF(EvtParticle* parent, double dibaryonMass,
	       EvtBToDiBaryonlnupQCDFF::FormFactors& FF) const;

private:

    std::vector<double> DPars;
    int nDPars;

};

#endif


