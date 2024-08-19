#--------------------------------------------------------------------------------
# Setup the project
CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)
PROJECT(G4Sim)

#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Set the policy to handle escape sequences correctly
if(POLICY CMP0053)
  CMAKE_POLICY(SET CMP0053 NEW)
endif()

# Define ANSI color codes using the correct escape sequence handling
STRING(ASCII 27 ESC)
SET(RESET   "${ESC}[0m")
SET(BOLD    "${ESC}[1m")
SET(RED     "${ESC}[31m")
SET(GREEN   "${ESC}[32m")
SET(YELLOW  "${ESC}[33m")
SET(BLUE    "${ESC}[34m")
SET(MAGENTA "${ESC}[35m")
SET(CYAN    "${ESC}[36m")

# Set author information
SET(AUTHOR_NAME "DIPTIRANJAN PATTANAIK")
SET(AUTHOR_ADDRESS "DIPTI.ASTROPART@GMAIL.COM")

# Define the box width
SET(BOX_WIDTH 40)  # Adjust this width based on your text length


#--------------------------------------------------------------------------------
# Set the C++ standard
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the compiler flag to disable shadow warnings
ADD_COMPILE_OPTIONS(-Wno-shadow)

#--------------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
OPTION(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)
else()
    FIND_PACKAGE(Geant4 REQUIRED)
endif()

FIND_PACKAGE(ROOT REQUIRED COMPONENTS RIO)

#--------------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
INCLUDE(${Geant4_USE_FILE} ${ROOT_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)
INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIRS})

#--------------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
FILE(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

FILE(GLOB MACRO_FILES
    "*.mac"
    )
FILE(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

FILE(GLOB MACRO_FILES)
FILE(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
#--------------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
ADD_EXECUTABLE(g4sim src/g4main.cc ${sources} ${headers})
TARGET_LINK_LIBRARIES(g4sim ${Geant4_LIBRARIES})
TARGET_LINK_LIBRARIES(g4sim ${ROOT_LIBRARIES})

ADD_CUSTOM_TARGET(G4Sim DEPENDS g4sim)
#--------------------------------------------------------------------------------
# Create the top and bottom borders of the box
STRING(REPEAT "-" ${BOX_WIDTH} BORDER)

# Print author information within a magenta-colored box
MESSAGE(STATUS "${MAGENTA}+${BORDER}+${RESET}")
MESSAGE(STATUS "${MAGENTA}| ${BOLD}${YELLOW}Author:${RESET} ${BOLD}${YELLOW}${AUTHOR_NAME}${RESET}${MAGENTA}          |${RESET}")
MESSAGE(STATUS "${MAGENTA}| ${BOLD}${YELLOW}Address:${RESET} ${BOLD}${YELLOW}${AUTHOR_ADDRESS}${RESET}${MAGENTA}     |${RESET}")
MESSAGE(STATUS "${MAGENTA}+${BORDER}+${RESET}")
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------

# Add a custom target to print the author name
ADD_CUSTOM_TARGET(success ALL
    COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/print_author_info.sh
)

# Make sure the author name is printed when building the main target
ADD_DEPENDENCIES(G4Sim success)
#--------------------------------------------------------------------------------
