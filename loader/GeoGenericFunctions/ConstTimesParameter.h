//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//


//--------------------------ConstTimesParameter-----------------------------//
//                                                                          //
// ConstTimesParameter, result of multiplying a parameter and a constant.   //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstTimesParameter_h
#define ConstTimesParameter_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ConstTimesParameter : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ConstTimesParameter)
  
      public:
  
    // Constructor
    ConstTimesParameter(double, const AbsParameter *);
  
    // Copy constructor
    ConstTimesParameter(const ConstTimesParameter &right);

    // Destructor
    virtual ~ConstTimesParameter();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ConstTimesParameter
    const ConstTimesParameter & operator=(const ConstTimesParameter &right);

    double        _constant;
    AbsParameter *_parameter;
  };
} // namespace GeoGenfun
#endif
