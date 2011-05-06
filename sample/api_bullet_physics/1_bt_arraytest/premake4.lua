	project "1_bt_arraytest"
		
	kind "ConsoleApp"
	targetdir "../../../bin"
	includedirs {
		"../../../include"	
		}

	flags { "Symbols"}
	
	includedirs {	"../common"		}
		
	links {
		"bullet_physics_util"
	}
	
	files {
		"main.cpp",
	}