SET(HEADERS
    VortexCriteria.h
)

SET(SOURCES
    VortexCriteria.cpp
)

include_directories(../../../libs/linalg
../../../libs/unifield
../../../libs/unigeom
../../../libs/unstructured
../../../libs/unisys
../../../libs/vistle_ext
../../impl/vortex_criteria
)

add_module(VortexCriteria VortexCriteria.cpp)
target_link_libraries(VortexCriteria vistle_univiz)

#COVISE_INSTALL_TARGET(VortexCores)
