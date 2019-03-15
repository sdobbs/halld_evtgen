
# the test executables require ROOT for histogramming etc.
find_package(ROOT REQUIRED)

add_library(root_interface INTERFACE IMPORTED)
target_include_directories(root_interface INTERFACE ${ROOT_INCLUDE_DIRS})
target_link_libraries(root_interface INTERFACE ${ROOT_LIBRARIES})

# build each of the executables
foreach( test_exe evt_dalitz evtgenlhc_test1 example1 exampleWriteHepMC )
    add_executable(${test_exe} ${test_exe}.cc)
    target_link_libraries(${test_exe} PRIVATE EvtGen)
    if( ${EVTGEN_PYTHIA} OR ${EVTGEN_PHOTOS} OR ${EVTGEN_TAUOLA} )
        target_compile_definitions(${test_exe} PRIVATE EVTGEN_EXTERNAL)
        target_link_libraries(${test_exe} PRIVATE EvtGenExternal)
    endif()
    target_link_libraries(${test_exe} PRIVATE root_interface)
endforeach()

# install the executables
install(TARGETS evt_dalitz evtgenlhc_test1 example1 exampleWriteHepMC
    RUNTIME DESTINATION test
    )

# install the decay files, macros, scripts, etc.
install(DIRECTORY exampleFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(FILES do_tests
	      DESTINATION ${CMAKE_INSTALL_PREFIX}/test
	      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
