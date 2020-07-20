//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//----------------------FixedConstant --------------------------------------//
//                                                                          //
//  Class FixedConstant                                                     //
//                                                                          //
//  FixedConstant allows use to treat "promote" fixed contants to constant  //
//  functions so that they automatically will inherit all the algebraic     //
//  operations we have so painstakingly defined for functions.              //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FixedConstant_h
#define FixedConstant_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FixedConstant : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(FixedConstant)

      public:

    // Constructor
    FixedConstant(double value);
  
    // Copy constructor
    FixedConstant(const FixedConstant &right);
  
    // Destructor
    virtual ~FixedConstant();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}
  
    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  private:

    // It is illegal to assign a fixed constant
    const FixedConstant & operator=(const FixedConstant &right);

    // The value of the constant:
    double _value;
  };
} // namespace GeoGenfun
#endif
