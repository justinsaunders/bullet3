/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2011 Advanced Micro Devices, Inc.  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


///This file was written by Erwin Coumans
///Separating axis rest based on work from Pierre Terdiman, see
///And contact clipping based on work from Simon Hobbs


#include "bullet_physics/base_level/collision/bt_convex_polyhedron.h"

#include <float.h> //for FLT_MAX
#include <assert.h>

int gActualNbTests=0;
int gExpectedNbTests=0;
int gActualSATPairTests=0;
bool gUseInternalObject=true;


// Clips a face to the back of a plane
template<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>
void btPolyhedralContactClipping<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>::clipFace(const BT_VERTEX_ARRAY& pVtxIn, BT_VERTEX_ARRAY& ppVtxOut, const btVector3& planeNormalWS,btScalar planeEqWS)
{
	
	int ve;
	btScalar ds, de;
	int numVerts = pVtxIn.size();
	if (numVerts < 2)
		return;

	btVector3 firstVertex=pVtxIn[pVtxIn.size()-1];
	btVector3 endVertex = pVtxIn[0];
	
	ds = VMDOT(planeNormalWS,firstVertex)+planeEqWS;

	for (ve = 0; ve < numVerts; ve++)
	{
		endVertex=pVtxIn[ve];

		de = VMDOT(planeNormalWS,endVertex)+planeEqWS;

		if (ds<0)
		{
			if (de<0)
			{
				// Start < 0, end < 0, so output endVertex
				ppVtxOut.push_back(endVertex);
			}
			else
			{
				// Start < 0, end >= 0, so output intersection
				ppVtxOut.push_back( 	VMLERP(firstVertex,endVertex,btScalar(ds * 1.f/(ds - de)) ) );
			}
		}
		else
		{
			if (de<0)
			{
				// Start >= 0, end < 0 so output intersection and end
				ppVtxOut.push_back(VMLERP(firstVertex,endVertex,btScalar(ds * 1.f/(ds - de))));
				ppVtxOut.push_back(endVertex);
			}
		}
		firstVertex = endVertex;
		ds = de;
	}
}



static bool TestSepAxis(const btConvexPolyhedron& hullA, const btConvexPolyhedron& hullB, const btTransform& transA,const btTransform& transB, const btVector3& sep_axis, float& depth)
{
	float Min0,Max0;
	float Min1,Max1;
	hullA.project(transA,sep_axis, Min0, Max0);
	hullB.project(transB, sep_axis, Min1, Max1);

	if(Max0<Min1 || Max1<Min0)
		return false;

	float d0 = Max0 - Min1;
	assert(d0>=0.0f);
	float d1 = Max1 - Min0;
	assert(d1>=0.0f);
	depth = d0<d1 ? d0:d1;
	return true;
}



inline bool IsAlmostZero(const btVector3& v)
{
	if(fabsf(VMGETX(v))>1e-6 || fabsf(VMGETY(v))>1e-6 || fabsf(VMGETZ(v))>1e-6)	return false;
	return true;
}


#ifdef TEST_INTERNAL_OBJECTS

inline void BoxSupport(const btVector3& extents, const btVector3& sv, btVector3& p)
{
	p[0] = sv[0] < 0.0f ? -extents[0] : extents[0];
	p[1] = sv[1] < 0.0f ? -extents[1] : extents[1];
	p[2] = sv[2] < 0.0f ? -extents[2] : extents[2];
}

void InverseTransformPoint3x3(btVector3& out, const btVector3& in, const btTransform& tr)
{
	const btMatrix3x3& rot = VMGETBASIS(tr);
	btVector3 r0 = rot.getRow(0);
	btVector3 r1 = rot.getRow(1);
	btVector3 r2 = rot.getRow(2);
	
	const btScalar x = VMGETX(r0)*VMGETX(in) + VMGETX(r1)*VMGETY(in) + VMGETX(r2)*VMGETZ(in);
	const btScalar y = VMGETY(r0)*VMGETX(in) + VMGETY(r1)*VMGETY(in) + VMGETY(r2)*VMGETZ(in);
	const btScalar z = VMGETZ(r0)*VMGETX(in) + VMGETZ(r1)*VMGETY(in) + VMGETZ(r2)*VMGETZ(in);

	VMSET(out,x,y,z);
}

