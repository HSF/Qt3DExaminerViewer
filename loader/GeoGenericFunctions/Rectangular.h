//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//
//----------------------Rectangular-----------------------------------------//
//                                                                          //
//  Class Rectangular                                                       //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Rectangular_h
#define Rectangular_h 1
#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/Parameter.h"
namespace GeoGenfun {

  /**
   * @author
   * @ingroup genfun
   */
  class Rectangular : public AbsFunction  {

    FUNCTION_OBJECT_DEF(Rectangular)

      public:

    // Constructor
    Rectangular();

    // Copy constructor
    Rectangular(const Rectangular &right);
  
    // Destructor
    virtual ~Rectangular();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override {return operator() (a[0]);}
  
    // Get the position of the first discontinuity
    const Parameter & x0() const; 
    Parameter & x0(); 

    // Get the position of the second discontinuity
    const Parameter & x1() const; 
    Parameter & x1(); 

    // Get the value of the function at its baseline
    const Parameter & baseline() const; 
    Parameter & baseline(); 

    // Get the value of the function at its top
    const Parameter & height() const; 
    Parameter & height(); 
  
    // Derivative.  
    Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign an adjustable constant
    const Rectangular & operator=(const Rectangular &right);

    // Here is the decay constant
    Parameter _x0;
    Parameter _x1;
    Parameter _baseline;
    Parameter _height;

  };
} // namespace Genfun
#endif
