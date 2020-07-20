//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ParameterNegation-------------------------------//
//                                                                          //
// ParameterNegation, result of negating a parameters.                      //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterNegation_h
#define ParameterNegation_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ParameterNegation : public AbsParameter  {
  
    PARAMETER_OBJECT_DEF(ParameterNegation)

      public:
  
    // Constructor
    ParameterNegation(const AbsParameter *arg1);
  
    // Copy constructor
    ParameterNegation(const ParameterNegation &right);

    // Destructor
    virtual ~ParameterNegation();
  
    // Retreive parameter value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ParameterNegation
    const ParameterNegation & operator=(const ParameterNegation &right);

    // The parameter to be negated:
    AbsParameter *_arg1;

  };
} // namespace GeoGenfun
#endif