bool TestInternalObjects( const btTransform& trans0, const btTransform& trans1, const btVector3& delta_c, const btVector3& axis, const btConvexPolyhedron& convex0, const btConvexPolyhedron& convex1, btScalar dmin)
{
	const btScalar dp = VMDOT(delta_c,axis);

	btVector3 localAxis0;
	InverseTransformPoint3x3(localAxis0, axis,trans0);
	btVector3 localAxis1;
	InverseTransformPoint3x3(localAxis1, axis,trans1);

	btVector3 p0;
	BoxSupport(convex0.m_extents, localAxis0, p0);
	btVector3 p1;
	BoxSupport(convex1.m_extents, localAxis1, p1);

	const btScalar Radius0 = p0[0]*VMGETX(localAxis0) + p0[1]*VMGETY(localAxis0) + p0[2]*VMGETZ(localAxis0);
	const btScalar Radius1 = p1[0]*VMGETX(localAxis1) + p1[1]*VMGETY(localAxis1) + p1[2]*VMGETZ(localAxis1);

	const btScalar MinRadius = Radius0>convex0.m_radius ? Radius0 : convex0.m_radius;
	const btScalar MaxRadius = Radius1>convex1.m_radius ? Radius1 : convex1.m_radius;

	const btScalar MinMaxRadius = MaxRadius + MinRadius;
	const btScalar d0 = MinMaxRadius + dp;
	const btScalar d1 = MinMaxRadius - dp;

	const btScalar depth = d0<d1 ? d0:d1;
	if(depth>dmin)
		return false;
	return true;
}
#endif //TEST_INTERNAL_OBJECTS


template<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>
bool btPolyhedralContactClipping<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>::findSeparatingAxis(	const btConvexPolyhedron& hullA, const btConvexPolyhedron& hullB, const btTransform& transA,const btTransform& transB, btVector3& sep)
{
	gActualSATPairTests++;

#ifdef TEST_INTERNAL_OBJECTS
	const btVector3 c0 = VMTRANS(transA,hullA.m_localCenter);
	const btVector3 c1 = VMTRANS(transB,hullB.m_localCenter);
	const btVector3 DeltaC2 = c0 - c1;
#endif

	float dmin = FLT_MAX;
	int curPlaneTests=0;

	int numFacesA = hullA.m_faces.size();
	// Test normals from hullA
	for(int i=0;i<numFacesA;i++)
	{
		const btVector3 Normal(hullA.m_faces[i].m_plane[0], hullA.m_faces[i].m_plane[1], hullA.m_faces[i].m_plane[2]);
		const btVector3 faceANormalWS = VMGETBASIS(transA) * Normal;

		curPlaneTests++;
#ifdef TEST_INTERNAL_OBJECTS
		gExpectedNbTests++;
		if(gUseInternalObject && !TestInternalObjects(transA,transB,DeltaC2, faceANormalWS, hullA, hullB, dmin))
			continue;
		gActualNbTests++;
#endif

		float d;
		if(!TestSepAxis( hullA, hullB, transA,transB, faceANormalWS, d))
			return false;

		if(d<dmin)
		{
			dmin = d;
			sep = faceANormalWS;
		}
	}

	int numFacesB = hullB.m_faces.size();
	// Test normals from hullB
	for(int i=0;i<numFacesB;i++)
	{
		const btVector3 Normal(hullB.m_faces[i].m_plane[0], hullB.m_faces[i].m_plane[1], hullB.m_faces[i].m_plane[2]);
		const btVector3 WorldNormal = VMGETBASIS(transB) * Normal;

		curPlaneTests++;
#ifdef TEST_INTERNAL_OBJECTS
		gExpectedNbTests++;
		if(gUseInternalObject && !TestInternalObjects(transA,transB,DeltaC2, WorldNormal, hullA, hullB, dmin))
			continue;
		gActualNbTests++;
#endif

		float d;
		if(!TestSepAxis(hullA, hullB,transA,transB, WorldNormal,d))
			return false;

		if(d<dmin)
		{
			dmin = d;
			sep = WorldNormal;
		}
	}

	btVector3 edgeAstart,edgeAend,edgeBstart,edgeBend;

	int curEdgeEdge = 0;
	// Test edges
	for(int e0=0;e0<hullA.m_uniqueEdges.size();e0++)
	{
		const btVector3 edge0 = hullA.m_uniqueEdges[e0];
		const btVector3 WorldEdge0 = VMGETBASIS(transA) * edge0;
		for(int e1=0;e1<hullB.m_uniqueEdges.size();e1++)
		{
			const btVector3 edge1 = hullB.m_uniqueEdges[e1];
			const btVector3 WorldEdge1 = VMGETBASIS(transB) * edge1;

			btVector3 Cross = VMCROSS(WorldEdge0,WorldEdge1);
			curEdgeEdge++;
			if(!IsAlmostZero(Cross))
			{
				Cross = VMNORMALIZED(Cross);

#ifdef TEST_INTERNAL_OBJECTS
				gExpectedNbTests++;
				if(gUseInternalObject && !TestInternalObjects(transA,transB,DeltaC2, Cross, hullA, hullB, dmin))
					continue;
				gActualNbTests++;
#endif

				float dist;
				if(!TestSepAxis( hullA, hullB, transA,transB, Cross, dist))
					return false;

				if(dist<dmin)
				{
					dmin = dist;
					sep = Cross;
				}
			}
		}

	}

	const btVector3 deltaC = VMGETTRANSLATION(transB) - VMGETTRANSLATION(transA);
	if((VMDOT(deltaC,sep))>0.0f)
		sep = -sep;

	return true;
}

