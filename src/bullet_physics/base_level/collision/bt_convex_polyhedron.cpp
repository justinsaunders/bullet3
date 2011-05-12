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

#include "bullet_physics/bt_math_conversions.h"
#include "bullet_physics/base_level/collision/bt_convex_polyhedron.h"

static const int MAX_CAP = 1024;
#include "bullet_physics/util/bt_hash_map.h"


btConvexPolyhedron::btConvexPolyhedron()
{

}
btConvexPolyhedron::~btConvexPolyhedron()
{

}


inline bool IsAlmostZero(const btVector3& v)
{
	if(fabsf(VMGETX(v))>1e-6 || fabsf(VMGETY(v))>1e-6 || fabsf(VMGETZ(v))>1e-6)	return false;
	return true;
}

struct btInternalVertexPair
{
	btInternalVertexPair()
	{
	}

	btInternalVertexPair(short int v0,short int v1)
		:m_v0(v0),
		m_v1(v1)
	{
		if (m_v1>m_v0)
			VMSWAP(m_v0,m_v1);
	}
	short int m_v0;
	short int m_v1;
	int getHash() const
	{
		return m_v0+(m_v1<<16);
	}
	bool equals(const btInternalVertexPair& other) const
	{
		return m_v0==other.m_v0 && m_v1==other.m_v1;
	}
};

struct btInternalEdge
{
	btInternalEdge()
		:m_face0(-1),
		m_face1(-1)
	{
	}
	short int m_face0;
	short int m_face1;
};


#ifdef TEST_INTERNAL_OBJECTS
bool btConvexPolyhedron::testContainment() const
{
	for(int p=0;p<8;p++)
	{
		btVector3 LocalPt;
		if(p==0)		LocalPt = m_localCenter + btVector3(m_extents[0], m_extents[1], m_extents[2]);
		else if(p==1)	LocalPt = m_localCenter + btVector3(m_extents[0], m_extents[1], -m_extents[2]);
		else if(p==2)	LocalPt = m_localCenter + btVector3(m_extents[0], -m_extents[1], m_extents[2]);
		else if(p==3)	LocalPt = m_localCenter + btVector3(m_extents[0], -m_extents[1], -m_extents[2]);
		else if(p==4)	LocalPt = m_localCenter + btVector3(-m_extents[0], m_extents[1], m_extents[2]);
		else if(p==5)	LocalPt = m_localCenter + btVector3(-m_extents[0], m_extents[1], -m_extents[2]);
		else if(p==6)	LocalPt = m_localCenter + btVector3(-m_extents[0], -m_extents[1], m_extents[2]);
		else if(p==7)	LocalPt = m_localCenter + btVector3(-m_extents[0], -m_extents[1], -m_extents[2]);

		for(int i=0;i<m_faces.size();i++)
		{
			const btVector3 Normal(m_faces[i].m_plane[0], m_faces[i].m_plane[1], m_faces[i].m_plane[2]);
			const btScalar d = VMDOT(LocalPt,Normal) + m_faces[i].m_plane[3];
			if(d>0.0f)
				return false;
		}
	}
	return true;
}
#endif

//

