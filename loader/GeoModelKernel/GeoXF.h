/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

//------------------------GeoTransformFunction--------------------------//
//                                                                      //
//  GeoTransformFunction, base class for function objects which         //
//  evaluate to a HepTransform.                                         //
//                                                                      //
//----------------------------------------------------------------------//
#ifndef GEOMODELKERNEL_GEOTRANSFORMFUNCTION_H
#define GEOMODELKERNEL_GEOTRANSFORMFUNCTION_H
#include "GeoGenericFunctions/AbsFunction.h"
#include "GeoGenericFunctions/Argument.h"
#include "GeoModelKernel/GeoDefinitions.h"
//-----------------------------------------------------------------------//
// Exact return type of arithmentic operations.  To the user, the return //
// type is TRANsFUNCTION, or const GeoXF::Function &.                    //
//-----------------------------------------------------------------------//

namespace GeoXF
{

  class Product;
  class PreMult;
  class PostMult;
  class Pow;


  class Function
  {

  public:

    // Default Constructor
    Function ();

    // Destructor
    virtual ~ Function ();

    // Function value:  N-dimensional functions must override these:
    virtual unsigned int dimensionality () const;	//returns 1;

    // Function value
    virtual GeoTrf::Transform3D operator         () (double argument) const = 0;
    virtual GeoTrf::Transform3D operator         () (const GeoGenfun::
						Argument & argument) const =
      0;

    // Every function must override this:
    virtual Function *clone () const = 0;

    // Copy constructor
      Function (const Function & right);

  private:

    // Assignment operator
    const Function & operator = (const Function & right);

  };






  class Pow:public Function
  {

  public:

    Pow (const GeoTrf::Transform3D &, GeoGenfun::GENFUNCTION f);

      virtual ~ Pow ();

    virtual GeoTrf::Transform3D operator         () (double argument) const;
    virtual GeoTrf::Transform3D operator         () (const GeoGenfun::
						Argument & argument) const;

    // Every function must override this:
    Pow *clone () const;

    // Copy constructor:
    Pow (const Pow & right);

    // For persistency:
    const GeoTrf::Transform3D & transform() const {
      return m_xf;
    }

    // For persistency:
    const GeoGenfun::AbsFunction *function() const {
      return m_function;
    }
    
  private:

    // Assignment operator
    const Pow & operator = (const Pow & right);

    const GeoTrf::Transform3D m_xf;
    const GeoGenfun::AbsFunction * m_function;

  };





  Product operator * (const Function & op1, const Function & op2);
  PreMult operator * (const GeoTrf::Transform3D & xf, const Function & op2);
  PostMult operator * (const Function & op2, const GeoTrf::Transform3D & xf);


  // Internally used class:: Product:

  class Product:public Function
  {

  public:


    Product (const Function * arg1, const Function * arg2);

      virtual ~ Product ();

    virtual unsigned int dimensionality () const;

    virtual GeoTrf::Transform3D operator         () (double argument) const;
    virtual GeoTrf::Transform3D operator         () (const GeoGenfun::
						Argument & argument) const;

    // Every function must override this:
    virtual Product *clone () const;

    // Copy constructor:
      Product (const Product & right);

    Product& operator= (const Product & right) = delete;

    const Function *arg1() const { return m_arg1;}
    const Function *arg2() const { return m_arg2;}
    
  private:

    const Function *m_arg1;
    const Function *m_arg2;

  };

  // Internally used class:: PreMult :

  class PreMult:public Function
  {

  public:


    PreMult (const GeoTrf::Transform3D & arg1, const Function * arg2);

      virtual ~ PreMult ();

    virtual unsigned int dimensionality () const;

    virtual GeoTrf::Transform3D operator         () (double argument) const;
    virtual GeoTrf::Transform3D operator         () (const GeoGenfun::
						Argument & argument) const;

    // Every function must override this:
    virtual PreMult *clone () const;

    // Copy constructor:
    PreMult (const PreMult & right);

    PreMult& operator= (const PreMult & right) = delete;

    const GeoTrf::Transform3D & arg1() const { return m_arg1;}
    const Function *arg2() const { return m_arg2;}


  private:

    const GeoTrf::Transform3D m_arg1;
    const Function *m_arg2;

  };

  // Internally used class:: PostMult :

  class PostMult:public Function
  {

  public:


    PostMult (const Function * arg1, const GeoTrf::Transform3D & arg2);

      virtual ~ PostMult ();

    virtual unsigned int dimensionality () const;

    virtual GeoTrf::Transform3D operator         () (double argument) const;
    virtual GeoTrf::Transform3D operator         () (const GeoGenfun::
						Argument & argument) const;

    // Every function must override this:
    virtual PostMult *clone () const;

    // Copy constructor:
      PostMult (const PostMult & right);

    PostMult& operator= (const PostMult & right) = delete;

    const Function *arg1() const { return m_arg1;}
    const GeoTrf::Transform3D & arg2() const { return m_arg2;}

    
  private:

    const Function *m_arg1;
    const GeoTrf::Transform3D m_arg2;

  };

  typedef const Function & TRANSFUNCTION;



}

#endif
