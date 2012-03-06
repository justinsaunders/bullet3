//
//  test_point3.cpp
//  test_vecmath_osx
//
//  Created by Justin on 28/02/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//
#include "UnitTest++.h"
#include "arch_defines.h"

using namespace Vectormath::Aos;


SUITE(Quat)
{
    TEST(float4Constructor)
    {
        Quat q(0.f, 1.f, 2.f, 3.f);
        
        CHECK_EQUAL(0.f, q.getX());
        CHECK_EQUAL(1.f, q.getY());
        CHECK_EQUAL(2.f, q.getZ());
        CHECK_EQUAL(3.f, q.getW());
    }
    
    TEST(copyConstructor)
    {
        Quat src(0.f, 1.f, 2.f, 3.f);
        Quat dst(src);
        
        CHECK_EQUAL(0.f, dst.getX());
        CHECK_EQUAL(1.f, dst.getY());
        CHECK_EQUAL(2.f, dst.getZ());
        CHECK_EQUAL(3.f, dst.getW());
    }
    
    TEST(Vector3Constructor)
    {
        Vector3 src(0.f, 1.f, 2.f);
        Quat dst(src, 3.0f);
        
        CHECK_EQUAL(0.f, dst.getX());
        CHECK_EQUAL(1.f, dst.getY());
        CHECK_EQUAL(2.f, dst.getZ());
        CHECK_EQUAL(3.f, dst.getW());        
    }
    
    TEST(Vector4Constructor)
    {
        Vector4 src(0.f, 1.f, 2.f, 3.f);
        Quat dst(src);
        
        CHECK_EQUAL(0.f, dst.getX());
        CHECK_EQUAL(1.f, dst.getY());
        CHECK_EQUAL(2.f, dst.getZ());
        CHECK_EQUAL(3.f, dst.getW());        
    }
    
    TEST(Matrix3Constructor)
    {
        const float sin0 = sin(45 * DEGTORAD);
        const float cos0 = cos(45 * DEGTORAD);
        
        Matrix3 m(Vector3(1.f, 0.f, 0.f),
                  Vector3(0.f, cos0, -sin0),
                  Vector3(0.f, sin0, cos0));
        
        Quat q(m);
        Matrix3 m2(q);
        
        
        CHECK_EQUAL(1.f, m2[0][0]);
        CHECK_EQUAL(0.f, m2[0][1]);
        CHECK_EQUAL(0.f, m2[0][2]);
        
        CHECK_EQUAL(0.f,   m2[1][0]);
        CHECK_CLOSE(cos0,  m2[1][1], kEpsilon);
        CHECK_CLOSE(-sin0, m2[1][2], kEpsilon);
        
        CHECK_EQUAL(0.f,  m2[2][0]);
        CHECK_CLOSE(sin0, m2[2][1], kEpsilon);
        CHECK_CLOSE(cos0, m2[2][2], kEpsilon);            }
    
    TEST(operatorAssignmentEquals)
    {
        Quat src(0.0f, 1.1f, 2.2f, 3.3f);
        Quat dst = src;
        
        CHECK_EQUAL(0.0f, dst.getX());
        CHECK_EQUAL(1.1f, dst.getY());
        CHECK_EQUAL(2.2f, dst.getZ());
        CHECK_EQUAL(3.3f, dst.getW());
    }
    
    TEST(set)
    {
        Quat q(0.0f, 0.0f, 0.0f, 0.0f);
        
        q.setX(0.0f);
        q.setY(1.1f);
        q.setZ(2.2f);
        q.setW(3.3f);
        
        CHECK_EQUAL(0.0f, q.getX());
        CHECK_EQUAL(1.1f, q.getY());
        CHECK_EQUAL(2.2f, q.getZ());
        CHECK_EQUAL(3.3f, q.getW());
    }
    
    TEST(setElem)
    {
        Quat q(0.0f, 0.0f, 0.0f, 0.0f);
        
        q.setElem(0, 0.0f);        
        q.setElem(1, 1.1f);
        q.setElem(2, 2.2f);
        q.setElem(3, 3.3f);
        
        CHECK_EQUAL(0.0f, q.getX());
        CHECK_EQUAL(1.1f, q.getY());
        CHECK_EQUAL(2.2f, q.getZ());
        CHECK_EQUAL(3.3f, q.getW());
    }
    
    TEST(getElem)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(0.0f, q.getElem(0));
        CHECK_EQUAL(1.1f, q.getElem(1));
        CHECK_EQUAL(2.2f, q.getElem(2));
        CHECK_EQUAL(3.3f, q.getElem(3));
    }
    
    TEST(operatorArraySubscript)
    {
        Quat q(0.0f, 0.0f, 0.0f, 0.0f);
        
        q[0] = -1.0f;      
        q[1] = 1.1f;
        q[2] = 2.2f;
        q[3] = 3.3f;
        
        CHECK_EQUAL(-1.0f, q[0]);
        CHECK_EQUAL(1.1f, q[1]);
        CHECK_EQUAL(2.2f, q[2]);
        CHECK_EQUAL(3.3f, q[3]);
    }
    
    TEST(operatorPlus)
    {
        Quat q1(0.f, 0.f, 3.3f, 4.4f);
        Quat q2(1.1f, 2.2f, 0.f, 0.f);
        Quat r = q1 + q2;
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
        CHECK_EQUAL(4.4f, r[3]);
    }
    
    TEST(operatorMinus)
    {
        Quat q1(0.f, 0.f, 3.3f, 4.4f);
        Quat q2(1.1f, 2.2f, 0.f, 0.f);
        Quat r = q1 - q2;
        
        CHECK_EQUAL(-1.1f, r[0]);
        CHECK_EQUAL(-2.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
        CHECK_EQUAL(4.4f, r[3]);
    }
    
    TEST(operatorMultiplyScalar)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        q = q * 2.0f;
        
        CHECK_EQUAL(0.0f, q[0]);
        CHECK_EQUAL(2.2f, q[1]);
        CHECK_EQUAL(4.4f, q[2]);
        CHECK_EQUAL(6.6f, q[3]);
    }
    
    
    TEST(operatorDivideScalar)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        q = q / 2.0f;
        
        CHECK_EQUAL(0.0f, q[0]);
        CHECK_EQUAL(0.55f, q[1]);
        CHECK_EQUAL(1.1f, q[2]);
        CHECK_EQUAL(1.65f, q[3]);
    }
    
    TEST(operatorPlusEquals)
    {
        Quat q1(0.f, 0.f, 3.3f, 4.4f);
        Quat q2(1.1f, 2.2f, 0.f, 0.f);
        Quat r = q1; r += q2;
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
        CHECK_EQUAL(4.4f, r[3]);
    }
    
    TEST(operatorMinusEquals)
    {
        Quat q1(0.f, 0.f, 3.3f, 4.4f);
        Quat q2(1.1f, 2.2f, 0.f, 0.f);
        Quat r = q1;  r -= q2;
        
        CHECK_EQUAL(-1.1f, r[0]);
        CHECK_EQUAL(-2.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
        CHECK_EQUAL(4.4f, r[3]);
    }
    
    TEST(operatorMultiplyScalarEquals)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        q *= 2.0f;
        
        CHECK_EQUAL(0.0f, q[0]);
        CHECK_EQUAL(2.2f, q[1]);
        CHECK_EQUAL(4.4f, q[2]);
        CHECK_EQUAL(6.6f, q[3]);
    }
    
    TEST(operatorDivideEquals)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        q /= 2.0f;
        
        CHECK_EQUAL(0.0f,  q[0]);
        CHECK_EQUAL(0.55f, q[1]);
        CHECK_EQUAL(1.1f,  q[2]);
        CHECK_EQUAL(1.65f, q[3]);
    }
    
    TEST(operatorNegate)
    {
        Quat q(0.0f, 1.1f, 2.2f, 3.3f);
        Quat r = -q;
        
        CHECK_EQUAL(0.0f, r[0]);
        CHECK_EQUAL(-1.1f, r[1]);
        CHECK_EQUAL(-2.2f, r[2]);
        CHECK_EQUAL(-3.3f, r[3]);
    }
    
    TEST(identity)
    {
        Quat q = Quat::identity();
        
        CHECK_EQUAL(0.f, q[0]);
        CHECK_EQUAL(0.f, q[1]);
        CHECK_EQUAL(0.f, q[2]);
        CHECK_EQUAL(1.f, q[3]);
    }
    
    TEST(rotation_two_axes)
    {
        const Vector3 up = Vector3::xAxis();
        const Vector3 left = Vector3::zAxis();
        const Vector3 input = up;
        
        const Quat    rotate = Quat::rotation(up, left);
        const Matrix3 m(rotate);
        const Vector3 output = m * input;
        
        CHECK_CLOSE(left[0], output[0], kEpsilon);
        CHECK_CLOSE(left[1], output[1], kEpsilon);
        CHECK_CLOSE(left[2], output[2], kEpsilon);
    }
    
    TEST(rotation_around_axis)
    {
        const Quat      rotate = Quat::rotation(45 * DEGTORAD, -Vector3::xAxis());
        const Matrix3   m(rotate);
        const float     sin0 = sin(45 * DEGTORAD);
        const float     cos0 = cos(45 * DEGTORAD);
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        
        CHECK_EQUAL(0.f,   m[1][0]);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_CLOSE(-sin0, m[1][2], kEpsilon);
        
        CHECK_EQUAL(0.f,  m[2][0]);
        CHECK_CLOSE(sin0, m[2][1], kEpsilon);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);                              
    }
    
    TEST(rotationX)
    {
        const float     radians = 45 * DEGTORAD;
        const Quat      rotate = Quat::rotationX(radians);
        const Matrix3   m(rotate);
        const float     sin0 = sin(radians);
        const float     cos0 = cos(radians);
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        
        CHECK_EQUAL(0.f,   m[1][0]);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_CLOSE(sin0, m[1][2], kEpsilon);
        
        CHECK_EQUAL(0.f,  m[2][0]);
        CHECK_CLOSE(-sin0, m[2][1], kEpsilon);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);
    }
    
    TEST(rotationY)
    {
        const float     radians = 45 * DEGTORAD;
        const Quat      rotate = Quat::rotationY(radians);
        const Matrix3   m(rotate);
        const float     sin0 = sin(radians);
        const float     cos0 = cos(radians);
     
        CHECK_CLOSE(cos0, m[0][0], kEpsilon);
        CHECK_EQUAL(0.f,  m[0][1]);
        CHECK_CLOSE(-sin0, m[0][2], kEpsilon);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);

        CHECK_CLOSE(sin0, m[2][0], kEpsilon);
        CHECK_EQUAL(0.f,   m[2][1]);
        CHECK_CLOSE(cos0,  m[2][2], kEpsilon);
    }
    
    TEST(rotationZ)
    {
        const float     radians = 45 * DEGTORAD;
        const Quat      rotate = Quat::rotationZ(radians);
        const Matrix3   m(rotate);
        const float     sin0 = sin(radians);
        const float     cos0 = cos(radians);
     
        CHECK_CLOSE(cos0, m[0][0], kEpsilon);
        CHECK_CLOSE(sin0, m[0][1], kEpsilon);
        CHECK_EQUAL(0.f,  m[0][2]);
        
        CHECK_CLOSE(-sin0, m[1][0], kEpsilon);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_EQUAL(0.f,   m[1][2]);
        
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
    }
    
    TEST(conj)
    {
        const Quat q(1.1f, 2.2f, 3.3f, 4.4f);
        const Quat cq = conj(q);
        
        CHECK_EQUAL(-1.1f, cq[0]);
        CHECK_EQUAL(-2.2f, cq[1]);
        CHECK_EQUAL(-3.3f, cq[2]);
        CHECK_EQUAL(4.4f,  cq[3]);
    }
    
    TEST(rotate_Vector3)
    {
        const Vector3 up = Vector3::xAxis();
        const Vector3 left = Vector3::zAxis();
        const Vector3 input = up;
        
        const Quat    q = Quat::rotation(up, left);
        const Vector3 output = rotate(q, input);
        
        CHECK_CLOSE(left[0], output[0], kEpsilon);
        CHECK_CLOSE(left[1], output[1], kEpsilon);
        CHECK_CLOSE(left[2], output[2], kEpsilon);
    }
    
    TEST(dot)
    {
        const Quat q(1.f, 2.f, 3.f, 4.f);
        CHECK_EQUAL(30.f, dot(q,q));
    }
    
    TEST(norm)
    {
        const Quat q(1.f, 2.f, 3.f, 4.f);
        CHECK_EQUAL(30.f, norm(q));
    }
    
    TEST(length)
    {
        const Quat q(1.f, 2.f, 3.f, 4.f);
        CHECK_CLOSE(sqrtf(30.f), length(q), kEpsilon);
    }
    
    TEST(normalize)
    {
        const Quat q(0.f, 2.0f, 0.f, 2.0f);
        const Quat r = normalize(q);
        
        CHECK_EQUAL(0.f,     r[0]);
        CHECK_CLOSE(0.7071f, r[1], kEpsilon);
        CHECK_EQUAL(0.f,     r[2]);
        CHECK_CLOSE(0.7071f, r[1], kEpsilon);
    }
    
    TEST(lerp)
    {
        const Quat q1(0.f, 1.f, 2.f, 3.f);
        const Quat q2(1.f, 1.f, 1.f, 1.f);
        const Quat r = lerp(0.5f, q1, q2);
        
        CHECK_EQUAL(0.5f, r[0]);
        CHECK_EQUAL(1.0f, r[1]);
        CHECK_EQUAL(1.5f, r[2]);
        CHECK_EQUAL(2.0f, r[3]);
    }
    
    TEST(slerp)
    {
        const Quat q0 = Quat::identity();
        const Quat q1 = Quat::rotationX(45 * DEGTORAD);
        const Quat q2 = Quat::rotationX(90 * DEGTORAD);
        const Quat r = slerp(0.5f, q0, q2);
        
        CHECK_CLOSE(q1[0], r[0], kEpsilon);
        CHECK_EQUAL(q1[1], r[1]);
        CHECK_EQUAL(q1[2], r[2]);
        CHECK_EQUAL(q1[3], r[3]);
    }
    
    TEST(squad)
    {
        const Quat e = Quat::identity();
        const Quat r = squad(0.5, 
                             Quat::rotationX(45 * DEGTORAD), 
                             Quat::rotationX(-45 * DEGTORAD), 
                             Quat::rotationZ(45 * DEGTORAD), 
                             Quat::rotationZ(-45 * DEGTORAD));
        
        CHECK_EQUAL(e[0], r[0]);
        CHECK_EQUAL(e[1], r[1]);
        CHECK_EQUAL(e[2], r[2]);
        CHECK_EQUAL(e[3], r[3]);
    }
    
