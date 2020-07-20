//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ConstOverParameter------------------------------//
//                                                                          //
// ConstOverParameter, result of multiplying a parameter with a constant.   //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstOverParameter_h
#define ConstOverParameter_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ConstOverParameter : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ConstOverParameter)
  
      public:
  
    // Constructor
    ConstOverParameter(double, const AbsParameter *);
  
    // Copy constructor
    ConstOverParameter(const ConstOverParameter &right);

    // Destructor
    virtual ~ConstOverParameter();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ConstOverParameter
    const ConstOverParameter & operator=(const ConstOverParameter &right);

    double        _constant;
    AbsParameter *_parameter;
  };
} // namespace GeoGenfun
#endif
