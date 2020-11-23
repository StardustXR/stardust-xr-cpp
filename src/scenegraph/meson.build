scenegraph_includes = include_directories('.')

lib_scenegraph = static_library(
	'scenegraph',
	files(
		'model.cpp',
	),
	include_directories: [scenegraph_includes]
)

scenegraph = declare_dependency(
	include_directories: scenegraph_includes,
	link_with: lib_scenegraph
)
