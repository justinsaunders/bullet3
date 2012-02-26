//
//  test_vec.cpp
//  test_vecmath_osx
//
//  Created by Justin on 16/02/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#include "UnitTest++.h"

#include "arch_defines.h"

using namespace Vectormath::Aos;

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE
    static const float kEpsilon = 1e-5f;
#else
    static const float kEpsilon = 2.5e-4f;
#endif

// Some test utilities
bool IsEqual(const Vector4& a, const Vector4& b)
{
    return a.getX() == b.getX() &&
            a.getY() == b.getY() &&
            a.getZ() == b.getZ() &&
            a.getW() == b.getW();
}

SUITE(Vector3)
{
    TEST(construct_float3)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        
        CHECK_EQUAL(0.0f, src.getX());
        CHECK_EQUAL(1.1f, src.getY());
        CHECK_EQUAL(2.2f, src.getZ());
    }
    
    TEST(construct_Vector3)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        Vector3 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(construct_Point3)
    {
        Point3 src(0.0f, 1.1f, 2.2f);
        Vector3 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(construct_scalar)
    {
        float src = 1.1f;
        Vector3 dst(src);
        
        CHECK_EQUAL(1.1f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(1.1f, dst.getZ());
    }

    TEST(xAxis)
    {
        Vector3 x(Vector3::xAxis());
        
        CHECK_EQUAL(1.0f, x.getX());
        CHECK_EQUAL(0.0f, x.getY());
        CHECK_EQUAL(0.0f, x.getZ());
    }
    
    TEST(yAxis)
    {
        Vector3 y(Vector3::yAxis());
        
        CHECK_EQUAL(0.0f, y.getX());
        CHECK_EQUAL(1.0f, y.getY());
        CHECK_EQUAL(0.0f, y.getZ());
    }
    
    TEST(zAxis)
    {
        Vector3 z(Vector3::zAxis());
        
        CHECK_EQUAL(0.0f, z.getX());
        CHECK_EQUAL(0.0f, z.getY());
        CHECK_EQUAL(1.0f, z.getZ());
    }
    
    TEST(lerp)
    {
        Vector3 v1(Vector3::xAxis());
        Vector3 v2(Vector3::yAxis());
        Vector3 r = lerp(0.5f, v1, v2);
        
        CHECK_EQUAL(0.5f, r.getX());
        CHECK_EQUAL(0.5f, r.getY());
        CHECK_EQUAL(0.0f, r.getZ());
    }
    
    TEST(slerp)
    {
        Vector3 v1(Vector3::xAxis());
        Vector3 v2(Vector3::yAxis());
        Vector3 r = slerp(0.5f, v1, v2);
        
        CHECK_CLOSE(0.7071f, r.getX(), kEpsilon);
        CHECK_CLOSE(0.7071f, r.getY(), kEpsilon);
        CHECK_EQUAL(0.0f, r.getZ());
    }
    
    TEST(loadXYZ)
    {
        float src[] = { 0.0f, 1.1f, 2.2f };
        Vector3 dst;
        
        loadXYZ(dst, src);
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(storeXYZ)
    {
        float dst[3];
        Vector3 v(0.0f, 1.1f, 2.2f);
        
        storeXYZ(v, dst);
        CHECK_EQUAL(0.0f, dst[0]);
        CHECK_EQUAL(1.1f, dst[1]);
        CHECK_EQUAL(2.2f, dst[2]);
    }

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE    
    TEST(load_store_halfFloats)
    {
        Vector3 ref(0.0f, -1.1f, 2.2f);
        Vector3 r;
        unsigned short halves[3];
        
        storeHalfFloats(ref, halves);
        loadHalfFloats(r, halves);
        
        CHECK_CLOSE(ref[0], r[0], 0.001f);
        CHECK_CLOSE(ref[1], r[1], 0.001f);
        CHECK_CLOSE(ref[2], r[2], 0.001f);
    }
#endif
    
    TEST(operatorAssignmentEquals)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        Vector3 dst = src;
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
    }
    
    TEST(set)
    {
        Vector3 r(0.0f, 0.0f, 0.0f);

        r.setX(0.0f);        
        r.setY(1.1f);
        r.setZ(2.2f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
    }
    
    TEST(setElem)
    {
        Vector3 r(0.0f, 0.0f, 0.0f);
        
        r.setElem(0, 0.0f);        
        r.setElem(1, 1.1f);
        r.setElem(2, 2.2f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
    }
    
    TEST(getElem)
    {
        Vector3 r(0.0f, 1.1f, 2.2f);
        
        CHECK_EQUAL(0.0f, r.getElem(0));
        CHECK_EQUAL(1.1f, r.getElem(1));
        CHECK_EQUAL(2.2f, r.getElem(2));
    }
    
    TEST(operatorArraySubscript)
    {
        Vector3 r(0.0f, 0.0f, 0.0f);
        
        r[0] = -1.0f;      
        r[1] = 1.1f;
        r[2] = 2.2f;
        
        CHECK_EQUAL(-1.0f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
    }
    
    TEST(operatorPlus)
    {
        Vector3 r = Vector3::xAxis() + Vector3::yAxis() + Vector3::zAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(1.0f, r[2]);
    }
    
    TEST(operatorMinus)
    {
        Vector3 r =  Vector3::xAxis() - Vector3::yAxis() - Vector3::zAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(-1.0f, r[1]);
        CHECK_EQUAL(-1.0f, r[2]);
    }

    
    TEST(operatorAddPoint3)
    {
        Point3  point(1.f, 1.f, 1.f);
        Point3 r = Vector3::xAxis() + point;
        
        CHECK_EQUAL(2.0f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(1.0f, r[2]);
    }
    
    TEST(operatorMultiply)
    {
        Vector3 v(0.0f, 1.1f, 2.2f);
        v = v * 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(2.2f, v[1]);
        CHECK_EQUAL(4.4f, v[2]);
    }
    
    TEST(operatorPlusEquals)
    {
        Vector3 r = Vector3::xAxis();
        r += Vector3::yAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(0.0f, r[2]);
    }
    
    TEST(operatorMinusEquals)
    {
        Vector3 r = Vector3::xAxis();
        r -= Vector3::yAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(-1.0f, r[1]);
        CHECK_EQUAL(0.0f, r[2]);        
    }
    
    TEST(operatorMultiplyEquals)
    {
        Vector3 v(0.0f, 1.1f, 2.2f);
        v *= 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(2.2f, v[1]);
        CHECK_EQUAL(4.4f, v[2]);
    }
    
    TEST(operatorDivide)
    {
        Vector3 v(0.0f, 1.1f, 2.2f);
        v = v / 2.0f;

        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(0.55f, v[1]);
        CHECK_EQUAL(1.1f, v[2]);
    }
    
    TEST(operatorDivideEquals)
    {
        Vector3 v(0.0f, 1.1f, 2.2f);
        v /= 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(0.55f, v[1]);
        CHECK_EQUAL(1.1f, v[2]);
    }
    
    TEST(operatorNegate)
    {
        Vector3 v(0.0f, 1.1f, 2.2f);
        Vector3 r = -v;
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(-1.1f, r[1]);
        CHECK_EQUAL(-2.2f, r[2]);
    }
    
    TEST(mulPerElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f), v2(3.f, 4.f, 5.f);
        Vector3 r = mulPerElem(v1, v2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(4.4f, r[1]);
        CHECK_EQUAL(11.0f, r[2]);
    }
    
    TEST(divPerElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f), v2(1.f, 1.f, 2.f);
        Vector3 r = divPerElem(v1, v2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(1.1f, r[2]);
    }
    
    TEST(recipPerElem)
    {
        Vector3 v(1.0f, 2.0f, 3.0f);
        Vector3 r = recipPerElem(v);
        
        CHECK_CLOSE(1.0f, r[0], kEpsilon);
        CHECK_CLOSE(0.5f, r[1], kEpsilon);
        CHECK_CLOSE(1.f/3.f, r[2], kEpsilon);
    }
    
#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE     
    TEST(sqrtPerElem)
    {
        Vector3 v(1.0f, 4.0f, 9.0f);
        Vector3 r = sqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);
    }
    
    TEST(rsqrtPerElem)
    {
        Vector3 v(1.0f, 4.0f, 9.0f);
        Vector3 r = rsqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(0.5f, r[1]);
        CHECK_EQUAL(1.f/3.f, r[2]);
    }
#endif
    
    TEST(absPerElem)
    {
        Vector3 v(0.0f, -4.0f, 9.0f);
        Vector3 r = absPerElem(v);

        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(4.f, r[1]);
        CHECK_EQUAL(9.f, r[2]);
    }
    
    TEST(copySignPerElem)
    {
        Vector3 v1(0.0f, -4.0f, 9.0f);
        Vector3 v2(1.f, 1.f, 1.f);
        
        Vector3 r = copySignPerElem(v2, v1);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(-1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);
    }
    
    TEST(maxPerElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f);
        Vector3 v2(1.f,1.f,1.f);
        
        Vector3 r = maxPerElem(v1, v2);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
    }
    
    TEST(maxElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f);
        CHECK_EQUAL(2.2f, maxElem(v1));
    }
    
    TEST(minPerElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f);
        Vector3 v2(1.f,1.f,1.f);
        
        Vector3 r = minPerElem(v1, v2);
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);        
    }
    
    TEST(minElem)
    {
        Vector3 v1(0.0f, 1.1f, 2.2f);
        CHECK_EQUAL(0.0f, minElem(v1));
    }
    
    TEST(sum)
    {
        Vector3 v1(0.0f, 1.f, 2.2f);
        CHECK_EQUAL(3.2f, sum(v1));
    }
    
    TEST(dot)
    {
        Vector3 v1(0.f, 1.f, 2.f);
        Vector3 v2(1.f, 2.f, 3.f);
        
        CHECK_EQUAL(8.f, dot(v1,v2));
    }
    
    TEST(lengthSqr)
    {
        Vector3 v1(1.f, 2.f, 3.f);
        CHECK_EQUAL(14.f, lengthSqr(v1));
    }
    
    TEST(length)
    {
        Vector3 v1(1.f, 2.f, 3.f);
        CHECK_EQUAL(sqrtf(14.f), length(v1));
    }
    
    TEST(normalize)
    {
        Vector3 v1(0.f, 4.0f, 0.f);
        Vector3 r = normalize(v1);
        
        CHECK_EQUAL(0.f, r[0]);
        CHECK_CLOSE(1.f, r[1], kEpsilon);
        CHECK_EQUAL(0.f, r[2]);
    }
    
    TEST(cross)
    {
        Vector3 r = cross(Vector3::xAxis(), Vector3::yAxis());
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(0.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);
    }

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE
    TEST(select)
    {
        Vector3 v1(0.f, 1.f, 2.f);
        Vector3 v2(1.f, 2.f, 3.f);
        
        Vector3 r = select(v1, v2, true);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);        
    }
