util_includes = include_directories('.')

lib_util = static_library(
	'util',
	files(
		'debug.cpp',
	),
        include_directories: [util_includes],
        dependencies: [stereokit]
)

util = declare_dependency(
	include_directories: util_includes,
	link_with: lib_util
)
