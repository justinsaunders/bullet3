INCLUDE_DIRECTORIES( .  )

SET(BulletUtil_SRCS
					pfx_mass.cpp
					pfx_mesh_creator.cpp
)

SET(BulletUtil_HDRS
					pfx_array.h
					pfx_array_implementation.h
					pfx_util_common.h
)




ADD_LIBRARY(BulletUtil ${BulletUtil_SRCS} ${BulletUtil_HDRS})

SET_TARGET_PROPERTIES(BulletUtil PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletUtil PROPERTIES SOVERSION ${BULLET_VERSION})
