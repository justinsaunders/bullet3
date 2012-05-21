//
//  test_matrix4.cpp
//  test_vecmath_osx
//
//  Created by Justin on 2/04/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#include "UnitTest++.h"

#include "arch_defines.h"

using namespace Vectormath::Aos;

SUITE(Matrix4)
{
    TEST(construct_Matrix4)
    {
        const Matrix4 m(Vector4::xAxis(), Vector4::yAxis(), Vector4::zAxis(),
                        Vector4::wAxis());
        const Matrix4 r(m);
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
    }

    TEST(construct_Vector4)
    {
        const Matrix4 m(Vector4::xAxis(), Vector4::yAxis(), Vector4::zAxis(),
                        Vector4::wAxis());
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
        CHECK_EQUAL(0.f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(construct_Transform3)
    {
        const Transform3 t3(Vector3(0.0f, 0.1f, 0.2f),
                            Vector3(0.3f, 0.4f, 0.5f),
                            Vector3(0.6f, 0.7f, 0.8f),
                            Vector3(0.9f, 1.0f, 1.1f)); 
        const Matrix4 m(t3);
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.1f, m[0][1]);
        CHECK_EQUAL(0.2f, m[0][2]);
        CHECK_EQUAL(0.0f, m[0][3]);
        
        CHECK_EQUAL(0.3f, m[1][0]);
        CHECK_EQUAL(0.4f, m[1][1]);
        CHECK_EQUAL(0.5f, m[1][2]);
        CHECK_EQUAL(0.0f, m[1][3]);
        
        CHECK_EQUAL(0.6f, m[2][0]);
        CHECK_EQUAL(0.7f, m[2][1]);
        CHECK_EQUAL(0.8f, m[2][2]);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(0.9f, m[3][0]);
        CHECK_EQUAL(1.0f, m[3][1]);
        CHECK_EQUAL(1.1f, m[3][2]);
        CHECK_EQUAL(1.0f, m[3][3]);
    }
    
    TEST(construct_Matrix3_Vector3)
    {
        const Matrix4 m(Matrix3(Vector3(0.0f, 0.1f, 0.2f),
                                Vector3(0.3f, 0.4f, 0.5f),
                                Vector3(0.6f, 0.7f, 0.8f)),
                        Vector3(0.9f, 1.0f, 1.1f));
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.1f, m[0][1]);
        CHECK_EQUAL(0.2f, m[0][2]);
        CHECK_EQUAL(0.0f, m[0][3]);
        
        CHECK_EQUAL(0.3f, m[1][0]);
        CHECK_EQUAL(0.4f, m[1][1]);
        CHECK_EQUAL(0.5f, m[1][2]);
        CHECK_EQUAL(0.0f, m[1][3]);
        
        CHECK_EQUAL(0.6f, m[2][0]);
        CHECK_EQUAL(0.7f, m[2][1]);
        CHECK_EQUAL(0.8f, m[2][2]);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(0.9f, m[3][0]);
        CHECK_EQUAL(1.0f, m[3][1]);
        CHECK_EQUAL(1.1f, m[3][2]);
        CHECK_EQUAL(1.0f, m[3][3]);
    }
    
    TEST(construct_Quat_Vector3)
    {
        const float _0   = 45 * DEGTORAD;
        const float sin0 = sinf(_0);
        const float cos0 = cosf(_0); 
        const Quat q     = Quat::rotationX(_0);
        const Matrix4 m(q, Vector3(1.1f, 2.2f, 3.3f));
        
        CHECK_EQUAL(1.f,   m[0][0]);
        CHECK_EQUAL(0.f,   m[0][1]);
        CHECK_EQUAL(0.f,   m[0][2]);
        CHECK_EQUAL(0.f,   m[0][3]);
        
        CHECK_EQUAL(0.f,   m[1][0]);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_CLOSE(sin0,  m[1][2], kEpsilon);
        CHECK_EQUAL(0.f,   m[1][3]);
        
        CHECK_EQUAL(0.f,   m[2][0]);
        CHECK_CLOSE(-sin0, m[2][1], kEpsilon);
        CHECK_CLOSE(cos0,  m[2][2], kEpsilon);
        CHECK_EQUAL(0.f,   m[2][3]);
        
        CHECK_EQUAL(1.1f,  m[3][0]);
        CHECK_EQUAL(2.2f,  m[3][1]);
        CHECK_EQUAL(3.3f,  m[3][2]);
        CHECK_EQUAL(1.0f,  m[3][3]);
    }
    
    TEST(construct_scalar)
    {
        Matrix4 m(1.1f);
        
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                CHECK_EQUAL(1.1f, m[i][j]); 
            }
        }
    }
    
    TEST(assignment)
    {
        const Matrix4 m(Vector4::xAxis(), Vector4::yAxis(), Vector4::zAxis(),
                        Vector4::wAxis());
        const Matrix4 r = m;
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
    }
    
    TEST(setUpper3x3)
    {
        Matrix4 r(9.9f);
        r.setUpper3x3(Matrix3::identity());
        
        CHECK_EQUAL(1.f,  r[0][0]);
        CHECK_EQUAL(0.f,  r[0][1]);
        CHECK_EQUAL(0.f,  r[0][2]);
        CHECK_EQUAL(9.9f, r[0][3]);
        
        CHECK_EQUAL(0.f,  r[1][0]);
        CHECK_EQUAL(1.f,  r[1][1]);
        CHECK_EQUAL(0.f,  r[1][2]);
        CHECK_EQUAL(9.9f, r[1][3]);
        
        CHECK_EQUAL(0.f,  r[2][0]);
        CHECK_EQUAL(0.f,  r[2][1]);
        CHECK_EQUAL(1.f,  r[2][2]);
        CHECK_EQUAL(9.9f, r[2][3]);
        
        CHECK_EQUAL(9.9f, r[3][0]);
        CHECK_EQUAL(9.9f, r[3][1]);
        CHECK_EQUAL(9.9f, r[3][2]);
        CHECK_EQUAL(9.9f, r[3][3]);        
    }
    
    TEST(getUpper3x3)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix3 r = m.getUpper3x3();
        
        CHECK_EQUAL(0.0f,  r[0][0]);
        CHECK_EQUAL(0.1f,  r[0][1]);
        CHECK_EQUAL(0.2f,  r[0][2]);
        
        CHECK_EQUAL(0.4f,  r[1][0]);
        CHECK_EQUAL(0.5f,  r[1][1]);
        CHECK_EQUAL(0.6f,  r[1][2]);
        
        CHECK_EQUAL(0.8f,  r[2][0]);
        CHECK_EQUAL(0.9f,  r[2][1]);
        CHECK_EQUAL(1.0f,  r[2][2]);
    }
    
    TEST(setTranslation)
    {
        Matrix4 m(9.9f);
        m.setTranslation(Vector3(1.1f, 2.2f, 3.3f));
        
        CHECK_EQUAL(9.9f, m[0][0]);
        CHECK_EQUAL(9.9f, m[0][1]);
        CHECK_EQUAL(9.9f, m[0][2]);
        CHECK_EQUAL(9.9f, m[0][3]);
        
        CHECK_EQUAL(9.9f, m[1][0]);
        CHECK_EQUAL(9.9f, m[1][1]);
        CHECK_EQUAL(9.9f, m[1][2]);
        CHECK_EQUAL(9.9f, m[1][3]);
        
        CHECK_EQUAL(9.9f, m[2][0]);
        CHECK_EQUAL(9.9f, m[2][1]);
        CHECK_EQUAL(9.9f, m[2][2]);
        CHECK_EQUAL(9.9f, m[2][3]);
        
        CHECK_EQUAL(1.1f, m[3][0]);
        CHECK_EQUAL(2.2f, m[3][1]);
        CHECK_EQUAL(3.3f, m[3][2]);
        CHECK_EQUAL(9.9f, m[3][3]);
    }
    
    TEST(getTranslation)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        const Vector3 t = m.getTranslation();
        
        
        CHECK_EQUAL(1.2f, m[3][0]);
        CHECK_EQUAL(1.3f, m[3][1]);
        CHECK_EQUAL(1.4f, m[3][2]);
        CHECK_EQUAL(1.5f, m[3][3]);
    }
    
    TEST(setCol)
    {
        Matrix4 m;
        
        m.setCol0(Vector4(0.0f, 0.1f, 0.2f, 0.3f));
        m.setCol1(Vector4(0.4f, 0.5f, 0.6f, 0.7f));
        m.setCol2(Vector4(0.8f, 0.9f, 1.0f, 1.1f));
        m.setCol3(Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.1f, m[0][1]);
        CHECK_EQUAL(0.2f, m[0][2]);
        CHECK_EQUAL(0.3f, m[0][3]);
        
        CHECK_EQUAL(0.4f, m[1][0]);
        CHECK_EQUAL(0.5f, m[1][1]);
        CHECK_EQUAL(0.6f, m[1][2]);
        CHECK_EQUAL(0.7f, m[1][3]);
        
        CHECK_EQUAL(0.8f, m[2][0]);
        CHECK_EQUAL(0.9f, m[2][1]);
        CHECK_EQUAL(1.0f, m[2][2]);
        CHECK_EQUAL(1.1f, m[2][3]);
        
        CHECK_EQUAL(1.2f, m[3][0]);
        CHECK_EQUAL(1.3f, m[3][1]);
        CHECK_EQUAL(1.4f, m[3][2]);
        CHECK_EQUAL(1.5f, m[3][3]);
        
        for (size_t i = 0; i < 4; i++) 
        {
            m.setCol(i, Vector4(i, i, i, i));
        }
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[0][3]);
        
        CHECK_EQUAL(1.0f, m[1][0]);
        CHECK_EQUAL(1.0f, m[1][1]);
        CHECK_EQUAL(1.0f, m[1][2]);
        CHECK_EQUAL(1.0f, m[1][3]);
        
        CHECK_EQUAL(2.0f, m[2][0]);
        CHECK_EQUAL(2.0f, m[2][1]);
        CHECK_EQUAL(2.0f, m[2][2]);
        CHECK_EQUAL(2.0f, m[2][3]);
        
        CHECK_EQUAL(3.0f, m[3][0]);
        CHECK_EQUAL(3.0f, m[3][1]);
        CHECK_EQUAL(3.0f, m[3][2]);
        CHECK_EQUAL(3.0f, m[3][3]);
    }
    
    TEST(getCol)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        Vector4 v0 = m.getCol0(), v1 = m.getCol1(), v2 = m.getCol2(),
                v3 = m.getCol3();
        
        CHECK_EQUAL(0.0f, v0[0]);
        CHECK_EQUAL(0.1f, v0[1]);
        CHECK_EQUAL(0.2f, v0[2]);
        CHECK_EQUAL(0.3f, v0[3]);
        
        CHECK_EQUAL(0.4f, v1[0]);
        CHECK_EQUAL(0.5f, v1[1]);
        CHECK_EQUAL(0.6f, v1[2]);
        CHECK_EQUAL(0.7f, v1[3]);
        
        CHECK_EQUAL(0.8f, v2[0]);
        CHECK_EQUAL(0.9f, v2[1]);
        CHECK_EQUAL(1.0f, v2[2]);
        CHECK_EQUAL(1.1f, v2[3]);
        
        CHECK_EQUAL(1.2f, v3[0]);
        CHECK_EQUAL(1.3f, v3[1]);
        CHECK_EQUAL(1.4f, v3[2]);
        CHECK_EQUAL(1.5f, v3[3]);
        
        Vector4 v[4];
        for (size_t i = 0; i < 4; i++) 
        {
            v[i] = m.getCol(i);
        }
        
        CHECK_EQUAL(0.0f, v[0][0]);
        CHECK_EQUAL(0.1f, v[0][1]);
        CHECK_EQUAL(0.2f, v[0][2]);
        CHECK_EQUAL(0.3f, v[0][3]);
        
        CHECK_EQUAL(0.4f, v[1][0]);
        CHECK_EQUAL(0.5f, v[1][1]);
        CHECK_EQUAL(0.6f, v[1][2]);
        CHECK_EQUAL(0.7f, v[1][3]);
        
        CHECK_EQUAL(0.8f, v[2][0]);
        CHECK_EQUAL(0.9f, v[2][1]);
        CHECK_EQUAL(1.0f, v[2][2]);
        CHECK_EQUAL(1.1f, v[2][3]);
        
        CHECK_EQUAL(1.2f, v[3][0]);
        CHECK_EQUAL(1.3f, v[3][1]);
        CHECK_EQUAL(1.4f, v[3][2]);
        CHECK_EQUAL(1.5f, v[3][3]);
    }
    
    TEST(setRow)
    {
        Matrix4 m;
        
        m.setRow(0, Vector4(0.0f, 0.1f, 0.2f, 0.3f));
        m.setRow(1, Vector4(0.4f, 0.5f, 0.6f, 0.7f));
        m.setRow(2, Vector4(0.8f, 0.9f, 1.0f, 1.1f));
        m.setRow(3, Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        CHECK_EQUAL(0.0f, m[0][0]);
        CHECK_EQUAL(0.1f, m[1][0]);
        CHECK_EQUAL(0.2f, m[2][0]);
        CHECK_EQUAL(0.3f, m[3][0]);
        
        CHECK_EQUAL(0.4f, m[0][1]);
        CHECK_EQUAL(0.5f, m[1][1]);
        CHECK_EQUAL(0.6f, m[2][1]);
        CHECK_EQUAL(0.7f, m[3][1]);
        
        CHECK_EQUAL(0.8f, m[0][2]);
        CHECK_EQUAL(0.9f, m[1][2]);
        CHECK_EQUAL(1.0f, m[2][2]);
        CHECK_EQUAL(1.1f, m[3][2]);
        
        CHECK_EQUAL(1.2f, m[0][3]);
        CHECK_EQUAL(1.3f, m[1][3]);
        CHECK_EQUAL(1.4f, m[2][3]);
        CHECK_EQUAL(1.5f, m[3][3]);
    }
    
    TEST(getRow)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        Vector4 v[4];
        for (size_t i = 0; i < 4; i++) 
        {
            v[i] = m.getRow(i);
        }
        
        CHECK_EQUAL(0.0f, v[0][0]);
        CHECK_EQUAL(0.1f, v[1][0]);
        CHECK_EQUAL(0.2f, v[2][0]);
        CHECK_EQUAL(0.3f, v[3][0]);
        
        CHECK_EQUAL(0.4f, v[0][1]);
        CHECK_EQUAL(0.5f, v[1][1]);
        CHECK_EQUAL(0.6f, v[2][1]);
        CHECK_EQUAL(0.7f, v[3][1]);
        
        CHECK_EQUAL(0.8f, v[0][2]);
        CHECK_EQUAL(0.9f, v[1][2]);
        CHECK_EQUAL(1.0f, v[2][2]);
        CHECK_EQUAL(1.1f, v[3][2]);
        
        CHECK_EQUAL(1.2f, v[0][3]);
        CHECK_EQUAL(1.3f, v[1][3]);
        CHECK_EQUAL(1.4f, v[2][3]);
        CHECK_EQUAL(1.5f, v[3][3]);
    }
    
    TEST(operatorArray)
    {
        // tested implicitly throughout this suite
    }
    
    TEST(getSetElem)
    {
        Matrix4 m;
        
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                m.setElem(i, j, i * 4 + j);
            }
        }
        
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                CHECK_EQUAL(i * 4 + j, m.getElem(i, j));
            }
        }
    }
    
    TEST(operatorAdd)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = Matrix4::identity() + m;
        
        CHECK_EQUAL(1.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(1.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(2.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(2.5f, r[3][3]);
    }
    
    TEST(operatorMinus)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = m - Matrix4::identity();
        
        CHECK_EQUAL(-1.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(-0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(0.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(0.5f, r[3][3]);
    }
    
    TEST(operatorMinusNegate)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = -m;
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(-0.1f, r[0][1]);
        CHECK_EQUAL(-0.2f, r[0][2]);
        CHECK_EQUAL(-0.3f, r[0][3]);
        
        CHECK_EQUAL(-0.4f, r[1][0]);
        CHECK_EQUAL(-0.5f, r[1][1]);
        CHECK_EQUAL(-0.6f, r[1][2]);
        CHECK_EQUAL(-0.7f, r[1][3]);
        
        CHECK_EQUAL(-0.8f, r[2][0]);
        CHECK_EQUAL(-0.9f, r[2][1]);
        CHECK_EQUAL(-1.0f, r[2][2]);
        CHECK_EQUAL(-1.1f, r[2][3]);
        
        CHECK_EQUAL(-1.2f, r[3][0]);
        CHECK_EQUAL(-1.3f, r[3][1]);
        CHECK_EQUAL(-1.4f, r[3][2]);
        CHECK_EQUAL(-1.5f, r[3][3]);
    }
    
    TEST(operatorMultiplyScalar)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = m * 2.0f;
     
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.2f, r[0][1]);
        CHECK_EQUAL(0.4f, r[0][2]);
        CHECK_EQUAL(0.6f, r[0][3]);
        
        CHECK_EQUAL(0.8f, r[1][0]);
        CHECK_EQUAL(1.0f, r[1][1]);
        CHECK_EQUAL(1.2f, r[1][2]);
        CHECK_EQUAL(1.4f, r[1][3]);
        
        CHECK_EQUAL(1.6f, r[2][0]);
        CHECK_EQUAL(1.8f, r[2][1]);
        CHECK_EQUAL(2.0f, r[2][2]);
        CHECK_EQUAL(2.2f, r[2][3]);
        
        CHECK_EQUAL(2.4f, r[3][0]);
        CHECK_EQUAL(2.6f, r[3][1]);
        CHECK_EQUAL(2.8f, r[3][2]);
        CHECK_EQUAL(3.0f, r[3][3]);
    }
    
    TEST(operatorMultiplyVector4)
    {
        Matrix4 m(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                  Vector4(1.1f, 2.2f, 3.3f, 1.0f));
        Vector4 v(1.0f, 2.0f, 3.0f, 1.0f);
        Vector4 r = m * v;
        
        CHECK_EQUAL(2.1f, r[0]);
        CHECK_EQUAL(4.2f, r[1]);
        CHECK_EQUAL(6.3f, r[2]);
        CHECK_EQUAL(1.0f, r[3]);
    }
    
    TEST(operatorMultiplyVector3)
    {
        Matrix4 m(Vector4(2.0f, 0.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 2.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 0.0f, 2.0f, 0.0f),
                  Vector4(1.1f, 2.2f, 3.3f, 1.0f));
        Vector3 v(1.0f, 2.0f, 3.0f);
        Vector4 r = m * v;
        
        CHECK_EQUAL(2.0f, r[0]);
        CHECK_EQUAL(4.0f, r[1]);
        CHECK_EQUAL(6.0f, r[2]);
        CHECK_EQUAL(0.0f, r[3]);
    }
    
    TEST(operatorMultiplyPoint3)
    {
        Matrix4 m(Vector4(1.0f, 0.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 1.0f, 0.0f, 0.0f),
                  Vector4(0.0f, 0.0f, 1.0f, 0.0f),
                  Vector4(1.1f, 2.2f, 3.3f, 1.0f));
        Point3 v(1.0f, 2.0f, 3.0f);
        Vector4 r = m * v;
        
        CHECK_EQUAL(2.1f, r[0]);
        CHECK_EQUAL(4.2f, r[1]);
        CHECK_EQUAL(6.3f, r[2]);
        CHECK_EQUAL(1.0f, r[3]);
    }
    
    TEST(operatorMultiplyMatrix4)
    {
        Matrix4 r = Matrix4::identity() * Matrix4::identity();
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
        
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));

        r = Matrix4::identity() * m;
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);
        
        r = m * Matrix4::identity();

        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);
    }
    
    TEST(operatorMultiplyTransform3)
    {
        Matrix4 r = Matrix4::identity() * Transform3::identity();
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
        
        // value * identity        
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        r = m * Transform3::identity();
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);     
        
        Transform3 t(Vector3(0.0f, 0.1f, 0.2f),
                     Vector3(0.4f, 0.5f, 0.6f),
                     Vector3(0.8f, 0.9f, 1.0f),
                     Vector3(1.2f, 1.3f, 1.4f));
        
        r = Matrix4::identity() * t;
        
        // identity * value
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.0f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.0f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(0.0f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.0f, r[3][3]);
    }
    
    TEST(operatorAddEquals)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = Matrix4::identity();
        r += m;
        
        CHECK_EQUAL(1.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(1.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(2.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(2.5f, r[3][3]);
    }
    
    TEST(operatorMinusEquals)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = m;
        r -= Matrix4::identity();
        
        CHECK_EQUAL(-1.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(-0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(0.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(0.5f, r[3][3]);
    }

    TEST(operatorMultiplyEqualScalar)
    {
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        Matrix4 r = m;
        r *= 2.0f;
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.2f, r[0][1]);
        CHECK_EQUAL(0.4f, r[0][2]);
        CHECK_EQUAL(0.6f, r[0][3]);
        
        CHECK_EQUAL(0.8f, r[1][0]);
        CHECK_EQUAL(1.0f, r[1][1]);
        CHECK_EQUAL(1.2f, r[1][2]);
        CHECK_EQUAL(1.4f, r[1][3]);
        
        CHECK_EQUAL(1.6f, r[2][0]);
        CHECK_EQUAL(1.8f, r[2][1]);
        CHECK_EQUAL(2.0f, r[2][2]);
        CHECK_EQUAL(2.2f, r[2][3]);
        
        CHECK_EQUAL(2.4f, r[3][0]);
        CHECK_EQUAL(2.6f, r[3][1]);
        CHECK_EQUAL(2.8f, r[3][2]);
        CHECK_EQUAL(3.0f, r[3][3]);
    }
    
    TEST(operatorMultiplyEqualMatrix4)
    {
        Matrix4 r = Matrix4::identity();
        r *= Matrix4::identity();
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
        
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        
        r = Matrix4::identity();
        r *= m;
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);
        
        r = m;
        r *= Matrix4::identity();
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);
    }
    
    TEST(operatorMultiplyEqualTransform3)
    {
        Matrix4 r = Matrix4::identity();
        r *= Transform3::identity();
        
        CHECK_EQUAL(1.f, r[0][0]);
        CHECK_EQUAL(0.f, r[0][1]);
        CHECK_EQUAL(0.f, r[0][2]);
        CHECK_EQUAL(0.f, r[0][3]);
        
        CHECK_EQUAL(0.f, r[1][0]);
        CHECK_EQUAL(1.f, r[1][1]);
        CHECK_EQUAL(0.f, r[1][2]);
        CHECK_EQUAL(0.f, r[1][3]);
        
        CHECK_EQUAL(0.f, r[2][0]);
        CHECK_EQUAL(0.f, r[2][1]);
        CHECK_EQUAL(1.f, r[2][2]);
        CHECK_EQUAL(0.f, r[2][3]);
        
        CHECK_EQUAL(0.f, r[3][0]);
        CHECK_EQUAL(0.f, r[3][1]);
        CHECK_EQUAL(0.f, r[3][2]);
        CHECK_EQUAL(1.f, r[3][3]);
        
        // value * identity        
        Matrix4 m(Vector4(0.0f, 0.1f, 0.2f, 0.3f),
                  Vector4(0.4f, 0.5f, 0.6f, 0.7f),
                  Vector4(0.8f, 0.9f, 1.0f, 1.1f),
                  Vector4(1.2f, 1.3f, 1.4f, 1.5f));
        r = m;
        r *= Transform3::identity();
        
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.3f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.7f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(1.1f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.5f, r[3][3]);     
        
        Transform3 t(Vector3(0.0f, 0.1f, 0.2f),
                     Vector3(0.4f, 0.5f, 0.6f),
                     Vector3(0.8f, 0.9f, 1.0f),
                     Vector3(1.2f, 1.3f, 1.4f));
        
        r = Matrix4::identity();
        r *= t;
        
        // identity * value
        CHECK_EQUAL(0.0f, r[0][0]);
        CHECK_EQUAL(0.1f, r[0][1]);
        CHECK_EQUAL(0.2f, r[0][2]);
        CHECK_EQUAL(0.0f, r[0][3]);
        
        CHECK_EQUAL(0.4f, r[1][0]);
        CHECK_EQUAL(0.5f, r[1][1]);
        CHECK_EQUAL(0.6f, r[1][2]);
        CHECK_EQUAL(0.0f, r[1][3]);
        
        CHECK_EQUAL(0.8f, r[2][0]);
        CHECK_EQUAL(0.9f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(0.0f, r[2][3]);
        
        CHECK_EQUAL(1.2f, r[3][0]);
        CHECK_EQUAL(1.3f, r[3][1]);
        CHECK_EQUAL(1.4f, r[3][2]);
        CHECK_EQUAL(1.0f, r[3][3]);
    }
    
    TEST(identity)
    {
        const Matrix4 r = Matrix4::identity();
        
        CHECK_EQUAL(1.0f, r[0][0]);
        CHECK_EQUAL(0.0f, r[0][1]);
        CHECK_EQUAL(0.0f, r[0][2]);
        CHECK_EQUAL(0.0f, r[0][3]);
        
        CHECK_EQUAL(0.0f, r[1][0]);
        CHECK_EQUAL(1.0f, r[1][1]);
        CHECK_EQUAL(0.0f, r[1][2]);
        CHECK_EQUAL(0.0f, r[1][3]);
        
        CHECK_EQUAL(0.0f, r[2][0]);
        CHECK_EQUAL(0.0f, r[2][1]);
        CHECK_EQUAL(1.0f, r[2][2]);
        CHECK_EQUAL(0.0f, r[2][3]);
        
        CHECK_EQUAL(0.0f, r[3][0]);
        CHECK_EQUAL(0.0f, r[3][1]);
        CHECK_EQUAL(0.0f, r[3][2]);
        CHECK_EQUAL(1.0f, r[3][3]);
    }
    
    TEST(rotationX)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix4 m = Matrix4::rotationX(radians);
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
        CHECK_EQUAL(1.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);
        
        CHECK_EQUAL(0.f,  m[1][0]);
        CHECK_CLOSE(cos0, m[1][1], kEpsilon);
        CHECK_CLOSE(sin0, m[1][2], kEpsilon);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_EQUAL(0.f,  m[2][0]);
        CHECK_CLOSE(-sin0,m[2][1], kEpsilon);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(rotationY)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix4 m = Matrix4::rotationY(radians);
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
        CHECK_CLOSE(cos0, m[0][0], kEpsilon);
        CHECK_EQUAL(0.f,  m[0][1]);
        CHECK_CLOSE(-sin0,m[0][2], kEpsilon);
        CHECK_EQUAL(0.f, m[0][3]);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_EQUAL(1.f, m[1][1]);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_CLOSE(sin0, m[2][0], kEpsilon);
        CHECK_EQUAL(0.f,  m[2][1]);
        CHECK_CLOSE(cos0, m[2][2], kEpsilon);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(rotationZ)
    {
        const float   radians = 45 * DEGTORAD;
        const Matrix4 m = Matrix4::rotationZ(radians);
        const float   sin0 = sin(radians);
        const float   cos0 = cos(radians);
        
        CHECK_CLOSE(cos0, m[0][0], kEpsilon);
        CHECK_CLOSE(sin0, m[0][1], kEpsilon);
        CHECK_EQUAL(0.f,  m[0][2]);
        CHECK_EQUAL(0.f,  m[0][3]);
        
        CHECK_CLOSE(-sin0, m[1][0], kEpsilon);
        CHECK_CLOSE(cos0,  m[1][1], kEpsilon);
        CHECK_EQUAL(0.f,   m[1][2]);
        CHECK_EQUAL(0.f,  m[1][3]);
        
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(1.f, m[2][2]);
        CHECK_EQUAL(0.f,  m[2][3]);

        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(rotationZYX)
    {
        const float radians = 45 * DEGTORAD;
        const Matrix4 z = Matrix4::rotationZ(radians);
        const Matrix4 y = Matrix4::rotationY(radians);
        const Matrix4 x = Matrix4::rotationX(radians);
        
        const Matrix4 expected = Matrix4::rotationZYX(Vector3(radians));
        const Matrix4 r = z * y * x; 
        
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                CHECK_CLOSE(expected[i][j], r[i][j], kEpsilon);
    }

    TEST(scale)
    {
        const Matrix4 m = Matrix4::scale(Vector3(1.1f, 2.2f, 3.3f));
        
        CHECK_EQUAL(1.1f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[0][3]);
        
        CHECK_EQUAL(0.0f, m[1][0]);
        CHECK_EQUAL(2.2f, m[1][1]);
        CHECK_EQUAL(0.0f, m[1][2]);
        CHECK_EQUAL(0.0f, m[1][3]);
        
        CHECK_EQUAL(0.0f, m[2][0]);
        CHECK_EQUAL(0.0f, m[2][1]);
        CHECK_EQUAL(3.3f, m[2][2]);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(translation)
    {
        const Matrix4 m = Matrix4::translation(Vector3(1.1f, 2.2f, 3.3f));
        
        CHECK_EQUAL(1.0f, m[0][0]);
        CHECK_EQUAL(0.0f, m[0][1]);
        CHECK_EQUAL(0.0f, m[0][2]);
        CHECK_EQUAL(0.0f, m[0][3]);
        
        CHECK_EQUAL(0.0f, m[1][0]);
        CHECK_EQUAL(1.0f, m[1][1]);
        CHECK_EQUAL(0.0f, m[1][2]);
        CHECK_EQUAL(0.0f, m[1][3]);
        
        CHECK_EQUAL(0.0f, m[2][0]);
        CHECK_EQUAL(0.0f, m[2][1]);
        CHECK_EQUAL(1.0f, m[2][2]);
        CHECK_EQUAL(0.0f, m[2][3]);
        
        CHECK_EQUAL(1.1f, m[3][0]);
        CHECK_EQUAL(2.2f, m[3][1]);
        CHECK_EQUAL(3.3f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(lookAt)
    {
        const Point3 eyePos(0.f, 0.f, 0.f);
        const Point3 target(10.f, 0.f, 0.f);
        const Vector3 up(0.f, 1.f, 0.f);
        const Matrix4 m = Matrix4::lookAt(eyePos, target, up);
     
        // left
        CHECK_EQUAL(0.f, m[0][0]);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(-1.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);

        // up 
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_CLOSE(1.f, m[1][1], kEpsilon);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        // forward
        CHECK_CLOSE(1.f, m[2][0], kEpsilon);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(0.f, m[2][2]);
        CHECK_EQUAL(0.f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL(0.f, m[3][2]);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
    TEST(perspective)
    {
        const float fovyRadians = 100 * DEGTORAD;
        const float aspect = 1.0f;
        const float zNear = 1.0f;
        const float zFar = 1000.0f;
        const Matrix4 m = Matrix4::perspective(fovyRadians, aspect, zNear, zFar);
        
        // tests
        const float ooTanHalfTheta = 1.0f / tanf(fovyRadians * 0.5f);
        
        CHECK_CLOSE(ooTanHalfTheta / aspect, m[0][0], kEpsilon);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);
      
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_CLOSE(ooTanHalfTheta, m[1][1], kEpsilon);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_EQUAL(-(zNear + zFar)/(zFar - zNear), m[2][2]);
        CHECK_EQUAL(-1.f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_EQUAL( (-2.f * zNear * zFar)/(zFar - zNear), m[3][2]);
        CHECK_EQUAL(0.f, m[3][3]);
    }
    
    TEST(frustum)
    {
        const float left = -100.f;
        const float right = 100.f;
        const float bottom = -100.f;
        const float top = 100.f;
        const float near = 1.f;
        const float far = 1000.f;
        const Matrix4 m = Matrix4::frustum(left, right, bottom, top, near, far);
        
        // tests
        const float A = (right + left)/(right - left);
        const float B = (top + bottom)/(top - bottom);
        const float C = (far + near)/(near - far);
        const float D = (2.f * far * near)/(near - far);
    
        CHECK_CLOSE((2.f * near)/(right - left), m[0][0], kEpsilon);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_CLOSE((2.f * near)/(top - bottom), m[1][1], kEpsilon);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_CLOSE(A, m[2][0], kEpsilon);
        CHECK_CLOSE(B, m[2][1], kEpsilon);
        CHECK_CLOSE(C, m[2][2], kEpsilon);
        CHECK_EQUAL(-1.f, m[2][3]);
        
        CHECK_EQUAL(0.f, m[3][0]);
        CHECK_EQUAL(0.f, m[3][1]);
        CHECK_CLOSE(D, m[3][2], kEpsilon);
        CHECK_EQUAL(0.f, m[3][3]);
    }
    
    TEST(orthographic)
    {
        const float left = -100.f;
        const float right = 100.f;
        const float bottom = -100.f;
        const float top = 100.f;
        const float near = 1.f;
        const float far = 1000.f;
        const Matrix4 m = Matrix4::orthographic(left, right, bottom, top, near, far);
        
        // tests
        const float tx = -(right+left)/(right-left);
        const float ty = -(top+bottom)/(top-bottom);
        const float tz = -(far+near)/(far-near);
        
        CHECK_CLOSE(2.f/(right-left), m[0][0], kEpsilon);
        CHECK_EQUAL(0.f, m[0][1]);
        CHECK_EQUAL(0.f, m[0][2]);
        CHECK_EQUAL(0.f, m[0][3]);
        
        CHECK_EQUAL(0.f, m[1][0]);
        CHECK_CLOSE(2.f/(top-bottom), m[1][1], kEpsilon);
        CHECK_EQUAL(0.f, m[1][2]);
        CHECK_EQUAL(0.f, m[1][3]);
        
        CHECK_EQUAL(0.f, m[2][0]);
        CHECK_EQUAL(0.f, m[2][1]);
        CHECK_CLOSE(-2.f/(far-near), m[2][2], kEpsilon);
        CHECK_EQUAL(0.f, m[2][3]);
        
        CHECK_EQUAL(tx, m[3][0]);
        CHECK_EQUAL(ty, m[3][1]);
        CHECK_CLOSE(tz, m[3][2], kEpsilon);
        CHECK_EQUAL(1.f, m[3][3]);
    }
    
}
