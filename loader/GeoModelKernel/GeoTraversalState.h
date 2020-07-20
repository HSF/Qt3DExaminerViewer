/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTRAVERSALSTATE_H
#define GEOMODELKERNEL_GEOTRAVERSALSTATE_H

/**
 * @class GeoTraversalState
 *
 * @brief This incorporates a traversal state, including the following information:
 *	* The absolute transform from the place where the action started.
 *	* The default absolute transform from the place where the action started.
 *	* The path to the node.
 *	* The depth
 */
 
#include "GeoModelKernel/GeoNodePath.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <stack>
#include <string>

class GeoTraversalState 
{
 public:
  GeoTraversalState ();
  virtual ~GeoTraversalState();

  //	Gets the transformation of the current node with respect
  //	to its parent.
  const GeoTrf::Transform3D & getTransform () const;
  
  //	Gets the name of the current node.
  const std::string & getName () const;
  
  //	Gets the default transformation of the current node with
  //	respect to its parent.
  const GeoTrf::Transform3D & getDefTransform () const;
  
  //	Gets the absolute name of the current node.
  const std::string & getAbsoluteName () const;
  
  //	Gets the default absolute transformation to the current
  //	node.
  const GeoTrf::Transform3D & getDefAbsoluteTransform () const;

  //	Gets the absolute transformation to the current node.
  const GeoTrf::Transform3D & getAbsoluteTransform () const;
  
  //	Sets the transform for the current node.
  void setTransform (const GeoTrf::Transform3D &transform);
  
  //	Sets the name for the current node.
  void setName (const std::string &name);
  
  //	Sets the default absolute transform for the current node.
  void setDefTransform (const GeoTrf::Transform3D &transform);
  
  //	Goes to the next level.  Pushes the last absolute
  //	transform onto the stack so that it can be retreived
  //	when going back to the previous level.
  void nextLevel (const GeoVPhysVol* pv);
  
  //	Goes to the previous level.  Pops the last absolute
  //	transform onto the stack, restoring the absolute
  //	transform to that of the previous level.
  void previousLevel ();
  
  //	Returns the path.
  const GeoNodePath * getPath () const;
  
  //	Sets the identifier for the current node.
  void setId (const Query<int> &id);
  
  //	Gets the id of the current node.
  const Query<int> getId () const;

 private:
  GeoTraversalState(const GeoTraversalState &right);
  GeoTraversalState & operator=(const GeoTraversalState &right);

  //	A list of tranformations for all nodes visited at all
  //	previous levels of traversal.
  std::stack<GeoTrf::Transform3D> m_absTransformList;
  
  //	A list of default tranformations for all nodes visited
  //	at all previous levels of traversal.
  std::stack<GeoTrf::Transform3D> m_defAbsTransformList;
  
  //	List of volume names.
  std::stack<std::string> m_absNameList;
  
  //	The absolute transform for the present volume.
  GeoTrf::Transform3D m_absTransform;
  
  //	The default absolute transform.
  GeoTrf::Transform3D m_defAbsTransform;
  
  //	The absolute name.
  std::string m_absName;
  
  //	The transform from parent to current.
  GeoTrf::Transform3D m_transform;
  
  //	The default transform from parent to current.
  GeoTrf::Transform3D m_defTransform;
  
  //	The relative name.
  std::string m_name;
  
  //	And identifier for this volume.
  Query<int> m_id;
  
  //	The path from the point at which the action started, to
  //	the current node.
  GeoNodePath m_path;

};

#endif
