/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_CONSTLINK_H
#define GEOMODELKERNEL_CONSTLINK_H

/**
 * @class ConstLink
 * 
 * @brief Smart links to reference-counted pointers.  Used in this
 *      package for calorimeter cells
 **	Template Smart Pointer Class.  This reference counted
 *	link allocates on demand. It audits the total pointer
 *	count and collects the garbage when nobody's looking.
 */

template <class T>
class ConstLink 
{
  public:
  

  /**
   * @brief Constructor
   */
  ConstLink();
  
  /**
   * @brief Copy Constructor
   */
  ConstLink(const ConstLink< T > &right);
  
  /**
   * @brief Constructor
   */
  ConstLink (const T *target);
  
  /**
   * @brief Destructor 
   */
  virtual ~ConstLink();
  
  /**
   * @brief Assignment
   */
  ConstLink< T > & operator=(const ConstLink< T > &right);
  
  /**
   * @brief Equality
   */
  int operator==(const ConstLink< T > &right) const;
  
  /**
   * @brief Inequality
   */
  int operator!=(const ConstLink< T > &right) const;
  
  
  /**
   * @brief Relational operator
   */
  int operator<(const ConstLink< T > &right) const;
  
  /**
   * @brief Relational operator
   */
  int operator>(const ConstLink< T > &right) const;
  
  /**
   * @brief Relational operator
   */
  int operator<=(const ConstLink< T > &right) const;
  
  /**
   * @brief Relational operator
   */
  int operator>=(const ConstLink< T > &right) const;
  
  
  
  /**
   *     @brief Dereference:  (*t).method();
   */
  virtual const T & operator * () const;
  
  /**
   *     @brief Dereference: t->method()
   */
  virtual const T * operator -> () const;
  
  /**
   *      @brief Check pointer validity:  if (t) {...}
   */
  operator bool () const;
  


  private: 
  /**
   * @brief Raw pointer to reference counted object.
   */ 
  mutable T *m_ptr;
};



template <class T>
inline ConstLink<T>::ConstLink()
  :m_ptr(nullptr)
{
}

template <class T>
inline ConstLink<T>::ConstLink(const ConstLink<T> &right)
  :m_ptr(right.m_ptr)
{
  if (m_ptr) m_ptr->ref();
}

template <class T>
inline ConstLink<T>::ConstLink (const T *target)
  :m_ptr(const_cast<T *> (target))
{
  if (target) target->ref();
}


template <class T>
inline ConstLink<T>::~ConstLink()
{
  if (m_ptr) m_ptr->unref();
}


template <class T>
inline ConstLink<T> & ConstLink<T>::operator=(const ConstLink<T> &right)
{
  if (this!=&right) {
    if (m_ptr) m_ptr->unref();
    m_ptr = right.m_ptr;
    if (m_ptr) m_ptr->ref();
  }
  return *this;
}


template <class T>
inline int ConstLink<T>::operator==(const ConstLink<T> &right) const
{
  return m_ptr==right.m_ptr;
}

template <class T>
inline int ConstLink<T>::operator!=(const ConstLink<T> &right) const
{
  return m_ptr!=right.m_ptr;
}


template <class T>
inline int ConstLink<T>::operator<(const ConstLink<T> &right) const
{
  return m_ptr<right.m_ptr;
}

template <class T>
inline int ConstLink<T>::operator>(const ConstLink<T> &right) const
{
  return m_ptr>right.m_ptr;
}

template <class T>
inline int ConstLink<T>::operator<=(const ConstLink<T> &right) const
{
  return m_ptr<=right.m_ptr;
}

template <class T>
inline int ConstLink<T>::operator>=(const ConstLink<T> &right) const
{
  return m_ptr>=right.m_ptr;
}



template <class T>
inline const T & ConstLink<T>::operator * () const
{
  return *m_ptr;
}

template <class T>
inline const T * ConstLink<T>::operator -> () const
{
  return m_ptr;
}

template <class T>
inline ConstLink<T>::operator bool () const
{
  return m_ptr;
}

#endif
