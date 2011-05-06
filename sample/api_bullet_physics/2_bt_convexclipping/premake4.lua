	project "2_bt_convexclipping"
		
	kind "ConsoleApp"
	targetdir "../../../bin"
	includedirs {
		"../../../include"	
		}

	flags { "Symbols"}
	
	includedirs {	"../common"		}
		
	links {
		"bullet_physics_util",
		"bullet_physics_base_level"
	}
	
	files {
		"main.cpp",
	}