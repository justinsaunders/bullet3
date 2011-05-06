	project "bullet_physics_util"
		
	kind "StaticLib"
	targetdir "../../build/lib"	
	includedirs {
		".",
		"../../../include"
	}
	files {
		"**.cpp",
		"../../../include/bullet_physics/util/**.h"

	}