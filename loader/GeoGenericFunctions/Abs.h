//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------Abs---------- --------------------------------------//
//                                                                          //
//  Class Abs                                                               //
//                                                                          //
//  Abs is a function that returns the absolute value of its argument       //
//--------------------------------------------------------------------------//
#ifndef Abs_h
#define Abs_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {
  class Abs : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Abs)
  
      public:

    // Constructor
    Abs();
  
    // Copy constructor
    Abs(const Abs &right);
  
    // Destructor
    virtual ~Abs();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override ;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}
  
  private:

    // It is illegal to assign a fixed constant
    const Abs & operator=(const Abs &right);

  };
}
#include "GeoGenericFunctions/Abs.icc"
#endif
