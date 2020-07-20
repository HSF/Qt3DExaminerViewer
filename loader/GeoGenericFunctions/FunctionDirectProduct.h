//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionDirectProduct---------------------------//
//                                                                          //
// FunctionDirectProduct, result of multiplication of taking the direct     //
// product of two functions:  f(x)*g(y)= h(x,y).  The direct product always //
// gives a function taking an argument of higher dimensionality than the    //
// arguments.                                                               //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionDirectProduct_h
#define FunctionDirectProduct_h 1
#include "GeoGenericFunctions/AbsFunction.h"

namespace GeoGenfun {

  class FunctionDirectProduct : public AbsFunction {

    FUNCTION_OBJECT_DEF(FunctionDirectProduct)

      public:

    // Constructor
    FunctionDirectProduct(const AbsFunction *arg1, const AbsFunction *arg2);

    // Copy constructor
    FunctionDirectProduct(const FunctionDirectProduct &right);
  
    // Destructor
    virtual ~FunctionDirectProduct();

    // Retreive function value
    virtual double operator ()(double argument) const override;    // Gives an error.
    virtual double operator ()(const Argument & a) const override; // Must use this one

    // Dimensionality
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a FunctionDirectProduct
    const FunctionDirectProduct & operator=(const FunctionDirectProduct &right);

    AbsFunction *_arg1;
    AbsFunction *_arg2;
    unsigned int _m;  // dimension of arg1
    unsigned int _n;  // dimension of arg2

  };
} // namespace GeoGenfun
#endif
