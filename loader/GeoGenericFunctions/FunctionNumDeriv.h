//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionNumDeriv--------------------------------//
//                                                                          //
// Class FunctionNumDeriv, providing numerical derivatives                  //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionNumDeriv_h
#define FunctionNumDeriv_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FunctionNumDeriv : public AbsFunction {
  
    FUNCTION_OBJECT_DEF(FunctionNumDeriv)

      public:

    // Constructor
    FunctionNumDeriv(const AbsFunction *arg1, unsigned int index=0);
  
    // Copy constructor.
    FunctionNumDeriv(const FunctionNumDeriv &right);
  
    // Destructor
    virtual ~FunctionNumDeriv();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;
  
    // Dimensionality 
    virtual unsigned int dimensionality() const override;

  private:
  
    // It is illegal to assign a FunctionNumDeriv
    const FunctionNumDeriv & operator=(const FunctionNumDeriv &right);

    // The function we're differntiating.
    const AbsFunction        *_arg1;
    const unsigned int       _wrtIndex;

    // helper data and methods to let us code Argument and double similarly
    double numericalDerivative 
	( double (FunctionNumDeriv::*f)(double)const, double x ) const;
    double f_x   (double x) const;
    double f_Arg (double x) const;
    mutable Argument 	    _xArg;

  };
} // namespace GeoGenfun
#endif
