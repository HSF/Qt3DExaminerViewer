//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------ASin-------------------------------------------------//
//                                                                          //
// Class ASin (arcsin)                                                      //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef ASin_h
#define ASin_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {
  class ASin : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ASin)

      public:

    // Constructor
    ASin();

    // Destructor
    virtual ~ASin();
  
    // Copy constructor
    ASin(const ASin &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  
  private:

    // It is illegal to assign a ASin
    const ASin & operator=(const ASin &right);

  };
} // end namespace Genfun

#endif
