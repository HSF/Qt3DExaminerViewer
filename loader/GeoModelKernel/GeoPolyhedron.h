#ifndef GeoPolyhedron_h
#define GeoPolyhedron_h
//--------------------------------------------------------------------//
// JFB:                                                               //
// GeoPolyhedron was HepPolyhedron, retrofitted to GeoModel           //
// infrastructure:                                                    //
//--------------------------------------------------------------------//


// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// Class Description:
// GeoPolyhedron is an intermediate class between description of a shape
// and visualization systems. It is intended to provide some service like:
//   - polygonization of shapes with triangulization (quadrilaterization)
//     of complex polygons;
//   - calculation of normals for faces and vertices;
//   - finding result of boolean operation on polyhedra;
//
// Public constructors:
//
//   GeoPolyhedronBox (dx,dy,dz)
//                                        - create polyhedron for Box;
//   GeoPolyhedronTrd1 (dx1,dx2,dy,dz)
//                                        - create polyhedron for G3 Trd1;
//   GeoPolyhedronTrd2 (dx1,dx2,dy1,dy2,dz)
//                                        - create polyhedron for G3 Trd2;
//   GeoPolyhedronTrap (dz,theta,phi, h1,bl1,tl1,alp1, h2,bl2,tl2,alp2)
//                                        - create polyhedron for G3 Trap;
//   GeoPolyhedronPara (dx,dy,dz,alpha,theta,phi)
//                                        - create polyhedron for G3 Para;
//   GeoPolyhedronTube (rmin,rmax,dz)
//                                        - create polyhedron for G3 Tube;
//   GeoPolyhedronTubs (rmin,rmax,dz,phi1,dphi)
//                                        - create polyhedron for G3 Tubs;
//   GeoPolyhedronCone (rmin1,rmax1,rmin2,rmax2,dz)
//                                        - create polyhedron for G3 Cone;
//   GeoPolyhedronCons (rmin1,rmax1,rmin2,rmax2,dz,phi1,dphi)
//                                        - create polyhedron for G3 Cons;
//   GeoPolyhedronPgon (phi,dphi,npdv,nz, z(*),rmin(*),rmax(*))
//                                        - create polyhedron for G3 Pgon;
//   GeoPolyhedronPcon (phi,dphi,nz, z(*),rmin(*),rmax(*))
//                                        - create polyhedron for G3 Pcon;
//   GeoPolyhedronSphere (rmin,rmax,phi,dphi,the,dthe)
//                                        - create polyhedron for Sphere;
//   GeoPolyhedronTorus (rmin,rmax,rtor,phi,dphi)
//                                        - create polyhedron for Torus;
// Public functions:
//
//   GetNoVertices ()       - returns number of vertices;
//   GetNoFacets ()         - returns number of faces;
//   GetNextVertexIndex (index,edgeFlag) - get vertex indeces of the
//                            quadrilaterals in order;
//                            returns false when finished each face;
//   GetVertex (index)      - returns vertex by index;
//   GetNextVertex (vertex,edgeFlag) - get vertices with edge visibility
//                            of the quadrilaterals in order;
//                            returns false when finished each face;
//   GetNextVertex (vertex,edgeFlag,normal) - get vertices with edge
//                            visibility and normal of the quadrilaterals
//                            in order; returns false when finished each face;
//   GetNextEdgeIndeces (i1,i2,edgeFlag) - get indeces of the next edge;
//                            returns false for the last edge;
//   GetNextEdgeIndeces (i1,i2,edgeFlag,iface1,iface2) - get indeces of
//                            the next edge with indeces of the faces
//                            to which the edge belongs;
//                            returns false for the last edge;
//   GetNextEdge (p1,p2,edgeFlag) - get next edge;
//                            returns false for the last edge;
//   GetNextEdge (p1,p2,edgeFlag,iface1,iface2) - get next edge with indeces
//                            of the faces to which the edge belongs;
//                            returns false for the last edge;
//   GetFacet (index,n,nodes,edgeFlags=0,normals=0) - get face by index;
//   GetNextFacet (n,nodes,edgeFlags=0,normals=0) - get next face with normals
//                            at the nodes; returns false for the last face;
//   GetNormal (index)      - get normal of face given by index;
//   GetUnitNormal (index)  - get unit normal of face given by index;
//   GetNextNormal (normal) - get normals of each face in order;
//                            returns false when finished all faces;
//   GetNextUnitNormal (normal) - get normals of unit length of each face
//                            in order; returns false when finished all faces;
//   GetSurfaceArea()       - get surface area of the polyhedron;
//   GetVolume()            - get volume of the polyhedron;
//   GetNumberOfRotationSteps()   - get number of steps for whole circle;
//   SetNumberOfRotationSteps (n) - set number of steps for whole circle;
//   ResetNumberOfRotationSteps() - reset number of steps for whole circle
//                            to default value;
// History:
//
// 20.06.96 Evgeni Chernyaev <Evgueni.Tcherniaev@cern.ch> - initial version
//
// 23.07.96 John Allison
// - added GetNoVertices, GetNoFacets, GetNextVertex, GetNextNormal
//
// 30.09.96 E.Chernyaev
// - added GetNextVertexIndex, GetVertex by Yasuhide Sawada
// - added GetNextUnitNormal, GetNextEdgeIndeces, GetNextEdge
// - improvements: angles now expected in radians
//                 int -> G4int, double -> G4double  
// - G4ThreeVector replaced by either G4Point3D or G4Normal3D
//
// 15.12.96 E.Chernyaev
// - private functions G4PolyhedronAlloc, G4PolyhedronPrism renamed
//   to AllocateMemory and CreatePrism
// - added private functions GetNumberOfRotationSteps, RotateEdge,
//   RotateAroundZ, SetReferences
// - rewritten G4PolyhedronCons;
// - added G4PolyhedronPara, ...Trap, ...Pgon, ...Pcon, ...Sphere, ...Torus,
//   so full List of implemented shapes now looks like:
//   BOX, TRD1, TRD2, TRAP, TUBE, TUBS, CONE, CONS, PARA, PGON, PCON,
//   SPHERE, TORUS
//
// 01.06.97 E.Chernyaev
// - RotateAroundZ modified and SetSideFacets added to allow Rmin=Rmax
//   in bodies of revolution
//
// 24.06.97 J.Allison
// - added static private member fNumberOfRotationSteps and static public
//   functions void SetNumberOfRotationSteps (G4int n) and
//   void ResetNumberOfRotationSteps ().  Modified
//   GetNumberOfRotationSteps() appropriately.  Made all three functions
//   inline (at end of this .hh file).
//   Usage:
//    G4Polyhedron::SetNumberOfRotationSteps
//     (fpView -> GetViewParameters ().GetNoOfSides ());
//    pPolyhedron = solid.CreatePolyhedron ();
//    G4Polyhedron::ResetNumberOfRotationSteps ();
//
// 19.03.00 E.Chernyaev
// - added boolean operations (add, subtract, intersect) on polyhedra;
//
// 25.05.01 E.Chernyaev
// - added GetSurfaceArea() and GetVolume();
//
#include "GeoModelKernel/GeoDefinitions.h"
#include <iostream>

