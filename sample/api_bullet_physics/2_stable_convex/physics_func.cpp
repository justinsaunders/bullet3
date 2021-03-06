/*
Physics Effects Copyright(C) 2010 Sony Computer Entertainment Inc.
All rights reserved.

Physics Effects is open software; you can redistribute it and/or
modify it under the terms of the BSD License.

Physics Effects is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the BSD License for more details.

A copy of the BSD License is distributed with
Physics Effects under the filename: physics_effects_license.txt
*/

#include "physics_func.h"
#include "../common/perf_func.h"

#include "../../../src/physics_effects/low_level/collision/pfx_detect_collision_func.h"
#include "../../../src/physics_effects/base_level/collision/pfx_gjk_solver.h"
#include "../../../src/physics_effects/base_level/collision/pfx_gjk_support_func.h"

#include "bullet_physics/util/bt_convex_hull_computer.h"
#include "bullet_physics/base_level/collision/bt_convex_polyhedron.h"
#include "bullet_physics/base_level/collision/bt_polyhedral_contact_clipping.h"
#include "bullet_physics/util/bt_aligned_object_array.h"
#include "bullet_physics/util/bt_inplace_array.h"

struct MyContactBridge
{
	sce::PhysicsEffects::PfxContactCache m_contactCache;
	btTransform	m_transA;
	btTransform	m_transB;
	btVector3	m_separatingNormal;
	btScalar	m_minDist;
	btScalar	m_maxDist;

	MyContactBridge(const btTransform& transA,const btTransform& transB,const btVector3& separatingNormal, btScalar minDist, btScalar maxDist)
		:m_transA(transA),m_transB(transB), m_separatingNormal(separatingNormal),m_minDist(minDist),m_maxDist(maxDist)
	{
	}
	inline void addContacts(const btVector3& planeNormalWS, btScalar planeEqWS, int numVertices, const btVector3* vertices)
	{

		PfxSubData subData;

		btVector3 point;
		// only keep points that are behind the witness face
		for (int i=0;i<numVertices;i++)
		{
			btScalar depth = VMDOT(planeNormalWS,vertices[i])+planeEqWS;
			if (depth <=m_maxDist && depth >=m_minDist)
			{
				btVector3 point = vertices[i];
				btVector3 pointA = point+m_separatingNormal*depth;
				btVector3 ptA(pointA);
				btVector3 ptB(point);
				{
					PfxPoint3 localA( VMTRANS(orthoInverse(m_transA), ptA));
					PfxPoint3 localB( VMTRANS(orthoInverse(m_transB), ptB));
					float dist = dot(m_separatingNormal,ptA-ptB);
					if (dist<0)
						m_contactCache.addContactPoint(depth,m_separatingNormal,localA,localB,subData);
				}
			}
		}
	}
};

typedef btPolyhedralContactClipping<MyContactBridge,btAlignedObjectArray<btVector3> > btDefaultPolyClipper;


///////////////////////////////////////////////////////////////////////////////
// Simulation Data

#define NUM_RIGIDBODIES 5000
#define NUM_JOINTS    5000
#define NUM_CONTACTS  4000

const float timeStep = 0.016f;
const float separateBias = 0.1f;
int iteration = 5;

//J ワールドサイズ
//E World size
PfxVector3 worldCenter(0.0f);
PfxVector3 worldExtent(500.0f);

//J 剛体
//E Rigid body
PfxRigidState states[NUM_RIGIDBODIES];
PfxRigidBody  bodies[NUM_RIGIDBODIES];
PfxCollidable collidables[NUM_RIGIDBODIES];
PfxSolverBody solverBodies[NUM_RIGIDBODIES];
int numRigidBodies = 0;

//J 地形を表現するためのラージメッシュ
//E Large mesh for representing a landscape
#include "landscape.h"
PfxLargeTriMesh gLargeMesh;

//J 凸メッシュ
//E Convex Mesh
#include "barrel.h"
PfxConvexMesh gConvex;

//J プロキシ
//E Proxies
PfxBroadphaseProxy proxies[NUM_RIGIDBODIES];

//J ジョイント
//E Joint
PfxConstraintPair jointPairs[NUM_JOINTS];
PfxJoint joints[NUM_JOINTS];
int numJoints = 0;

//J ペア
//E Pairs
unsigned int pairSwap;
unsigned int numPairs[2];
PfxBroadphasePair pairsBuff[2][NUM_CONTACTS];

//J コンタクト
//E Contacts
PfxContactManifold contacts[NUM_CONTACTS];
int numContacts;

PfxUInt32 contactIdPool[NUM_CONTACTS];
int numContactIdPool;

//J 一時バッファ
//E Temporary buffers
#define POOL_BYTES (5*1024*1024)
unsigned char SCE_PFX_ALIGNED(128) poolBuff[POOL_BYTES];

//J 一時バッファ用スタックアロケータ
//E Stack allocator for temporary buffers
PfxHeapManager pool(poolBuff,POOL_BYTES);

///////////////////////////////////////////////////////////////////////////////
// Simulation Function

int frame = 0;
int curNumPairs = 0;
int curTotalContacts = 0;

