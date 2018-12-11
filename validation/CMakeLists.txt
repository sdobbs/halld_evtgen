
# the validation executables require ROOT for histogramming etc.
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# build each of the executables
add_executable(testCPVDecays testCPVDecays.cc)
target_link_libraries(testCPVDecays PRIVATE EvtGen ${ROOT_LIBRARIES})
if( ${EVTGEN_PYTHIA} OR ${EVTGEN_PHOTOS} OR ${EVTGEN_TAUOLA} )
    target_compile_definitions(testCPVDecays PRIVATE EVTGEN_EXTERNAL)
    target_link_libraries(testCPVDecays PRIVATE EvtGenExternal)
endif()

# make sure that any decay files, etc. are also copied
file(GLOB CPV_DECAY_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/CPVDecayFiles/*.dec
    )
foreach(decfile ${CPV_DECAY_FILES})
    configure_file(${decfile} ${CMAKE_CURRENT_BINARY_DIR}/${decfile} COPYONLY)
endforeach()

# install the executables
install(TARGETS testCPVDecays
    RUNTIME DESTINATION validation
    )

# install the decay files, etc.
install(FILES ${CPV_DECAY_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/validation/CPVDecayFiles)

