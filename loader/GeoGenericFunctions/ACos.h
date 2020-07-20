//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//---------------------ACos-------------------------------------------------//
//                                                                          //
// Class ACos (arc cos)                                                     //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef ACos_h
#define ACos_h 1

#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class ACos : public AbsFunction  {

    FUNCTION_OBJECT_DEF(ACos)

      public:

    // Constructor
    ACos();

    // Destructor
    virtual ~ACos();
  
    // Copy constructor
    ACos(const ACos &right);
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}
  
  private:

    // It is illegal to assign a ACosine
    const ACos & operator=(const ACos &right);

  };

} // end namespace GeoGenfun

#endif
