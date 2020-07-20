/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEODEFINITIONS_H
#define GEOMODELKERNEL_GEODEFINITIONS_H

// for GNU: ignore this specific warning, otherwise just include Eigen/Dense
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include <Eigen/Dense>
#pragma GCC diagnostic pop
#else
#include <Eigen/Dense>
#endif

namespace GeoTrf {
  typedef Eigen::Quaternion<double> Rotation3D;
  typedef Eigen::Translation<double, 3> Translation3D;
  typedef Eigen::AngleAxisd AngleAxis3D;
  typedef Eigen::Affine3d   Transform3D;
  typedef Eigen::Matrix<double, 3, 1> Vector3D;
  typedef Eigen::Matrix<double, 2, 1> Vector2D;
  typedef Eigen::Matrix<double, 3, 3> RotationMatrix3D;

  /// check if we can use Eigen Hyperplane for this
  struct Plane3D {
  protected:
    double a_, b_, c_, d_;
    
  public:
    /**
     *  Default constructor - creates plane z=0. */
    Plane3D() 
    : a_(0.), b_(0.), c_(1.), d_(0.) 
    {}

    /**
     * Constructor from four numbers - creates plane a*x+b*y+c*z+d=0. */
    Plane3D(double a1, double b1, double c1, double d1)
    :  a_(a1), b_(b1), c_(c1), d_(d1) 
    {}

    /**
     * Constructor from normal and point. */
    Plane3D(const Vector3D& n, const Vector3D& p)
    : a_(n.x()), b_(n.y()), c_(n.z()), d_(-n.dot(p)) 
    {}
    
    /**
     * Returns the a-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double a() const { return a_; }
    /**
     * Returns the b-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double b() const { return b_; }
    /**
     * Returns the c-coefficient in the plane equation: a*x+b*y+c*z+d=0. */
    double c() const { return c_; }
    /**
     * Returns the free member of the plane equation: a*x+b*y+c*z+d=0. */
    double d() const { return d_; }
    
    /**
     * Returns normal. */
    Vector3D normal() const { return Vector3D(a_,b_,c_); }
  };

  class Translate3D : public Transform3D {
  public:
    Translate3D(double x, double y, double z)
      : Transform3D(Translation3D(x,y,z))
      {}
    virtual ~Translate3D() {}
  };

  class TranslateX3D : public Transform3D {
  public:
    TranslateX3D(double x)
      : Transform3D(Translation3D(x,0,0))
      {}
    virtual ~TranslateX3D() {}
  };

  class TranslateY3D : public Transform3D {
  public:
    TranslateY3D(double y)
      : Transform3D(Translation3D(0,y,0))
      {}
    virtual ~TranslateY3D() {}
  };

  class TranslateZ3D : public Transform3D {
  public:
    TranslateZ3D(double z)
      : Transform3D(Translation3D(0,0,z))
      {}
    virtual ~TranslateZ3D() {}
  };

  class Rotate3D : public Transform3D {
  public:
    Rotate3D(double angle, const Vector3D& axis)
      : Transform3D(AngleAxis3D(angle,axis))
      {}
    virtual ~Rotate3D() {}
  };

  class RotateX3D : public Transform3D {
  public:
    RotateX3D(double angle)
      : Transform3D(AngleAxis3D(angle,Vector3D::UnitX()))
      {}
    virtual ~RotateX3D() {}
  };

  class RotateY3D : public Transform3D {
  public:
    RotateY3D(double angle)
      : Transform3D(AngleAxis3D(angle,Vector3D::UnitY()))
      {}
    virtual ~RotateY3D() {}
  };

  class RotateZ3D : public Transform3D {
  public:
    RotateZ3D(double angle)
      : Transform3D(AngleAxis3D(angle,Vector3D::UnitZ()))
      {}
    virtual ~RotateZ3D() {}
  };

  class GeoRotation : public RotationMatrix3D {
  public:
    GeoRotation(double phi, double theta, double psi);
    virtual ~GeoRotation() {}
  };

  class GeoTransformRT : public Transform3D {
  public:
    GeoTransformRT(const GeoRotation& rot, const Vector3D& trans)
      : Transform3D(Translation3D(trans)*Transform3D(AngleAxis3D(rot)))
      {}
    virtual ~GeoTransformRT() {}
  };
}

#endif // GEOMODELKERNEL_GEODEFINITIONS_H
