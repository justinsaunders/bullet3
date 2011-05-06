/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2011 Advanced Micro Devices, Inc.  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
//Author Takahiro Harada

#include <AdlPrimitives/Scan/PrefixScan.h>
#include <AdlPrimitives/Sort/RadixSort.h>
#include <AdlPrimitives/Search/BoundSearch.h>
#include <AdlPrimitives/Fill/Fill.h>


#define NUM_TESTS 10

int g_nPassed = 0;
int g_nFailed = 0;


#define TEST_INIT bool g_testFailed = 0;
#define TEST_ASSERT(x) if( !(x) ){g_testFailed = 1;}
//#define TEST_ASSERT(x) if( !(x) ){g_testFailed = 1;ADLASSERT(x);}
#define TEST_REPORT(testName) printf("[%s] %s\n",(g_testFailed)?"X":"O", testName); if(g_testFailed) g_nFailed++; else g_nPassed++;

void memCpyTest( Device* deviceData )
{
	TEST_INIT;
	int maxSize = 64*1024;
	Buffer<u32> buff( deviceData, maxSize );

	u32* hostBuff = new u32[maxSize];

	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = getRandom( 1024, maxSize );

		for(int i=0; i<size; i++) hostBuff[i] = i;

		buff.write( hostBuff, size );

		DeviceUtils::waitForCompletion( deviceData );
		for(int i=0; i<size; i++) hostBuff[i] = 0;

		buff.read( hostBuff, size );

		DeviceUtils::waitForCompletion( deviceData );
		for(int i=0; i<size; i++) TEST_ASSERT( hostBuff[i] == i );
	}

	delete [] hostBuff;
	TEST_REPORT( "memCpyTest" );
}

void kernelTest( Device* deviceData )
{
	TEST_INIT;
	Kernel* kernel = KernelManager::query( deviceData, ".\\Kernel", "VectorAddKernel" );

	{
		int size = 1024;
		Buffer<int> buf0( deviceData, size );
		Buffer<int> buf1( deviceData, size );
		Buffer<float4> cBuf( deviceData, 1, BufferBase::BUFFER_CONST );
		int* hostBuf0 = new int[size];
		int* hostBuf1 = new int[size];
		for(int i=0; i<size; i++) { hostBuf0[i] = i; hostBuf1[i] = 1; }
		buf0.write( hostBuf0, size );
		buf1.write( hostBuf1, size );
		DeviceUtils::waitForCompletion( deviceData );

		float4 constBuffer;
		constBuffer.x = (float)size;
		constBuffer.y = 2.f;
		constBuffer.z = 0.f;
		constBuffer.w = 0.f;
		{
			Launcher::BufferInfo bInfo[] = { Launcher::BufferInfo( (Buffer<float>*)&buf0 ), Launcher::BufferInfo( (Buffer<float>*)&buf1, true ) };

			Launcher launcher( deviceData, kernel );
			launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(Launcher::BufferInfo) );
			launcher.setConst( (Buffer<float4>&)cBuf, constBuffer );
			launcher.launch1D( size );

			buf0.read( hostBuf0, size );
			buf1.read( hostBuf1, size );
			DeviceUtils::waitForCompletion( deviceData );
		}

		for(int i=0; i<size; i++) { TEST_ASSERT( hostBuf0[i] == i+1+2 ); }

		delete [] hostBuf0;
		delete [] hostBuf1;
	}
	TEST_REPORT( "kernelTest" );
}

void stopwatchTest( Device* deviceData )
{
	{
		Stopwatch sw( deviceData );

		sw.start();
		Sleep(2);
		sw.split();
		Sleep(2);
		sw.stop();

		float t[2];
		sw.getMs( t, 2 );
	}
}

