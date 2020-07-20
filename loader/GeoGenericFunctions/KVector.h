//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//----------------------------------- --------------------------------------//
//                                                                          //
//  Class KVector for internal use                                          //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef KVector_h
#define KVector_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class KVector : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(KVector)

      public:

    // Constructor
    KVector(unsigned int dimensionality=1,
	    double       value=1);
  
    // Copy constructor
    KVector(const KVector &right);
  
    // Destructor
    virtual ~KVector();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;

    // Get the dimensionality, as specified in the constructor:
    virtual unsigned int dimensionality() const override;  
  
    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a fixed constant
    const KVector & operator=(const KVector &right);


    // The dimensionality, length of the argument list:
    unsigned int _dimensionality;

    // The value of the index() component...
    double _value;
  };

} // namespace Genfun
#endif
