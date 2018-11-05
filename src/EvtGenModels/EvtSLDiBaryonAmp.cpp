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
// Module: EvtSLDiBaryonAmp.cc
//
// Description: Routine to implement semileptonic decays to dibaryonic final 
// state. Details of amplitude calculation to be found in arXiv:1107.0801.  
//
// Modification history:
//
//    Mark Smith     July 18, 2017     Module created
//    Heavily adapted from the EvtSLBaryonAmp module
//    Ryan Newcombe   May 2018  Added capability for Rarita-Schwinger
//    daughters.  Indexing convention follows EvtSLBaryonAmp
//    John B          Oct 2018 Optimise amplitude calculation
//
//--------------------------------------------------------------------------

#include "EvtGenBase/EvtPatches.hh"

#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtGammaMatrix.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtRaritaSchwinger.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"

#include "EvtGenModels/EvtBToDiBaryonlnupQCDFF.hh"
#include "EvtGenModels/EvtSLDiBaryonAmp.hh"

using std::endl;

EvtSLDiBaryonAmp::EvtSLDiBaryonAmp(EvtBToDiBaryonlnupQCDFF* formFactors) :
    ffModel(formFactors)
{
}

void EvtSLDiBaryonAmp::CalcAmp(EvtParticle *parent, EvtAmp& amp) const {

    static EvtId EM = EvtPDL::getId("e-");
    static EvtId MUM = EvtPDL::getId("mu-");
    static EvtId TAUM = EvtPDL::getId("tau-");
    static EvtId EP = EvtPDL::getId("e+");
    static EvtId MUP = EvtPDL::getId("mu+");
    static EvtId TAUP = EvtPDL::getId("tau+");

    // The baryon, charged lepton and neutrino daughters
    EvtParticle* baryon1 = parent->getDaug(0);
    EvtParticle* baryon2 = parent->getDaug(1);
    EvtParticle* lepton = parent->getDaug(2);
    EvtParticle* neutrino = parent->getDaug(3);

    // 4-momenta
    EvtVector4R p1 = baryon1->getP4();
    EvtVector4R p2 = baryon2->getP4();

    EvtVector4R pSum = p1 + p2;
    EvtVector4R p = parent->getP4() - pSum;
    EvtVector4R pDiff = p2 - p1;

    // Particle id's
    EvtId Id1 = baryon1->getId();
    EvtId Id2 = baryon2->getId();
    EvtId l_num = lepton->getId();

    EvtSpinType::spintype type1 = EvtPDL::getSpinType(Id1);
    EvtSpinType::spintype type2 = EvtPDL::getSpinType(Id2);
    
    // Number of chiral components of the baryon spinors
    int N1 = EvtSpinType::getSpinStates(type1);
    int N2 = EvtSpinType::getSpinStates(type2);

    // Invariant mass of the two baryon particle system
    double m_dibaryon = sqrt(pSum.mass2());

    // Complex number i
    EvtComplex I(0, 1);

    // Lepton currents, same for all baryon options
    EvtVector4C l1, l2;

    if (l_num == EM || l_num == MUM || l_num == TAUM) {

	l1 = EvtLeptonVACurrent(lepton->spParent(0), neutrino->spParentNeutrino());
      
	l2 = EvtLeptonVACurrent(lepton->spParent(1), neutrino->spParentNeutrino());

    } else if (l_num == EP || l_num == MUP || l_num == TAUP) {
        
	l1 = EvtLeptonVACurrent(neutrino->spParentNeutrino(), lepton->spParent(0));

        l2 = EvtLeptonVACurrent(neutrino->spParentNeutrino(), lepton->spParent(1));

    } else {

        EvtGenReport(EVTGEN_ERROR,"EvtSLDiBaryonAmp") << "Wrong lepton number"<<endl;

    }
    
    // Sigma summation terms. Store in a vector to avoid recalculating this for each i,j loop
    std::vector<EvtGammaMatrix> sigmaVect;
    for (int k = 0; k < 4; k++) {
	
	EvtGammaMatrix sigmaSum;
	for (int index = 0; index < 4; index++) {
	    sigmaSum += EvtGammaMatrix::sigmaLower(k, index)*p.get(index);
	}
	
	sigmaVect.push_back(sigmaSum);
	
    }

    // Vector and axial-vector terms; these get reset within the loop over k
    EvtVector4C term1;
    EvtVector4C term2;

    // Handle case of two Dirac-type daughters, e.g. ppbar, pN(1440)
    if (type1 == EvtSpinType::DIRAC && type2 == EvtSpinType::DIRAC) {

	// Form factor parameters
	EvtBToDiBaryonlnupQCDFF::FormFactors FF;
	ffModel->getDiracFF(parent, m_dibaryon, FF);

	// First baryon
	for (int i = 0; i < N1; i++) {

	    // Get the baryon spinor in the B rest frame. Also just use u and not i*u,
	    // since the imaginary constant factor is not needed for the probability
	    EvtDiracSpinor u = baryon1->spParent(i);

	    // Second baryon
	    for(int j = 0; j < N2; j++) {
		
		EvtDiracSpinor v = baryon2->spParent(j); 
		EvtDiracSpinor g5v = EvtGammaMatrix::g5()*v;

		// Calculate and set the 4 complex amplitude components
		for (int k = 0; k < 4; k++) {
		
		    EvtGammaMatrix sigmaSum = sigmaVect[k];
 
		    // Need two terms owing to the requirements of using the product operators
		    EvtDiracSpinor vgTermA = (FF.G1*EvtGammaMatrix::g(k) + I*FF.G2*sigmaSum)*g5v;
		    EvtDiracSpinor vgTermB = (FF.G3*p.get(k) + FF.G4*pSum.get(k) + FF.G5*pDiff.get(k))*g5v;

		    // S current = ubar adjoint*vTerms
		    term1.set(k, EvtLeptonSCurrent(u, vgTermA+vgTermB));

		    EvtDiracSpinor vfTermA = (FF.F1*EvtGammaMatrix::g(k) + I*FF.F2*sigmaSum)*v;
		    EvtDiracSpinor vfTermB = (FF.F3*p.get(k) + FF.F4*pSum.get(k) + FF.F5*pDiff.get(k))*v;

		    // S current = ubar adjoint*vTerms
		    term2.set(k, EvtLeptonSCurrent(u, vfTermA+vfTermB));

		}
	    
		// Set the decay amplitude element
		EvtVector4C term = term1 - term2;
		amp.vertex(i,j,0,l1*term);
		amp.vertex(i,j,1,l2*term);

	    } // j
	    
	} // i

    } else if ((type1 == EvtSpinType::DIRAC && type2 == EvtSpinType::RARITASCHWINGER) || 
	       (type1 == EvtSpinType::RARITASCHWINGER && type2 == EvtSpinType::DIRAC)) {
    
	// Handle the case of one Dirac-type daughter (not including the leptons), e.g. one proton, and one 
	// Rarita-Schwinger-type (spin 3/2) daughter e.g. B -> p N(1520) l nu

	// Form factor parameters
	EvtBToDiBaryonlnupQCDFF::FormFactors FF;
	ffModel->getRaritaFF(parent, m_dibaryon, FF);

	if (type1 == EvtSpinType::DIRAC) {

	    // First baryon is Dirac
	    for (int i = 0; i < N1; i++) {

		// Get the baryon spinor in the B rest frame. Also just use u and not i*u,
		// since the imaginary constant factor is not needed for the probability
		EvtDiracSpinor u = baryon1->spParent(i);
		
		// Second baryon is RS-type
		for (int j = 0; j < N2; j++) {
		    
		    EvtRaritaSchwinger vRS = baryon2->spRSParent(j); 
		    
		    // Store products of g5 with the spinor components as well as
		    // EvtDiracSpinors to limit constructor calls inside k loop
		    std::vector<EvtDiracSpinor> g5vVect, vVect;
		    for (int index = 0; index < 4; index++) {
			
			EvtDiracSpinor v = vRS.getSpinor(index);
			EvtDiracSpinor g5v = EvtGammaMatrix::g5()*v;
			
			g5vVect.push_back(g5v);
			vVect.push_back(v);
			
		    }
		    
		    // Calculate and set the 4 complex amplitude components
		    for (int k = 0; k < 4; k++) {
			
			EvtGammaMatrix sigmaSum = sigmaVect[k];
			EvtDiracSpinor g5v = g5vVect[k];
			EvtDiracSpinor v = vVect[k];

			// Need two terms owing to the requirements of using the product operators
			EvtDiracSpinor vgTermA = (FF.G1*EvtGammaMatrix::g(k) + I*FF.G2*sigmaSum)*g5v;
			EvtDiracSpinor vgTermB = (FF.G3*p.get(k) + FF.G4*pSum.get(k) + FF.G5*pDiff.get(k))*g5v;

			// S current = ubar adjoint*vTerms
			term1.set(k, EvtLeptonSCurrent(u, vgTermA+vgTermB));
		
			EvtDiracSpinor vfTermA = (FF.F1*EvtGammaMatrix::g(k) + I*FF.F2*sigmaSum)*v;
			EvtDiracSpinor vfTermB = (FF.F3*p.get(k) + FF.F4*pSum.get(k) + FF.F5*pDiff.get(k))*v;

			// S current = ubar adjoint*vTerms
			term2.set(k, EvtLeptonSCurrent(u, vfTermA+vfTermB));
			
		    }
		    
		    // Set the decay amplitude element
		    EvtVector4C term = term1 - term2;
		    amp.vertex(i,j,0,l1*term);
		    amp.vertex(i,j,1,l2*term);
		    
		} // j
		
	    } // i
	    
	} else if (type2 == EvtSpinType::DIRAC) {
	    
	    // Same as before, but where the first daughter is RS-type, e.g. B -> N(1520) p l nu
	
	    // First baryon is RS
	    for (int i = 0; i < N1; i++) {
		
		// Get the baryon spinor in the B rest frame		
		EvtRaritaSchwinger uRS = baryon1->spRSParent(i);

		// Store EvtDiracSpinors to reduce constructor calls inside k loop
		std::vector<EvtDiracSpinor> uVect;
		for (int index = 0; index < 4; index++) {
		    
		    // Just use u and not i*u, since the imaginary constant factor is not needed 
		    // for the probability
		    EvtDiracSpinor u = uRS.getSpinor(index);
		    uVect.push_back(u);
		    
		}

		// Second baryon is Dirac	
		for (int j = 0; j < N2; j++) {
		    
		    EvtDiracSpinor v = baryon2->spParent(j);
		    EvtDiracSpinor g5v = EvtGammaMatrix::g5()*v;
		    		    		    
		    // Calculate and set the 4 complex amplitude components
		    for (int k = 0; k < 4; k++) {
			
			EvtGammaMatrix sigmaSum = sigmaVect[k];
			EvtDiracSpinor u = uVect[k];
			
			// Need two terms owing to the requirements of using the product operators
			EvtDiracSpinor vgTermA = (FF.G1*EvtGammaMatrix::g(k) + I*FF.G2*sigmaSum)*g5v;
			EvtDiracSpinor vgTermB = (FF.G3*p.get(k) + FF.G4*pSum.get(k) + FF.G5*pDiff.get(k))*g5v;

			// S current = ubar adjoint*vTerms
			term1.set(k, EvtLeptonSCurrent(u, vgTermA+vgTermB));
		
			EvtDiracSpinor vfTermA = (FF.F1*EvtGammaMatrix::g(k) + I*FF.F2*sigmaSum)*v;
			EvtDiracSpinor vfTermB = (FF.F3*p.get(k) + FF.F4*pSum.get(k) + FF.F5*pDiff.get(k))*v;

			// S current = ubar adjoint*vTerms
			term2.set(k, EvtLeptonSCurrent(u, vfTermA+vfTermB));

		    }
		    
		    // Set the decay amplitude element
		    EvtVector4C term = term1 - term2;
		    amp.vertex(i,j,0,l1*term);
		    amp.vertex(i,j,1,l2*term);
		    
		} // j
		
	    } // i
	    
	} // RS daughter check

    } // Have Dirac and RS baryons

}
