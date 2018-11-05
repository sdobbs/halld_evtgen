

//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtBToDiBaryonlnupQCD.cc
//
// Description: Routine to implement B -> Baryon Anti-baryon l nu decays. 
// The form factors are from  arXiv.1107.0801 (2011)
//              
//
// Modification history:
//
//   Mark Smith     17/07/2017        Module created
//   Ryan Newcombe  May 2018          Additional baryons and
//   Rarita-Schwinger-type particles  
//   John B         Oct 2018          Optimisations
//
//------------------------------------------------------------------------

#include "EvtGenBase/EvtPatches.hh"

#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtSpinType.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtIdSet.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "EvtGenBase/EvtVector4R.hh"

#include "EvtGenModels/EvtBToDiBaryonlnupQCD.hh"
#include "EvtGenModels/EvtBToDiBaryonlnupQCDFF.hh"
#include "EvtGenModels/EvtSLDiBaryonAmp.hh"

using std::endl;

EvtBToDiBaryonlnupQCD::EvtBToDiBaryonlnupQCD() :
    ffModel(0),
    calcAmp(0)
{
}

EvtBToDiBaryonlnupQCD::~EvtBToDiBaryonlnupQCD() {
  delete ffModel;
  ffModel = 0;
  delete calcAmp;
  calcAmp = 0;
}

std::string EvtBToDiBaryonlnupQCD::getName() {

  return "BToDiBaryonlnupQCD";     

}

EvtDecayBase* EvtBToDiBaryonlnupQCD::clone() {

  return new EvtBToDiBaryonlnupQCD;

}

void EvtBToDiBaryonlnupQCD::decay(EvtParticle *p) {
    
    p->initializePhaseSpace(getNDaug(), getDaugs(), true);

    calcAmp->CalcAmp(p, _amp2);

}

void EvtBToDiBaryonlnupQCD::init() {

    if ( !(getNArg() == 6 || getNArg() == 7) ) {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "EvtBToDiBaryonlnupQCD model expected "
					    << " 6 or 7 arguments but found:" << getNArg() << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();

    }

    if (getNDaug() != 4) {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") 
	    << "Wrong number of daughters in EvtBToDiBaryonlnupQCD model: "
	    << "4 daughters expected but found: " << getNDaug() << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();
    }


    // We expect B -> baryon baryon lepton neutrino
    EvtSpinType::spintype parentType = EvtPDL::getSpinType(getParentId());
    EvtSpinType::spintype leptonType = EvtPDL::getSpinType(getDaug(2));
    EvtSpinType::spintype neutrinoType = EvtPDL::getSpinType(getDaug(3));

    if (parentType != EvtSpinType::SCALAR) {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "EvtBToDiBaryonlnupQCD model expected "
					    << " a SCALAR parent, found:"
					    << EvtPDL::name(getParentId()) << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();
    }

    if (leptonType != EvtSpinType::DIRAC) {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "EvtBToDiBaryonlnupQCD model expected "
					    << " a DIRAC 3rd daughter, found:"
					    << EvtPDL::name(getDaug(2)) << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();
    }

    if (neutrinoType != EvtSpinType::NEUTRINO) {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "EvtBToDiBaryonlnupQCD model expected "
					    << " a NEUTRINO 4th daughter, found:"
					    << EvtPDL::name(getDaug(3)) << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();
    }

    // Get the 6 form factor D parameters from model arguments in the decay file
    std::vector<double> DPars(6);
    for (int i = 0; i < 6; i++) {
	DPars[i] = getArg(i);
    }

    // Form factor model
    ffModel = new EvtBToDiBaryonlnupQCDFF(DPars);

    // Set amplitude calculation pointer.    
    // Accomodate for spin 1/2 (DIRAC) or 3/2 (RARITASCHWINGER) baryons
    EvtSpinType::spintype baryon1Type = EvtPDL::getSpinType(getDaug(0));
    EvtSpinType::spintype baryon2Type = EvtPDL::getSpinType(getDaug(1));

    if ( (baryon1Type == EvtSpinType::DIRAC && baryon2Type == EvtSpinType::RARITASCHWINGER) ||
	 (baryon1Type == EvtSpinType::RARITASCHWINGER && baryon2Type == EvtSpinType::DIRAC) ||
	 (baryon1Type == EvtSpinType::DIRAC && baryon2Type == EvtSpinType::DIRAC) ) {

	calcAmp = new EvtSLDiBaryonAmp(ffModel);

    } else {

	EvtGenReport(EVTGEN_ERROR,"EvtGen") 
	    << "Wrong baryon spin type in EvtBToDiBaryonlnupQCD model. "
	    << "Expected spin type " << EvtSpinType::DIRAC 
	    << " or " << EvtSpinType::RARITASCHWINGER 
	    << ", found spin types " << baryon1Type << " and " << baryon2Type << endl;
	EvtGenReport(EVTGEN_ERROR,"EvtGen") << "Will terminate execution!" << endl;
	::abort();
    }

}

