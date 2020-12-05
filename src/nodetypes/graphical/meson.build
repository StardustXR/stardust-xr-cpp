graphical_nodetypes_includes = include_directories('.')

lib_graphical_nodetypes = static_library(
	'graphical_nodetypes',
	files(
		'model.cpp',
	),
	include_directories: [graphical_nodetypes_includes]
)

graphical_nodetypes = declare_dependency(
	include_directories: graphical_nodetypes_includes,
	link_with: lib_graphical_nodetypes
)
