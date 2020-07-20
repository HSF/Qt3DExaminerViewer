//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//--------------------------Pi----------------------------------------------//
//                                                                          //
// Pi, result of multiplication of N functions                              //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef Pi_h
#define Pi_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class Pi : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Pi)
  
      public:
  
    // Constructor
    Pi();
  
    // Copy constructor
    Pi(const Pi &right);

    // Destructor
    virtual ~Pi();
  
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

    // It is illegal to assign a Pi
    const Pi & operator=(const Pi &right);

    std::vector<const AbsFunction *> _fcn;

  };
} // namespace GeoGenfun
#endif
