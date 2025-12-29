cmake_minimum_required(VERSION 3.25)

project(Fronter)

# --- Project Configuration ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release)

# --- Platform Specific Flags ---
if (MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	add_compile_definitions(UNICODE _UNICODE)
elseif (UNIX)
	add_compile_options(-pthread -Wall -O2 -Wno-unused-variable)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
endif ()

# --- Dependencies ---
include(FetchContent)

# 1. SQLiteCpp
# We FORCE build from source to avoid VCPKG mismatch (DLL vs Static & C++ Standard issues)
message(STATUS "Fetching SQLiteCpp from source to ensure C++23/Filesystem support...")
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
		SQLiteCpp
		GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
		GIT_TAG 3.3.3
		)
FetchContent_MakeAvailable(SQLiteCpp)
# Create an alias so linking below remains consistent if the target name differs
if(TARGET SQLiteCpp AND NOT TARGET SQLiteCpp::SQLiteCpp)
    add_library(SQLiteCpp::SQLiteCpp ALIAS SQLiteCpp)
endif()

# 2. CURL
# Try Config mode first (CURL::libcurl target)
find_package(CURL 8.17.0 QUIET CONFIG)

# If Config failed, try Module mode
if (NOT CURL_FOUND)
	find_package(CURL 8.17.0 QUIET)
endif ()

# If Module failed, try PkgConfig (Common on Linux)
if (NOT CURL_FOUND)
	find_package(PkgConfig QUIET)
	pkg_check_modules(PC_CURL QUIET libcurl>=8.17.0)

	if (PC_CURL_FOUND)
		message(STATUS "Found libcurl via PkgConfig")
		set(CURL_FOUND TRUE)
		if (NOT TARGET CURL::libcurl)
			add_library(CURL::libcurl INTERFACE IMPORTED)
			target_include_directories(CURL::libcurl INTERFACE ${PC_CURL_INCLUDE_DIRS})
			target_link_libraries(CURL::libcurl INTERFACE ${PC_CURL_LIBRARIES})
			target_link_directories(CURL::libcurl INTERFACE ${PC_CURL_LIBRARY_DIRS})
			target_compile_options(CURL::libcurl INTERFACE ${PC_CURL_CFLAGS_OTHER})
			target_link_options(CURL::libcurl INTERFACE ${PC_CURL_LDFLAGS_OTHER})
		endif ()
	endif ()
endif ()

# If all local searches failed, build from source
if (NOT CURL_FOUND)
	message(STATUS "CURL not found installed, fetching from source...")
	set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
	set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
	set(CURL_DISABLE_TESTS ON CACHE BOOL "" FORCE)

	FetchContent_Declare(
			CURL
			GIT_REPOSITORY https://github.com/curl/curl.git
			GIT_TAG curl-8_17_0
			)
	FetchContent_MakeAvailable(CURL)
endif ()

# 3. wxWidgets
find_package(wxWidgets 3.2.0 QUIET COMPONENTS net core base)

if (wxWidgets_FOUND)
    # Check if we have targets (VCPKG/Config mode) or variables (Module mode)
    if (TARGET wx::core)
        set(WX_LIBS wx::net wx::core wx::base)
    else()
	    include(${wxWidgets_USE_FILE})
	    set(WX_LIBS ${wxWidgets_LIBRARIES})
    endif()
else ()
	message(STATUS "wxWidgets not found installed, fetching v3.3.1 from source...")

	set(wxBUILD_SHARED ON CACHE BOOL "" FORCE)
	set(wxBUILD_SAMPLES OFF CACHE BOOL "" FORCE)
	set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)
	set(wxBUILD_PRECOMP OFF CACHE BOOL "" FORCE)

	if (UNIX AND NOT APPLE)
		set(wxBUILD_TOOLKIT gtk3 CACHE STRING "" FORCE)
	endif ()

	FetchContent_Declare(
			wxWidgets
			GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
			GIT_TAG v3.3.1
			)
	FetchContent_MakeAvailable(wxWidgets)

	if (NOT TARGET wx::net)
		add_library(wx::net ALIAS wxnet)
		add_library(wx::core ALIAS wxcore)
		add_library(wx::base ALIAS wxbase)
	endif ()
	set(WX_LIBS wx::net wx::core wx::base)
endif ()

# 4. commonItems
find_package(commonItems QUIET)
if (NOT commonItems_FOUND)
	message(STATUS "commonItems not found, fetching from source...")
	set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

	FetchContent_Declare(
			commonItems
			GIT_REPOSITORY https://github.com/tategotoazarasi/commonItems.git
			GIT_TAG 45863c7
			)
	FetchContent_MakeAvailable(commonItems)
endif ()

# --- Sources ---
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Fronter/Source)

file(GLOB MAIN_SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB CONFIGURATION_OPTIONS_SOURCES "${PROJECT_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB FRAMES_SOURCES "${PROJECT_SOURCE_DIR}/Frames/*.cpp")
file(GLOB FRAMES_TABS_SOURCES "${PROJECT_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB UTILS_SOURCES "${PROJECT_SOURCE_DIR}/Utils/*.cpp")
file(GLOB UTILS_LOCALIZATION_SOURCES "${PROJECT_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB LOGWATCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB MODCOPIER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
file(GLOB UPDATECHECKER_SOURCES "${PROJECT_SOURCE_DIR}/UpdateChecker/*.cpp")

if (WIN32)
	set(CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
elseif (UNIX)
	set(CONVERTERLAUNCHER_SOURCES "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/LinuxConverterLauncher.cpp")
endif ()
file(GLOB CONVERTERLAUNCHER_COMMON "${PROJECT_SOURCE_DIR}/WorkerThreads/ConverterLauncher/ConverterLauncher.cpp")
list(APPEND CONVERTERLAUNCHER_SOURCES ${CONVERTERLAUNCHER_COMMON})

# --- Target ---
add_executable(ConverterFrontend WIN32
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
               )

# --- Includes ---
target_include_directories(ConverterFrontend PRIVATE
                           "${PROJECT_SOURCE_DIR}"
                           )

# --- Linking ---
# Note: SQLiteCpp::SQLiteCpp here refers to the ALIAS created above for the source build
target_link_libraries(ConverterFrontend PRIVATE
                      commonItems::commonItems
                      SQLiteCpp::SQLiteCpp
                      CURL::libcurl
                      ${WX_LIBS}
                      )

# --- Platform Specifics ---
if (WIN32)
	target_compile_definitions(ConverterFrontend PRIVATE WXUSINGDLL)
	target_compile_definitions(ConverterFrontend PRIVATE UNICODE _UNICODE)
endif ()

# --- Post Build ---
if (UNIX)
	add_custom_command(TARGET ConverterFrontend POST_BUILD
	                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	                   COMMAND chmod u+x ./Copy_Files.sh
	                   COMMAND ./Copy_Files.sh
	                   COMMENT "Copying resources via script..."
	                   )
elseif (WIN32)
	add_custom_command(TARGET ConverterFrontend POST_BUILD
	                   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration
	                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Fronter/Resources/converter.ico ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
	                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
	                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_languages.yml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_english.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_french.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_german.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_italian.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_latin.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_russian.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Fronter/Resources/converter_l_simp_chinese.yml" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Configuration/"
	                   COMMENT "Copying resources..."
	                   )
endif ()