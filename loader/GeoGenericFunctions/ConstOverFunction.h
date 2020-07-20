//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstOverFunction-------------------------------//
//                                                                          //
// ConstOverFunction, result of dividing a constant by a function           //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstOverFunction_hh
#define ConstOverFunction_hh
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class ConstOverFunctionRecorder;

namespace GeoGenfun {

  class ConstOverFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstOverFunction)
  
      public:
  
    // Constructor
    ConstOverFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstOverFunction(const ConstOverFunction &right);

    // Destructor
    virtual ~ConstOverFunction();
  
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

    // It is illegal to assign a ConstOverFunction
    const ConstOverFunction & operator=(const ConstOverFunction &right);

    double             _constant;
    const AbsFunction *_arg;

    // For persistification:
    friend class ::ConstOverFunctionRecorder;

  };
} // namespace GeoGenfun

#endif


