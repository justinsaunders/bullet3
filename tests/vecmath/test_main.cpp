//
//  main.cpp
//  test_vecmath_osx
//
//  Created by Justin on 16/02/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//
#include <iostream>
#include "arch_defines.h"
#include "UnitTest++.h"

// Must match arch_defines.h
static const char* sArchitectures[] = 
{    
    "Unknown",
    "x86",
    "ARM",
    "PPC" 
};

static const char* sImplementations[] = 
{
    "Reference (FPU)",
    "SSE",
    "NEON",
    "PPU"
};

int main (int argc, const char *argv[])
{
    std::cout << "Test Vecmath:" << sArchitectures[TEST_VECMATH_ARCH_CURRENT]
        << ", " << sImplementations[TEST_VECMATH_IMPL_CURRENT] << "\n";
    
    return UnitTest::RunAllTests();
}