#ifndef DEFAULT_NUMBER_OF_STEPS
#define DEFAULT_NUMBER_OF_STEPS 24
#endif

class GeoFacet
{
  friend class GeoPolyhedron;
  friend std::ostream & operator<< (std::ostream &, const GeoFacet & facet);

private:
  struct
  {
    int v, f;
  }
  m_edge[4];

public:
    GeoFacet (int v1 = 0, int f1 = 0, int v2 = 0, int f2 = 0,
	      int v3 = 0, int f3 = 0, int v4 = 0, int f4 = 0)
  {
    m_edge[0].v = v1;
    m_edge[0].f = f1;
    m_edge[1].v = v2;
    m_edge[1].f = f2;
    m_edge[2].v = v3;
    m_edge[2].f = f3;
    m_edge[3].v = v4;
    m_edge[3].f = f4;
  }
};

class GeoPolyhedron
{
  friend std::ostream & operator<< (std::ostream &, const GeoPolyhedron & ph);

private:
  static int s_fNumberOfRotationSteps;

protected:
  int m_nvert, m_nface;
  GeoTrf::Vector3D *m_pV;
  GeoFacet *m_pF;

  // Allocate memory for GeoPolyhedron
  void AllocateMemory (int Nvert, int Nface);

  // Find neighbouring facet
  int FindNeighbour (int iFace, int iNode, int iOrder) const;

