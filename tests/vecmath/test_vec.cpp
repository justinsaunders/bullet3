//
//  test_vec.cpp
//  test_vecmath_osx
//
//  Created by Justin on 16/02/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#include "UnitTest++.h"

#include "vecmath/sse/vectormath_aos.h"

using namespace Vectormath::Aos;

SUITE(Vector3)
{
    TEST(construct_float3)
    {
        Vector3 src(0.0f, 1.1f, 2.2f);
        CHECK_EQUAL(0.0f, src.getX());
        CHECK_EQUAL(1.1f, src.getY());
        CHECK_EQUAL(2.2f, src.getZ());
    }
}



