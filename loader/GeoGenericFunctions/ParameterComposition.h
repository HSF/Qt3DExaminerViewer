//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------ParameterComposition----------------------------//
//                                                                          //
// ParameterComposition, result of composition of two parameters            //
//                                                                          //
//--------------------------------------------------------------------------//

#ifndef ParameterComposition_hh
#define ParameterComposition_hh 
#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/AbsParameter.h"

namespace GeoGenfun {

  class ParameterComposition : public AbsParameter {

    PARAMETER_OBJECT_DEF(ParameterComposition)

      public:

    // Constructor
    ParameterComposition(const AbsFunction *arg1, const AbsParameter *arg2);

    // Copy constructor
    ParameterComposition(const ParameterComposition &right);
  
    // Destructor
    virtual ~ParameterComposition();

    // Retreive parameter value
    virtual double getValue() const;


  private:

    // It is illegal to assign a ParameterComposition
    const ParameterComposition & operator=(const ParameterComposition &right);

    const AbsFunction  *_arg1;
    AbsParameter *_arg2;  
  };
} // namespace GeoGenfun
#endif
