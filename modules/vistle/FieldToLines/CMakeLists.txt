SET(HEADERS
  FieldToLines.h
)

SET(SOURCES
  FieldToLines.cpp
)

include_directories(../../../libs/linalg
../../../libs/unifield
../../../libs/unigeom
../../../libs/unstructured
../../../libs/unisys
../../../libs/vistle_ext
../../impl/field_to_lines
)

add_module(FieldToLines ${SOURCES} ${HEADERS})
target_link_libraries(FieldToLines vistle_univiz)

#COVISE_INSTALL_TARGET(FieldToLines)
