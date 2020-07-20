/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVOLUMEACTION_H
#define GEOMODELKERNEL_GEOVOLUMEACTION_H

/**
 * @class GeoVolumeAction
 *
 * @brief This type of action sees and traverses a simpler tree of
 *	positioned volumes.  It does not descend into
 *	transformations or other properties, but simply
 *	accumulates their effect into an internal state that can
 *	be accessed during traversal.  That state includes, for
 *	example, the position relative to the parent.
 */

#include "GeoModelKernel/GeoTraversalState.h"

class GeoVolumeAction 
{
 public:
  enum Type
  { TOP_DOWN, BOTTOM_UP };

 public:
  GeoVolumeAction (Type type = TOP_DOWN);
  virtual ~GeoVolumeAction();

  //	Callback method. Overriden by users.
  virtual void handleVPhysVol (const GeoVPhysVol *);

  //	Cause the action to terminate.
  void terminate ();

  //	Determine whether the action should terminate.
  bool shouldTerminate () const;
  
  //	Returns the traversal state.  From this one can get
  //	placement information about the volume, even if it is a
  //	shared instance.
  const GeoTraversalState* getState () const;
  
  //	Returns the traversal state, as above.  Non const type.
  GeoTraversalState* getState ();
  
  //	Returns the type: GeoVolumeAction::TOP_DOWN or GeoVolume
  //	Action::BOTTOM_UP.
  GeoVolumeAction::Type getType () const;
  
 private:
  GeoVolumeAction(const GeoVolumeAction &right);
  GeoVolumeAction & operator=(const GeoVolumeAction &right);
  
  //	This type specifies that the action should be carried
  //	out from the bottom up or from the top down.
  Type m_type;

  //	Termination flag; causes an abortion of action execution.
  bool m_terminate;

  GeoTraversalState m_traversalState;
  
};

#endif
