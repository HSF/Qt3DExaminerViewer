//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstMinusParameter-----------------------------//
//                                                                          //
// ConstMinusParameter, result of subtracting a parameter from a constant.  //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstMinusParameter_h
#define ConstMinusParameter_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ConstMinusParameter : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ConstMinusParameter)
  
      public:
  
    // Constructor
    ConstMinusParameter(double, const AbsParameter *);
  
    // Copy constructor
    ConstMinusParameter(const ConstMinusParameter &right);

    // Destructor
    virtual ~ConstMinusParameter();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ConstMinusParameter
    const ConstMinusParameter & operator=(const ConstMinusParameter &right);

    double        _constant;
    AbsParameter *_parameter;
  };
} // namespace GeoGenfun
#endif
