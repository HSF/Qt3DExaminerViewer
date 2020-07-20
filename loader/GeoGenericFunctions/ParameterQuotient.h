//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//


//--------------------------ParameterQuotient-------------------------------//
//                                                                          //
// ParameterQuotient, result of division of two parameters.                 //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterQuotient_h
#define ParameterQuotient_h 1
#include "GeoGenericFunctions/AbsParameter.h"
namespace GeoGenfun {

  class ParameterQuotient : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ParameterQuotient)
  
      public:
  
    // Constructor
    ParameterQuotient(const AbsParameter *arg1, const AbsParameter *arg2);
  
    // Copy constructor
    ParameterQuotient(const ParameterQuotient &right);

    // Destructor
    virtual ~ParameterQuotient();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ParameterQuotient
    const ParameterQuotient & operator=(const ParameterQuotient &right);

    AbsParameter *_arg1;
    AbsParameter *_arg2;
  };
} // namespace GeoGenfun
#endif
