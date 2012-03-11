//
//  test_point3.cpp
//  test_vecmath_osx
//
//  Created by Justin on 11/03/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#include "UnitTest++.h"

#include "arch_defines.h"

using namespace Vectormath::Aos;


SUITE(Point3)
{
    TEST(construct_float3)
    {
        Point3 src(0.0f, 1.1f, 2.2f);
        
        CHECK_EQUAL(0.0f, src.getX());
        CHECK_EQUAL(1.1f, src.getY());
        CHECK_EQUAL(2.2f, src.getZ());
    }
    
    TEST(construct_Point3)
    {
        Point3 src(0.0f, 1.1f, 2.2f);
        Point3 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(construct_Vector3)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        Point3 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(construct_scalar)
    {
        float src = 1.1f;
        Point3 dst(src);
        
        CHECK_EQUAL(1.1f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(1.1f, dst.getZ());
    }

    TEST(operatorAssignmentEquals)
    {
        Point3 src(0.0f, 1.1f, 2.2f);
        Point3 dst = src;
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(set)
    {
        Point3 r(0.0f, 0.0f, 0.0f);
        
        r.setX(0.0f);        
        r.setY(1.1f);
        r.setZ(2.2f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
    }

    TEST(setElem)
    {
        Point3 r(0.0f, 0.0f, 0.0f);
        
        r.setElem(0, 0.0f);        
        r.setElem(1, 1.1f);
        r.setElem(2, 2.2f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
    }
    
    TEST(getElem)
    {
        Point3 r(0.0f, 1.1f, 2.2f);
        
        CHECK_EQUAL(0.0f, r.getElem(0));
        CHECK_EQUAL(1.1f, r.getElem(1));
        CHECK_EQUAL(2.2f, r.getElem(2));
    }
    
    TEST(operatorArraySubscript)
    {
        Point3 r(0.0f, 0.0f, 0.0f);
        
        r[0] = -1.0f;      
        r[1] = 1.1f;
        r[2] = 2.2f;
        
        CHECK_EQUAL(-1.0f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
    }
    
    TEST(operatorMinusPoint3)
    {
        Vector3 r = Point3(0.f,0.f,0.f) - Point3(1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(-1.1f, r[0]);
        CHECK_EQUAL(-2.2f, r[1]);
        CHECK_EQUAL(-3.3f, r[2]);
    }
    
    TEST(operatorPlusVector3)
    {
        Point3 r = Point3(1.1f, 2.2f, 3.3f) + Vector3::zAxis();
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(4.3f, r[2]);
    }
       
    TEST(operatorMinusVector3)
    {
        Point3 r =  Point3(0.f,0.f,0.f) - Vector3(1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(-1.1f, r[0]);
        CHECK_EQUAL(-2.2f, r[1]);
        CHECK_EQUAL(-3.3f, r[2]);
    }
   
    TEST(operatorPlusEquals)
    {
        Point3 r(1.1f, 2.2f, 3.3f);
        r += Vector3::yAxis();
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(3.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
    }
    
    TEST(operatorMinusEquals)
    {
        Point3 r(1.1f, 2.2f, 3.3f);
        r -= Vector3::zAxis();
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(2.3f, r[2]);        
    }
    
    TEST(mulPerElem)
    {
        Point3 p1(0.0f, 1.1f, 2.2f), p2(3.f, 4.f, 5.f);
        Point3 r = mulPerElem(p1, p2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(4.4f, r[1]);
        CHECK_EQUAL(11.0f, r[2]);
    }
    
    TEST(divPerElem)
    {
        Point3 p1(0.0f, 1.1f, 2.2f), p2(1.f, 1.f, 2.f);
        Point3 r = divPerElem(p1, p2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(1.1f, r[2]);
    }
    
    TEST(recipPerElem)
    {
        Point3 v(1.0f, 2.0f, 3.0f);
        Point3 r = recipPerElem(v);
        
        CHECK_CLOSE(1.0f, r[0], kEpsilon);
        CHECK_CLOSE(0.5f, r[1], kEpsilon);
        CHECK_CLOSE(1.f/3.f, r[2], kEpsilon);
    }
    
#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE     
    TEST(sqrtPerElem)
    {
        Point3 v(1.0f, 4.0f, 9.0f);
        Point3 r = sqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);
    }
    
    TEST(rsqrtPerElem)
    {
        Point3 v(1.0f, 4.0f, 9.0f);
        Point3 r = rsqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(0.5f, r[1]);
        CHECK_EQUAL(1.f/3.f, r[2]);
    }
#endif
    
    TEST(absPerElem)
    {
        Point3 v(0.0f, -4.0f, 9.0f);
        Point3 r = absPerElem(v);

        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(4.f, r[1]);
        CHECK_EQUAL(9.f, r[2]);
    }
    
    TEST(copySignPerElem)
    {
        Point3 v1(0.0f, -4.0f, 9.0f);
        Point3 v2(1.f, 1.f, 1.f);
        
        Point3 r = copySignPerElem(v2, v1);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(-1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);
    }
    
    TEST(maxPerElem)
    {
        Point3 v1(0.0f, 1.1f, 2.2f);
        Point3 v2(1.f,1.f,1.f);
        
        Point3 r = maxPerElem(v1, v2);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
    }
    
    TEST(maxElem)
    {
        Point3 v1(0.0f, 1.1f, 2.2f);
        CHECK_EQUAL(2.2f, maxElem(v1));
    }
    
    TEST(minPerElem)
    {
        Point3 v1(0.0f, 1.1f, 2.2f);
        Point3 v2(1.f,1.f,1.f);
        
        Point3 r = minPerElem(v1, v2);
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);        
    }
    
    TEST(minElem)
    {
        Point3 v1(0.0f, 1.1f, 2.2f);
        CHECK_EQUAL(0.0f, minElem(v1));
    }
    
    TEST(sum)
    {
        Point3 v1(0.0f, 1.f, 2.2f);
        CHECK_EQUAL(3.2f, sum(v1));
    }

    TEST(scale)
    {
        Point3 p(0.0f, 1.1f, 2.2f);
        Point3 r = scale(p, 2.f);
        
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(4.4f, r[2]);
    }
    
    TEST(scaleByVector3)
    {
        Point3 p(0.0f, 1.1f, 2.2f);
        Vector3 v(1.0f, 2.f, 3.f);
        Point3 r = scale(p, v);
        
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_CLOSE(6.6f, r[2], kEpsilon);
    }
    
    TEST(projection)
    {
        Point3 p(1.0f, 0.0f, 0.0f);
        Vector3 v(3.0f, 1.0f, 1.f);
        
        float r = projection(p, v);
        CHECK_EQUAL(3.0f, r);
    }
    
    TEST(distSqrFromOrigin)
    {
        Point3 p(1.f, 2.f, 3.f);
        CHECK_EQUAL(14.f, distSqrFromOrigin(p));
    }
    
    TEST(distFromOrigin)
    {
        Point3 p(1.f, 2.f, 3.f);
        CHECK_EQUAL(sqrtf(14.f), distFromOrigin(p));    
    }
    
    TEST(distSqr)
    {
        Point3 p1(1.f, 2.f, 3.f);
        Point3 p2(0.f, 0.f, 0.f);
        
        CHECK_EQUAL(14.f, distSqr(p1,p2));
    }
    
    TEST(dist)
    {
        Point3 p1(1.f, 2.f, 3.f);
        Point3 p2(0.f, 0.f, 0.f);
        
        CHECK_EQUAL(sqrtf(14.f), dist(p1,p2));
    }

    TEST(lerp)
    {
        Point3 v1(Vector3::xAxis());
        Point3 v2(Vector3::yAxis());
        Point3 r = lerp(0.5f, v1, v2);
        
        CHECK_EQUAL(0.5f, r.getX());
        CHECK_EQUAL(0.5f, r.getY());
        CHECK_EQUAL(0.0f, r.getZ());
    }

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE
    TEST(select)
    {
        Point3 v1(0.f, 1.f, 2.f);
        Point3 v2(1.f, 2.f, 3.f);
        
        Point3 r = select(v1, v2, true);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);        
    }
#endif
    
    TEST(loadXYZ)
    {
        float src[] = { 0.0f, 1.1f, 2.2f };
        Point3 dst;
        
        loadXYZ(dst, src);
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(storeXYZ)
    {
        float dst[3];
        Point3 v(0.0f, 1.1f, 2.2f);
        
        storeXYZ(v, dst);
        CHECK_EQUAL(0.0f, dst[0]);
        CHECK_EQUAL(1.1f, dst[1]);
        CHECK_EQUAL(2.2f, dst[2]);
    }
    
#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE    
    TEST(load_store_halfFloats)
    {
        Point3 ref(0.0f, -1.1f, 2.2f);
        Point3 r;
        unsigned short halves[3];
        
        storeHalfFloats(ref, halves);
        loadHalfFloats(r, halves);
        
        CHECK_CLOSE(ref[0], r[0], 0.001f);
        CHECK_CLOSE(ref[1], r[1], 0.001f);
        CHECK_CLOSE(ref[2], r[2], 0.001f);
    }
#endif

}