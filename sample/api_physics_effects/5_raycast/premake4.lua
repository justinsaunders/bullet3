	project "5_raycast"
		
	kind "WindowedApp"
	targetdir "../../../bin"
	includedirs {"../../../include"}
		
	links {
		"physicseffects2_lowlevel",
		"physicseffects2_baselevel",
		"physicseffects2_util",
		"opengl32"
	}
	
	flags       {"WinMain"}
	
	files {
		"main.cpp",
		"physics_func.cpp",
		"barrel.h",
		"landscape.h",
		"../common/ctrl_func.win32.cpp",
		"../common/perf_func.win32.cpp",
		"../common/render_func.win32.cpp"
	}