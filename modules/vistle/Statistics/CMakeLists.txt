SET(HEADERS
    Statistics.h
)

SET(SOURCES
    Statistics.cpp
)

include_directories(../../../libs/linalg
../../../libs/unifield
../../../libs/unigeom
../../../libs/unstructured
../../../libs/unisys
../../../libs/vistle_ext
../../impl/statistics
)

add_module(Statistics "statistics" ${SOURCES} ${HEADERS})
target_link_libraries(Statistics vistle_univiz)

#COVISE_INSTALL_TARGET(Statistics)
