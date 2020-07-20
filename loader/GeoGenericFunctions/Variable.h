//==========================================================================//
// Imported from the QAT library (qat.pitt.edu) by the copyright holder     //
// Joe Boudreau.  Software licensed under the terms and conditions of the   //
// GNU Lesser Public License v3.                                            //
//==========================================================================//

//----------------------X------------ --------------------------------------//
//                                                                          //
//  Class Variable                                                          //
//                                                                          //
//  Variable is a function that returns the variable itself. Dimensionality //
//  is specified in the constructor;  if it is greater than one (default)   //
//  it means that the variable is part of a vector. Which part is specified //
//  by the "selection index" argument to the constructor.                   //
//                                                                          //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Variable_h
#define Variable_h 1
#include "GeoGenericFunctions/AbsFunction.h"
namespace GeoGenfun {

  class Variable : public AbsFunction  {
  
    FUNCTION_OBJECT_DEF(Variable)

      public:

    // Constructor
    Variable(unsigned int selectionIndex=0,
             unsigned int dimensionality=1);
  
    // Copy constructor
    Variable(const Variable &right);
  
    // Destructor
    virtual ~Variable();
  
    // Retrieve function value
    virtual double operator ()(double argument) const override; 
    virtual double operator ()(const Argument & a) const override;

    // Get the dimensionality, as specified in the constructor:
    virtual unsigned int dimensionality() const override;  
  
    // Get the selectionIndex:
    unsigned int index() const;

    // Derivative.  
    virtual Derivative partial (unsigned int) const override;

    // Does this function have an analytic derivative?
    virtual bool hasAnalyticDerivative() const override {return true;}

  private:

    // It is illegal to assign a fixed constant
    const Variable & operator=(const Variable &right);

    // The selection index is used to select a particular element from
    // the argument.  By default it is zero.....
    unsigned int _selectionIndex;

    // The dimensionality, length of the argument list:
    unsigned int _dimensionality;

  };

} // namespace GeoGenfun
#endif
