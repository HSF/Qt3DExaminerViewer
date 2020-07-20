//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//------------------------AbsFunction-----------------------------------//
//                                                                      //
//  AbsFunction, base class for function objects                        //
//                                                                      //
//----------------------------------------------------------------------//
#ifndef AbsFunction_h
#define AbsFunction_h 1
#include "GeoGenericFunctions/Argument.h"

namespace GeoGenfun {

  class AbsParameter;

  //-----------------------------------------------------------------------//
  // Exact return type of arithmentic operations.  To the user, the return //
  // type is GENFUNCTION, or const AbsFunction &.                          //
  //-----------------------------------------------------------------------//

  class FunctionProduct;
  class FunctionSum;
  class FunctionDifference;
  class FunctionQuotient;
  class FunctionNegation;
  class FunctionDirectProduct;
  class FunctionComposition;
  class ConstPlusFunction;
  class ConstTimesFunction;
  class ConstMinusFunction;
  class ConstOverFunction;
  class FunctionPlusParameter;
  class FunctionTimesParameter;
  class FunctionNumDeriv;
  class Variable;
  class FunctionNoop;
  class ParameterComposition;

  typedef FunctionNoop Derivative;

  class AbsFunction {
  
  public:
  
    // Default Constructor
    AbsFunction();
  
    // Copy Constructor:
    AbsFunction(const AbsFunction &right);
  
    // Destructor
    virtual ~AbsFunction();
  
    // Function value:  N-dimensional functions must override these:
    virtual unsigned int dimensionality() const ;      // returns 1;

    // Function value
    virtual double operator() (double argument)          const=0;   
    virtual double operator() (const Argument &argument) const=0; 

    // Every function must override this:
    virtual AbsFunction * clone() const=0;
  
    // Function composition.  Do not attempt to override:
    virtual FunctionComposition operator () (const AbsFunction &f) const;
    
    // Parameter composition.  Do not attempt to override:
    virtual ParameterComposition operator() ( const AbsParameter &p) const;

    // Derivative, (All functions)  (do not override)
    Derivative partial(const Variable &v) const;

    // Derivative (1D functions only) (do not override)
    Derivative prime() const;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const {return false;}

    // Derivative.  Overriders may be provided, numerical method by default!
    virtual Derivative partial(unsigned int) const;
  
  private:

    // It is illegal to assign a function.
    const AbsFunction & operator=(const AbsFunction &right);
  
  };

FunctionProduct           operator * (const AbsFunction &op1, const AbsFunction &op2);
FunctionSum               operator + (const AbsFunction &op1, const AbsFunction &op2);
FunctionDifference        operator - (const AbsFunction &op1, const AbsFunction &op2);
FunctionQuotient          operator / (const AbsFunction &op1, const AbsFunction &op2);
FunctionNegation          operator - (const AbsFunction &op1);

ConstTimesFunction        operator * (double c, const AbsFunction &op2);
ConstPlusFunction         operator + (double c, const AbsFunction &op2);
ConstMinusFunction        operator - (double c, const AbsFunction &op2);
ConstOverFunction         operator / (double c, const AbsFunction &op2);

ConstTimesFunction        operator * (const AbsFunction &op2, double c);
ConstPlusFunction         operator + (const AbsFunction &op2, double c);
ConstPlusFunction         operator - (const AbsFunction &op2, double c);
ConstTimesFunction        operator / (const AbsFunction &op2, double c);

FunctionTimesParameter    operator * (const AbsFunction &op1, const AbsParameter &op2);
FunctionPlusParameter     operator + (const AbsFunction &op1, const AbsParameter &op2);
FunctionPlusParameter     operator - (const AbsFunction &op1, const AbsParameter &op2);
FunctionTimesParameter    operator / (const AbsFunction &op1, const AbsParameter &op2);

FunctionTimesParameter    operator * (const AbsParameter   &op1, const AbsFunction &op2);
FunctionPlusParameter     operator + (const AbsParameter   &op1, const AbsFunction &op2);
FunctionPlusParameter     operator - (const AbsParameter   &op1, const AbsFunction &op2);
FunctionTimesParameter    operator / (const AbsParameter   &op1, const AbsFunction &op2);

FunctionDirectProduct     operator % (const AbsFunction &op1, const AbsFunction &op2);

typedef const AbsFunction & GENFUNCTION;

} // namespace GeoGenfun


#define FUNCTION_OBJECT_DEF(classname) \
public:                                \
 using AbsFunction::operator();		       \
  virtual classname *clone() const override;   \
private:                               

//----------------------------------------------------------------------------
//
//  This macro implements the ugly boilerplate
//
  
#define FUNCTION_OBJECT_IMP(classname)       \
classname *classname::clone() const          \
{                                            \
  return new classname(*this);               \
}


#define FUNCTION_OBJECT_IMP_INLINE(classname)       \
inline classname *classname::clone() const          \
{                                            \
  return new classname(*this);               \
}


//----------------------------------------------------------------------------


#include "GeoGenericFunctions/FunctionProduct.h"
#include "GeoGenericFunctions/FunctionSum.h"
#include "GeoGenericFunctions/FunctionDifference.h"
#include "GeoGenericFunctions/FunctionQuotient.h"
#include "GeoGenericFunctions/FunctionNegation.h"
#include "GeoGenericFunctions/FunctionDirectProduct.h"
#include "GeoGenericFunctions/FunctionComposition.h"
#include "GeoGenericFunctions/ConstPlusFunction.h"
#include "GeoGenericFunctions/ConstTimesFunction.h"
#include "GeoGenericFunctions/ConstMinusFunction.h"
#include "GeoGenericFunctions/ConstOverFunction.h"
#include "GeoGenericFunctions/FunctionPlusParameter.h"
#include "GeoGenericFunctions/FunctionTimesParameter.h"
#include "GeoGenericFunctions/FunctionNoop.h"
#include "GeoGenericFunctions/ParameterComposition.h"

#endif
