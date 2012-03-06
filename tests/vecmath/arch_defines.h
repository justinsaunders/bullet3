//
//  arch_defines.h
//  test_vecmath_osx
//
//  Created by Justin on 20/02/12.
//  Copyright (c) 2012 Justin Saunders. All rights reserved.
//

#ifndef test_vecmath_osx_arch_defines_h
#define test_vecmath_osx_arch_defines_h

// Supported architectures
#define TEST_VECMATH_ARCH_UNKNOWN   0
#define TEST_VECMATH_ARCH_X86       1
#define TEST_VECMATH_ARCH_ARM       2
#define TEST_VECMATH_ARCH_PPC       3

// Autodetect
#if defined(__i386__) || defined(__x86_64__) || defined(__x86_32)
    #define TEST_VECMATH_ARCH_CURRENT TEST_VECMATH_ARCH_X86
#elif defined ( __arm__ )
    #define TEST_VECMATH_ARCH_CURRENT TEST_VECMATH_ARCH_ARM
#elif defined ( __ppc__ )
    #define TEST_VECMATH_ARCH_CURRENT TEST_VECMATH_ARCH_PPC
#endif

// Nothing defined, fall back to ref
#if !defined( TEST_VECMATH_ARCH_CURRENT )
    #define TEST_VECMATH_ARCH_CURRENT TEST_VECMATH_ARCH_UNKNOWN
#endif

// Implementation types
#define TEST_VECMATH_IMPL_REF   0
#define TEST_VECMATH_IMPL_SSE   1
#define TEST_VECMATH_IMPL_NEON  2
#define TEST_VECMATH_IMPL_PPU   3

// Override an implementation with reference if user requests
#ifndef FORCE_REF_IMPL
    #if TEST_VECMATH_ARCH_CURRENT == TEST_VECMATH_ARCH_X86
        #define TEST_VECMATH_IMPL_CURRENT TEST_VECMATH_IMPL_SSE
        #include "vecmath/sse/vectormath_aos.h"
    #elif TEST_VECMATH_ARCH_CURRENT == TEST_VECMATH_ARCH_ARM
        #define TEST_VECMATH_IMPL_CURRENT TEST_VECMATH_IMPL_NEON
    #endif
#endif

// Nothing defined, fall back to ref
#if !defined( TEST_VECMATH_IMPL_CURRENT )
    #define TEST_VECMATH_IMPL_CURRENT TEST_VECMATH_IMPL_REF
    #include "vecmath/std/vectormath_aos.h"
#endif

// Some test utilities
#if TEST_VECMATH_IMPL_CURRENT != TEST_VECMATH_IMPL_SSE
static const float kEpsilon = 1e-5f;
#else
static const float kEpsilon = 5.6e-4f;
#endif

static const float DEGTORAD = M_PI/180;

#endif
