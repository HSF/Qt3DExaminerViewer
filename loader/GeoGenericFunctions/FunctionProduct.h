//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionProduct---------------------------------//
//                                                                          //
// FunctionProduct, result of multiplication of two functions.              //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef FunctionProduct_h
#define FunctionProduct_h 1
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class FunctionProductRecorder;

namespace GeoGenfun {

  class FunctionProduct : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionProduct)

      public:

    // Constructor
    FunctionProduct(const AbsFunction *arg1, const AbsFunction *arg2);

    // Copy constructor
    FunctionProduct(const FunctionProduct &right);
  
    // Destructor
    virtual ~FunctionProduct();

    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;

    // Dimensionality 
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a FunctionProduct
    const FunctionProduct & operator=(const FunctionProduct &right);

    const AbsFunction *_arg1;
    const AbsFunction *_arg2;  

    // For persistification:
    friend class ::FunctionProductRecorder;

  };
} // namespace GeoGenfun

#endif
