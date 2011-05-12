	project "bt_sample_2_stable_convex"
		
	kind "WindowedApp"
	targetdir "../../../bin"
	includedirs {"../../../include"}
		
	links {
		"bullet_physics_base_level",
		"bullet_physics_util",
		"physics_effects_low_level",
		"physics_effects_base_level",
		"physics_effects_util",
		"opengl32"
	}
	
	flags       {"WinMain"}
	
	files {
		"main.cpp",
		"physics_func.cpp",
		"../common/ctrl_func.win32.cpp",
		"../common/perf_func.win32.cpp",
		"../common/render_func.win32.cpp"
	}