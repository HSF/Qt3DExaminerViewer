//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ParameterDifference-----------------------------//
//                                                                          //
// ParameterDifference, result of subtraction of two parameters.            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterDifference_h
#define ParameterDifference_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ParameterDifference : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ParameterDifference)
  
      public:
  
    // Constructor
    ParameterDifference(const AbsParameter *arg1, const AbsParameter *arg2);
  
    // Copy constructor
    ParameterDifference(const ParameterDifference &right);

    // Destructor
    virtual ~ParameterDifference();
  
    // Retreive parameter value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ParameterDifference
    const ParameterDifference & operator=(const ParameterDifference &right);

    AbsParameter *_arg1;
    AbsParameter *_arg2;
  };
} // namespace GeoGenfun
#endif
