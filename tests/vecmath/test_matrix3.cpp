//
//  test_matrix3.cpp
//  test_vecmath_osx
//
//  Created by Justin on 12/03/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#include "UnitTest++.h"

#include "arch_defines.h"

using namespace Vectormath::Aos;

SUITE(Matrix3)
{
    
    TEST(construct_Vector3)
    {
        const Matrix3 m(Vector3::xAxis(), Vector3::yAxis(), Vector3::zAxis());
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
    }
    
    TEST(construct_Matrix3)
    {
        const Matrix3 m(Vector3::xAxis(), Vector3::yAxis(), Vector3::zAxis());
        const Matrix3 r(m);
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
    }
    
    TEST(construct_Quat)
    {
        const float _0   = 45 * DEGTORAD;
        const float sin0 = sinf(_0);
        const float cos0 = cosf(_0); 
        const Quat q     = Quat::rotationX(_0);
        const Matrix3 m(q);
        
        CHECK_EQUAL(1.f,   m[0][0]);
        CHECK_EQUAL(0.f,   m[0][1]);
        CHECK_EQUAL(0.f,   m[0][2]);
        CHECK_EQUAL(0.f,   m[1][0]);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_CLOSE(sin0,  m[1][2], kEpsilon);
        CHECK_EQUAL(0.f,   m[2][0]);
        CHECK_CLOSE(-sin0, m[2][1], kEpsilon);
        CHECK_CLOSE(cos0,  m[2][2], kEpsilon);
    }
    
    TEST(construct_scalar)
    {
        const Matrix3 m(3.3f);
        
        CHECK_EQUAL(3.3f, m[0][0]);
        CHECK_EQUAL(3.3f, m[0][1]);
        CHECK_EQUAL(3.3f, m[0][2]);
        CHECK_EQUAL(3.3f, m[1][0]);
        CHECK_EQUAL(3.3f, m[1][1]);
        CHECK_EQUAL(3.3f, m[1][2]);
        CHECK_EQUAL(3.3f, m[2][0]);
        CHECK_EQUAL(3.3f, m[2][1]);
        CHECK_EQUAL(3.3f, m[2][2]);
    }
    
    TEST(operator_assignment_equals)
    {
        const Matrix3 m = Matrix3(Vector3::xAxis(), 
                                  Vector3::yAxis(), 
                                  Vector3::zAxis());

        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
    }
    
    TEST(set_get_col)
    {
        Matrix3 m(9.9f);
        
        m.setCol0(Vector3::xAxis());
        m.setCol1(Vector3::yAxis());
        m.setCol2(Vector3::zAxis());
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);

        Vector3 c0 = m.getCol0();
        Vector3 c1 = m.getCol1();
        Vector3 c2 = m.getCol2();
        
        CHECK_EQUAL(1.f, c0[0]);
        CHECK_EQUAL(0.f, c0[1]);
        CHECK_EQUAL(0.f, c0[2]);
        CHECK_EQUAL(0.f, c1[0]);
        CHECK_EQUAL(1.f, c1[1]);
        CHECK_EQUAL(0.f, c1[2]);
        CHECK_EQUAL(0.f, c2[0]);
        CHECK_EQUAL(0.f, c2[1]);
        CHECK_EQUAL(1.f, c2[2]);
        
        m.setCol(0, Vector3(0.0f, 1.1f, 2.2f));
        m.setCol(1, Vector3(3.3f, 4.4f, 5.5f));
        m.setCol(2, Vector3(6.6f, 7.7f, 8.8f));
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(1.1f, m[0][1]);
        CHECK_EQUAL(2.2f, m[0][2]);
        CHECK_EQUAL(3.3f, m[1][0]);
        CHECK_EQUAL(4.4f, m[1][1]);
        CHECK_EQUAL(5.5f, m[1][2]);
        CHECK_EQUAL(6.6f, m[2][0]);
        CHECK_EQUAL(7.7f, m[2][1]);
        CHECK_EQUAL(8.8f, m[2][2]);
        
        c0 = m.getCol(0);
        c1 = m.getCol(1);
        c2 = m.getCol(2);
        
        CHECK_EQUAL(0.0f, c0[0]);
        CHECK_EQUAL(1.1f, c0[1]);
        CHECK_EQUAL(2.2f, c0[2]);
        CHECK_EQUAL(3.3f, c1[0]);
        CHECK_EQUAL(4.4f, c1[1]);
        CHECK_EQUAL(5.5f, c1[2]);
        CHECK_EQUAL(6.6f, c2[0]);
        CHECK_EQUAL(7.7f, c2[1]);
        CHECK_EQUAL(8.8f, c2[2]);
    }
    
    TEST(set_get_row)
    {
        Matrix3 m(9.9f);
        
        m.setRow(0, Vector3(0.0f, 1.1f, 2.2f));
        m.setRow(1, Vector3(3.3f, 4.4f, 5.5f));
        m.setRow(2, Vector3(6.6f, 7.7f, 8.8f));
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(1.1f, m[1][0]);
        CHECK_EQUAL(2.2f, m[2][0]);
        CHECK_EQUAL(3.3f, m[0][1]);
        CHECK_EQUAL(4.4f, m[1][1]);
        CHECK_EQUAL(5.5f, m[2][1]);
        CHECK_EQUAL(6.6f, m[0][2]);
        CHECK_EQUAL(7.7f, m[1][2]);
        CHECK_EQUAL(8.8f, m[2][2]);
        
        Vector3 r0 = m.getRow(0);
        Vector3 r1 = m.getRow(1);
        Vector3 r2 = m.getRow(2);
        
        CHECK_EQUAL(0.0f, r0[0]);
        CHECK_EQUAL(1.1f, r0[1]);
        CHECK_EQUAL(2.2f, r0[2]);
        CHECK_EQUAL(3.3f, r1[0]);
        CHECK_EQUAL(4.4f, r1[1]);
        CHECK_EQUAL(5.5f, r1[2]);
        CHECK_EQUAL(6.6f, r2[0]);
        CHECK_EQUAL(7.7f, r2[1]);
        CHECK_EQUAL(8.8f, r2[2]);
    }
    
    TEST(operator_array_subscript)
    {
        Matrix3 m(9.9f);
        
        m[0] = Vector3::xAxis();
        m[1] = Vector3::yAxis();
        m[2] = Vector3::zAxis();
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
        
        const Matrix3 m2(m);
        Vector3 c0 = m2[0];
        Vector3 c1 = m2[1];
        Vector3 c2 = m2[2];
        
        CHECK_EQUAL(1.f, c0[0]);
        CHECK_EQUAL(0.f, c0[1]);
        CHECK_EQUAL(0.f, c0[2]);
        CHECK_EQUAL(0.f, c1[0]);
        CHECK_EQUAL(1.f, c1[1]);
        CHECK_EQUAL(0.f, c1[2]);
        CHECK_EQUAL(0.f, c2[0]);
        CHECK_EQUAL(0.f, c2[1]);
        CHECK_EQUAL(1.f, c2[2]);
    }
    
    TEST(get_set_elem)
    {
        Matrix3 m(10.f);
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                m.setElem(i, j, (i * 3 + j) * 0.1f);
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.1f, m[0][1]);
        CHECK_EQUAL(0.2f, m[0][2]);
        CHECK_EQUAL(0.3f, m[1][0]);
        CHECK_EQUAL(0.4f, m[1][1]);
        CHECK_EQUAL(0.5f, m[1][2]);
        CHECK_EQUAL(0.6f, m[2][0]);
        CHECK_EQUAL(0.7f, m[2][1]);
        CHECK_EQUAL(0.8f, m[2][2]);
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_EQUAL((i * 3 + j) * 0.1f, m.getElem(i, j));
    }
    
    TEST(operator_plus)
    {
        const Matrix3 m1(1.1f);
        const Matrix3 m2(2.2f);
        Matrix3 r = m1 + m2;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(3.3f, r[i][j], kEpsilon);
    }
    
    TEST(operator_minus)
    {
        const Matrix3 m1(1.1f);
        const Matrix3 m2(2.2f);
        Matrix3 r = m2 - m1;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(1.1f, r[i][j], kEpsilon);
    }
    
    TEST(operator_multiply_scalar)
    {
        const Matrix3 m1(1.1f);
        Matrix3 r = m1 * 2.0f;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(2.2f, r[i][j], kEpsilon);
    }

    TEST(operator_multiply_vector3)
    {
        const Matrix3 id(Vector3::xAxis(), Vector3::yAxis(), Vector3::zAxis());
        Vector3 r = id * Vector3(1.1f, 2.2f, 3.3f);
        
        CHECK_EQUAL(1.1f, r[0]);
        CHECK_EQUAL(2.2f, r[1]);
        CHECK_EQUAL(3.3f, r[2]);
    }
    
    TEST(operator_multiply_matrix3)
    {
        const Matrix3 id(Vector3::xAxis(), Vector3::yAxis(), Vector3::zAxis());
        const Matrix3 m(Vector3(1.1f, 2.2f, 3.3f), 
                        Vector3(4.4f, 5.5f, 6.6f),
                        Vector3(7.7f, 8.8f, 9.9f));
        
        Matrix3 r = id * m;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(m[i][j], r[i][j], kEpsilon);
        
        r = Matrix3(0.f);
        r = m * id;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(m[i][j], r[i][j], kEpsilon);
    }
    
    TEST(operator_plus_equals)
    {
        const Matrix3 m1(1.1f);
        Matrix3 r(2.2f);
        r += m1;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(3.3f, r[i][j], kEpsilon);    
    }
    
    TEST(operator_minus_equals)
    {
        const Matrix3 m1(1.1f);
        Matrix3 r(2.2f);
        r -= m1;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(1.1f, r[i][j], kEpsilon);
    }
    
    TEST(operator_multiply_equal_scalar)
    {
        Matrix3 m1(1.1f);
        m1 *= 2.0f;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(2.2f, m1[i][j], kEpsilon);
    }
    
    TEST(operator_multiply_equal_matrix3)
    {
        const Matrix3 id(Vector3::xAxis(), Vector3::yAxis(), Vector3::zAxis());
        Matrix3 m(Vector3(1.1f, 2.2f, 3.3f), 
                  Vector3(4.4f, 5.5f, 6.6f),
                  Vector3(7.7f, 8.8f, 9.9f));
        Matrix3 r = m;
        r *=id;
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(m[i][j], r[i][j], kEpsilon);
    }
    
    TEST(identity)
    {
        const Matrix3 m = Matrix3::identity();
        
        CHECK_EQUAL(1.0f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[1][0]);
        CHECK_EQUAL(1.0f, m[1][1]);
        CHECK_EQUAL(0.0f, m[1][2]);
        CHECK_EQUAL(0.0f, m[2][0]);
        CHECK_EQUAL(0.0f, m[2][1]);
        CHECK_EQUAL(1.0f, m[2][2]);
    }
    
    TEST(rotationX)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix3 m = Matrix3::rotationX(radians);
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        
        CHECK_EQUAL(0.f,  m[1][0]);
        CHECK_CLOSE(cos0, m[1][1], kEpsilon);
        CHECK_CLOSE(sin0, m[1][2], kEpsilon);
        
        CHECK_EQUAL(0.f,  m[2][0]);
        CHECK_CLOSE(-sin0,m[2][1], kEpsilon);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);
    }
    
    TEST(rotationY)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix3 m = Matrix3::rotationY(radians);
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
        CHECK_CLOSE(cos0, m[0][0], kEpsilon);
        CHECK_EQUAL(0.f,  m[0][1]);
        CHECK_CLOSE(-sin0,m[0][2], kEpsilon);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        
        CHECK_CLOSE(sin0, m[2][0], kEpsilon);
        CHECK_EQUAL(0.f,  m[2][1]);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);
    }
    
    TEST(rotationZ)
    {
        const float     radians = 45 * DEGTORAD;
        const Matrix3   m = Matrix3::rotationZ(radians);
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
    
    TEST(rotationZYX)
    {
        const float radians = 45 * DEGTORAD;
        const Matrix3 z = Matrix3::rotationZ(radians);
        const Matrix3 y = Matrix3::rotationY(radians);
        const Matrix3 x = Matrix3::rotationX(radians);
        
        const Matrix3 expected = Matrix3::rotationZYX(Vector3(radians));
        const Matrix3 r = z * y * x; 
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(expected[i][j], r[i][j], kEpsilon);
    }
    
    TEST(rotation_around_axis)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix3 m = Matrix3::rotation(radians, -Vector3::xAxis());
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
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
    
    TEST(rotation_from_quat)
    {
        const float radians = 45 * DEGTORAD;
        Quat q = Quat::rotationX(radians);
        Matrix3 expected = Matrix3::rotationX(radians);
        
        Matrix3 r = Matrix3::rotation(q);
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(expected[i][j], r[i][j], kEpsilon);
    }
    
    TEST(scale)
    {
        Matrix3 m = Matrix3::scale(Vector3(1.1f, 2.2f, 3.3f));
        
        CHECK_EQUAL(1.1f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[1][0]);
        CHECK_EQUAL(2.2f, m[1][1]);
        CHECK_EQUAL(0.0f, m[1][2]);
        CHECK_EQUAL(0.0f, m[2][0]);
        CHECK_EQUAL(0.0f, m[2][1]);
        CHECK_EQUAL(3.3f, m[2][2]);
    }
    
    TEST(operator_pre_multiply_scalar)
    {
        Matrix3 m = 3.3f * Matrix3::identity();
        
        CHECK_EQUAL(3.3f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[1][0]);
        CHECK_EQUAL(3.3f, m[1][1]);
        CHECK_EQUAL(0.0f, m[1][2]);
        CHECK_EQUAL(0.0f, m[2][0]);
        CHECK_EQUAL(0.0f, m[2][1]);
        CHECK_EQUAL(3.3f, m[2][2]);
    }
    
    TEST(appendScale)
    {
        
    }
}