void broadphase()
{
	pairSwap = 1-pairSwap;

	unsigned int &numPreviousPairs = numPairs[1-pairSwap];
	unsigned int &numCurrentPairs = numPairs[pairSwap];
	PfxBroadphasePair *previousPairs = pairsBuff[1-pairSwap];
	PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

	//J 剛体が最も分散している軸を見つける
	//E Find the axis along which all rigid bodies are most widely positioned
	int axis = 0;
	{
		PfxVector3 s(0.0f),s2(0.0f);
		for(int i=0;i<numRigidBodies;i++) {
			PfxVector3 c = states[i].getPosition();
			s += c;
			s2 += mulPerElem(c,c);
		}
		PfxVector3 v = s2 - mulPerElem(s,s) / (float)numRigidBodies;
		if(v[1] > v[0]) axis = 1;
		if(v[2] > v[axis]) axis = 2;
	}

	//J ブロードフェーズプロキシの更新
	//E Create broadpahse proxies
	{
		for(int i=0;i<numRigidBodies;i++) {
			pfxUpdateBroadphaseProxy(proxies[i],states[i],collidables[i],worldCenter,worldExtent,axis);
		}

		int workBytes = sizeof(PfxBroadphaseProxy) * numRigidBodies;
		void *workBuff = pool.allocate(workBytes);
				
		pfxParallelSort(proxies,numRigidBodies,workBuff,workBytes);

		pool.deallocate(workBuff);
	}

	//J 交差ペア探索
	//E Find overlapped pairs
	{
		PfxFindPairsParam findPairsParam;
		findPairsParam.pairBytes = pfxGetPairBytesOfFindPairs(NUM_CONTACTS);
		findPairsParam.pairBuff = pool.allocate(findPairsParam.pairBytes);
		findPairsParam.workBytes = pfxGetWorkBytesOfFindPairs(NUM_CONTACTS);
		findPairsParam.workBuff = pool.allocate(findPairsParam.workBytes);
		findPairsParam.proxies = proxies;
		findPairsParam.numProxies = numRigidBodies;
		findPairsParam.maxPairs = NUM_CONTACTS;
		findPairsParam.axis = axis;

		PfxFindPairsResult findPairsResult;

		int ret = pfxFindPairs(findPairsParam,findPairsResult);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxFindPairs failed %d\n",ret);
		
		pool.deallocate(findPairsParam.workBuff);

		curNumPairs = findPairsResult.numPairs;

		//J 交差ペア合成
		//E Decompose overlapped pairs into 3 arrays
		PfxDecomposePairsParam decomposePairsParam;
		decomposePairsParam.pairBytes = pfxGetPairBytesOfDecomposePairs(numPreviousPairs,findPairsResult.numPairs);
		decomposePairsParam.pairBuff = pool.allocate(decomposePairsParam.pairBytes);
		decomposePairsParam.workBytes = pfxGetWorkBytesOfDecomposePairs(numPreviousPairs,findPairsResult.numPairs);
		decomposePairsParam.workBuff = pool.allocate(decomposePairsParam.workBytes);
		decomposePairsParam.previousPairs = previousPairs;
		decomposePairsParam.numPreviousPairs = numPreviousPairs;
		decomposePairsParam.currentPairs = findPairsResult.pairs; // Set pairs from pfxFindPairs()
		decomposePairsParam.numCurrentPairs = findPairsResult.numPairs; // Set the number of pairs from pfxFindPairs()

		PfxDecomposePairsResult decomposePairsResult;

		ret = pfxDecomposePairs(decomposePairsParam,decomposePairsResult);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDecomposePairs failed %d\n",ret);

		pool.deallocate(decomposePairsParam.workBuff);

		PfxBroadphasePair *outNewPairs = decomposePairsResult.outNewPairs;
		PfxBroadphasePair *outKeepPairs = decomposePairsResult.outKeepPairs;
		PfxBroadphasePair *outRemovePairs = decomposePairsResult.outRemovePairs;
		PfxUInt32 numOutNewPairs = decomposePairsResult.numOutNewPairs;
		PfxUInt32 numOutKeepPairs = decomposePairsResult.numOutKeepPairs;
		PfxUInt32 numOutRemovePairs = decomposePairsResult.numOutRemovePairs;

		//J 廃棄ペアのコンタクトをプールに戻す
		//E Put removed contacts into the contact pool
		for(PfxUInt32 i=0;i<numOutRemovePairs;i++) {
			contactIdPool[numContactIdPool++] = pfxGetContactId(outRemovePairs[i]);
		}

		//J 新規ペアのコンタクトのリンクと初期化
		//E Add new contacts and initialize
		for(PfxUInt32 i=0;i<numOutNewPairs;i++) {
			int cId = 0;
			if(numContactIdPool > 0) {
				cId = contactIdPool[--numContactIdPool];
			}
			else {
				cId = numContacts++;
			}
			if(cId >= NUM_CONTACTS) {
				cId = 0;
			}
			SCE_PFX_ASSERT(cId < NUM_CONTACTS);
			pfxSetContactId(outNewPairs[i],cId);
			PfxContactManifold &contact = contacts[cId];
			contact.reset(pfxGetObjectIdA(outNewPairs[i]),pfxGetObjectIdB(outNewPairs[i]));
		}

		//J 新規ペアと維持ペアを合成
		//E Merge 'new' and 'keep' pairs
		numCurrentPairs = 0;
		for(PfxUInt32 i=0;i<numOutKeepPairs;i++) {
			currentPairs[numCurrentPairs++] = outKeepPairs[i];
		}
		for(PfxUInt32 i=0;i<numOutNewPairs;i++) {
			currentPairs[numCurrentPairs++] = outNewPairs[i];
		}
		
		pool.deallocate(decomposePairsParam.pairBuff);
		pool.deallocate(findPairsParam.pairBuff);
	}
	
	{
		int workBytes = sizeof(PfxBroadphasePair) * numCurrentPairs;
		void *workBuff = pool.allocate(workBytes);
		
		pfxParallelSort(currentPairs,numCurrentPairs,workBuff,workBytes);
		
		pool.deallocate(workBuff);
	}
}

