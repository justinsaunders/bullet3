	
	hasCL = findOpenCL_INTEL()
	
	if (hasCL) then

		project "gpu_research_unit_test_INTEL"

		initOpenCL_INTEL()
		
		language "C++"
				
		kind "ConsoleApp"
		targetdir "../../../../bin"
		includedirs {"..", "../../../../include/gpu_research"}
		
		links {
		"OpenCL"
		}
		
		files {
			"../main.cpp",
			"../RadixSortBenchmark.h",
			"../UnitTests.h"
		}
		
	end