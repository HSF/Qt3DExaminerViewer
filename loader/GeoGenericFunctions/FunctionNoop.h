//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//--------------------------FunctionNoop------------------------------------//
//                                                                          //
// Class FunctionNoop, output of the unary + operator:                      //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef FunctionNoop_h
#define FunctionNoop_h 1
#include "GeoGenericFunctions/AbsFunction.h"

// For persistification:
class FunctionNoopRecorder;


namespace GeoGenfun {

 class FunctionNoop : public AbsFunction {
  
    FUNCTION_OBJECT_DEF(FunctionNoop)

      public:

    // Constructor
    FunctionNoop(const AbsFunction *arg1);
  
    // Copy constructor.
    FunctionNoop(const FunctionNoop &right);
  
    // Destructor
    virtual ~FunctionNoop();
  
    // Retreive function value
    virtual double operator ()(double argument) const override;
    virtual double operator ()(const Argument & a) const override;
  
    // Dimensionality 
    virtual unsigned int dimensionality() const override;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

 private:
  
    // It is illegal to assign a FunctionNoop
    const FunctionNoop & operator=(const FunctionNoop &right);

    // The function we're negating.  
    const AbsFunction *_arg1;

   // For persistification:
   friend class ::FunctionNoopRecorder;
   
   
 };
} // namespace GeoGenfun
#endif
