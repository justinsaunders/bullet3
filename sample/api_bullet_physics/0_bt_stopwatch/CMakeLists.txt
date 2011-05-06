cmake_minimum_required(VERSION 2.4)


#this line has to appear before 'PROJECT' in order to be able to disable incremental linking
SET(MSVC_INCREMENTAL_DEFAULT ON)

PROJECT(App_0_Basic)


SET(App_0_Basic_SRCS
	main.cpp
	../common/glut/btg_glut_demo.cpp
)

SET(App_0_Basic_HDRS
	../common/glut/btg_glut_demo.h
	../common/btg_demobase.h
)

INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/sample/api_bullet_physics/common
)


ADD_EXECUTABLE(App_0_Basic
	${App_0_Basic_SRCS}
	${App_0_Basic_HDRS}
)

TARGET_LINK_LIBRARIES(App_0_Console
)

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
		SET_TARGET_PROPERTIES(App_0_Basic PROPERTIES  DEBUG_POSTFIX "_Debug")
		SET_TARGET_PROPERTIES(App_0_Basic PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
		SET_TARGET_PROPERTIES(App_0_Basic PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF()



	
