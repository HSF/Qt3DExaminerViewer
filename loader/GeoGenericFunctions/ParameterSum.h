//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ParameterSum------------------------------------//
//                                                                          //
// ParameterSum, result of addition of two parameters.                      //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef ParameterSum_h
#define ParameterSum_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ParameterSum : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ParameterSum)
  
      public:
  
    // Constructor
    ParameterSum(const AbsParameter *arg1, const AbsParameter *arg2);
  
    // Copy constructor
    ParameterSum(const ParameterSum &right);

    // Destructor
    virtual ~ParameterSum();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ParameterSum
    const ParameterSum & operator=(const ParameterSum &right);

    AbsParameter *_arg1;
    AbsParameter *_arg2;
  };
} // namespace GeoGenfun
#endif
