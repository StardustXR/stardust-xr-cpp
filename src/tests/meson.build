tests_includes = include_directories('.')

lib_tests = static_library(
	'tests',
	files(
		'flatscreenpointer.cpp',
	),
	include_directories: [tests_includes]
)

tests = declare_dependency(
	include_directories: tests_includes,
	link_with: lib_tests
)
