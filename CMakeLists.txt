if (UNIVIZ_VISTLE)
    vistle_category("UniViz" "vector field visualization modules by Filip Sadlo")

    add_definitions(-DVISTLE)
    add_subdirectory(libs/vistle_ext)
    add_subdirectory(modules/vistle)
endif()

if (UNIVIZ_COVISE)
    add_definitions(-DCOVISE)
    add_subdirectory(modules/covise)
endif()

