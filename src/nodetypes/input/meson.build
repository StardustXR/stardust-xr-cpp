input_nodetypes_includes = include_directories('.')

lib_input_nodetypes = static_library(
	'input_nodetypes',
	files(
		'inputhandler.cpp',
		'inputmethod.cpp'
	),
	include_directories: [input_nodetypes_includes]
)

input_nodetypes = declare_dependency(
	include_directories: input_nodetypes_includes,
	link_with: lib_input_nodetypes
)
