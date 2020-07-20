/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_QUERY_H
#define GEOMODELKERNEL_QUERY_H

/**
 * @class Query
 *
 * @brief  This class can be used to return the result of a query      
 * that can fail.  Objects of this class can convert themselves to the       
 * result type T.  Client code can check for a valid result; if it doesn't,  
 * and the result is valid, no problem.   But if the result is invalid you   
 * will print an error message and terminate.                                
 *
 * Joe Boudreau
 * based on Barton & Nackman's "Scientific and Engineering C++"              
 */

#include <stdexcept>
template < class T > class Query
{

public:

  // Constructor: 
  inline Query (const T &);

  // Default constructor: 
  inline Query ();

  // Convert to "T"  
  inline operator         T () const;

  // Test Validity 
  inline bool isValid () const;

private:

  bool m_failed;
  T m_instance;

};


template < class T > inline Query < T >::Query (const T & t):
m_failed (false),
m_instance (t)
{
}

template < class T > inline Query < T >::Query ():
m_failed (true),
m_instance (T())
{
}

template < class T > inline Query < T >::operator         T () const
{
  if (m_failed)
    throw std::range_error ("Failed query");
  return m_instance;
}

template < class T > inline bool Query < T >::isValid () const
{
  return !m_failed;
}


#endif
