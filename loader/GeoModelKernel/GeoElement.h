/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOELEMENT_H
#define GEOMODELKERNEL_GEOELEMENT_H

/**
 * @class GeoElement
 *
 * @brief This class represents chemical elements.  Elements are
 * allocated on the heap and deleted automatically when their reference count 
 * falls to zero.
 */

#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/Units.h"
#include <string>

class GeoElement : public RCBase
{
 public:
  GeoElement (const std::string &Name, const std::string &Symbol, double Z, double A);
  
  int operator==(const GeoElement &right) const;
  int operator!=(const GeoElement &right) const;
  
  //	Returns the effective number of nucleons in the element.
  double getN () const;

  //	The name of the element, e.g. "Carbon".
  const std::string& getName () const;

  //	The chemical symbol for the element, e.g. C, O, S, Na....
  const std::string& getSymbol () const;

  //	The atomic number Z for the material.
  const double& getZ () const;

  //	The average atomic mass for the element.
  const double& getA () const;
  
  //        Tsai formula for the radiation length
  double getRadTsai () const;
  
 protected:
  virtual ~GeoElement();
  
 private:
  GeoElement(const GeoElement &right);
  GeoElement & operator=(const GeoElement &right);
  
  std::string m_name;
  std::string m_symbol;

  double m_z;
  double m_a;  
};

inline int GeoElement::operator==(const GeoElement &right) const
{
  return
    m_name ==
    right.m_name && m_symbol == right.m_symbol && m_z == right.m_z && m_a == right.m_a;
}

inline int GeoElement::operator!=(const GeoElement &right) const
{
  return m_name != right.m_name ||
    m_symbol != right.m_symbol || m_z != right.m_z || m_a != right.m_a;
}

inline double GeoElement::getN () const
{
  return m_a * (GeoModelKernelUnits::mole / GeoModelKernelUnits::gram);
}

inline const std::string& GeoElement::getName () const
{
  return m_name;
}

inline const std::string& GeoElement::getSymbol () const
{
  return m_symbol;
}

inline const double& GeoElement::getZ () const
{
  return m_z;
}

inline const double& GeoElement::getA () const
{
  return m_a;
}

#endif
