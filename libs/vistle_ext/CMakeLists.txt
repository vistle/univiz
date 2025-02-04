set (univiz_HEADERS
    vistle_ext.h
  ../unifield/unifield.h
  ../unigeom/unigeom.h
  ../linalg/linalg.h
  ../unisys/unisys.h
  ../unstructured/unstructured.h
)

set (univiz_SOURCES
    vistle_ext.cpp
  ../unisys/unisys.cpp
  ../unifield/unifield.cpp
  ../unigeom/unigeom.cpp
  ../unstructured/unstructured.cpp
)

include_directories(../linalg)

vistle_add_library(vistle_univiz INTERNAL ${VISTLE_LIB_TYPE} ${univiz_SOURCES} ${univiz_HEADERS})
target_link_libraries(vistle_univiz PRIVATE vistle_core vistle_module)

