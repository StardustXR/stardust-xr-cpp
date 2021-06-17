tests_includes = include_directories('.')

lib_tests = static_library(
	'tests',
	files(
		'flatscreenpointer.cpp',
		'skhand.cpp',
	),
        include_directories: [tests_includes],
        dependencies: [stereokit]
)

tests = declare_dependency(
	include_directories: tests_includes,
	link_with: lib_tests
)