#endif
}

SUITE(Vector4)
{
    TEST(construct_float4)
    {
        Vector4 src(0.0f, 1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(0.0f, src.getX());
        CHECK_EQUAL(1.1f, src.getY());
        CHECK_EQUAL(2.2f, src.getZ());
        CHECK_EQUAL(3.3f, src.getW());
    }
    
    TEST(construct_Vector3_and_W)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        Vector4 dst(src, 3.3f);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(3.3f, dst.getW());
    }
    
    TEST(construct_Vector3)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        Vector4 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(0.0f, dst.getW());
    }
    
    TEST(construct_Point3)
    {
        Point3 src(0.0f, 1.1f, 2.2f);
        Vector4 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(1.0f, dst.getW());
    }
    
    TEST(construct_Quat)
    {
        Quat src(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 dst(src);
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(3.3f, dst.getW());
    }
    
    TEST(construct_scalar)
    {
        float src = 1.1f;
        Vector4 dst(src);
        
        CHECK_EQUAL(1.1f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(1.1f, dst.getZ());
        CHECK_EQUAL(1.1f, dst.getW());
    }
    
    TEST(operatorAssignmentEquals)
    {
        Vector4 src(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 dst = src;
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(3.3f, dst.getW());
    }
    
    TEST(set)
    {
        Vector4 r(0.0f, 0.0f, 0.0f, 0.0f);
        
        r.setX(0.0f);
        r.setY(1.1f);
        r.setZ(2.2f);
        r.setW(3.3f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
        CHECK_EQUAL(3.3f, r.getW());
    }
    
    TEST(setElem)
    {
        Vector4 r(0.0f, 0.0f, 0.0f, 0.0f);
        
        r.setElem(0, 0.0f);        
        r.setElem(1, 1.1f);
        r.setElem(2, 2.2f);
        r.setElem(3, 3.3f);
        
        CHECK_EQUAL(0.0f, r.getX());
        CHECK_EQUAL(1.1f, r.getY());
        CHECK_EQUAL(2.2f, r.getZ());
        CHECK_EQUAL(3.3f, r.getW());
    }
    
    TEST(getElem)
    {
        Vector4 r(0.0f, 1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(0.0f, r.getElem(0));
        CHECK_EQUAL(1.1f, r.getElem(1));
        CHECK_EQUAL(2.2f, r.getElem(2));
        CHECK_EQUAL(3.3f, r.getElem(3));
    }
    
    TEST(operatorArraySubscript)
    {
        Vector4 r(0.0f, 0.0f, 0.0f, 0.0f);
        
        r[0] = -1.0f;      
        r[1] = 1.1f;
        r[2] = 2.2f;
        r[3] = 3.3f;
        
        CHECK_EQUAL(-1.0f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
        CHECK_EQUAL(3.3f, r[3]);
    }
    
    TEST(operatorPlus)
    {
        Vector4 r = Vector4::xAxis() + Vector4::yAxis() + Vector4::zAxis()
                        + Vector4::wAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(1.0f, r[2]);
        CHECK_EQUAL(1.0f, r[3]);
    }
    
    TEST(operatorMinus)
    {
        Vector4 r =  Vector4::xAxis() - Vector4::yAxis() - Vector4::zAxis()
                        - Vector4::wAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(-1.0f, r[1]);
        CHECK_EQUAL(-1.0f, r[2]);
        CHECK_EQUAL(-1.0f, r[3]);
    }
    
    TEST(operatorMultiply)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        v = v * 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(2.2f, v[1]);
        CHECK_EQUAL(4.4f, v[2]);
        CHECK_EQUAL(6.6f, v[3]);
    }
        
    TEST(operatorDivide)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        v = v / 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(0.55f, v[1]);
        CHECK_EQUAL(1.1f, v[2]);
        CHECK_EQUAL(1.65f, v[3]);
    }
    
    TEST(operatorPlusEquals)
    {
        Vector4 r = Vector4::xAxis();
        r += Vector4::yAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(0.0f, r[2]);
        CHECK_EQUAL(0.0f, r[3]);
    }
    
    TEST(operatorMinusEquals)
    {
        Vector4 r = Vector4::xAxis();
        r -= Vector4::yAxis();
        
        CHECK_EQUAL(1.0f, r[0]);
        CHECK_EQUAL(-1.0f, r[1]);
        CHECK_EQUAL(0.0f, r[2]);
        CHECK_EQUAL(0.0f, r[3]);
    }
    
    TEST(operatorMultiplyEquals)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        v *= 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(2.2f, v[1]);
        CHECK_EQUAL(4.4f, v[2]);
        CHECK_EQUAL(6.6f, v[3]);
    }

    TEST(operatorDivideEquals)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        v /= 2.0f;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(0.55f, v[1]);
        CHECK_EQUAL(1.1f, v[2]);
        CHECK_EQUAL(1.65f, v[3]);
    }
    
    TEST(operatorNegate)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 r = -v;
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(-1.1f, r[1]);
        CHECK_EQUAL(-2.2f, r[2]);
        CHECK_EQUAL(-3.3f, r[3]);
    }
    
    TEST(xAxis)
    {
        Vector4 x(Vector4::xAxis());
        
        CHECK_EQUAL(1.0f, x.getX());
        CHECK_EQUAL(0.0f, x.getY());
        CHECK_EQUAL(0.0f, x.getZ());
        CHECK_EQUAL(0.0f, x.getW());
    }
    
    TEST(yAxis)
    {
        Vector4 y(Vector4::yAxis());
        
        CHECK_EQUAL(0.0f, y.getX());
        CHECK_EQUAL(1.0f, y.getY());
        CHECK_EQUAL(0.0f, y.getZ());
        CHECK_EQUAL(0.0f, y.getW());
    }
    
    TEST(zAxis)
    {
        Vector4 z(Vector4::zAxis());
        
        CHECK_EQUAL(0.0f, z.getX());
        CHECK_EQUAL(0.0f, z.getY());
        CHECK_EQUAL(1.0f, z.getZ());
        CHECK_EQUAL(0.0f, z.getW());
    }
    
    TEST(wAxis)
    {
        Vector4 w(Vector4::wAxis());
        
        CHECK_EQUAL(0.0f, w.getX());
        CHECK_EQUAL(0.0f, w.getY());
        CHECK_EQUAL(0.0f, w.getZ());
        CHECK_EQUAL(1.0f, w.getW());
    }
    
    TEST(operatorScalarMultiply)
    {
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        v = 2.0f * v;
        
        CHECK_EQUAL(0.0f, v[0]);
        CHECK_EQUAL(2.2f, v[1]);
        CHECK_EQUAL(4.4f, v[2]);
        CHECK_EQUAL(6.6f, v[3]);
    }
    
    TEST(mulPerElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 v2(2.f, 2.f, 2.f, 2.f);
        Vector4 r = mulPerElem(v1, v2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(4.4f, r[2]);
        CHECK_EQUAL(6.6f, r[3]);
    }
    
    TEST(divPerElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 v2(2.f, 2.f, 2.f, 2.f);
        Vector4 r = divPerElem(v1, v2);
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(0.55f, r[1]);
        CHECK_EQUAL(1.1f, r[2]);
        CHECK_EQUAL(1.65f, r[3]);        
    }
    
    TEST(recipPerElem)
    {
        Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 r = recipPerElem(v);
        
        CHECK_CLOSE(1.0f, r[0], kEpsilon);
        CHECK_CLOSE(0.5f, r[1], kEpsilon);
        CHECK_CLOSE(1.f/3.f, r[2], kEpsilon);
        CHECK_CLOSE(1.f/4.f, r[3], kEpsilon);
    }

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE  
    TEST(sqrtPerElem)
    {
        Vector4 v(1.f, 4.f, 9.f, 16.f);
        Vector4 r = sqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);
        CHECK_EQUAL(4.f, r[3]);
    }
    
    TEST(rsqrtPerElem)
    {
        Vector4 v(1.f, 4.f, 9.f, 16.f);
        Vector4 r = rsqrtPerElem(v);
        
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(0.5f, r[1]);
        CHECK_EQUAL(1.f/3.f, r[2]);
        CHECK_EQUAL(1.f/4.f, r[3]);
    }
#endif
    
    TEST(absPerElem)
    {
        Vector4 v(0.0f, -4.0f, 9.0f, -1.0f);
        Vector4 r = absPerElem(v);
        
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(4.f, r[1]);
        CHECK_EQUAL(9.f, r[2]);
        CHECK_EQUAL(1.f, r[3]);
    }
    
    TEST(copySignPerElem)
    {
        Vector4 v1(0.0f, -4.0f, 9.0f, -1.0f);
        Vector4 v2(1.f, 1.f, 1.f, 1.f);
        
        Vector4 r = copySignPerElem(v2, v1);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(-1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);
        CHECK_EQUAL(-1.f, r[3]);
    }
    
    TEST(maxPerElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 v2(1.f,1.f,1.f, 1.f);
        
        Vector4 r = maxPerElem(v1, v2);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(1.1f, r[1]);
        CHECK_EQUAL(2.2f, r[2]);
        CHECK_EQUAL(3.3f, r[3]);
    }
    
    TEST(minPerElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        Vector4 v2(1.f, 1.f, 1.f, 1.f);
        
        Vector4 r = minPerElem(v1, v2);
        CHECK_EQUAL(0.f, r[0]);
        CHECK_EQUAL(1.f, r[1]);
        CHECK_EQUAL(1.f, r[2]);
        CHECK_EQUAL(1.f, r[3]);
    }
    
    TEST(maxElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        CHECK_EQUAL(3.3f, maxElem(v1));
    }

    TEST(minElem)
    {
        Vector4 v1(0.0f, 1.1f, 2.2f, 3.3f);
        CHECK_EQUAL(0.0f, minElem(v1));
    }
    
    TEST(sum)
    {
        Vector4 v1(0.0f, 1.f, 2.2f, 3.3f);
        CHECK_EQUAL(6.5f, sum(v1));
    }
    
    TEST(dot)
    {
        Vector4 v1(0.f, 1.f, 2.f, 3.f);
        Vector4 v2(1.f, 2.f, 3.f, 4.f);
        
        CHECK_EQUAL(20.f, dot(v1,v2));
    }
    
    TEST(lengthSqr)
    {
        Vector4 v1(1.f, 2.f, 3.f, 4.f);
        CHECK_EQUAL(30.f, lengthSqr(v1));
    }
    
    TEST(length)
    {
        Vector4 v1(1.f, 2.f, 3.f, 4.f);
        CHECK_EQUAL(sqrtf(30.f), length(v1));
    }
    
    TEST(normalize)
    {
        Vector4 v1(0.f, 2.0f, 0.f, 2.0f);
        Vector4 r = normalize(v1);
        
        CHECK_EQUAL(0.f, r[0]);
        CHECK_CLOSE(0.7071f, r[1], kEpsilon);
        CHECK_EQUAL(0.f, r[2]);
        CHECK_CLOSE(0.7071f, r[1], kEpsilon);
    }
    
    TEST(outer)
    {
        Vector4 v1(1.f, 1.f, 1.f, 1.f);
        Vector4 v2(0.f, 1.f, 2.f, 3.f);
        Matrix4 r = outer(v1, v2);
        
        CHECK(IsEqual(r.getCol0(),Vector4(0.f, 0.f, 0.f, 0.f)));
        CHECK(IsEqual(r.getCol1(), Vector4(1.f, 1.f, 1.f, 1.f)));
        CHECK(IsEqual(r.getCol2(), Vector4(2.f, 2.f, 2.f, 2.f)));
        CHECK(IsEqual(r.getCol3(), Vector4(3.f, 3.f, 3.f, 3.f)));
    }
    
    TEST(lerp)
    {
        Vector4 v1(Vector4::xAxis());
        Vector4 v2(Vector4::wAxis());
        Vector4 r = lerp(0.5f, v1, v2);
        
        CHECK_EQUAL(0.5f, r.getX());
        CHECK_EQUAL(0.0f, r.getY());
        CHECK_EQUAL(0.0f, r.getZ());
        CHECK_EQUAL(0.5f, r.getW());
    }
    
    TEST(slerp)
    {
        Vector4 v1(Vector4::xAxis());
        Vector4 v2(Vector4::wAxis());
        Vector4 r = slerp(0.5f, v1, v2);
        
        CHECK_CLOSE(0.7071f, r.getX(), kEpsilon);
        CHECK_EQUAL(0.0f, r.getY());
        CHECK_EQUAL(0.0f, r.getZ());
        CHECK_CLOSE(0.7071f, r.getW(), kEpsilon);
    }
    
    TEST(select)
    {
        Vector4 v1(0.f, 1.f, 2.f, 3.f);
        Vector4 v2(1.f, 2.f, 3.f, 4.f);
        
        Vector4 r = select(v1, v2, true);
        CHECK_EQUAL(1.f, r[0]);
        CHECK_EQUAL(2.f, r[1]);
        CHECK_EQUAL(3.f, r[2]);
        CHECK_EQUAL(4.f, r[3]);
    }

#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE  
    TEST(loadXYZW)
    {
        float src[] = { 0.0f, 1.1f, 2.2f, 3.3f };
        Vector4 dst;
        
        loadXYZW(dst, src);
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(3.3f, dst.getW());
    }
    
    TEST(storeXYZW)
    {
        float dst[4];
        Vector4 v(0.0f, 1.1f, 2.2f, 3.3f);
        
        storeXYZW(v, dst);
        CHECK_EQUAL(0.0f, dst[0]);
        CHECK_EQUAL(1.1f, dst[1]);
        CHECK_EQUAL(2.2f, dst[2]);
        CHECK_EQUAL(3.3f, dst[3]);
    }
    
    TEST(load_store_halfFloats)
    {
        const Vector4 ref(0.0f, -1.1f, 2.2f, -2.2f);
        Vector4 r;
        unsigned short halves[4];
        
        storeHalfFloats(ref, halves);
        loadHalfFloats(r, halves);
        
        CHECK_CLOSE(ref[0], r[0], 0.001f);
        CHECK_CLOSE(ref[1], r[1], 0.001f);
        CHECK_CLOSE(ref[2], r[2], 0.001f);
        CHECK_CLOSE(ref[3], r[3], 0.001f);
    }
#endif
}