//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ParameterProduct--------------------------------//
//                                                                          //
// ParameterProduct, result of multiplication of two parameters.            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterProduct_h
#define ParameterProduct_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ParameterProduct : public AbsParameter  {
  
    PARAMETER_OBJECT_DEF(ParameterProduct)

      public:
  
    // Constructor
    ParameterProduct(const AbsParameter *arg1, const AbsParameter *arg2);
  
    // Copy constructor
    ParameterProduct(const ParameterProduct &right);

    // Destructor
    virtual ~ParameterProduct();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ParameterProduct
    const ParameterProduct & operator=(const ParameterProduct &right);

    AbsParameter *_arg1;
    AbsParameter *_arg2;
  };
} // namespace GeoGenfun
#endif
