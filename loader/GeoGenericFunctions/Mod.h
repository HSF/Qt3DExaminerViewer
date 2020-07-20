//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
#ifndef Mod_h
#define Mod_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {
  class Mod : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Mod)
  
      public:

    // Constructor
    Mod(double y);
  
    // Copy constructor
    Mod(const Mod &right);
  
    // Destructor
    virtual ~Mod();
  
    // Retrieve function value

    virtual double operator ()(double argument) const override; 
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}

    // Retrieve the modulus:
    double modulus() const {return _y;}
    
  private:

    // It is illegal to assign a fixed constant
    const Mod & operator=(const Mod &right);

    double _y; // mod (as a double)

  };
} // namespace GeoGenfun
#endif