#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE
    TEST(select)
    {
        const Quat q1(0.f, 1.f, 2.f, 3.f);
        const Quat q2(1.f, 1.f, 1.f, 1.f);
        const Quat r1 = select(q1, q2, false);
        const Quat r2 = select(q1, q2, true);
        
        CHECK_EQUAL(0.f, r1[0]);
        CHECK_EQUAL(1.f, r1[1]);
        CHECK_EQUAL(2.f, r1[2]);
        CHECK_EQUAL(3.f, r1[3]);
        
        CHECK_EQUAL(1.f, r2[0]);
        CHECK_EQUAL(1.f, r2[1]);
        CHECK_EQUAL(1.f, r2[2]);
        CHECK_EQUAL(1.f, r2[3]);
    }
#endif
    
    TEST(loadXYZW)
    {
        const float a[5] = { 1.f, 2.f, 3.f, 4.f, 5.f };
        Quat q; loadXYZW(q, a+1);
        
        CHECK_EQUAL(2.f, q[0]);
        CHECK_EQUAL(3.f, q[1]);
        CHECK_EQUAL(4.f, q[2]);
        CHECK_EQUAL(5.f, q[3]);
    }
    
    TEST(storeXYZW)
    {
        float a[5] = { 0.f, 0.f, 0.f, 0.f, 0.f };
        Quat q(1.f, 2.f, 3.f, 4.f);
        storeXYZW(q, a+1);
        
        CHECK_EQUAL(a[0], 0.f);
        CHECK_EQUAL(a[1], 1.f);
        CHECK_EQUAL(a[2], 2.f);
        CHECK_EQUAL(a[3], 3.f);
        CHECK_EQUAL(a[4], 4.f);
    }
}