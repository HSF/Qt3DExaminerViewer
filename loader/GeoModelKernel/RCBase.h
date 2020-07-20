/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @class RCBase
 *
 * @brief This is a base class for objects whose memory is managed
 *	through reference counting.  Reference-counted objects
 *	can only be created using
 *	operator new, the cannot be created on the stack.
 *
 *	The methods ref() and unref() can be called to increase
 *	and decrease the reference count of an object.  When
 *	the reference count decreases to zero, the object deletes
 *	itself
 */

#ifndef GEOMODELKERNEL_RCBASE_H
#define GEOMODELKERNEL_RCBASE_H

#include <atomic>

class RCBase 
{
 public:
  RCBase();

  //	Increase the reference count
  void ref () const;

  //	Decreases the reference count.  When the reference count
  //	falls to zero, the object deletes itself.
  void unref () const;

  //	Return the reference count.
  unsigned int refCount () const;

 protected:
  virtual ~RCBase();

 private:
  RCBase(const RCBase &right);
  RCBase & operator=(const RCBase &right);

  //	The reference count
  mutable std::atomic<unsigned> m_count;

};

#endif
