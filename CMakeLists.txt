cmake_minimum_required(VERSION 3.14)
cmake_policy(VERSION 3.14)

if(NOT DEFINED PROJECT_NAME)
	set(THZ_AUTO_GAMING_IS_ROOT True)
else()
	set(THZ_AUTO_GAMING_IS_ROOT False)
endif()

project(THzAutoGaming VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)
FetchContent_Declare(
	THzCommon
	GIT_REPOSITORY https://github.com/Austin-Powers/THzCommon
	GIT_TAG        548d42fa83bbb008ea5a1a29ab87e09e28cffabd # latest 09.10.23
)
FetchContent_Declare(
	THzImage
	GIT_REPOSITORY https://github.com/Austin-Powers/THzImage
	GIT_TAG        8b4afc529c4c33a37593db65c469845403c752d6 # latest 17.02.23
)
FetchContent_Declare(
	gsl
	GIT_REPOSITORY https://github.com/microsoft/GSL.git
	GIT_TAG        a3534567187d2edc428efd3f13466ff75fe5805c # release-v4.0.0
)
FetchContent_MakeAvailable(gsl)
if(NOT DEFINED THzCommon_SOURCE_DIR)
	message("THzCommon not found...")
	message("Fetching THzCommon")
	FetchContent_MakeAvailable(THzCommon)
endif()
if(NOT DEFINED THzImage_SOURCE_DIR)
	message("THzImage not found...")
	message("Fetching THzImage")
	FetchContent_MakeAvailable(THzImage)
endif()

add_library(${PROJECT_NAME} STATIC
	src/input/normalDeviationStrategy.cpp
	src/input/parameters.cpp
	src/input/windowsInterface.cpp
	src/simulations/tileMatchingPuzzle.cpp

	include/THzAutoGaming/input/common.hpp
	include/THzAutoGaming/input/emulator.hpp
	include/THzAutoGaming/input/iDeviationStrategy.hpp
	include/THzAutoGaming/input/normalDeviationStrategy.hpp
	include/THzAutoGaming/input/parameters.hpp
	include/THzAutoGaming/input/windowsInterface.hpp
	include/THzAutoGaming/optimisation/evolution/algorithm.hpp
	include/THzAutoGaming/simulations/tileMatchingPuzzle.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	Microsoft.GSL::GSL
	THzCommon
	THzImage
)

if(THZ_AUTO_GAMING_IS_ROOT)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	# Not needed here as there will be no parent project, but its good to remember this command
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
	enable_testing()
	add_subdirectory(test)
endif()
