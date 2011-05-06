	
	hasCL = findOpenCL()
	
	if (hasCL) then

		project "test_large_problem_sorting"

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