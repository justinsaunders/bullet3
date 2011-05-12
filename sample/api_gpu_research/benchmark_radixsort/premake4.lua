	
	hasCL = findOpenCL()
	
	if (hasCL) then

		project "gpu_benchmark_radixsort"

		language "C++"
				
		kind "ConsoleApp"
		targetdir "../../../bin"
		includedirs {"../../../include/gpu_research"}
		
		links {
		"OpenCL"
		}
		
		files {
			"test_large_problem_sorting.cpp",
		}
		
	end