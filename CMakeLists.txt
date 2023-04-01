cmake_minimum_required(VERSION 3.0.0)
project(image_processing VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(image_processing main.cc imageapp.cc mainwindow.cc Image.cc)
include_directories(include)

# List of UI files to copy
set(UI_FILES
    menubar.xml
    main_container.xml
)

# Copy UI files to the build directory
foreach(UI_FILE ${UI_FILES})
    configure_file(UI/${UI_FILE} ${CMAKE_CURRENT_BINARY_DIR}/UI/${UI_FILE} COPYONLY)
endforeach()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

target_link_libraries(image_processing ${GTKMM_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