template<DeviceType type>
void scanTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;

	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;

	HostBuffer<u32> buf0( deviceHost, maxSize );
	HostBuffer<u32> buf1( deviceHost, maxSize );
	Buffer<u32> buf2( deviceGPU, maxSize );
	Buffer<u32> buf3( deviceGPU, maxSize );

	PrefixScan<type>::Data* data0 = PrefixScan<type>::allocate( deviceGPU, maxSize );
	PrefixScan<TYPE_HOST>::Data* data1 = PrefixScan<TYPE_HOST>::allocate( deviceHost, maxSize );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize );

		for(int i=0; i<size; i++) buf0[i] = 1;
		buf2.write( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );

		u32 sumHost, sumGPU;
		PrefixScan<TYPE_HOST>::execute( data1, buf0, buf1, size, &sumHost );
		PrefixScan<type>::execute( data0, buf2, buf3, size, &sumGPU );

		buf3.read( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );
		TEST_ASSERT( sumHost == sumGPU );
		for(int i=0; i<size; i++) TEST_ASSERT( buf1[i] == buf0[i] );
	}

	PrefixScan<TYPE_HOST>::deallocate( data1 );
	PrefixScan<type>::deallocate( data0 );

	TEST_REPORT( "scanTest" );
}

template<DeviceType type, RadixSortBase::Option SORT_TYPE>
bool radixSortTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;
	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;

	HostBuffer<SortData> buf0( deviceHost, maxSize );
	HostBuffer<SortData> buf1( deviceHost, maxSize );
	Buffer<SortData> buf2( deviceGPU, maxSize );

	RadixSort<TYPE_HOST>::Data* dataH = RadixSort<TYPE_HOST>::allocate( deviceHost, maxSize, RadixSortBase::SORT_SIMPLE );
	RadixSort<type>::Data* dataC = RadixSort<type>::allocate( deviceGPU, maxSize, SORT_TYPE );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize-512 );
		size = NEXTMULTIPLEOF( size, 512 );

		for(int i=0; i<size; i++) buf0[i] = SortData( getRandom(0,0xff), i );
		buf2.write( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );

		RadixSort<TYPE_HOST>::execute( dataH, buf0, size );
		RadixSort<type>::execute( dataC, buf2, size );

		buf2.read( buf1.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );
		for(int i=0; i<size; i++) TEST_ASSERT( buf0[i].m_value == buf1[i].m_value && buf0[i].m_key == buf1[i].m_key );
	}

	RadixSort<TYPE_HOST>::deallocate( dataH );
	RadixSort<type>::deallocate( dataC );

	return g_testFailed;
}

template<DeviceType type>
void radixSortSimpleTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;
	g_testFailed = radixSortTest<type, RadixSortBase::SORT_SIMPLE>(deviceGPU, deviceHost);
	TEST_REPORT( "radixSortSimpleTest" );
}

template<DeviceType type>
void radixSortStandardTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;
	g_testFailed = radixSortTest<type, RadixSortBase::SORT_STANDARD>(deviceGPU, deviceHost);
	TEST_REPORT( "radixSortStandardTest" );
}

template<DeviceType type>
void radixSortAdvancedTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;
	g_testFailed = radixSortTest<type, RadixSortBase::SORT_ADVANCED>(deviceGPU, deviceHost);
	TEST_REPORT( "radixSortAdvancedTest" );
}

template<DeviceType type>
void boundSearchTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;

	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;
	int bucketSize = 256;

	HostBuffer<SortData> buf0( deviceHost, maxSize );
	HostBuffer<u32> lowerH( deviceHost, maxSize );
	HostBuffer<u32> upperH( deviceHost, maxSize );

	Buffer<SortData> buf( deviceGPU, maxSize );
	Buffer<u32> lower( deviceGPU, maxSize );
	Buffer<u32> upper( deviceGPU, maxSize );

	BoundSearch<type>::Data* dataH = BoundSearch<type>::allocate( deviceGPU );
	RadixSort<TYPE_HOST>::Data* dataHSort = RadixSort<TYPE_HOST>::allocate( deviceHost, maxSize, RadixSortBase::SORT_SIMPLE );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize );
		for(int i=0; i<size; i++) buf0[i] = SortData( getRandom(0,bucketSize), i );
		RadixSort<TYPE_HOST>::execute( dataHSort, buf0, size );
		buf.write( buf0.m_ptr, size );
		{
			u32* host = new u32[size];
			for(int i=0; i<size; i++) host[i] = -1;
			lower.write( host, size );
			upper.write( host, size );
		}
		DeviceUtils::waitForCompletion( deviceGPU );

		BoundSearch<type>::execute( dataH, buf, size, lower, bucketSize, BoundSearchBase::BOUND_LOWER );
		BoundSearch<type>::execute( dataH, buf, size, upper, bucketSize, BoundSearchBase::BOUND_UPPER );

		lower.read( lowerH.m_ptr, bucketSize );
		upper.read( upperH.m_ptr, bucketSize );
		DeviceUtils::waitForCompletion( deviceGPU );
