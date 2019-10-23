SET(HEADERS
    VortexCores.h
)

SET(SOURCES
    VortexCores.cpp
)

include_directories(../../../libs/linalg
../../../libs/unifield
../../../libs/unigeom
../../../libs/unstructured
../../../libs/unisys
../../../libs/vistle_ext
../../impl/vortex_cores
)

add_module(VortexCores VortexCores.cpp ../../impl/vortex_cores/computeVortexCores.cpp)
target_link_libraries(VortexCores vistle_univiz)

#COVISE_INSTALL_TARGET(VortexCores)