  // Find normal at node
  GeoTrf::Vector3D FindNodeNormal (int iFace, int iNode) const;

  // Create GeoPolyhedron for prism with quadrilateral base
  void CreatePrism ();

  // Generate facets by revolving an edge around Z-axis
  void RotateEdge (int k1, int k2, double r1, double r2,
		   int v1, int v2, int vEdge,
		   bool ifWholeCircle, int ns, int &kface);

  // Set side facets for the case of incomplete rotation
  void SetSideFacets (int ii[4], int vv[4],
		      int *kk, double *r, double dphi, int ns, int &kface);

  // Create GeoPolyhedron for body of revolution around Z-axis
  void RotateAroundZ (int nstep, double phi, double dphi,
		      int np1, int np2,
		      const double *z, double *r, int nodeVis, int edgeVis);

  // For each edge set reference to neighbouring facet
  void SetReferences ();

  // Invert the order on nodes in facets
  void InvertFacets ();

public:
  // Constructor
    GeoPolyhedron (int Nvert = 0, int Nface = 0):m_nvert (Nvert), m_nface (Nface),
    m_pV (Nvert ? new GeoTrf::Vector3D[Nvert + 1] : 0),
    m_pF (Nface ? new GeoFacet[Nface + 1] : 0)
  {
  }

  // Copy constructor
  GeoPolyhedron (const GeoPolyhedron & from);

  // Destructor
  virtual ~ GeoPolyhedron ()
  {
    delete[]m_pV;
    delete[]m_pF;
  }

  // Assignment
  virtual GeoPolyhedron & operator= (const GeoPolyhedron & from);

  // Get number of vertices
  int GetNoVertices () const
  {
    return m_nvert;
  }

  // Get number of facets
  int GetNoFacets () const
  {
    return m_nface;
  }

  // Transform the polyhedron
  GeoPolyhedron & Transform (const GeoTrf::RotationMatrix3D & rot,
			     const GeoTrf::Vector3D & trans);

  // Get next vertex index of the quadrilateral
  bool GetNextVertexIndex (int &index, int &edgeFlag) const;

  // Get vertex by index 
  GeoTrf::Vector3D GetVertex (int index) const;

  // Get next vertex + edge visibility of the quadrilateral
  bool GetNextVertex (GeoTrf::Vector3D & vertex, int &edgeFlag) const;

  // Get next vertex + edge visibility + normal of the quadrilateral
  bool GetNextVertex (GeoTrf::Vector3D & vertex, int &edgeFlag,
		      GeoTrf::Vector3D & normal) const;

  // Get indeces of the next edge with indeces of the faces
  bool GetNextEdgeIndeces (int &i1, int &i2, int &edgeFlag,
			   int &iface1, int &iface2) const;

  // Get indeces of the next edge
  bool GetNextEdgeIndeces (int &i1, int &i2, int &edgeFlag) const;

  // Get next edge
  bool GetNextEdge (GeoTrf::Vector3D & p1, GeoTrf::Vector3D & p2, int &edgeFlag) const;

  // Get next edge
  bool GetNextEdge (GeoTrf::Vector3D & p1, GeoTrf::Vector3D & p2, int &edgeFlag,
		    int &iface1, int &iface2) const;

  // Get face by index
  void GetFacet (int iFace, int &n, int *iNodes,
		 int *edgeFlags = 0, int *iFaces = 0) const;

  // Get face by index
  void GetFacet (int iFace, int &n, GeoTrf::Vector3D * nodes,
		 int *edgeFlags = 0, GeoTrf::Vector3D * normals = 0) const;

  // Get next face with normals at the nodes
  bool GetNextFacet (int &n, GeoTrf::Vector3D * nodes,
		     int *edgeFlags = 0, GeoTrf::Vector3D * normals = 0) const;

  // Get normal of the face given by index
  GeoTrf::Vector3D GetNormal (int iFace) const;

  // Get unit normal of the face given by index
  GeoTrf::Vector3D GetUnitNormal (int iFace) const;

  // Get normal of the next face
  bool GetNextNormal (GeoTrf::Vector3D & normal) const;

