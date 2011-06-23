	
	hasCL = findOpenCL_AMD()
	hasDX11 = findDirectX11()
	
	if (hasCL) then

		project "gpu_research_unit_test_AMD"

		initOpenCL_AMD()

		if (hasDX11) then
			initDirectX11()
		end
		
		language "C++"
				
		kind "ConsoleApp"
		targetdir "../../../../bin"
		includedirs {"..","../../../../include/gpu_research"}
		
		links {
		"OpenCL"
		}
		
		files {
			"../main.cpp",
			"../RadixSortBenchmark.h",
			"../UnitTests.h"
		}
		
	end