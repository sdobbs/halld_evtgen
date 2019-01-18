# Top level CMakeLists.txt for EvtGen

# Enforce an out-of-source build.
# Should be the first action in the top level CMakeLists.txt
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "EvtGen requires an out-of-source build.")
    message(STATUS "Please remove the following files from ${CMAKE_BINARY_DIR}:")
    message(STATUS "    CMakeCache.txt")
    message(STATUS "    CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there, pointing it to:")
    message(STATUS "    ${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "in-source build detected")
endif()

# Also require a minimum version of CMake
cmake_minimum_required(VERSION 3.8.0)

# Project setup
if(${CMAKE_VERSION} VERSION_LESS 3.9.0)
    project(EvtGen
        VERSION 2.0.0
        )
elseif(${CMAKE_VERSION} VERSION_LESS 3.12.0)
    project(EvtGen
        VERSION 2.0.0
        DESCRIPTION "Monte Carlo generator of particle decays, in particular the weak decays of heavy flavour particles such as B mesons."
        )
else()
    project(EvtGen
        VERSION 2.0.0
        DESCRIPTION "Monte Carlo generator of particle decays, in particular the weak decays of heavy flavour particles such as B mesons."
        HOMEPAGE_URL "https://evtgen.hepforge.org"
        )
endif()

# Prepend this project's custom module path(s) to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
# Include needed modules to perform any custom setup
# Install paths
include(InstallPaths)
# Compilation/linking flags and related settings
include(CompilerFlags)
# EvtGen external dependencies
option(EVTGEN_PYTHIA "Enable/disable linking with Pythia8" OFF)
option(EVTGEN_PHOTOS "Enable/disable linking with Photos++" OFF)
option(EVTGEN_TAUOLA "Enable/disable linking with Tauola++" OFF)
message(STATUS "EvtGen: Linking with Pythia8  ${EVTGEN_PYTHIA}")
message(STATUS "EvtGen: Linking with Photos++ ${EVTGEN_PHOTOS}")
message(STATUS "EvtGen: Linking with Tauola++ ${EVTGEN_TAUOLA}")
include(ExternalDependencies)

# Now build the library
add_subdirectory(src)

# Copy the particle property and decay tables
configure_file(DECAY.DEC ${CMAKE_CURRENT_BINARY_DIR}/DECAY.DEC COPYONLY)
configure_file(DECAY.XML ${CMAKE_CURRENT_BINARY_DIR}/DECAY.XML COPYONLY)
configure_file(evt.pdl ${CMAKE_CURRENT_BINARY_DIR}/evt.pdl COPYONLY)

# Build the executables in the test and validation directories
add_subdirectory(test)
add_subdirectory(validation)

# Install the include directories
install(DIRECTORY EvtGen DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY EvtGenBase DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY EvtGenExternal DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY EvtGenModels DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install the particle properties and decay tables
install(FILES DECAY.DEC DECAY.XML evt.pdl DESTINATION ${CMAKE_INSTALL_PREFIX})

