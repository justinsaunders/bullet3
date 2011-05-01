solution "MySolution"

	configurations {"Debug", "Release"}
	configuration "Release"
		flags { "Optimize", "StaticRuntime", "NoRTTI", "NoExceptions"}
	configuration "Debug"
		flags { "Symbols", "StaticRuntime" , "NoRTTI", "NoExceptions"}
	platforms {"x32", "x64"}

	configuration "x64"		
		targetsuffix "_64"
	configuration {"x64", "debug"}
		targetsuffix "_x64_debug"
	configuration {"x64", "release"}
		targetsuffix "_x64"
	configuration {"x32", "debug"}
		targetsuffix "_debug"

	newoption {
    trigger     = "no-pedemos",
    description = "Disable Physics Effects demos"
  }
  
  newoption {
    trigger     = "no-pelibs",
    description = "Disable Physics Effects libraries"
  }
 
  
  newoption {
    trigger     = "no-capi",
    description = "Disable C-API and its demos"
  } 
  

	function findOpenCL()
	-- todo: add NVIDIA / Intel OpenCL environment vars
		configuration {}
		local amdopenclpath = os.getenv("AMDAPPSDKROOT")
		if (amdopenclpath) then
			defines { "ADL_ENABLE_CL" }
			includedirs {
				"$(AMDAPPSDKROOT)/include"				
			}
			configuration "x32"
				libdirs {"$(AMDAPPSDKROOT)/lib/x86"}
			configuration "x64"
				libdirs {"$(AMDAPPSDKROOT)/lib/x86_64"}
			configuration {}
			return true
		end
		return false
	end


	language "C++"
	location "build"
	targetdir "bin"

--	include "../sample/benchmark/radixsort"

-- the c-api option is work in progress and disabled by default
if  _OPTIONS["with-capi"] then
	include "../sample/c_api/device"
	include "../sample/c_api/buffer"
	include "../sample/c_api/sort"
	include "../src/c_api"
end	
	
	if not _OPTIONS["no-pelibs"] then
		include "../src/physics_effects/base_level"
		include "../src/physics_effects/low_level"
		include "../src/physics_effects/util"
	end
	
  
	if not _OPTIONS["no-pedemos"] and not _OPTIONS["no-pelibs"] then
		include "../sample/api_physics_effects/0_console"
		include "../sample/api_physics_effects/1_simple"
		include "../sample/api_physics_effects/2_stable"
		include "../sample/api_physics_effects/3_sleep"
		include "../sample/api_physics_effects/3_sleep"
		include "../sample/api_physics_effects/4_motion_type"
		include "../sample/api_physics_effects/5_raycast"
		include "../sample/api_physics_effects/6_joint"
  end
	
--	include "../sample/graphics/20_opengltest"