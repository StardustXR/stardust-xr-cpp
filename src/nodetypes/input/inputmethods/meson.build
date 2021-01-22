input_methods_includes = include_directories('.')

lib_input_methods = static_library(
	'input_methods',
	files(
#		'inputhandler.cpp',
#		'inputmethod.cpp'
	),
	include_directories: [input_methods_includes]
)

input_methods = declare_dependency(
	include_directories: input_methods_includes,
	link_with: lib_input_methods
)
