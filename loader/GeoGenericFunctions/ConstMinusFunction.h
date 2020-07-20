//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstMinusFunction------------------------------//
//                                                                          //
// ConstMinusFunction, result of subtracting a function from constant       //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstMinusFunction_hh
#define ConstMinusFunction_hh 
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification
class ConstMinusFunctionRecorder;

namespace GeoGenfun {

  class ConstMinusFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ConstMinusFunction)
  
      public:
  
    // Constructor
    ConstMinusFunction(double constant, const AbsFunction *arg);
  
    // Copy constructor
    ConstMinusFunction(const ConstMinusFunction &right);

    // Destructor
    virtual ~ConstMinusFunction();
  
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

    // It is illegal to assign a ConstMinusFunction
    const ConstMinusFunction & operator=(const ConstMinusFunction &right);

    double             _constant;
    const AbsFunction *_arg;

    // For persistification:
    friend class ::ConstMinusFunctionRecorder;
    
  };
} // namespace GeoGenfun
#endif
