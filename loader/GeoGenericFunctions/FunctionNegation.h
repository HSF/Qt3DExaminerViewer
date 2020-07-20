//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionNegation--------------------------------//
//                                                                          //
// Class FunctionNegation, output of the unary - operator                   //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionNegation_h
#define FunctionNegation_h 1
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class FunctionNegationRecorder;

namespace GeoGenfun {

  class FunctionNegation : public AbsFunction {
  
    FUNCTION_OBJECT_DEF(FunctionNegation)

      public:

    // Constructor
    FunctionNegation(const AbsFunction *arg1);
  
    // Copy constructor.
    FunctionNegation(const FunctionNegation &right);
  
    // Destructor
    virtual ~FunctionNegation();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;
  
    // Dimensionality 
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:
  
    // It is illegal to assign a FunctionNegation
    const FunctionNegation & operator=(const FunctionNegation &right);

    // The function we're negating.  
    const AbsFunction *_arg1;

    // For persistification:
    friend class ::FunctionNegationRecorder;

  };
} // namespace GeoGenfun
#endif
