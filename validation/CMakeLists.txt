
# the validation executables require ROOT for histogramming etc.
find_package(ROOT REQUIRED)

add_library(root_interface INTERFACE IMPORTED)
target_include_directories(root_interface INTERFACE ${ROOT_INCLUDE_DIRS})
target_link_libraries(root_interface INTERFACE ${ROOT_LIBRARIES})

# build each of the executables
foreach( validation_exe compareRootFiles genExampleRootFiles genRootDecayChain testCPVDecays )
    add_executable(${validation_exe} ${validation_exe}.cc)
    target_link_libraries(${validation_exe} PRIVATE EvtGen)
    if( ${EVTGEN_PYTHIA} OR ${EVTGEN_PHOTOS} OR ${EVTGEN_TAUOLA} )
        target_compile_definitions(${validation_exe} PRIVATE EVTGEN_EXTERNAL)
        target_link_libraries(${validation_exe} PRIVATE EvtGenExternal)
    endif()
    target_link_libraries(${validation_exe} PRIVATE root_interface)
endforeach()

# install the executables
install(TARGETS compareRootFiles genExampleRootFiles genRootDecayChain testCPVDecays
    RUNTIME DESTINATION validation
    )

# install the decay files, macros, scripts, etc.
install(DIRECTORY B0Files DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY CPVDecayFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY UpsilonFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY TauolaFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY Bs0Files DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY tauFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY rootFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY PHSPFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY BpFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY gifFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(DIRECTORY DalitzFiles DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(FILES BKstarGamma.dec BuDst0rhop.dec Kspimumu.dec photosTest.dec DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(FILES compareDalitz.C PhaseSpacePlots.C photosPlots.C plotBKstarGamma.C PlotKspimumu.C DESTINATION ${CMAKE_INSTALL_PREFIX}/validation)
install(FILES compareAllDecays.sh
	      compareB0PythiaDecays.sh
	      compareBpPythiaDecays.sh
	      compareBs0PythiaDecays.sh
	      compareTauolaDecays.sh
	      compareTauPythiaDecays.sh
	      compareUpsilonPythiaDecays.sh
	      genAllDecayExamples.sh
	      genB0PythiaDecays.sh
	      genBpPythiaDecays.sh
	      genBs0PythiaDecays.sh
	      genDalitzDecays.sh
	      genDDalitzModes.sh
	      genPHSP.sh
	      genTauolaDecays.sh
	      genTauPythiaDecays.sh
	      genUpsilonPythiaDecays.sh
	      runKspimumu.sh
	      runPhotosTest.sh
	      DESTINATION ${CMAKE_INSTALL_PREFIX}/validation
	      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
