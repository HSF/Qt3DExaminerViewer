//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstPlusFunction-------------------------------//
//                                                                          //
// ConstPlusFunction, result of adding a function and a constant            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstPlusFunction_hh 
#define ConstPlusFunction_hh 
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class ConstPlusFunctionRecorder;

namespace GeoGenfun {

  class ConstPlusFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstPlusFunction)
  
      public:
  
    // Constructor
    ConstPlusFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstPlusFunction(const ConstPlusFunction &right);

    // Destructor
    virtual ~ConstPlusFunction();
  
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

    // It is illegal to assign a ConstPlusFunction
    const ConstPlusFunction & operator=(const ConstPlusFunction &right);

    double             _constant;
    const AbsFunction *_arg;

    // For persistification:
    friend class ::ConstPlusFunctionRecorder;
    
  };
} // namespace GeoGenfun
#endif
