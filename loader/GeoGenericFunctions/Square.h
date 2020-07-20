//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//--------------------------------------------------------------------------//
//  Class Square                                                            //
//  Square(x) returns x*x.                                                  //
//--------------------------------------------------------------------------//
#ifndef Square_h
#define Square_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Square : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Square)
  
      public:

    // Constructor
    Square();
  
    // Copy constructor
    Square(const Square &right);
  
    // Destructor
    virtual ~Square();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}
  
    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Square & operator=(const Square &right);

  };
} // namespace GeoGenfun
#endif
