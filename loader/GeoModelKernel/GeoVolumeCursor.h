/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVOLUMECURSOR_H
#define GEOMODELKERNEL_GEOVOLUMECURSOR_H

#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

class GeoVAlignmentStore;

class GeoVolumeCursor final : public GeoNodeAction
{
 public:
  GeoVolumeCursor (PVConstLink parent, GeoVAlignmentStore* store=nullptr);
  virtual ~GeoVolumeCursor() override;
 
  GeoVolumeCursor(const GeoVolumeCursor &right) = delete;
  GeoVolumeCursor & operator=(const GeoVolumeCursor &right) = delete;

  /// Advance
  void next();

  /// Check if we are at the last volume.
  bool atEnd() const;

  /// Returns the selected physical volume or nullptr if not found.
  PVConstLink getVolume () const;

  /// Returns the transformation to the volume.
  GeoTrf::Transform3D getTransform () const;
  
  /// Returns the default ransformation to the volume.
  GeoTrf::Transform3D getDefTransform () const;

  /// Returns the name of the volume.  From nametag or serial denominator.
  std::string getName () const;
      
  /// Returns the id of the volume.  From the identifier.
  Query<int> getId () const;

  bool hasAlignableTransform() const;

 private:
  /// Handles a Transform.
  virtual void handleTransform (const GeoTransform *xform) override;

  /// Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *vol) override;

  /// Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *vol) override;

  /// Handles a Name Tag.
  virtual void handleNameTag (const GeoNameTag *nameTag) override;

  /// Handles a Serial Denominator.
  virtual void handleSerialDenominator (const GeoSerialDenominator *sD) override;

  /// Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *sT) override;

  /// Handles an Identifier Tag.
  virtual void handleIdentifierTag (const GeoIdentifierTag *idTag) override;

  /// Handles a Serial Identifier.
  virtual void handleSerialIdentifier(const GeoSerialIdentifier *sI) override;

  /// Ressucitate (undo terminate)
  void resuscitate();


  PVConstLink                           m_parent;
  PVConstLink                           m_volume;
  GeoTrf::Transform3D                           m_transform;
  GeoTrf::Transform3D                           m_defTransform;
  
  unsigned int                          m_majorIndex;
  unsigned int                          m_minorIndex;
  unsigned int                          m_minorLimit;
  const GeoSerialTransformer           *m_serialTransformer;
  
  const GeoNameTag                     *m_nameTag;
  const GeoSerialDenominator           *m_serialDenominator;
  const GeoIdentifierTag               *m_idTag;
  std::vector<const GeoTransform *>     m_pendingTransformList;
  unsigned int                          m_serialDenomPosition;
  const GeoSerialIdentifier            *m_serialIdentifier;
  unsigned int                          m_serialIdentPosition;
  unsigned int                          m_volCount;
  
  bool                                  m_hasAlignTrans;

  GeoVAlignmentStore                   *m_alignStore;
};

#endif