template<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>
void	btPolyhedralContactClipping<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>::clipFaceAgainstHull(const btVector3& separatingNormal, const btConvexPolyhedron& hullA,  const btTransform& transA, BT_VERTEX_ARRAY& worldVertsB1, const btTransform& transB,const btScalar minDist, btScalar maxDist,BT_CONTACT_CACHE& resultOut)
{
	BT_VERTEX_ARRAY worldVertsB2;
	BT_VERTEX_ARRAY* pVtxIn = &worldVertsB1;
	BT_VERTEX_ARRAY* pVtxOut = &worldVertsB2;

	int closestFaceA=-1;
	{
		btScalar dmin = FLT_MAX;
		for(int face=0;face<hullA.m_faces.size();face++)
		{
			const btVector3 Normal(hullA.m_faces[face].m_plane[0], hullA.m_faces[face].m_plane[1], hullA.m_faces[face].m_plane[2]);
			const btVector3 faceANormalWS = VMGETBASIS(transA) * Normal;
		
			btScalar d = VMDOT(faceANormalWS,separatingNormal);
			if (d < dmin)
			{
				dmin = d;
				closestFaceA = face;
			}
		}
	}
	if (closestFaceA<0)
		return;

	const btFace& polyA = hullA.m_faces[closestFaceA];

		// clip polygon to back of planes of all faces of hull A that are adjacent to witness face
	int numContacts = pVtxIn->size();
	int numVerticesA = polyA.m_indices.size();
	for(int e0=0;e0<numVerticesA;e0++)
	{
		/*const btVector3& a = hullA.m_vertices[polyA.m_indices[e0]];
		const btVector3& b = hullA.m_vertices[polyA.m_indices[(e0+1)%numVerticesA]];
		const btVector3 edge0 = a - b;
		const btVector3 WorldEdge0 = transA.getBasis() * edge0;
		*/

		int otherFace = polyA.m_connectedFaces[e0];
		btVector3 localPlaneNormal (hullA.m_faces[otherFace].m_plane[0],hullA.m_faces[otherFace].m_plane[1],hullA.m_faces[otherFace].m_plane[2]);
		btScalar localPlaneEq = hullA.m_faces[otherFace].m_plane[3];

		btVector3 planeNormalWS = VMGETBASIS(transA)*localPlaneNormal;
		btScalar planeEqWS=localPlaneEq-VMDOT(planeNormalWS,VMGETTRANSLATION(transA));
		//clip face

		clipFace(*pVtxIn, *pVtxOut,planeNormalWS,planeEqWS);
		VMSWAP(pVtxIn,pVtxOut);
		if (!pVtxIn->size())
			break;
		pVtxOut->resize(0);
	}

	btVector3 localPlaneNormal (polyA.m_plane[0],polyA.m_plane[1],polyA.m_plane[2]);
	btScalar localPlaneEq = polyA.m_plane[3];
	btVector3 planeNormalWS = VMGETBASIS(transA)*localPlaneNormal;
	btScalar planeEqWS=localPlaneEq-VMDOT(planeNormalWS,VMGETTRANSLATION(transA));

	resultOut.addContacts(planeNormalWS, planeEqWS, pVtxIn->size(), &pVtxIn->at(0));

}
template<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>
void	btPolyhedralContactClipping<typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>::clipHullAgainstHull(const btVector3& separatingNormal, const btConvexPolyhedron& hullA, const btConvexPolyhedron& hullB, const btTransform& transA,const btTransform& transB, const btScalar minDist, btScalar maxDist,BT_CONTACT_CACHE& resultOut)
{

	btScalar curMaxDist=maxDist;
	int closestFaceB=-1;

	{
		btScalar dmax = -FLT_MAX;
		for(int face=0;face<hullB.m_faces.size();face++)
		{
			const btVector3 Normal(hullB.m_faces[face].m_plane[0], hullB.m_faces[face].m_plane[1], hullB.m_faces[face].m_plane[2]);
			const btVector3 WorldNormal = VMGETBASIS(transB) * Normal;

			btScalar d = VMDOT(WorldNormal,separatingNormal);
			if (d > dmax)
			{
				dmax = d;
				closestFaceB = face;
			}
		}
	}



	if (closestFaceB<0)
	{
		return;
	}



	// setup initial clip face (minimizing face from hull B)
	BT_VERTEX_ARRAY worldVertsB1;
	{
		const btFace& polyB = hullB.m_faces[closestFaceB];
		const int numVertices = polyB.m_indices.size();
		for(int e0=0;e0<numVertices;e0++)
		{
			const btVector3& b = hullB.m_vertices[polyB.m_indices[e0]];
			worldVertsB1.push_back(VMTRANS(transB,b));
		}
	}

	clipFaceAgainstHull(separatingNormal, hullA, transA,worldVertsB1, transB,minDist, maxDist,resultOut);

}

