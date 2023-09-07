set(PROJECTNAME THzAutoGaming.Tests)

add_executable(${PROJECTNAME}
	input/emulator.cpp
	input/normalDeviationStrategy.cpp
	input/parameters.cpp
	optimisation/evolution/algorithm.cpp
	simulations/tileMatchingPuzzle.cpp
)

target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECTNAME} PUBLIC
	THzAutoGaming
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})

add_test(NAME ${PROJECTNAME} COMMAND ${PROJECTNAME})
