cmake_minimum_required(VERSION 3.16)
project(oreon-system-manager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Find system packages
find_package(PkgConfig REQUIRED)

# Set up Qt6 paths
qt6_standard_project_setup()

# Include directories
include_directories(src)

# Source files - ALL FUNCTIONALITY RESTORED
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/systemutils.cpp
    src/privilegedexecutor.cpp
    src/packagemanager.cpp
    src/repositorymanager.cpp
    src/containermanager.cpp
    src/audiomanager.cpp
    src/drivermanager.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/systemutils.h
    src/privilegedexecutor.h
    src/packagemanager.h
    src/repositorymanager.h
    src/containermanager.h
    src/audiomanager.h
    src/drivermanager.h
)

# UI files
set(UIS
    ui/mainwindow.ui
)

# Process UI files
qt6_add_resources(RESOURCES resources/resources.qrc)

# Add executable with Qt6
qt6_add_executable(oreon-system-manager
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RESOURCES}
)

# Link Qt6 libraries using PRIVATE keyword
target_link_libraries(oreon-system-manager PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

# Set executable properties
set_target_properties(oreon-system-manager PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Install target
install(TARGETS oreon-system-manager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install desktop file
install(FILES resources/oreon-system-manager.desktop
    DESTINATION share/applications
)

# Set version and other properties
set_target_properties(oreon-system-manager PROPERTIES
    VERSION ${PROJECT_VERSION}
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Set include directories for UI files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui) 