  // Get normal of unit length of the next face 
  bool GetNextUnitNormal (GeoTrf::Vector3D & normal) const;

  // Boolean operations 
  GeoPolyhedron add (const GeoPolyhedron & p) const;
  GeoPolyhedron subtract (const GeoPolyhedron & p) const;
  GeoPolyhedron intersect (const GeoPolyhedron & p) const;

  // Get area of the surface of the polyhedron
  double GetSurfaceArea () const;

  // Get volume of the polyhedron
  double GetVolume () const;

  // Get number of steps for whole circle
  static int GetNumberOfRotationSteps ()
  {
    return s_fNumberOfRotationSteps;
  }

  // Set number of steps for whole circle
  static void SetNumberOfRotationSteps (int n);

  // Reset number of steps for whole circle to default value
  static void ResetNumberOfRotationSteps ()
  {
    s_fNumberOfRotationSteps = DEFAULT_NUMBER_OF_STEPS;
  }



};

class GeoPolyhedronTrd2:public GeoPolyhedron
{
public:
  GeoPolyhedronTrd2 (double Dx1, double Dx2,
		     double Dy1, double Dy2, double Dz);
    virtual ~ GeoPolyhedronTrd2 ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronTrd1:public GeoPolyhedronTrd2
{
public:
  GeoPolyhedronTrd1 (double Dx1, double Dx2, double Dy, double Dz);
  virtual ~ GeoPolyhedronTrd1 ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronBox:public GeoPolyhedronTrd2
{
public:
  GeoPolyhedronBox (double Dx, double Dy, double Dz);
  virtual ~ GeoPolyhedronBox ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronTrap:public GeoPolyhedron
{
public:
  GeoPolyhedronTrap (double Dz, double Theta, double Phi,
		     double Dy1,
		     double Dx1, double Dx2, double Alp1,
		     double Dy2, double Dx3, double Dx4, double Alp2);
  virtual ~ GeoPolyhedronTrap ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronPara:public GeoPolyhedronTrap
{
public:
  GeoPolyhedronPara (double Dx, double Dy, double Dz,
		     double Alpha, double Theta, double Phi);
  virtual ~ GeoPolyhedronPara ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronCons:public GeoPolyhedron
{
public:
  GeoPolyhedronCons (double Rmn1, double Rmx1,
		     double Rmn2, double Rmx2, double Dz,
		     double Phi1, double Dphi);
  virtual ~ GeoPolyhedronCons ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronCone:public GeoPolyhedronCons
{
public:
  GeoPolyhedronCone (double Rmn1, double Rmx1,
		     double Rmn2, double Rmx2, double Dz);
  virtual ~ GeoPolyhedronCone ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronTubs:public GeoPolyhedronCons
{
public:
  GeoPolyhedronTubs (double Rmin, double Rmax, double Dz,
		     double Phi1, double Dphi);
  virtual ~ GeoPolyhedronTubs ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronTube:public GeoPolyhedronCons
{
public:
  GeoPolyhedronTube (double Rmin, double Rmax, double Dz);
  virtual ~ GeoPolyhedronTube ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronPgon:public GeoPolyhedron
{
public:
  GeoPolyhedronPgon (double phi, double dphi, int npdv, int nz,
		     const double *z, const double *rmin, const double *rmax);
  virtual ~ GeoPolyhedronPgon ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronPcon:public GeoPolyhedronPgon
{
public:
  GeoPolyhedronPcon (double phi, double dphi, int nz,
		     const double *z, const double *rmin, const double *rmax);
  virtual ~ GeoPolyhedronPcon ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronSphere:public GeoPolyhedron
{
public:
  GeoPolyhedronSphere (double rmin, double rmax,
		       double phi, double dphi, double the, double dthe);
  virtual ~ GeoPolyhedronSphere ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};

class GeoPolyhedronTorus:public GeoPolyhedron
{
public:
  GeoPolyhedronTorus (double rmin, double rmax, double rtor,
		      double phi, double dphi);
  virtual ~ GeoPolyhedronTorus ();
  virtual GeoPolyhedron & operator = (const GeoPolyhedron & from)
  {
    return GeoPolyhedron::operator = (from);
  }
};


#endif
