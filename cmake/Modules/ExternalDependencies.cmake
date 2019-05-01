
set(HEPMC2_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/HepMC" CACHE PATH "Location of HepMC 2 installation")
if(DEFINED ENV{PYTHIAVER})
    set(PYTHIA8_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/pythia$ENV{PYTHIAVER}" CACHE PATH "Location of Pythia8 installation")
else()
    set(PYTHIA8_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/pythia8230" CACHE PATH "Location of Pythia8 installation")
endif()
set(PHOTOSPP_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/PHOTOS" CACHE PATH "Location of Photos++ installation")
set(TAUOLAPP_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external/TAUOLA" CACHE PATH "Location of Tauola++ installation")

find_package(HepMC2 REQUIRED)
if(${EVTGEN_PYTHIA})
    find_package(Pythia8 REQUIRED)
endif()
if(${EVTGEN_PHOTOS})
    find_package(Photos++ REQUIRED)
endif()
if(${EVTGEN_TAUOLA})
    find_package(Tauola++ REQUIRED)
endif()
