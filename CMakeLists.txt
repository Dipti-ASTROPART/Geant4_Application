#--------------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(G4Sim)

#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Set the policy to handle escape sequences correctly
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW)
endif()

# Define ANSI color codes using the correct escape sequence handling
string(ASCII 27 ESC)
set(RESET   "${ESC}[0m")
set(BOLD    "${ESC}[1m")
set(RED     "${ESC}[31m")
set(GREEN   "${ESC}[32m")
set(YELLOW  "${ESC}[33m")
set(BLUE    "${ESC}[34m")
set(MAGENTA "${ESC}[35m")
set(CYAN    "${ESC}[36m")

# Set author information
set(AUTHOR_NAME "DIPTIRANJAN PATTANAIK")
set(AUTHOR_ADDRESS "DIPTI.ASTROPART@GMAIL.COM")

# Define the box width
set(BOX_WIDTH 40)  # Adjust this width based on your text length


#--------------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED COMPONENTS RIO)

#--------------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE} ${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${ROOT_INCLUDE_DIRS})

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the compiler flag to disable shadow warnings
add_compile_options(-Wno-shadow)

#--------------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

file(GLOB MACRO_FILES
    "*.mac"
    )
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB MACRO_FILES)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
#--------------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(g4sim src/g4main.cc ${sources} ${headers})
target_link_libraries(g4sim ${Geant4_LIBRARIES})
target_link_libraries(g4sim ${ROOT_LIBRARIES})

add_custom_target(G4Sim DEPENDS g4sim)
#--------------------------------------------------------------------------------
# Create the top and bottom borders of the box
string(REPEAT "-" ${BOX_WIDTH} BORDER)

# Print author information within a magenta-colored box
message(STATUS "${MAGENTA}+${BORDER}+${RESET}")
message(STATUS "${MAGENTA}| ${BOLD}${YELLOW}Author:${RESET} ${BOLD}${YELLOW}${AUTHOR_NAME}${RESET}${MAGENTA}          |${RESET}")
message(STATUS "${MAGENTA}| ${BOLD}${YELLOW}Address:${RESET} ${BOLD}${YELLOW}${AUTHOR_ADDRESS}${RESET}${MAGENTA}     |${RESET}")
message(STATUS "${MAGENTA}+${BORDER}+${RESET}")
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------

# Add a custom target to print the author name
add_custom_target(success ALL
    COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/print_author_info.sh
)

# Make sure the author name is printed when building the main target
add_dependencies(G4Sim success)
#--------------------------------------------------------------------------------
