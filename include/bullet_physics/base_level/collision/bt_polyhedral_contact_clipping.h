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

#ifndef BT_POLYHEDRAL_CONTACT_CLIPPING_H
#define BT_POLYHEDRAL_CONTACT_CLIPPING_H



#include "bullet_physics/bt_math_conversions.h"
class btConvexPolyhedron;



/// Clips a face to the back of a plane
template <typename BT_CONTACT_CACHE, typename BT_VERTEX_ARRAY>
struct btPolyhedralContactClipping
{
	static void clipHullAgainstHull(const btVector3& separatingNormal, const btConvexPolyhedron& hullA, const btConvexPolyhedron& hullB, const btTransform& transA,const btTransform& transB, const btScalar minDist, btScalar maxDist, BT_CONTACT_CACHE& resultOut);
	static void	clipFaceAgainstHull(const btVector3& separatingNormal, const btConvexPolyhedron& hullA,  const btTransform& transA, BT_VERTEX_ARRAY& worldVertsB1, const btTransform& transB, const btScalar minDist, btScalar maxDist, BT_CONTACT_CACHE& resultOut);

	static bool findSeparatingAxis(	const btConvexPolyhedron& hullA, const btConvexPolyhedron& hullB, const btTransform& transA,const btTransform& transB, btVector3& sep);

	///the clipFace method is used internally
	static void clipFace(const BT_VERTEX_ARRAY& pVtxIn, BT_VERTEX_ARRAY& ppVtxOut, const btVector3& planeNormalWS,btScalar planeEqWS);

};

#include "bt_polyhedral_contact_clipping.inl"



#endif // BT_POLYHEDRAL_CONTACT_CLIPPING_H


