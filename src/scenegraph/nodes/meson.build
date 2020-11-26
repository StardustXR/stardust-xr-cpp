scenegraph_nodes_includes = include_directories('.')

lib_scenegraph_nodes = static_library(
	'scenegraph_nodes',
	files(
		'lifecycle.cpp',
		'model.cpp',
	),
	include_directories: [scenegraph_nodes_includes]
)

scenegraph_nodes = declare_dependency(
	include_directories: scenegraph_nodes_includes,
	link_with: lib_scenegraph_nodes
)
