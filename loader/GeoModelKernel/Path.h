/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_PATH_H
#define GEOMODELKERNEL_PATH_H

/**
 * @class Path
 * 
 * @brief Represents a path from one point in the general tree, to the present point.
 */

#include <vector>
#include <cstdlib>

template <class T>
class Path 
{
 public:
  Path();
  Path(const Path< T > &right);
  ~Path();

  Path< T > & operator=(const Path< T > &right);

  //	Pops the last node from the stack.
  const T * pop ();
  
  //	Pushes another node on to the tail of the path.
  void push (const T *node);

  //	Returns the length of the path.
  unsigned int getLength () const;

  //	Returns the ith Item.
  const T * getItem (unsigned int i) const;

  //	Gets the head of the path.
  const T * getHead () const;

  //	Gets the tail of the path.
  const T * getTail () const;
 
  private:
  //	A list of physical volumes betwee the head of the path
  //	and the tail of the path.
  std::vector<const T *> m_nodeList;

};

template <class T>
inline Path<T>::Path()
{
  m_nodeList.reserve(2);
}

template <class T>
inline Path<T>::Path(const Path<T> &right)
  : m_nodeList(right.m_nodeList)
{
}

template <class T>
inline Path<T>::~Path()
{
}

template <class T>
inline Path<T> & Path<T>::operator=(const Path<T> &right)
{
  if (this != &right)
    {
      m_nodeList = right.m_nodeList;
    }
  return *this;
}

template <class T>
inline const T * Path<T>::pop ()
{
  if (m_nodeList.size () == 0)
    {
      return nullptr;
    }
  else
    {
      const T *gvp = m_nodeList.back ();
      m_nodeList.pop_back ();
      return gvp;
    }
}

template <class T>
inline void Path<T>::push (const T *node)
{
  m_nodeList.push_back (node);
}

template <class T>
inline unsigned int Path<T>::getLength () const
{
  return m_nodeList.size ();
}

template <class T>
inline const T * Path<T>::getItem (unsigned int i) const
{
  if (m_nodeList.size () == 0)
	{
      return nullptr;
    }
  else
    {
      return m_nodeList[i];
    }
}

template <class T>
inline const T * Path<T>::getHead () const
{
  if (m_nodeList.size () == 0)
    {
      return nullptr;
    }
  else
    {
      return m_nodeList.front ();
    }
}

template <class T>
inline const T * Path<T>::getTail () const
{
  if (m_nodeList.size () == 0)
    {
      return nullptr;
    }
  else
    {
      return m_nodeList.back ();
    }
}

#endif
