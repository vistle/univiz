SET(HEADERS
    FlowTopo.h
)

SET(SOURCES
    FlowTopo.cpp
)

include_directories(../../../libs/linalg
../../../libs/unifield
../../../libs/unigeom
../../../libs/unstructured
../../../libs/unisys
../../../libs/vistle_ext
../../impl/flow_topo
)

add_module(FlowTopo "flow topology" ${SOURCES} ${HEADERS})
target_link_libraries(FlowTopo vistle_univiz)

#COVISE_INSTALL_TARGET(FlowTopo)