void EvtBToDiBaryonlnupQCD::initProbMax() {

    // Set maximum prob using dec file parameter if present
    if (getNArg() == 7) {

        setProbMax(getArg(6));

    } else {

	// Default probability for the B -> p p l nu mode, where l = e, mu or tau
        setProbMax(3.0e6);

	// Specific decay modes, where we have one proton plus a second
	// baryon that can be any (excited) state. They all have lower
	// maximum probabilities compared to the default pp mode in order
	// to improve accept/reject generation efficiency
	static EvtIdSet BMesons("B-", "B+");
	static EvtIdSet Delta("Delta+", "anti-Delta-"); 
	static EvtIdSet LambdaC("Lambda_c+", "anti-Lambda_c-");  
	static EvtIdSet LambdaC1("Lambda_c(2593)+", "anti-Lambda_c(2593)-");
	static EvtIdSet LambdaC2("Lambda_c(2625)+", "anti-Lambda_c(2625)-");
	static EvtIdSet N1440("N(1440)+", "anti-N(1440)-");
	static EvtIdSet N1520("N(1520)+", "anti-N(1520)-");
	static EvtIdSet N1535("N(1535)+", "anti-N(1535)-");
	static EvtIdSet N1650("N(1650)+", "anti-N(1650)-");
	static EvtIdSet N1700("N(1700)+", "anti-N(1700)-");
	static EvtIdSet N1710("N(1710)+", "anti-N(1710)-");
	static EvtIdSet N1720("N(1720)+", "anti-N(1720)-");

	EvtId parId = getParentId();
	EvtId bar1Id = getDaug(0);
	EvtId bar2Id = getDaug(1);

	// These probabilties are sensitive to the sub-decay modes of the excited baryon states,
	// which limit the available phase space and allows for events to be generated within the
	// 10,000 event trial limit. Otherwise the amplitude varies too much (by more than a factor
	// of a million) and events fail to be generated correctly. In case of problems, specify
	// the maximum probability by passing an extra 7th model parameter
	if (BMesons.contains(parId)) {

	    if (Delta.contains(bar1Id) || Delta.contains(bar2Id)) {
		// Delta
		setProbMax(2.2e6);

	    } else if (LambdaC.contains(bar1Id) || LambdaC.contains(bar2Id)) {
		// Lambda_c+
		setProbMax(1000.0);

	    } else if (LambdaC1.contains(bar1Id) || LambdaC1.contains(bar2Id)) {
		// Lambda_c+(2593)
		setProbMax(200.0);

	    } else if (LambdaC2.contains(bar1Id) || LambdaC2.contains(bar2Id)) {
		// Lambda_c+(2625)
		setProbMax(500.0);

	    } else if (N1440.contains(bar1Id) || N1440.contains(bar2Id)) {
		// N(1440)
		setProbMax(1.25e6);

	    } else if (N1520.contains(bar1Id) || N1520.contains(bar2Id)) {
		// N(1520)
		setProbMax(1.25e6);

	    } else if (N1535.contains(bar1Id) || N1535.contains(bar2Id)) {
		// N(1535)
		setProbMax(1.25e6);

	    } else if (N1650.contains(bar1Id) || N1650.contains(bar2Id)) {
		// N(1650)
		setProbMax(1.25e6);

	    } else if (N1700.contains(bar1Id) || N1700.contains(bar2Id)) {
		// N(1700)
		setProbMax(1.25e6);

	    } else if (N1710.contains(bar1Id) || N1710.contains(bar2Id)) {
		// N(1710)
		setProbMax(1.25e6);

	    } else if (N1720.contains(bar1Id) || N1720.contains(bar2Id)) {
		// N(1720)
		setProbMax(1.25e6);

	    } // Baryon combinations

	} // B parent

    } // Specific modes

}
