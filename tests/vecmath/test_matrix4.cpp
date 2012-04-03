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
}
