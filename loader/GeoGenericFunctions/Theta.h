//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Theta------------------------------------------------//
//                                                                          //
// Class Theta, also known as the heavyside step function.  The derivative  //
// is zero.  Everywhere, except at the origin.  However QatGenericFuctions  //
// cannot handle the delta-function, so in this implementation the          //
// derivative is zero, everywhere.                                          //
//--------------------------------------------------------------------------//
#ifndef Theta_h
#define Theta_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Theta : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Theta)

      public:

    // Constructor
    Theta();

    // Destructor
    virtual ~Theta();
  
    // Copy constructor
    Theta(const Theta &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  
  private:

    // It is illegal to assign a Theta
    const Theta & operator=(const Theta &right);

  };
} // namespace GeoGenfun

#endif
