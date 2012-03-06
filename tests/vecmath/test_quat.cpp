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
}