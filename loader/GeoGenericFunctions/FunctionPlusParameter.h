//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionPlusParameter---------------------------//
//                                                                          //
// ConstOverFunction, result of adding a function and a parameter           //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionPlusParameter_h
#define FunctionPlusParameter_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {
  
  class FunctionPlusParameter : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(FunctionPlusParameter)
      
      public:
    
    // Constructor
    FunctionPlusParameter(const AbsParameter *parameter, const AbsFunction *function);
  
    // Copy constructor
    FunctionPlusParameter(const FunctionPlusParameter &right);

    // Destructor
    virtual ~FunctionPlusParameter();
  
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

    // It is illegal to assign a FunctionPlusParameter
    const FunctionPlusParameter & operator=(const FunctionPlusParameter &right);

    const AbsFunction  *_function;
    AbsParameter       *_parameter;

  };
} // namespace GeoGenfun
#endif
