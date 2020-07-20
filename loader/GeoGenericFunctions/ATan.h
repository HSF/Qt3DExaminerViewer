//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------ATan--------- --------------------------------------//
//                                                                          //
//  Class ATan                                                              //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ATan_h
#define ATan_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {
  class ATan : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ATan)

      public:

    // Constructor
    ATan();

    // Destructor
    virtual ~ATan();
  
    // Copy constructor
    ATan(const ATan &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  
  private:

    // It is illegal to assign a ATan
    const ATan & operator=(const ATan &right);

  };
} // end namespace Genfun

#endif
