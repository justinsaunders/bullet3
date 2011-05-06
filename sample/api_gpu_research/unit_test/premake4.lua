	
	hasCL = findOpenCL()
	
	if (hasCL) then

		project "gpu_research_unit_test"

		language "C++"
				
		kind "ConsoleApp"
		targetdir "../../../bin"
		includedirs {"../../../include/gpu_research"}
		
		links {
		"OpenCL"
		}
		
		files {
			"main.cpp",
			"RadixSortBenchmark.h",
			"UnitTests.h"
		}
		
	end