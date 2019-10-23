macro(add_covise_module category targetname)
    add_module(${targetname} ${ARGN})
endmacro(add_covise_module)

macro(covise_wnoerror)
endmacro()

if (UNIVIZ_VISTLE)
add_definitions(-DVISTLE)
endif()

add_subdirectory(libs)
add_subdirectory(modules)
