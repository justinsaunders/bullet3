	project "3_bt_simpleconvex"
		
	kind "WindowedApp"
	targetdir "../../../bin"
	includedirs {"../../../include"}
		
	links {
		"bullet_physics_util",
		"bullet_physics_base_level",
		"physicseffects2_lowlevel",
		"physicseffects2_baselevel",
		"physicseffects2_util",
		"opengl32"
	}
	
	flags       {"WinMain"}
	
	files {
		"main.cpp",
		"physics_func.cpp",
		"../common/ctrl_func.win32.cpp",
		"../common/perf_func.win32.cpp",
		"../common/render_func.win32.cpp",
	}