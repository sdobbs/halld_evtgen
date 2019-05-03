////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Environment:                                                               //
//      This software is part of the EvtGen package.                          //
//                                                                            //
// Copyright Information: See EvtGen/COPYRIGHT                                //
//                                                                            //
// Module: EvtBsMUMUKK.hh                                                     //
//                                                                            //  
// Description: Routine to implement Bs -> J/psi KK                           //
//                                                                            //
// Modification history:                                                      //
//      Veronika Chobanova, Jeremy Dalseno, Diego Martinez Santos             //
//      April 21, 2016       Module created   LHCb collaboration              //
//      Marcos Romero Lamas                                                   //
//      February 8, 2019     Module updated   LHCb collaboration              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef EvtBsMuMuKK_HH
#define EvtBsMuMuKK_HH

#include <string>
#include "EvtGenBase/EvtDecayAmp.hh"
#include "EvtGenBase/EvtComplex.hh"
 
class EvtParticle;

class EvtBsMuMuKK : public EvtDecayAmp
{

public:
  
  EvtBsMuMuKK() {;}
  
  virtual std::string getName() { return "BS_MUMUKK"; }

  virtual EvtDecayBase* clone();

  virtual void init();

  virtual void initProbMax();

  virtual void decay(EvtParticle *p);

protected:

  EvtComplex Flatte(const double m0, const double m) const;
 
  EvtComplex GetRho(const double m0, const double m) const;
  
  EvtComplex Breit_Wigner(const double Gamma0, const double m0, const double m,
			  const int J, const double q0, const double q) const;
  
  double Integral(const double Gamma0, const double m0, const int JR, const int JB,
		  const double q0, const double M_KK_ll, const double M_KK_ul, const int fcntype) const;
  
  double X_J(const int J, const double q, const int isB) const;
  
  double Wignerd(int J, int l, int alpha, double theta) const;
  
  EvtComplex AngularDist(int J, int l, int alpha, double cK, double cL, double chi) const;
  
  EvtComplex AmpTime(const int q, const EvtComplex& gplus, const EvtComplex& gminus,
		     const double delta, const double lambda_abs, const double Amp,
		     const double phis, const int eta) const;

private:

  double MBs, MJpsi, Mf0, Mphi, Mf2p, MKp, MKm, MK0, Mpip, Mpi0, Mmu;
  double Gamma0phi, Gamma0f2p;
  double kin_lower_limit, kin_upper_limit, kin_middle;
  double p30Kp_mid_CMS, p30Kp_ll_CMS, p30Kp_phi_CMS, p30Kp_f2p_CMS; 
  double p30Jpsi_mid_CMS, p30Jpsi_ll_CMS, p30Jpsi_phi_CMS, p30Jpsi_f2p_CMS;
  double int_const_NR, int_Flatte_f0, int_BW_phi, int_BW_f2p;
  double f_S_NR, f_f0, f_phi, f_f2p, f_phi_0, f_phi_perp, f_f2p_0, f_f2p_perp;
  double A_S_NR, A_f0, A_phi_0, A_phi_perp, A_phi_par, A_f2p_0, A_f2p_perp;
  double A_f2p_par;
  double delta_S_NR, delta_f0, delta_phi_0, delta_phi_perp, delta_phi_par;
  double delta_f2p_0, delta_f2p_perp, delta_f2p_par;
  double phis_S_NR, phis_f0, phis_phi_0, phis_phi_perp, phis_phi_par;
  double phis_f2p_0, phis_f2p_perp, phis_f2p_par;
  double lambda_S_NR_abs, lambda_f0_abs, lambda_phi_0_abs, lambda_phi_perp_abs;
  double lambda_phi_par_abs, lambda_f2p_0_abs, lambda_f2p_perp_abs;
  double lambda_f2p_par_abs;
  double Gamma, deltaGamma, ctau, deltaMs;

};

#endif
