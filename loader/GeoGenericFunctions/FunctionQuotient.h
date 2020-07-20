//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionQuotient--------------------------------//
//                                                                          //
// FunctionProduct, result of multiplication of two functions.              //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionQuotient_h
#define FunctionQuotient_h 1
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class FunctionQuotientRecorder;

namespace GeoGenfun {

  class FunctionQuotient : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionQuotient)

      public:

    // Constructor
    FunctionQuotient(const AbsFunction *arg1, const AbsFunction *arg2);

    // Copy constructor
    FunctionQuotient(const FunctionQuotient &right);
  
    // Destructor
    virtual ~FunctionQuotient();
  
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
  
    // It is illegal to assign a FunctionQuotient
    const FunctionQuotient & operator=(const FunctionQuotient &right);
  
    const AbsFunction *_arg1;
    const AbsFunction *_arg2;

    // For persistification:
    friend class ::FunctionQuotientRecorder;
    
  };
} // namespace GeoGenfun
#endif
