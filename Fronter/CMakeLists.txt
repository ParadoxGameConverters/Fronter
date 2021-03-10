cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Fronter)
SET(CMAKE_CXX_COMPILER g++-10)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Source)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_compile_options(-lstdc++fs)
add_compile_options(-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread -Wall -O2 -std=c++20 -Wno-unused-variable)
add_link_options(-no-pie)

include_directories("../commonItems")
include_directories("/usr/include/wx-3.1-unofficial")
include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.1-unofficial3")
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB CONFIGURATION_OPTIONS_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB FRAMES_SOURCES "${PROJECT_SOURCE_DIR}/Frames/*.cpp")
file(GLOB FRAMES_TABS_SOURCES "${PROJECT_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB UTILS_SOURCES "${PROJECT_SOURCE_DIR}/Utils/*.cpp")
list(REMOVE_ITEM UTILS_SOURCES "${PROJECT_SOURCE_DIR}/Utils/WinFunctions.cpp")
file(GLOB UTILS_LOCALIZATION_SOURCES "${PROJECT_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
list(REMOVE_ITEM CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
file(GLOB LOGWATCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB MODCOPIER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
set(COMMON_SOURCES "../commonItems/CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/StringUtils.cpp")

add_executable(Fronter
	${MAIN_SOURCES}
	${CONFIGURATION_SOURCES}
	${CONFIGURATION_OPTIONS_SOURCES}
	${FRAMES_SOURCES}
	${FRAMES_TABS_SOURCES}
	${UTILS_SOURCES}
	${UTILS_LOCALIZATION_SOURCES}
	${CONVERTERLAUNCHER_SOURCES}
	${LOGWATCHER_SOURCES}
	${MODCOPIER_SOURCES}
	${COMMON_SOURCES}
)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(Fronter ${wxWidgets_LIBRARIES})

add_custom_command(TARGET Fronter POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND chmod u+x ../Copy_Files.sh)
add_custom_command(TARGET Fronter POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ../Copy_Files.sh)
