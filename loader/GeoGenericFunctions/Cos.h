//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------Cos--------------------------------------------------//
//                                                                          //
// Class Cos                                                                //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Cos_h
#define Cos_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Cos : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Cos)

      public:

    // Constructor
    Cos();

    // Destructor
    virtual ~Cos();
  
    // Copy constructor
    Cos(const Cos &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  private:

    // It is illegal to assign a Cosine
    const Cos & operator=(const Cos &right);

  };
} // namespace GeoGenfun
#endif
