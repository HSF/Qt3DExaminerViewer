/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOACCESSVOLANDSTACTION_H
#define GEOMODELKERNEL_GEOACCESSVOLANDSTACTION_H

/**
 *  @class GeoAccessVolAndSTAction
 *
 *  @brief  Accesses volumes and serial transformers, returning the ith volume 
 *    and also its transformation relative to the parent.
 *
 *  Used only by Geo2G4. Hence should not depend on the Alignment Store
 */

#include "GeoModelKernel/GeoNodeAction.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

class GeoVPhysVol;
class GeoTransform;

class GeoAccessVolAndSTAction final : public GeoNodeAction
{
 public:
  GeoAccessVolAndSTAction(unsigned int index);
  virtual ~GeoAccessVolAndSTAction() override;

  virtual void handleTransform(const GeoTransform* xform) override;
  virtual void handlePhysVol(const GeoPhysVol* vol) override;
  virtual void handleFullPhysVol(const GeoFullPhysVol* vol) override;

  PVConstLink getVolume() const;
  const GeoTrf::Transform3D & getTransform() const;
  const GeoTrf::Transform3D & getDefTransform() const;
  const std::string & getName() const;

  virtual void handleNameTag(const GeoNameTag *nameTag) override final;
  virtual void handleSerialDenominator(const GeoSerialDenominator* sD) override final;

  virtual void handleSerialTransformer(const GeoSerialTransformer* sT) override final;
  const GeoSerialTransformer* getSerialTransformer() const;

  virtual void handleIdentifierTag(const GeoIdentifierTag* idTag) override final;

  Query<unsigned int> getId() const;

 private:
  GeoAccessVolAndSTAction(const GeoAccessVolAndSTAction &right);
  const GeoAccessVolAndSTAction & operator=(const GeoAccessVolAndSTAction &right);

  /// A pointer to the ith physical volume under this one.
  PVConstLink m_volume;

  /// A pointer to the Serial Transformer
  const GeoSerialTransformer* m_serialTransformer;
 
  /// The transformation and default transformation to the ith volume.
  GeoTrf::Transform3D m_transform;
  GeoTrf::Transform3D m_defTransform;

  unsigned int m_index;
  unsigned int m_counter;

  /// The name of the volume.  From a nametag or a serial denominator
  mutable std::string m_name;

  /// The identifier of the volume.  From an identifier tag.
  mutable Query<unsigned int> m_id;

  /// A pointer to a name tag.  If the volume is named.
  const GeoNameTag *m_nameTag;
  const GeoSerialDenominator *m_serialDenominator;
  const GeoIdentifierTag *m_idTag;

  std::vector<const GeoTransform *>  m_pendingTransformList;
  unsigned int m_serialDenomPosition;
};

#endif


