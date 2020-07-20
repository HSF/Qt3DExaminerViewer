//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Sin--------------------------------------------------//
//                                                                          //
// Class Sin                                                                //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef Sin_h
#define Sin_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Sin : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sin)

      public:

    // Constructor
    Sin();

    // Destructor
    virtual ~Sin();
  
    // Copy constructor
    Sin(const Sin &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  
  private:

    // It is illegal to assign a Sin
    const Sin & operator=(const Sin &right);

  };
} // namespace GeoGenfun

#endif
