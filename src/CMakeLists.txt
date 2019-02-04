
# Use glob to find the sources for the main and external libraries
file(GLOB EVTGEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EvtGenBase/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EvtGenModels/*.cpp
    )

file(GLOB EVTGEN_EXTERNAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/EvtGenExternal/*.cpp
    )


# Add the main EvtGen library...
add_library(objlib OBJECT ${EVTGEN_SOURCES})
set_target_properties(objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlib PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(objlib PRIVATE ${HEPMC2_INCLUDE_DIR})
target_compile_definitions(objlib PRIVATE EVTGEN_CPP11)

add_library(EvtGen SHARED $<TARGET_OBJECTS:objlib>)
set_target_properties(EvtGen PROPERTIES OUTPUT_NAME EvtGen)
set_target_properties(EvtGen PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR} )
set_target_properties(EvtGen PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
target_include_directories(EvtGen PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(EvtGen PUBLIC ${HEPMC2_INCLUDE_DIR})
target_compile_definitions(EvtGen PUBLIC EVTGEN_CPP11)
target_link_libraries(EvtGen ${HEPMC2_LIBRARIES})

add_library(EvtGenStatic STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties(EvtGenStatic PROPERTIES OUTPUT_NAME EvtGen)
set_target_properties(EvtGenStatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/archive)
target_include_directories(EvtGenStatic PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(EvtGenStatic PUBLIC ${HEPMC2_INCLUDE_DIR})
target_compile_definitions(EvtGenStatic PUBLIC EVTGEN_CPP11)
target_link_libraries(EvtGenStatic ${HEPMC2_LIBRARIES})


# Add the EvtGenExternal library...
if( ${EVTGEN_PYTHIA} OR ${EVTGEN_PHOTOS} OR ${EVTGEN_TAUOLA} )
    add_library(objlib_ext OBJECT ${EVTGEN_EXTERNAL_SOURCES})
    set_target_properties(objlib_ext PROPERTIES POSITION_INDEPENDENT_CODE 1)
    target_include_directories(objlib_ext PRIVATE ${CMAKE_SOURCE_DIR})
    target_include_directories(objlib_ext PRIVATE ${HEPMC2_INCLUDE_DIR})
    target_compile_definitions(objlib_ext PRIVATE EVTGEN_CPP11)
    if(${EVTGEN_PYTHIA})
        target_include_directories(objlib_ext PRIVATE ${PYTHIA8_INCLUDE_DIR})
        target_compile_definitions(objlib_ext PRIVATE EVTGEN_PYTHIA)
    endif()
    if(${EVTGEN_PHOTOS})
        target_include_directories(objlib_ext PRIVATE ${PHOTOS++_INCLUDE_DIR})
        target_compile_definitions(objlib_ext PRIVATE EVTGEN_PHOTOS)
    endif()
    if(${EVTGEN_TAUOLA})
        target_include_directories(objlib_ext PRIVATE ${TAUOLA++_INCLUDE_DIR})
        target_compile_definitions(objlib_ext PRIVATE EVTGEN_TAUOLA)
    endif()

    add_library(EvtGenExternal SHARED $<TARGET_OBJECTS:objlib_ext>)
    set_target_properties(EvtGenExternal PROPERTIES OUTPUT_NAME EvtGenExternal)
    set_target_properties(EvtGenExternal PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR} )
    set_target_properties(EvtGenExternal PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    target_link_libraries(EvtGenExternal ${HEPMC2_LIBRARIES})
    if(${EVTGEN_PYTHIA})
        target_link_libraries(EvtGenExternal ${PYTHIA8_LIBRARIES})
    endif()
    if(${EVTGEN_PHOTOS})
        target_link_libraries(EvtGenExternal ${PHOTOS++_LIBRARIES})
    endif()
    if(${EVTGEN_TAUOLA})
        target_link_libraries(EvtGenExternal ${TAUOLA++_LIBRARIES})
    endif()

    add_library(EvtGenExternalStatic STATIC $<TARGET_OBJECTS:objlib_ext>)
    set_target_properties(EvtGenExternalStatic PROPERTIES OUTPUT_NAME EvtGenExternal)
    set_target_properties(EvtGenExternalStatic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/archive)
    target_link_libraries(EvtGenExternalStatic ${HEPMC2_LIBRARIES})
    if(${EVTGEN_PYTHIA})
        target_link_libraries(EvtGenExternalStatic ${PYTHIA8_LIBRARIES})
    endif()
    if(${EVTGEN_PHOTOS})
        target_link_libraries(EvtGenExternalStatic ${PHOTOS++_LIBRARIES})
    endif()
    if(${EVTGEN_TAUOLA})
        target_link_libraries(EvtGenExternalStatic ${TAUOLA++_LIBRARIES})
    endif()
endif()

# Install the libraries
install(
    TARGETS EvtGen EvtGenStatic
    EXPORT "EvtGenTargets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/archive
    )

if( ${EVTGEN_PYTHIA} OR ${EVTGEN_PHOTOS} OR ${EVTGEN_TAUOLA} )
    install(
        TARGETS EvtGenExternal EvtGenExternalStatic
        EXPORT "EvtGenTargets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/archive
        )
endif()
