	project "bullet_physics_base_level"
		
	kind "StaticLib"
	targetdir "../../build/lib"	
	includedirs {
		".",
		"../../../include",
	}
	files {
		"**.cpp",
		"../../../include/bullet_physics/base_level/**.h"
	}