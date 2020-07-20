//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//


//--------------------------FunctionDifference------------------------------//
//                                                                          //
// FunctionDifference, result of subtraction of two functions               //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionDifference_h
#define FunctionDifference_h 1
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class FunctionDifferenceRecorder;
    
namespace GeoGenfun {

  class FunctionDifference : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionDifference)
  
      public:

    // Constructor
    FunctionDifference(const AbsFunction* arg1, const AbsFunction *arg2);
  
    // Copy constructor
    FunctionDifference(const FunctionDifference &right);
  
    // Destructor
    virtual ~FunctionDifference();
  
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
  
    // It is illegal to assign a FunctionDifference
    const FunctionDifference & operator=(const FunctionDifference &right);

    const AbsFunction *_arg1;
    const AbsFunction *_arg2;

    // For persistification:
    friend class ::FunctionDifferenceRecorder;
    
  };
} // namespace GeoGenfun
#endif