void collision()
{
	unsigned int numCurrentPairs = numPairs[pairSwap];
	PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];
	
	//J 衝突検出
	//E Detect collisions
	{
		PfxDetectCollisionParam param;
		param.contactPairs = currentPairs;
		param.numContactPairs = numCurrentPairs;
		param.offsetContactManifolds = contacts;
		param.offsetRigidStates = states;
		param.offsetCollidables = collidables;
		param.numRigidBodies = numRigidBodies;

		int ret = pfxDetectCollision(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDetectCollision failed %d\n",ret);
	}

	curTotalContacts = 0;

	for(PfxUInt32 i=0;i<numCurrentPairs;i++) {
		PfxConstraintPair &pair = currentPairs[i];
	
		PfxUInt16 iA = pfxGetObjectIdA(pair);
		PfxUInt16 iB = pfxGetObjectIdB(pair);
		PfxUInt32 iConstraint = pfxGetConstraintId(pair);

		PfxContactManifold &contact = contacts[iConstraint];
		curTotalContacts += contact.getNumContacts();
	}


	//J リフレッシュ
	//E Refresh contacts
	{
		PfxRefreshContactsParam param;
		param.contactPairs = currentPairs;
		param.numContactPairs = numCurrentPairs;
		param.offsetContactManifolds = contacts;
		param.offsetRigidStates = states;
		param.numRigidBodies = numRigidBodies;

		int ret = pfxRefreshContacts(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxRefreshContacts failed %d\n",ret);
	}


	

}

void constraintSolver()
{
	PfxPerfCounter pc;

	unsigned int numCurrentPairs = numPairs[pairSwap];
	PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

	pc.countBegin("setup solver bodies");
	{
		PfxSetupSolverBodiesParam param;
		param.states = states;
		param.bodies = bodies;
		param.solverBodies = solverBodies;
		param.numRigidBodies = numRigidBodies;
		
		int ret = pfxSetupSolverBodies(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupSolverBodies failed %d\n",ret);
	}
	pc.countEnd();

	pc.countBegin("setup contact constraints");
	{
		PfxSetupContactConstraintsParam param;
		param.contactPairs = currentPairs;
		param.numContactPairs = numCurrentPairs;
		param.offsetContactManifolds = contacts;
		param.offsetRigidStates = states;
		param.offsetRigidBodies = bodies;
		param.offsetSolverBodies = solverBodies;
		param.numRigidBodies = numRigidBodies;
		param.timeStep = timeStep;
		param.separateBias = separateBias;
		
		int ret = pfxSetupContactConstraints(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupJointConstraints failed %d\n",ret);
	}
	pc.countEnd();

	pc.countBegin("setup joint constraints");
	{
		PfxSetupJointConstraintsParam param;
		param.jointPairs = jointPairs;
		param.numJointPairs = numJoints;
		param.offsetJoints = joints;
		param.offsetRigidStates = states;
		param.offsetRigidBodies = bodies;
		param.offsetSolverBodies = solverBodies;
		param.numRigidBodies = numRigidBodies;
		param.timeStep = timeStep;

		for(int i=0;i<numJoints;i++) {
			pfxUpdateJointPairs(jointPairs[i],i,joints[i],states[joints[i].m_rigidBodyIdA],states[joints[i].m_rigidBodyIdB]);
		}

		int ret = pfxSetupJointConstraints(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupJointConstraints failed %d\n",ret);
	}
	pc.countEnd();

	pc.countBegin("solve constraints");
	{
		PfxSolveConstraintsParam param;
		param.workBytes = pfxGetWorkBytesOfSolveConstraints(numRigidBodies,numCurrentPairs,numJoints);
		param.workBuff = pool.allocate(param.workBytes);
		param.contactPairs = currentPairs;
		param.numContactPairs = numCurrentPairs;
		param.offsetContactManifolds = contacts;
		param.jointPairs = jointPairs;
		param.numJointPairs = numJoints;
		param.offsetJoints = joints;
		param.offsetRigidStates = states;
		param.offsetSolverBodies = solverBodies;
		param.numRigidBodies = numRigidBodies;
		param.iteration = iteration;

		int ret = pfxSolveConstraints(param);
		if(ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSolveConstraints failed %d\n",ret);
		
		pool.deallocate(param.workBuff);
	}
	pc.countEnd();

	//pc.printCount();
}

void integrate()
{
	PfxUpdateRigidStatesParam param;
	param.states = states;
	param.bodies = bodies;
	param.numRigidBodies = numRigidBodies;
	param.timeStep = timeStep;
	
	pfxUpdateRigidStates(param);
}

void physics_simulate()
{
	PfxPerfCounter pc;

	for(int i=1;i<numRigidBodies;i++) {
		pfxApplyExternalForce(states[i],bodies[i],bodies[i].getMass()*PfxVector3(0.0f,-9.8f,0.0f),PfxVector3(0.0f),timeStep);
	}
	
	perf_push_marker("broadphase");
	pc.countBegin("broadphase");
	broadphase();
	pc.countEnd();
	perf_pop_marker();
	
	perf_push_marker("collision");
	pc.countBegin("collision");
	collision();
	pc.countEnd();
	perf_pop_marker();
	
	perf_push_marker("solver");
	pc.countBegin("solver");
	constraintSolver();
	pc.countEnd();
	perf_pop_marker();
	
	perf_push_marker("integrate");
	pc.countBegin("integrate");
	integrate();
	pc.countEnd();
	perf_pop_marker();
	
	frame++;
	
	//if(frame%100 == 0) 
	{
		float broadphaseTime = pc.getCountTime(0);
		float collisionTime  = pc.getCountTime(2);
		float solverTime     = pc.getCountTime(4);
		float integrateTime  = pc.getCountTime(6);
		SCE_PFX_PRINTF("#pairs = %d, #contacts = %d\n", curNumPairs, curTotalContacts);

		SCE_PFX_PRINTF("frame %3d broadphase %.2f collision %.2f solver %.2f integrate %.2f | total %.2f\n",frame,
			broadphaseTime,collisionTime,solverTime,integrateTime,
			broadphaseTime+collisionTime+solverTime+integrateTime);


	}
}

btConvexPolyhedron* initializePolyhedralFeatures(PfxInplaceArray<btVector3,MAXSIZE>& tmpVertices)
{
	
	btConvexPolyhedron* polyhedron = new btConvexPolyhedron();

	
	btConvexHullComputer conv;
	float* ptr = (float*) &tmpVertices[0];

	conv.compute(ptr, sizeof(btVector3),tmpVertices.size(),0.f,0.f);

	

	PfxInplaceArray<btVector3,MAXSIZE> faceNormals;
	int numFaces = conv.faces.size();
	faceNormals.resize(numFaces);
	btConvexHullComputer* convexUtil = &conv;

	
	
	polyhedron->m_faces.resize(numFaces);
	int numVertices = convexUtil->vertices.size();
	polyhedron->m_vertices.resize(numVertices);
	for (int p=0;p<numVertices;p++)
	{
		polyhedron->m_vertices[p] = convexUtil->vertices[p];
	}

	for (int i=0;i<numFaces;i++)
	{
		int face = convexUtil->faces[i];
		//printf("face=%d\n",face);
		const btConvexHullComputer::Edge*  firstEdge = &convexUtil->edges[face];
		const btConvexHullComputer::Edge*  edge = firstEdge;

		btVector3 edges[3];
		int numEdges = 0;
		//compute face normals

		btScalar maxCross2 = 0.f;
		int chosenEdge = -1;

		do
		{
			
			int src = edge->getSourceVertex();
			polyhedron->m_faces[i].m_indices.push_back(src);
			int targ = edge->getTargetVertex();
			btVector3 wa = convexUtil->vertices[src];

			btVector3 wb = convexUtil->vertices[targ];
			btVector3 newEdge = wb-wa;
			VMNORMALIZE(newEdge);
			if (numEdges<2)
				edges[numEdges++] = newEdge;

			edge = edge->getNextEdgeOfFace();
		} while (edge!=firstEdge);

		btScalar planeEq = 1e30f;

		
		if (numEdges==2)
		{
			faceNormals[i] = VMCROSS(edges[0],edges[1]);
			VMNORMALIZE(faceNormals[i]);
			polyhedron->m_faces[i].m_plane[0] = faceNormals[i].getX();
			polyhedron->m_faces[i].m_plane[1] = faceNormals[i].getY();
			polyhedron->m_faces[i].m_plane[2] = faceNormals[i].getZ();
			polyhedron->m_faces[i].m_plane[3] = planeEq;

		}
		else
		{
			VMASSERT(0);//degenerate?
			VMSET(faceNormals[i],0.f,0.f,0.f);
		}

		for (int v=0;v<polyhedron->m_faces[i].m_indices.size();v++)
		{
			btScalar eq = VMDOT(polyhedron->m_vertices[polyhedron->m_faces[i].m_indices[v]],faceNormals[i]);
			if (planeEq>eq)
			{
				planeEq=eq;
			}
		}
		polyhedron->m_faces[i].m_plane[3] = -planeEq;
	}


	polyhedron->initialize();

	return polyhedron;
}

void getBoxVertices(PfxVector3& halfExtents, PfxInplaceArray<btVector3,MAXSIZE>& verticesOut)
{
	verticesOut.push_back(PfxVector3(halfExtents.getX(),halfExtents.getY(),halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(halfExtents.getX(),halfExtents.getY(),-halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(halfExtents.getX(),-halfExtents.getY(),halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(halfExtents.getX(),-halfExtents.getY(),-halfExtents.getZ()));

	verticesOut.push_back(PfxVector3(-halfExtents.getX(),halfExtents.getY(),halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(-halfExtents.getX(),halfExtents.getY(),-halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(-halfExtents.getX(),-halfExtents.getY(),halfExtents.getZ()));
	verticesOut.push_back(PfxVector3(-halfExtents.getX(),-halfExtents.getY(),-halfExtents.getZ()));
}

///////////////////////////////////////////////////////////////////////////////
// Create Scene

void createBrick(int id,const PfxVector3 &pos,const PfxQuat &rot,const PfxVector3 &boxSize,PfxFloat mass)
{
	PfxBox box(boxSize);
	PfxShape shape;
	shape.reset();
	shape.setBox(box);
	PfxInplaceArray<btVector3,MAXSIZE> vertices;
	getBoxVertices(box.m_half,vertices);
	shape.m_convexPolyhedron = initializePolyhedralFeatures(vertices);

	collidables[id].reset();
	collidables[id].addShape(shape);
	collidables[id].finish();
	bodies[id].reset();
	bodies[id].setRestitution(0.0f);
	bodies[id].setMass(mass);
	bodies[id].setInertia(pfxCalcInertiaBox(boxSize,mass));
	states[id].reset();
	states[id].setPosition(pos);
	states[id].setOrientation(rot);
	states[id].setMotionType(kPfxMotionTypeActive);
	states[id].setRigidBodyId(id);
}

void createBrickFromShape(int id,
						  const PfxVector3 &pos,
						  const PfxQuat &rot,
						  const PfxVector3 &boxSize,
						  PfxFloat mass,
						  PfxShape& shape)
{
	collidables[id].reset();
	collidables[id].addShape(shape);
	collidables[id].finish();
	bodies[id].reset();
	bodies[id].setRestitution(0.0f);
	bodies[id].setMass(mass);
	bodies[id].setInertia(pfxCalcInertiaBox(boxSize,mass));
	states[id].reset();
	states[id].setPosition(pos);
	states[id].setOrientation(rot);
	states[id].setMotionType(kPfxMotionTypeActive);
	states[id].setRigidBodyId(id);
}


void createWall(const PfxVector3 &offsetPosition,int stackSize,const PfxVector3 &boxSize)
{
	PfxFloat bodyMass = 0.5f;

	PfxFloat diffX = boxSize[0] * 1.02f;
	PfxFloat diffY = boxSize[1] * 1.02f;
	PfxFloat diffZ = boxSize[2] * 1.02f;

	PfxFloat offset = -stackSize * (diffZ * 2.0f) * 0.5f;
	PfxVector3 pos(0.0f, diffY, 0.0f);

	while(stackSize) {
		for(int i=0;i<stackSize;i++) {
			pos[2] = offset + (PfxFloat)i * (diffZ * 2.0f);
		
			createBrick(numRigidBodies++,offsetPosition+pos,PfxQuat::identity(),boxSize,bodyMass);
		}
		offset += diffZ;
		pos[1] += (diffY * 2.0f);
		stackSize--;
	}
}

void createPyramid(const PfxVector3 &offsetPosition,int stackSize,const PfxVector3 &boxSize)
{
	PfxFloat space = 0.0001f;
	PfxVector3 pos(0.0f, boxSize[1], 0.0f);

	PfxFloat diffX = boxSize[0] * 1.02f;
	PfxFloat diffY = boxSize[1] * 1.02f;
	PfxFloat diffZ = boxSize[2] * 1.02f;

	PfxFloat offsetX = -stackSize * (diffX * 2.0f + space) * 0.5f;
	PfxFloat offsetZ = -stackSize * (diffZ * 2.0f + space) * 0.5f;
	while(stackSize) {
		for(int j=0;j<stackSize;j++) {
			pos[2] = offsetZ + (PfxFloat)j * (diffZ * 2.0f + space);
			for(int i=0;i<stackSize;i++) {
				pos[0] = offsetX + (PfxFloat)i * (diffX * 2.0f + space);
				createBrick(numRigidBodies++,offsetPosition+pos,PfxQuat::identity(),boxSize,1.0f);
			}
		}
		offsetX += diffX;
		offsetZ += diffZ;
		pos[1] += (diffY * 2.0f + space);
		stackSize--;
	}
}





PfxShape createConvexShape()
{
	PfxCreateConvexMeshParam param;

	param.verts = BarrelVtx;
	param.numVerts = BarrelVtxCount;
	param.vertexStrideBytes = sizeof(float)*6;

	param.triangles = BarrelIdx;
	param.numTriangles = BarrelIdxCount/3;
	param.triangleStrideBytes = sizeof(unsigned short)*3;

	PfxInt32 ret = pfxCreateConvexMesh(gConvex,param,2);
	if(ret != SCE_PFX_OK) {
		SCE_PFX_PRINTF("Can't create gConvex mesh.\n");
	}
	

	PfxShape shape;
	shape.reset();
	shape.setConvexMesh(&gConvex);

	PfxInplaceArray<btVector3,MAXSIZE> vertices;

	for (int i=0;i<gConvex.m_numVerts;i++)
	{
		vertices.push_back(gConvex.m_verts[i]);
	}
	
	shape.m_convexPolyhedron = initializePolyhedralFeatures(vertices);
	return shape;
}

void createTowerCircle(const PfxVector3 &offsetPosition,int stackSize,int rotSize,const PfxVector3 &boxSize)
{
	PfxFloat radius = 1.f * rotSize * boxSize[0] / SCE_PFX_PI;

	PfxShape shape;

	shape = createConvexShape();
	// create active boxes
	PfxQuat rotY = PfxQuat::identity();
	PfxFloat posY = boxSize[1];

	for(int i=0;i<stackSize;i++) {
		for(int j=0;j<rotSize;j++) {
			createBrickFromShape(numRigidBodies++,offsetPosition+rotate(rotY,PfxVector3(0.0f , posY, radius)),rotY,boxSize,0.5f,shape);

			rotY *= PfxQuat::rotationY(SCE_PFX_PI/(rotSize*0.5f));
		}

		posY += boxSize[1] * 2.0f;
		rotY *= PfxQuat::rotationY(SCE_PFX_PI/(PfxFloat)rotSize);
	}
}

void createScenePrimitives()
{
	// sphere
	{
		int id = numRigidBodies++;
		PfxSphere sphere(1.0f);
		PfxShape shape;
		shape.reset();
		shape.setSphere(sphere);
		collidables[id].reset();
		collidables[id].addShape(shape);
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(1.0f);
		bodies[id].setInertia(pfxCalcInertiaSphere(1.0f,1.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(-5.0f,5.0f,0.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}

	// box
	{
		int id = numRigidBodies++;
		PfxBox box(1.0f,1.0f,1.0f);
		PfxShape shape;
		shape.reset();
		shape.setBox(box);
		PfxInplaceArray<btVector3,MAXSIZE> vertices;
		getBoxVertices(box.m_half,vertices);
		shape.m_convexPolyhedron = initializePolyhedralFeatures(vertices);

		collidables[id].reset();
		collidables[id].addShape(shape);
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(1.0f);
		bodies[id].setInertia(pfxCalcInertiaBox(PfxVector3(1.0f),1.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(0.0f,5.0f,5.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}

	// capsule
	{
		int id = numRigidBodies++;
		PfxCapsule capsule(1.5f,0.5f);
		PfxShape shape;
		shape.reset();
		shape.setCapsule(capsule);
		collidables[id].reset();
		collidables[id].addShape(shape);
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(2.0f);
		bodies[id].setInertia(pfxCalcInertiaCylinderX(2.0f,0.5f,2.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(5.0f,5.0f,0.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}

	// cylinder
	{
		int id = numRigidBodies++;
		PfxCylinder cylinder(0.5f,1.5f);
		PfxShape shape;
		shape.reset();
		shape.setCylinder(cylinder);
		collidables[id].reset();
		collidables[id].addShape(shape);
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(3.0f);
		bodies[id].setInertia(pfxCalcInertiaCylinderX(0.5f,1.5f,3.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(0.0f,10.0f,0.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}

	// convex mesh
	{
		PfxCreateConvexMeshParam param;

		param.verts = BarrelVtx;
		param.numVerts = BarrelVtxCount;
		param.vertexStrideBytes = sizeof(float)*6;

		param.triangles = BarrelIdx;
		param.numTriangles = BarrelIdxCount/3;
		param.triangleStrideBytes = sizeof(unsigned short)*3;

		PfxInt32 ret = pfxCreateConvexMesh(gConvex,param);
		if(ret != SCE_PFX_OK) {
			SCE_PFX_PRINTF("Can't create gConvex mesh.\n");
		}

		int id = numRigidBodies++;
		PfxShape shape;
		shape.reset();
		shape.setConvexMesh(&gConvex);
		collidables[id].reset();
		collidables[id].addShape(shape);
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(3.0f);
		bodies[id].setInertia(pfxCalcInertiaSphere(1.0f,1.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(0.0f,15.0f,0.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}

	// combined primitives
	{
		int id = numRigidBodies++;

		//E Both shapes and incides buffer have to be kept when creating a combined shape.
		static PfxShape shapes[3];
		PfxUInt16 shapeIds[3]={0,1,2};
		collidables[id].reset(shapes,shapeIds,3);
		{
			PfxBox box(0.5f,0.5f,1.5f);
			PfxShape shape;
			shape.reset();
			shape.setBox(box);
			shape.setOffsetPosition(PfxVector3(-2.0f,0.0f,0.0f));
			collidables[id].addShape(shape);
		}
		{
			PfxBox box(0.5f,1.5f,0.5f);
			PfxShape shape;
			shape.reset();
			shape.setBox(box);
			shape.setOffsetPosition(PfxVector3(2.0f,0.0f,0.0f));
			collidables[id].addShape(shape);
		}
		{
			PfxCapsule cap(1.5f,0.5f);
			PfxShape shape;
			shape.reset();
			shape.setCapsule(cap);
			collidables[id].addShape(shape);
		}
		collidables[id].finish();
		bodies[id].reset();
		bodies[id].setMass(3.0f);
		bodies[id].setInertia(pfxCalcInertiaBox(PfxVector3(2.5f,1.0f,1.0f),3.0f));
		states[id].reset();
		states[id].setPosition(PfxVector3(0.0f,5.0f,0.0f));
		states[id].setMotionType(kPfxMotionTypeActive);
		states[id].setRigidBodyId(id);
	}
}

void createSceneJoints()
{
	const int n = 10;

	int startId = numRigidBodies;

	PfxVector3 boxSize(1.0f);
	PfxFloat boxMass = 1.0f;

	for(int i=0;i<n;i++) {
		createBrick(numRigidBodies++,PfxVector3(0,3.0f+i*2.5f*boxSize[1],0),PfxQuat::identity(),boxSize,boxMass);
	}

	for(int i=startId;i<startId+n;i++) {
		PfxRigidState &stateA = states[i];
		PfxRigidState &stateB = states[(i+1)%numRigidBodies];
		PfxVector3 anchor;
		if(i == numRigidBodies-1) {
			anchor = stateA.getPosition() + PfxVector3(0,boxSize[1],0);
		}
		else {
			anchor = ( stateA.getPosition() + stateB.getPosition() ) * 0.5f;
		}

		PfxSwingTwistJointInitParam jparam;
		jparam.anchorPoint = anchor;
		jparam.twistAxis = PfxVector3(0,1,0);

		pfxInitializeSwingTwistJoint(joints[numJoints],stateA,stateB,jparam);
		joints[numJoints].m_constraints[4].m_damping = 0.1f;
		joints[numJoints].m_constraints[5].m_damping = 0.1f;

		pfxUpdateJointPairs(jointPairs[numJoints],numJoints,joints[numJoints],stateA,stateB);

		SCE_PFX_ASSERT(numJoints<NUM_JOINTS);
		numJoints++;
	}

	states[startId].setLinearVelocity(PfxVector3(0,0,5));
	states[startId].setLinearDamping(0.95f);
	states[startId].setAngularDamping(0.95f);
}

void createSceneStacking()
{
       const float cubeSize = 1.0f;
/*
       createPyramid(PfxVector3(-20.0f,0.0f,0.0f),12,PfxVector3(cubeSize,cubeSize,cubeSize));
       createWall(PfxVector3(-2.0f,0.0f,0.0f),12,PfxVector3(cubeSize,cubeSize,cubeSize));
       createWall(PfxVector3(4.0f,0.0f,0.0f),12,PfxVector3(cubeSize,cubeSize,cubeSize));
       createWall(PfxVector3(10.0f,0.0f,0.0f),12,PfxVector3(cubeSize,cubeSize,cubeSize));
       createTowerCircle(PfxVector3(25.0f,0.0f,0.0f),8,24,PfxVector3(cubeSize,cubeSize,cubeSize));
*/
	createTowerCircle(PfxVector3(0.0f,0.0f,0.0f),8,24,PfxVector3(1));
}

void createSceneBoxGround()
{
	int id = numRigidBodies++;
	PfxBox box(150.0f,2.5f,150.0f);
	PfxShape shape;
	shape.reset();
	shape.setBox(box);
	PfxInplaceArray<btVector3,MAXSIZE> vertices;
	getBoxVertices(box.m_half,vertices);
	shape.m_convexPolyhedron = initializePolyhedralFeatures(vertices);

	collidables[id].reset();
	collidables[id].addShape(shape);
	collidables[id].finish();
	bodies[id].reset();
	states[id].reset();
	states[id].setPosition(PfxVector3(0.0f,-2.5f,0.0f));
	states[id].setMotionType(kPfxMotionTypeFixed);
	states[id].setRigidBodyId(id);
}

void createSceneLandscape()
{
	PfxCreateLargeTriMeshParam param;

	param.verts = LargeMeshVtx;
	param.numVerts = LargeMeshVtxCount;
	param.vertexStrideBytes = sizeof(float)*6;

	param.triangles = LargeMeshIdx;
	param.numTriangles = LargeMeshIdxCount/3;
	param.triangleStrideBytes = sizeof(unsigned short)*3;

	if(gLargeMesh.m_numIslands > 0) {
		pfxReleaseLargeTriMesh(gLargeMesh);
	}

	PfxInt32 ret = pfxCreateLargeTriMesh(gLargeMesh,param);
	if(ret != SCE_PFX_OK) {
		SCE_PFX_PRINTF("Can't create large mesh.\n");
	}

	int id = numRigidBodies++;
	PfxShape shape;
	shape.reset();
	shape.setLargeTriMesh(&gLargeMesh);
	collidables[id].reset();
	collidables[id].addShape(shape);
	collidables[id].finish();
	bodies[id].reset();
	states[id].reset();
	states[id].setPosition(PfxVector3(0.0f,-5.0f,0.0f));
	states[id].setOrientation(PfxQuat::rotationX(0.5f)*PfxQuat::rotationY(0.7f));
	states[id].setMotionType(kPfxMotionTypeFixed);
	states[id].setRigidBodyId(id);
}

void physics_create_scene(int sceneId)
{
	const int numScenes = 4;
	int sid = sceneId % numScenes;
	
	numRigidBodies= 0;
	pairSwap = 0;
	numPairs[0] = 0;
	numPairs[1] = 0;
	numContacts = 0;
	numContactIdPool = 0;
	numJoints = 0;
	frame = 0;
	
	switch(sid) {
		case 0: // simple primitives
		createSceneBoxGround();
		createScenePrimitives();
		break;
		
		case 1: // joints
		createSceneBoxGround();
		createSceneJoints();
		break;

		case 2: // stacking
		createSceneBoxGround();
		createSceneStacking();
		break;

		case 3: // landscape
		createSceneLandscape();
		createScenePrimitives();
		break;
	}

	SCE_PFX_PRINTF("----- Size of rigid body buffer ------\n");
	SCE_PFX_PRINTF("                    size *   num = total\n");
	SCE_PFX_PRINTF("PfxRigidState      %5d * %5d = %5d bytes\n",sizeof(PfxRigidState),numRigidBodies,sizeof(PfxRigidState)*numRigidBodies);
	SCE_PFX_PRINTF("PfxRigidBody       %5d * %5d = %5d bytes\n",sizeof(PfxRigidBody),numRigidBodies,sizeof(PfxRigidBody)*numRigidBodies);
	SCE_PFX_PRINTF("PfxCollidable      %5d * %5d = %5d bytes\n",sizeof(PfxCollidable),numRigidBodies,sizeof(PfxCollidable)*numRigidBodies);
	SCE_PFX_PRINTF("PfxJoint           %5d * %5d = %5d bytes\n",sizeof(PfxJoint),numJoints,sizeof(PfxJoint)*numJoints);
	SCE_PFX_PRINTF("PfxSolverBody      %5d * %5d = %5d bytes\n",sizeof(PfxSolverBody),numRigidBodies,sizeof(PfxSolverBody)*numRigidBodies);
	SCE_PFX_PRINTF("PfxBroadphaseProxy %5d * %5d = %5d bytes\n",sizeof(PfxBroadphaseProxy),numRigidBodies,sizeof(PfxBroadphaseProxy)*numRigidBodies);
	SCE_PFX_PRINTF("PfxContactManifold %5d * %5d = %5d bytes\n",sizeof(PfxContactManifold),NUM_CONTACTS,sizeof(PfxContactManifold)*NUM_CONTACTS);
	SCE_PFX_PRINTF("PfxBroadphasePair  %5d * %5d = %5d bytes\n",sizeof(PfxBroadphasePair),NUM_CONTACTS,sizeof(PfxBroadphasePair)*NUM_CONTACTS);

	int totalBytes = 
		(sizeof(PfxRigidState) + sizeof(PfxRigidBody) + sizeof(PfxCollidable) + sizeof(PfxSolverBody) + sizeof(PfxBroadphaseProxy)) * numRigidBodies +
		(sizeof(PfxContactManifold) + sizeof(PfxBroadphasePair)) * NUM_CONTACTS;
	SCE_PFX_PRINTF("----------------------------------------------------------\n");
	SCE_PFX_PRINTF("Total %5d bytes\n",totalBytes);
}

///////////////////////////////////////////////////////////////////////////////
// Initialize / Finalize Engine

pfx_detect_collision_func orgFunc;


#include "../../../src/bullet_physics/util/btQuickProf.h"

void myCustomCollisionDummy(
				PfxContactCache &contacts,
				const PfxShape &shapeA,const PfxTransform3 &offsetTransformA,const PfxTransform3 &worldTransformA,int shapeIdA,
				const PfxShape &shapeB,const PfxTransform3 &offsetTransformB,const PfxTransform3 &worldTransformB,int shapeIdB,
				float contactThreshold)
{
	BT_PROFILE("myCustomCollisionDummy");

	(void)contacts;
	(void)shapeA,(void)offsetTransformA,(void)worldTransformA,(void)shapeIdA;
	(void)shapeB,(void)offsetTransformB,(void)worldTransformB,(void)shapeIdB;
	(void)contactThreshold;

	bool foundSep  = false;
	btVector3 sep;
	PfxContactCache copyContacts;

	if((shapeA.getType() == sce::PhysicsEffects::kPfxShapeBox) && (shapeB.getType() == sce::PhysicsEffects::kPfxShapeBox))
	{
		PfxBox boxA = shapeA.getBox();
		PfxBox boxB = shapeB.getBox();
		PfxFloat d = SCE_PFX_FLT_MAX;
		PfxVector3 nml;
		PfxPoint3 pA,pB;
		sce::PhysicsEffects::PfxGjkSolver gjk;

		gjk.setup((void*)&boxA,(void*)&boxB,pfxGetSupportVertexBox,pfxGetSupportVertexBox);
		d = gjk.collide(nml,pA,pB,worldTransformA,worldTransformB,SCE_PFX_FLT_MAX);
		if(d <= 0.f) {
			copyContacts.addContactPoint(d,nml,offsetTransformA*pA,offsetTransformB*pB,PfxSubData());
			foundSep  = true;
			sep = nml;
		}

	} 
	/*else
	{
		orgFunc(copyContacts,shapeA,offsetTransformA,worldTransformA,shapeIdA,shapeB,offsetTransformB,worldTransformB,shapeIdB,contactThreshold);
		if (copyContacts.getNumContacts())
		{
		
		}
	}
	*/

	if((shapeA.getType() == sce::PhysicsEffects::kPfxShapeConvexMesh) && (shapeB.getType() == sce::PhysicsEffects::kPfxShapeConvexMesh))
	{
		BT_PROFILE("gjk getClosestPoints");

		const PfxConvexMesh* boxA = shapeA.getConvexMesh();
		const PfxConvexMesh* boxB = shapeB.getConvexMesh();
		PfxFloat d = SCE_PFX_FLT_MAX;
		PfxVector3 nml;
		PfxPoint3 pA,pB;
		sce::PhysicsEffects::PfxGjkSolver gjk;

		gjk.setup((void*)boxA,(void*)boxB,pfxGetSupportVertexConvex,pfxGetSupportVertexConvex);
		d = gjk.collide(nml,pA,pB,worldTransformA,worldTransformB,SCE_PFX_FLT_MAX);
		if(d <= 0.f) {
			copyContacts.addContactPoint(d,nml,offsetTransformA*pA,offsetTransformB*pB,PfxSubData());
			foundSep  = true;
			sep = nml;
		}

	} 




	if (shapeA.m_convexPolyhedron && shapeB.m_convexPolyhedron)
	{
		
		btConvexPolyhedron* polA = (btConvexPolyhedron*)shapeA.m_convexPolyhedron;
		btConvexPolyhedron* polB = (btConvexPolyhedron*)shapeB.m_convexPolyhedron;

		if (!foundSep)
		{
			BT_PROFILE("findSeparatingAxis");
			foundSep = btDefaultPolyClipper::findSeparatingAxis(*polA, *polB, worldTransformA,worldTransformB, sep);
		}
		if (foundSep)
		{
			BT_PROFILE("clipHullAgainstHull");

//			SCE_PFX_PRINTF("found sep!\n");

			float minDist = -1e30f;
			float maxDist = 0.01f;
			
			MyContactBridge contactBridge(worldTransformA,worldTransformB,sep, minDist,maxDist);

			btDefaultPolyClipper::clipHullAgainstHull(sep,*polA,*polB,worldTransformA,worldTransformB,minDist,maxDist,contactBridge);

			if (contactBridge.m_contactCache.getNumContacts())
			{
//				SCE_PFX_PRINTF("found %d contacts\n", contactCache.getNumContacts());
				for (int i=0;i<contactBridge.m_contactCache.getNumContacts();i++)
				{
					if (contactBridge.m_contactCache.getContactPoint(i).m_distance<=contactThreshold)
						contacts.addContactPoint(contactBridge.m_contactCache.getContactPoint(i));
				}
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Initialize / Finalize Engine


bool physics_init()
{
	//D:\develop\bullet3\src\low_level\collision\pfx_detect_collision_func.cpp(437):int pfxSetDetectCollisionFunc(PfxUInt8 shapeTypeA,PfxUInt8 shapeTypeB,pfx_detect_collision_func func)
#if 1
	orgFunc = pfxGetDetectCollisionFunc(kPfxShapeConvexMesh,kPfxShapeConvexMesh);
	pfxSetDetectCollisionFunc(kPfxShapeConvexMesh,kPfxShapeConvexMesh,myCustomCollisionDummy);
	pfxSetDetectCollisionFunc(kPfxShapeConvexMesh,kPfxShapeBox,myCustomCollisionDummy);
	pfxSetDetectCollisionFunc(kPfxShapeBox,kPfxShapeConvexMesh,myCustomCollisionDummy);
	pfxSetDetectCollisionFunc(kPfxShapeBox,kPfxShapeBox,myCustomCollisionDummy);
#endif
	return true;
}

void physics_release()
{
}

///////////////////////////////////////////////////////////////////////////////
// Pick

PfxVector3 physics_pick_start(const PfxVector3 &p1,const PfxVector3 &p2)
{
	return PfxVector3(0.0f);
}

void physics_pick_update(const PfxVector3 &p)
{
}

void physics_pick_end()
{
}

///////////////////////////////////////////////////////////////////////////////
// Get Information

int physics_get_num_rigidbodies()
{
	return numRigidBodies;
}

const PfxRigidState& physics_get_state(int id)
{
	return states[id];
}

const PfxRigidBody& physics_get_body(int id)
{
	return bodies[id];
}

const PfxCollidable& physics_get_collidable(int id)
{
	return collidables[id];
}

int physics_get_num_contacts()
{
	return numPairs[pairSwap];
}

const PfxContactManifold &physics_get_contact(int id)
{
	return contacts[pfxGetConstraintId(pairsBuff[pairSwap][id])];
}
