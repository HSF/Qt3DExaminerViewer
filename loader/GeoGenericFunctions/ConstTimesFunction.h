//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstOverFunction-------------------------------//
//                                                                          //
// ConstOverFunction, result of multiplying a constant and a function       //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstTimesFunction_hh
#define ConstTimesFunction_hh
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class ConstTimesFunctionRecorder;

namespace GeoGenfun {

  class ConstTimesFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstTimesFunction)
  
      public:
  
    // Constructor
    ConstTimesFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstTimesFunction(const ConstTimesFunction &right);

    // Destructor
    virtual ~ConstTimesFunction();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;

    // Dimensionality 
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function.hhave an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a ConstTimesFunction
    const ConstTimesFunction & operator=(const ConstTimesFunction &right);

    double             _constant;
    const AbsFunction *_arg;

    // For persistification
    friend class ::ConstTimesFunctionRecorder;
    
  };
} // namespace GeoGenfun
#endif
