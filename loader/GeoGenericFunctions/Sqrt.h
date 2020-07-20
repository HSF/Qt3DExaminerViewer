//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------Sqrt--------- --------------------------------------//
//                                                                          //
//  Class Sqrt                                                              //
//                                                                          //
//  Sqrt is a function that returns the square root of the argument         //
//--------------------------------------------------------------------------//
#ifndef Sqrt_h
#define Sqrt_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Sqrt : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sqrt)
  
      public:

    // Constructor
    Sqrt();
  
    // Copy constructor
    Sqrt(const Sqrt &right);
  
    // Destructor
    virtual ~Sqrt();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}
  
    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Sqrt & operator=(const Sqrt &right);

  };
} // namespace GeoGenfun
#endif


