//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

// -------------------------------------------------------------------//
//                                                                    //
// AbsParameter, a base class for all parameters, which are           //
// essentially adjustable constants.                                  //
//                                                                    //
//--------------------------------------------------------------------//
#ifndef _AbsParameter_h_
#define _AbsParameter_h_

namespace GeoGenfun {
  class Parameter;
  class ParameterSum;
  class ParameterDifference;
  class ParameterProduct;
  class ParameterQuotient;
  class ParameterNegation;
  class ConstPlusParameter;
  class ConstMinusParameter;
  class ConstTimesParameter;
  class ConstOverParameter;

  class AbsParameter {
    
  public:
    
    // Default Constructor
    AbsParameter();
    
    // Copy constructor:
    AbsParameter(const AbsParameter &);

    // Destructor
    virtual ~AbsParameter();
  
    // Parameter value
    virtual double getValue()  const=0;   

    // Every parameter must override this:
    AbsParameter * clone() const;

    // Extra lingual type information:
    virtual Parameter *parameter() {return 0;}
    virtual const Parameter *parameter() const {return 0;}

  private:

    // Cannot assign an AbsParameter:
    AbsParameter & operator= (const AbsParameter &);

    virtual AbsParameter *_clone() const=0;

  };

// Now for some additional operations:


ConstTimesParameter               operator * (double c, const AbsParameter &op2);
ConstPlusParameter                operator + (double c, const AbsParameter &op2);
ConstMinusParameter               operator - (double c, const AbsParameter &op2);
ConstOverParameter                operator / (double c, const AbsParameter &op2);

ConstTimesParameter               operator * (const AbsParameter &op2, double c);
ConstPlusParameter                operator + (const AbsParameter &op2, double c);
ConstPlusParameter                operator - (const AbsParameter &op2, double c);
ConstTimesParameter               operator / (const AbsParameter &op2, double c);


ParameterProduct                  operator * (const AbsParameter &op1, const AbsParameter &op2);
ParameterSum                      operator + (const AbsParameter &op1, const AbsParameter &op2);
ParameterDifference               operator - (const AbsParameter &op1, const AbsParameter &op2);
ParameterQuotient                 operator / (const AbsParameter &op1, const AbsParameter &op2);
ParameterNegation                 operator - (const AbsParameter &op1);

typedef const AbsParameter & GENPARAMETER;

} // namespace GeoGenfun   

// Poor man's covariant return type:
#define PARAMETER_OBJECT_DEF(classname)              \
public:                                              \
  classname *clone() const;                          \
private:                                             \
  virtual AbsParameter* _clone() const;

// Poor man's covariant return type:  
#define PARAMETER_OBJECT_IMP(classname)              \
classname *classname::clone () const {               \
  return (classname *) _clone();                     \
}                                                    \
AbsParameter *classname::_clone () const {           \
  return new classname(*this);                       \
}


#include "GeoGenericFunctions/ParameterProduct.h"
#include "GeoGenericFunctions/ParameterSum.h"
#include "GeoGenericFunctions/ParameterDifference.h"
#include "GeoGenericFunctions/ParameterQuotient.h"
#include "GeoGenericFunctions/ParameterNegation.h"
#include "GeoGenericFunctions/ConstPlusParameter.h"
#include "GeoGenericFunctions/ConstMinusParameter.h"
#include "GeoGenericFunctions/ConstTimesParameter.h"
#include "GeoGenericFunctions/ConstOverParameter.h"

#endif
