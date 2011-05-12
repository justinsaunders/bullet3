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

///Contributors:
///2011-may-1: This file was created by Erwin Coumans

#include <stdio.h>
#include "bullet_physics/base_level/collision/bt_polyhedral_contact_clipping.h"
#include "bullet_physics/util/bt_inplace_array.h"
#include "bullet_physics/base_level/collision/bt_convex_polyhedron.h"
#include "bullet_physics/util/bt_aligned_object_array.h"

struct TestContactCache
{
	
	inline void addContacts(const btVector3& planeNormalWS, btScalar planeEqWS, int numVertices, const btVector3* vertices)
	{
	}
};

#include <float.h> //for FLT_MAX
#include <assert.h>

//typedef PfxInplaceArray<btVector3,SCE_PFX_NUMMESHVERTICES> btVertexArray;
typedef btAlignedObjectArray<btVector3> btVertexArray;
typedef btPolyhedralContactClipping<TestContactCache,btVertexArray> MyPolyClipper;



int main(int argc, char* argv[])
{
	btConvexPolyhedron hullA;
	btConvexPolyhedron hullB;
	btTransform transA;
	btTransform transB;
	btVector3 sep;

	MyPolyClipper::findSeparatingAxis(hullA,hullB,transA,transB,sep);
	TestContactCache contacts;
	float minDist(0),maxDist(0);

	MyPolyClipper::clipHullAgainstHull(sep,hullA,hullB,transA,transB,minDist,maxDist,contacts);
	return 0;
}


