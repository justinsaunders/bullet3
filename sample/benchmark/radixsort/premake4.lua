	project "test_large_problem_sorting"
	
	hasCL = findOpenCL()
	
	if (hasCL) then
		language "C++"
				
		kind "ConsoleApp"
		targetdir "../../../bin"
		includedirs {"../../../include"}
		
		links {
--		"physicseffects2_baselevel",
		"OpenCL"
		}
		
		files {
			"test_large_problem_sorting.cpp",
		}
		
	end