void	btConvexPolyhedron::initialize()
{

	btHashMap<btInternalVertexPair,btInternalEdge,MAX_CAP> edges;

	float TotalArea = 0.0f;
	
	VMSET(m_localCenter,0, 0, 0);
	for(int i=0;i<m_faces.size();i++)
	{
		int numVertices = m_faces[i].m_indices.size();
		int NbTris = numVertices;
		for(int j=0;j<NbTris;j++)
		{
			int k = (j+1)%numVertices;
			btInternalVertexPair vp(m_faces[i].m_indices[j],m_faces[i].m_indices[k]);
			btInternalEdge* edptr = edges.find(vp);
			btVector3 edge = m_vertices[vp.m_v1]-m_vertices[vp.m_v0];
			VMNORMALIZE(edge);

			bool found = false;

			for (int p=0;p<m_uniqueEdges.size();p++)
			{
				
				if (IsAlmostZero(m_uniqueEdges[p]-edge) || 
					IsAlmostZero(m_uniqueEdges[p]+edge))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				m_uniqueEdges.push_back(edge);
			}

			if (edptr)
			{
				VMASSERT(edptr->m_face0>=0);
				VMASSERT(edptr->m_face1<0);
				edptr->m_face1 = i;
			} else
			{
				btInternalEdge ed;
				ed.m_face0 = i;
				edges.insert(vp,ed);
			}
		}
	}

	for(int i=0;i<m_faces.size();i++)
	{
		int numVertices = m_faces[i].m_indices.size();
		m_faces[i].m_connectedFaces.resize(numVertices);

		for(int j=0;j<numVertices;j++)
		{
			int k = (j+1)%numVertices;
			btInternalVertexPair vp(m_faces[i].m_indices[j],m_faces[i].m_indices[k]);
			btInternalEdge* edptr = edges.find(vp);
			VMASSERT(edptr);
			VMASSERT(edptr->m_face0>=0);
			VMASSERT(edptr->m_face1>=0);

			int connectedFace = (edptr->m_face0==i)?edptr->m_face1:edptr->m_face0;
			m_faces[i].m_connectedFaces[j] = connectedFace;
		}
	}

	for(int i=0;i<m_faces.size();i++)
	{
		int numVertices = m_faces[i].m_indices.size();
		int NbTris = numVertices-2;
		
		const btVector3& p0 = m_vertices[m_faces[i].m_indices[0]];
		for(int j=1;j<=NbTris;j++)
		{
			int k = (j+1)%numVertices;
			const btVector3& p1 = m_vertices[m_faces[i].m_indices[j]];
			const btVector3& p2 = m_vertices[m_faces[i].m_indices[k]];
			float Area = (VMLENGTH(VMCROSS((p0 - p1),(p0 - p2)))) * 0.5f;
			btVector3 Center = (p0+p1+p2)/3.0f;
			m_localCenter += Area * Center;
			TotalArea += Area;
		}
	}
	m_localCenter /= TotalArea;

#ifdef TEST_INTERNAL_OBJECTS
	if(1)
	{
		m_radius = FLT_MAX;
		for(int i=0;i<m_faces.size();i++)
		{
			const btVector3 Normal(m_faces[i].m_plane[0], m_faces[i].m_plane[1], m_faces[i].m_plane[2]);
			const btScalar dist = fabsf(VMDOT(m_localCenter,Normal) + m_faces[i].m_plane[3]);
			if(dist<m_radius)
				m_radius = dist;
		}

	
		btScalar MinX = FLT_MAX;
		btScalar MinY = FLT_MAX;
		btScalar MinZ = FLT_MAX;
		btScalar MaxX = -FLT_MAX;
		btScalar MaxY = -FLT_MAX;
		btScalar MaxZ = -FLT_MAX;
		for(int i=0; i<m_vertices.size(); i++)
		{
			const btVector3& pt = m_vertices[i];
			if(VMGETX(pt)<MinX)	
				MinX = VMGETX(pt);
			if(VMGETX(pt)>MaxX)	
				MaxX = VMGETX(pt);
			if(VMGETY(pt)<MinY)	
				MinY = VMGETY(pt);
			if(VMGETY(pt)>MaxY)	
				MaxY = VMGETY(pt);
			if(VMGETZ(pt)<MinZ)	
				MinZ = VMGETZ(pt);
			if(VMGETZ(pt)>MaxZ)	
				MaxZ = VMGETZ(pt);
		}
		VMSET(mC,MaxX+MinX, MaxY+MinY, MaxZ+MinZ);
		VMSET(mE,MaxX-MinX, MaxY-MinY, MaxZ-MinZ);



//		const btScalar r = m_radius / sqrtf(2.0f);
		const btScalar r = m_radius / sqrtf(3.0f);
		const int LargestExtent = VMMAXAXIS(mE);
		const btScalar Step = (mE[LargestExtent]*0.5f - r)/1024.0f;
		m_extents[0] = m_extents[1] = m_extents[2] = r;
		m_extents[LargestExtent] = mE[LargestExtent]*0.5f;
		bool FoundBox = false;
		for(int j=0;j<1024;j++)
		{
			if(testContainment())
			{
				FoundBox = true;
				break;
			}

			m_extents[LargestExtent] -= Step;
		}
		if(!FoundBox)
		{
			m_extents[0] = m_extents[1] = m_extents[2] = r;
		}
		else
		{
			// Refine the box
			const btScalar Step = (m_radius - r)/1024.0f;
			const int e0 = (1<<LargestExtent) & 3;
			const int e1 = (1<<e0) & 3;

			for(int j=0;j<1024;j++)
			{
				const btScalar Saved0 = m_extents[e0];
				const btScalar Saved1 = m_extents[e1];
				m_extents[e0] += Step;
				m_extents[e1] += Step;

				if(!testContainment())
				{
					m_extents[e0] = Saved0;
					m_extents[e1] = Saved1;
					break;
				}
			}
		}
	}
#endif


}


void btConvexPolyhedron::project(const btTransform& trans, const btVector3& dir, float& min, float& max) const
{
	min = VM_FLT_MAX;
	max = -VM_FLT_MAX;
	int numVerts = m_vertices.size();
	for(int i=0;i<numVerts;i++)
	{
		btVector3 pt = VMTRANS(trans,m_vertices[i]);
		float dp = VMDOT(pt,dir);
		if(dp < min)	min = dp;
		if(dp > max)	max = dp;
	}
	if(min>max)
	{
		float tmp = min;
		min = max;
		max = tmp;
	}
}

