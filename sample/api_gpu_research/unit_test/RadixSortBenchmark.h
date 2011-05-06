

template<DeviceType TYPE>
void run( Device* device, int minSize = 512, int maxSize = 64*1024 )//, int increment = 512 )
{
	ADLASSERT( TYPE == device->m_type );

	Stopwatch sw( device );

//	RadixSort<TYPE>::Data* data0 = RadixSort<TYPE>::allocate( device, maxSize, RadixSortBase::SORT_SIMPLE );
	RadixSort<TYPE>::Data* data0 = RadixSort<TYPE>::allocate( device, maxSize, RadixSortBase::SORT_STANDARD );
	RadixSort<TYPE>::Data* data1 = RadixSort<TYPE>::allocate( device, maxSize, RadixSortBase::SORT_STANDARD );
	RadixSort<TYPE>::Data* data2 = RadixSort<TYPE>::allocate( device, maxSize, RadixSortBase::SORT_ADVANCED );

	Buffer<SortData> buf0( device, maxSize );
	Buffer<SortData> buf1( device, maxSize );
	Buffer<SortData> buf2( device, maxSize );

	SortData* input = new SortData[ maxSize ];

//	for(int iter = minSize; iter<=maxSize; iter+=increment)
	for(int iter = minSize; iter<=maxSize; iter*=2)
	{
		int size = NEXTMULTIPLEOF( iter, 512 );

		for(int i=0; i<size; i++) input[i] = SortData( getRandom(0,0xff), i );

		buf0.write( input, size );
		buf1.write( input, size );
		buf2.write( input, size );
		DeviceUtils::waitForCompletion( device );


		sw.start();

		RadixSort<TYPE>::execute( data0, buf0, size );

		sw.split();

		RadixSort<TYPE>::execute( data1, buf1, size );

		sw.split();

		RadixSort<TYPE>::execute( data2, buf2, size );

		sw.stop();


		float t[3];
		sw.getMs( t, 3 );
//		printf("	%d	%3.2f	%3.2f	%3.2f\n", size, t[0], t[1], t[2]);
		printf("	%d	%3.2f	%3.2f\n", size, t[1], t[2]);
	}

	RadixSort<TYPE>::deallocate( data0 );
	RadixSort<TYPE>::deallocate( data1 );
	RadixSort<TYPE>::deallocate( data2 );

	delete [] input;
}

template<DeviceType TYPE>
void radixSortBenchmark()
{
	Device* device;
	{
		DeviceUtils::Config cfg;
		device = DeviceUtils::allocate( TYPE, cfg );
	}

	run<TYPE>( device, 512, 1024*128*4 );

	DeviceUtils::deallocate( device );
}
