//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//--------------------------Sigma-------------------------------------------//
//                                                                          //
// Sigma, result of addition of N functions                                 //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef Sigma_h
#define Sigma_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Sigma : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Sigma)
  
      public:
  
    // Constructor
    Sigma();
  
    // Copy constructor
    Sigma(const Sigma &right);

    // Destructor
    virtual ~Sigma();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;

    // Dimensionality 
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

    // Add a function to the sum:
    void accumulate (const AbsFunction & fcn);

  private:

    // It is illegal to assign a Sigma
    const Sigma & operator=(const Sigma &right);

    std::vector<const AbsFunction *> _fcn;

  };
} // namespace GeoGenfun
#endif
