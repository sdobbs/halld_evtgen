//--------------------------------------------------------------------------
//
// Copyright Information: See EvtGen/COPYRIGHT
//
// Module: EvtBLLNuL.cpp
//
// Description: The main file for the model "BLLNUL" which simulates
//              the rare four-leptonic B-decays
//              B^-(p) -> ell^+(k_1) ell^-(k_2) neu (k_3) ell^-(k_4)
//
// Modification history:
// 
// Anna Danilina (anna.danilina@cern.ch) and 
// Nikolai Nikitin (Nikolai.Nikitine@cern.ch) Nov 2018  Module created
// John B                                               Code optimisations
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtSpinType.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtIdSet.hh"

#include "EvtGenModels/EvtBLLNuL.hh"
#include "EvtGenModels/EvtBLLNuLAmp.hh"

EvtBLLNuL::EvtBLLNuL() :
    calcAmp_(0)
{
}

EvtBLLNuL::~EvtBLLNuL()
{
    delete calcAmp_;
}

std::string EvtBLLNuL::getName() 
{    
    // The model name
    return "BLLNUL";
}

EvtDecayBase* EvtBLLNuL::clone() 
{
    return new EvtBLLNuL();
}

void EvtBLLNuL::init() 
{
    // check that there are 4 daughters
    checkNDaug(4);

    // We expect that the parent to be a scalar (B meson) and
    // the daughters to be ell+ (k1), ell- (k2), neutrino (k3)
    // and the last lepton ell- (k4)
    
    // Check spin types
    checkSpinParent(EvtSpinType::SCALAR);
    
    checkSpinDaughter(0, EvtSpinType::DIRAC);    // ell+(k_1)
    checkSpinDaughter(1, EvtSpinType::DIRAC);    // ell-(k_2)
    checkSpinDaughter(2, EvtSpinType::NEUTRINO); // neu (k_3)
    checkSpinDaughter(3, EvtSpinType::DIRAC);    // ell-(k_4)
    
    // Check that we have a charged B parent
    static EvtIdSet BMesons("B-", "B+");
    if (!BMesons.contains(getParentId())) {
	
	EvtGenReport(EVTGEN_ERROR, "EvtBLLNuL") << "Expecting the parent to be a charged B. Found PDG = "
						<< EvtPDL::getStdHep(getParentId()) << std::endl;
	::abort();
	
    }
    
    // Make sure the first two leptons are charged conjugates of each other
    int id1 = EvtPDL::getStdHep(getDaug(0));
    int id2 = EvtPDL::getStdHep(getDaug(1));
    if (id1 != -id2) {
	
	EvtGenReport(EVTGEN_ERROR, "EvtBLLNuL") << "Expecting the first 2 leptons, with PDG codes " << id1 << " and "
						<< id2 << ", to be charged conjugates of each other" << std::endl;
	::abort();
	
    }
    
    // Check that the last lepton has the same charge as the B parent
    int q3 = EvtPDL::chg3(getDaug(3))/3;
    int qB = EvtPDL::chg3(getParentId())/3;
    if (q3 != qB) {
	
	EvtGenReport(EVTGEN_ERROR, "EvtBLLNuL") << "The 3rd lepton charge " << q3 
						<< " does not match the B charge " 
						<< qB << std::endl;
	::abort();
    }

    // Also check that the 2nd lepton has the same charge as the 3rd one
    int q2 = EvtPDL::chg3(getDaug(1))/3;
    if (q2 != q3) {

	EvtGenReport(EVTGEN_ERROR, "EvtBLLNuL") << "The 2nd lepton charge " << q2
						<< " does not match the 3rd lepton charge " 
						<< q3 << std::endl;
	::abort();
    }
    
    // Identify if the decay has 3 charged leptons with the same flavour. 
    // If so, then we need to include amplitude terms where the 2nd and 3rd 
    // same-sign leptons are interchanged: k2 <-> k4
    bool symmetry(false);
    int id3 = EvtPDL::getStdHep(getDaug(3));

    if (abs(id1) == abs(id2) && abs(id1) == abs(id3)) {
	symmetry = true;
    }

    // Specify the qSq minimum cut-off as 4*(muon mass)^2 = 0.044655 and the 
    // kSq minimum cut off as 4*(electron mass)^2 = 1.044e-6
    double muMass = EvtPDL::getMeanMass(EvtPDL::getId("mu+"));
    double eMass = EvtPDL::getMeanMass(EvtPDL::getId("e+"));
    double qSqMin = 4.0*muMass*muMass;
    double kSqMin = 4.0*eMass*eMass;

    // Optionally set these cut-offs using two decay file parameters. We may
    // have a 3rd parameter (max prob), so check for at least 2 parameters
    if (getNArg() >= 2) {
	qSqMin = getArg(0);
	kSqMin = getArg(1);
    }

    // Define the amplitude calculation pointer with the qSq and kSq cut-offs,
    // also specifying if the decay mode has flavour symmetry
    calcAmp_ = new EvtBLLNuLAmp(qSqMin, kSqMin, symmetry);
}

void EvtBLLNuL::initProbMax() 
{
    // Set the maximum probability of the decay 
    double maxProb(3.2);

    // Optional 3rd decay file parameter, e.g. if qSq and/or kSq min have changed.
    // Note that both qSq and kSq parameters must still be specified in the decay
    // file to ensure that the maximum probability value is the 3rd parameter!
    if (getNArg() == 3) {
	maxProb = getArg(2);
    }

    setProbMax(maxProb);
}

void EvtBLLNuL::decay(EvtParticle *p) 
{    
    p->initializePhaseSpace(getNDaug(), getDaugs());
    
    calcAmp_->CalcAmp(p, _amp2);

}
