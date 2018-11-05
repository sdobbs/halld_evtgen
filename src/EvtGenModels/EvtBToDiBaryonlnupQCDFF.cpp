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
// Module: EvtBTopplnupQCDFF.cc
//
// Description: Routine to implement form factor calculation for 
// B->Baryon Anti-baryon l nu from pQCD counting rules.
// Taken from arXiv:1107.0801
//
//
// Modification history:
//
//  Mark Smith     17/07/2017        Module created
//  Ryan Newcombe  May 2018          Added function to get form factors for
//  Rarita-Schwinger daughters
//  John B         Oct 2018          Simplified code
//
//--------------------------------------------------------------------------

#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenModels/EvtBToDiBaryonlnupQCDFF.hh"

EvtBToDiBaryonlnupQCDFF::EvtBToDiBaryonlnupQCDFF() :
    DPars(),
    nDPars(0)
{
}

EvtBToDiBaryonlnupQCDFF::EvtBToDiBaryonlnupQCDFF(std::vector<double>& DParameters):
    DPars(DParameters),
    nDPars(DParameters.size())
{
}

void EvtBToDiBaryonlnupQCDFF::getFF(EvtParticle*, double dibaryonMass,
				    EvtBToDiBaryonlnupQCDFF::FormFactors& FF)
{

    if (nDPars == 6 && dibaryonMass > 0.0) {

	// 5/3*[1/M^2]^3
	double t = 5.0/(3.0*pow(dibaryonMass, 6.0));

	double Dp = DPars[0];
	double Dpb = DPars[1];
	double D2 = DPars[2];
	double D3 = DPars[3];
	double D4 = DPars[4];
	double D5 = DPars[5];

	FF.F1 = (Dp + 0.2*Dpb)*t;
	FF.F2 = -D2*t;
	FF.F3 = -D3*t;
	FF.F4 = -D4*t;
	FF.F5 = -D5*t;

	FF.G1 = (Dp - 0.2*Dpb)*t;
	FF.G2 = -FF.F2;
	FF.G3 = -FF.F3;
	FF.G4 = -FF.F4;
	FF.G5 = -FF.F5;

    }
    
}

void EvtBToDiBaryonlnupQCDFF::getDiracFF(EvtParticle* parent, double dibaryonMass,
					 EvtBToDiBaryonlnupQCDFF::FormFactors &FF)
{

    this->getFF(parent, dibaryonMass, FF);

}

void EvtBToDiBaryonlnupQCDFF::getRaritaFF(EvtParticle* parent, double dibaryonMass,
					 EvtBToDiBaryonlnupQCDFF::FormFactors &FF)
{

    this->getFF(parent, dibaryonMass, FF);

}