/*
		for(u32 i=1; i<(u32)bucketSize; i++)
		{
			for(u32 j=lowerH[i-1]; j<lowerH[i]; j++)
			{
				TEST_ASSERT( buf0[j].m_key < i );
			}
		}

		for(u32 i=0; i<(u32)bucketSize; i++)
		{
			int jMin = (i==0)?0:upperH[i-1];
			for(u32 j=jMin; j<upperH[i]; j++)
			{
				TEST_ASSERT( buf0[j].m_key <= i );
			}
		}
*/
		for(u32 i=0; i<(u32)bucketSize; i++)
		{
			for(u32 j=lowerH[i]; j<upperH[i]; j++)
			{
				TEST_ASSERT( buf0[j].m_key == i );
			}
		}

	}
	
	BoundSearch<type>::deallocate( dataH );
	RadixSort<TYPE_HOST>::deallocate( dataHSort );

	TEST_REPORT( "boundSearchTest" );
}

template<DeviceType type>
void fillIntTest( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;

	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;

	HostBuffer<int> buf0( deviceHost, maxSize );
	HostBuffer<int> buf1( deviceHost, maxSize );
	Buffer<int> buf2( deviceGPU, maxSize );

	Fill<TYPE_HOST>::Data* data0 = Fill<TYPE_HOST>::allocate( deviceHost );
	Fill<type>::Data* data1 = Fill<type>::allocate( deviceGPU );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize );
		for(int i=0; i<size; i++) buf0[i] = -1;
		buf2.write( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );

		Fill<TYPE_HOST>::execute( data0, buf0, 12, size );
		Fill<type>::execute( data1, buf2, 12, size );

		buf2.read( buf1.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );
		for(int i=0; i<size; i++) TEST_ASSERT( buf1[i] == buf0[i] );
	}

	Fill<TYPE_HOST>::deallocate( data0 );
	Fill<type>::deallocate( data1 );

	TEST_REPORT( "fillIntTest" );
}

template<DeviceType type>
void fillInt2Test( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;

	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;

	HostBuffer<int2> buf0( deviceHost, maxSize );
	HostBuffer<int2> buf1( deviceHost, maxSize );
	Buffer<int2> buf2( deviceGPU, maxSize );

	Fill<TYPE_HOST>::Data* data0 = Fill<TYPE_HOST>::allocate( deviceHost );
	Fill<type>::Data* data1 = Fill<type>::allocate( deviceGPU );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize );
		for(int i=0; i<size; i++) buf0[i] = make_int2( -1, -1 );
		buf2.write( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );

		Fill<TYPE_HOST>::execute( data0, buf0, make_int2( 12, 12 ), size );
		Fill<type>::execute( data1, buf2, make_int2( 12, 12 ), size );

		buf2.read( buf1.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );
		for(int i=0; i<size; i++) TEST_ASSERT( buf1[i] == buf0[i] );
	}

	Fill<TYPE_HOST>::deallocate( data0 );
	Fill<type>::deallocate( data1 );

	TEST_REPORT( "fillInt2Test" );
}

