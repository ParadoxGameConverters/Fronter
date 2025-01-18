cmake_minimum_required(VERSION 3.5)

project(Fronter)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Fronter/Source)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Fronter/Source)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_compile_options(-lstdc++fs)
add_compile_options(-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread -Wall -O2 -Wno-unused-variable)
add_link_options(-no-pie)

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})

include_directories("external/commonItems")
include_directories("external/commonItems/external/json/single_include")
include_directories("external/SQLiteCpp/include")
include_directories("external/SQLiteCpp/sqlite3")
include_directories("/usr/include/wx-3.2-unofficial")
include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2-unofficial")
include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB CONFIGURATION_OPTIONS_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB FRAMES_SOURCES "${PROJECT_SOURCE_DIR}/Frames/*.cpp")
file(GLOB FRAMES_TABS_SOURCES "${PROJECT_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB UTILS_SOURCES "${PROJECT_SOURCE_DIR}/Utils/*.cpp")
file(GLOB UTILS_LOCALIZATION_SOURCES "${PROJECT_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
list(REMOVE_ITEM CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
file(GLOB LOGWATCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB MODCOPIER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
file(GLOB UPDATECHECKER_SOURCES "${PROJECT_SOURCE_DIR}/UpdateChecker/*.cpp")
file(GLOB SQLITE_SOURCES "${PROJECT_SOURCE_DIR}/../../external/SQLiteCpp/src/*.cpp")

add_subdirectory(external/commonItems EXCLUDE_FROM_ALL)

add_executable(ConverterFrontend
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
	${UPDATECHECKER_SOURCES}
	${SQLITE_SOURCES}
)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(ConverterFrontend commonLib ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES})

add_custom_command(TARGET ConverterFrontend POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND chmod u+x ../Copy_Files.sh)
add_custom_command(TARGET ConverterFrontend POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ../Copy_Files.sh)
