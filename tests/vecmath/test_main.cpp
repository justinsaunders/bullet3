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
    std::cout << "*** Test Vecmath ***\n";
    std::cout << "Architecture: " << sArchitectures[TEST_VECMATH_ARCH_CURRENT] << "\n";
    std::cout << "Implementation: " << sImplementations[TEST_VECMATH_IMPL_CURRENT];
#ifdef FORCE_REF_IMPL
    std::cout << " (Forcing reference implementation)\n";
#endif
    
    return UnitTest::RunAllTests();
}

