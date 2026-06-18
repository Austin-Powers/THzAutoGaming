project('THzAutoGaming', 'cpp', version: '1.0.0', license: 'MIT')

include_dirs = include_directories('include')

sources = files(
    'src/action/handler.cpp',
	'src/input/linuxInterface.cpp',
	'src/input/normalDeviationStrategy.cpp',
	'src/input/parameters.cpp',
	'src/input/windowsInterface.cpp',
	'src/simulations/tileMatchingPuzzle.cpp',
	'src/utility/commonConditions.cpp',
	'src/utility/imageLoader.cpp',
	'src/utility/loopControl.cpp',
)

gsl_proj = subproject('microsoft-gsl')
gsl_dep = gsl_proj.get_variable('microsoft_gsl_dep')

thzcommon_proj = subproject('THzCommon')
thzcommon_dep = thzcommon_proj.get_variable('thzcommon_dep')

thzimage_proj = subproject('THzImage')
thzimage_dep = thzimage_proj.get_variable('thzimage_dep')

dependencies = [gsl_dep, thzcommon_dep, thzimage_dep]

thzautogaming_lib = library(
    meson.project_name(),
    sources,
    include_directories: include_dirs,
    dependencies: dependencies,
    override_options: ['cpp_std=c++20'],
)

thzautogaming_dep = declare_dependency(include_directories: include_dirs, link_with: thzautogaming_lib)

test_sources = files(
    'test/action/handler.cpp',
	'test/input/emulator.cpp',
	'test/input/normalDeviationStrategy.cpp',
	'test/input/parameters.cpp',
	'test/optimisation/evolution/algorithm.cpp',
	'test/simulations/tileMatchingPuzzle.cpp',
	'test/utility/commonConditions.cpp',
	'test/utility/imageLoader.cpp',
	'test/utility/loopControl.cpp',
)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_main_dep')

test_deps = []
test_deps += dependencies
test_deps += gtest_dep
test_deps += gmock_dep

test_exe = executable(
	'THzAutoGamingTests',
	sources + test_sources,
	include_directories: include_dirs,
	dependencies: test_deps,
	override_options: ['cpp_std=c++20'],
)

test('THzAutoGamingTests', test_exe)