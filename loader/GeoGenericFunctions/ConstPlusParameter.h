//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//


//--------------------------ConstPlusParameter------------------------------//
//                                                                          //
// ConstPlusParameter, result of adding a constant to a parameters.         //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ConstPlusParameter_h
#define ConstPlusParameter_h 1
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ConstPlusParameter : public AbsParameter  {

    PARAMETER_OBJECT_DEF(ConstPlusParameter)
  
      public:
  
    // Constructor
    ConstPlusParameter(double, const AbsParameter *);
  
    // Copy constructor
    ConstPlusParameter(const ConstPlusParameter &right);

    // Destructor
    virtual ~ConstPlusParameter();
  
    // Retreive function value
    virtual double getValue() const;

  private:

    // It is illegal to assign a ConstPlusParameter
    const ConstPlusParameter & operator=(const ConstPlusParameter &right);

    double        _constant;
    AbsParameter *_parameter;
  };
} // namespace GeoGenfun
#endif