template<DeviceType type>
void fillInt4Test( Device* deviceGPU, Device* deviceHost )
{
	TEST_INIT;

	ADLASSERT( type == deviceGPU->m_type );

	int maxSize = 1024*256;

	HostBuffer<int4> buf0( deviceHost, maxSize );
	HostBuffer<int4> buf1( deviceHost, maxSize );
	Buffer<int4> buf2( deviceGPU, maxSize );

	Fill<TYPE_HOST>::Data* data0 = Fill<TYPE_HOST>::allocate( deviceHost );
	Fill<type>::Data* data1 = Fill<type>::allocate( deviceGPU );

	int dx = maxSize/NUM_TESTS;
	for(int iter=0; iter<NUM_TESTS; iter++)
	{
		int size = min2( 128+dx*iter, maxSize );
		for(int i=0; i<size; i++) buf0[i] = make_int4( -1 );
		buf2.write( buf0.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );

		Fill<TYPE_HOST>::execute( data0, buf0, make_int4( 12 ), size );
		Fill<type>::execute( data1, buf2, make_int4( 12 ), size );

		buf2.read( buf1.m_ptr, size );
		DeviceUtils::waitForCompletion( deviceGPU );
		for(int i=0; i<size; i++) TEST_ASSERT( buf1[i] == buf0[i] );
	}

	Fill<TYPE_HOST>::deallocate( data0 );
	Fill<type>::deallocate( data1 );

	TEST_REPORT( "fillInt4Test" );
}


#if defined(ADL_ENABLE_DX11)
	#define RUN_GPU( func ) func(ddcl); func(dddx);
	#define RUN_GPU_TEMPLATE( func ) func<TYPE_DX11>( dddx, ddhost ); func<TYPE_CL>( ddcl, ddhost ); 
#else
	#define RUN_GPU( func ) func(ddcl);
	#define RUN_GPU_TEMPLATE( func ) func<TYPE_CL>( ddcl, ddhost ); 
#endif
#define RUN_ALL( func ) RUN_GPU( func ); func(ddhost);

void runAllTest()
{
	g_nPassed = 0;
	g_nFailed = 0;

	AdlAllocate();

	Device* ddcl;
	Device* ddhost;
#if defined(ADL_ENABLE_DX11)
	Device* dddx;
#endif

	{
		DeviceUtils::Config cfg;
//		cfg.m_type = DeviceUtils::Config::DEVICE_CPU;
		ddcl = DeviceUtils::allocate( TYPE_CL, cfg );
		ddhost = DeviceUtils::allocate( TYPE_HOST, cfg );
//		cfg.m_type = DeviceUtils::Config::DEVICE_GPU;
#if defined(ADL_ENABLE_DX11)
		dddx = DeviceUtils::allocate( TYPE_DX11, cfg );
#endif
	}

	{
		boundSearchTest<TYPE_HOST>( ddhost, ddhost );
//		fillTest<TYPE_HOST>( ddhost, ddhost );
//		fillTest<TYPE_CL>( ddcl, ddhost );

		RUN_GPU_TEMPLATE( boundSearchTest );

		RUN_GPU_TEMPLATE( fillIntTest );
		RUN_GPU_TEMPLATE( fillInt2Test );
		RUN_GPU_TEMPLATE( fillInt4Test );

		RUN_ALL( stopwatchTest );
		RUN_ALL( memCpyTest );
//		RUN_GPU( kernelTest );
		RUN_GPU_TEMPLATE( scanTest );
		RUN_GPU_TEMPLATE( radixSortStandardTest );
		RUN_GPU_TEMPLATE( radixSortSimpleTest );
		RUN_GPU_TEMPLATE( boundSearchTest );
	}

	DeviceUtils::deallocate( ddcl );
	DeviceUtils::deallocate( ddhost );
#if defined(ADL_ENABLE_DX11)
	DeviceUtils::deallocate( dddx );
#endif

	printf("=========\n%d Passed\n%d Failed\n", g_nPassed, g_nFailed);


	AdlDeallocate();
}
