#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8...3.19)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(hepemshow)

# Have linker set RPATH, not RUNPATH
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--disable-new-dtags")

#----------------------------------------------------------------------------
# Find G4HepEm (sets G4HepEm_geant4_FOUND if a complete, Geant4 dependent build)
find_package(G4HepEm REQUIRED)


#----------------------------------------------------------------------------
# Find Geant4: only if G4HepEm was built with Geant4
if(G4HepEm_geant4_FOUND)
  find_package(Geant4 REQUIRED)
endif()


#-------------------------------------------------------------------------------
# Set the headers, sources and include directory:
# For the Simulation application:
set(headers_SIM
  ${CMAKE_SOURCE_DIR}/Simulation/include/Box.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/EventLoop.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/Geometry.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/Hist.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/Physics.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/PrimaryGenerator.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/Results.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/SteppingLoop.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/TrackStack.hh
  ${CMAKE_SOURCE_DIR}/Simulation/include/URandom.hh
)

set(sources_SIM
  ${CMAKE_SOURCE_DIR}/Simulation/src/Box.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/EventLoop.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/Geometry.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/Hist.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/Physics.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/PrimaryGenerator.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/Results.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/SteppingLoop.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/TrackStack.cc
  ${CMAKE_SOURCE_DIR}/Simulation/src/URandom.cc
)

# For the Data-Generation application: only if G4HepEm was built with Geant4
if(G4HepEm_geant4_FOUND)
  set(headers_GEN
    ${CMAKE_SOURCE_DIR}/DataGeneration/include/G4Setup.hh
  )

  set(sources_GEN
    ${CMAKE_SOURCE_DIR}/DataGeneration/src/G4Setup.cc
  )
endif()


#----------------------------------------------------------------------------
# Build target(s):
#
# The Simulation application:
add_executable(HepEmShow
  ${CMAKE_SOURCE_DIR}/HepEmShow.cc
  ${sources_SIM}
 )

target_include_directories(HepEmShow
  PRIVATE
  ${CMAKE_SOURCE_DIR}/Simulation/include/
)

target_link_libraries(HepEmShow
  G4HepEm::g4HepEmData
  G4HepEm::g4HepEmDataJsonIO
)

# The Data-Generation application: only if G4HepEm was built with Geant4
if(G4HepEm_geant4_FOUND)
  add_executable(HepEmShow-DataGeneration
    ${CMAKE_SOURCE_DIR}/HepEmShow-DataGeneration.cc
    ${sources_GEN}
  )

  target_include_directories(HepEmShow-DataGeneration
    PRIVATE
    ${CMAKE_SOURCE_DIR}/DataGeneration/include/
  )

  target_link_libraries (HepEmShow-DataGeneration
    ${Geant4_LIBRARIES}
    G4HepEm::g4HepEmData
    G4HepEm::g4HepEmDataJsonIO
    G4HepEm::g4HepEmInit
    G4HepEm::g4HepEm
  )
endif()
