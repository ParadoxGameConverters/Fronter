cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Fronter)
SET(CMAKE_CXX_COMPILER g++-9)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Source)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release-Linux)

add_compile_options("-std=c++17")
add_compile_options("-lstdc++fs")
add_compile_options("-O2")
add_compile_options("-pthread")
#add_compile_options("-L/usr/lib/x86_64-linux-gnu -pthread   -lwx_gtk3u_xrc-3.0 -lwx_gtk3u_html-3.0 -lwx_gtk3u_qa-3.0 -lwx_gtk3u_adv-3.0 -lwx_gtk3u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0")
#add_compile_options("-I/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.0 -I/usr/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread")
add_compile_options("`/usr/bin/wx-config --libs`")
add_compile_options("`/usr/bin/wx-config --cxxflags`")

include_directories("../commonItems")
include_directories("/usr/include/wx-3.0/")
include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.0/")
#include_directories("/usr/lib/x86_64-linux-gnu/wx/include/base-unicode-3.1-unofficial3/")
#include_directories("/usr/include/wx-3.1-unofficial/")
#include_directories("../wx/3.1.3/include/msvc/wx/")
#include_directories("../wx/3.1.3/include/wx/")
#include_directories("../wx/3.1.3/include/")
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB CONFIGURATION_OPTIONS_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB FRAMES_SOURCES "${PROJECT_SOURCE_DIR}/Frames/*.cpp")
file(GLOB FRAMES_TABS_SOURCES "${PROJECT_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB UTILS_SOURCES "${PROJECT_SOURCE_DIR}/Utils/*.cpp")
file(GLOB UTILS_LOCALIZATION_SOURCES "${PROJECT_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
file(GLOB LOGWATCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB MODCOPIER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
set(COMMON_SOURCES "../commonItems/CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "../commonItems/StringUtils.cpp")

add_executable(CK2ToEU4 
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

#add_custom_command(TARGET CK2ToEU4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND chmod u+x Copy_Files.sh)
#add_custom_command(TARGET CK2ToEU4 POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ./Copy_Files.sh)
