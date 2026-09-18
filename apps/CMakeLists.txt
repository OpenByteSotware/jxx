# In your CMakeLists.txt for the folder you want to scan
file(GLOB SUBDIR_CMAKELISTS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt
)

foreach(subdir_cmake ${SUBDIR_CMAKELISTS})
    get_filename_component(subdir_path ${subdir_cmake} DIRECTORY)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir_path}")
        message(STATUS "Adding subdirectory: ${subdir_path}")
        add_subdirectory(${subdir_path})
    endif()
endforeach()
