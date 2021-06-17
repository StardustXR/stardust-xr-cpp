scenegraph_nodes_includes = include_directories('.')

lib_scenegraph_nodes = static_library(
	'scenegraph_nodes',
	files(
		'environment.cpp',
		'field.cpp',
		'input.cpp',
		'lifecycle.cpp',
		'model.cpp',
		'spatial.cpp',
	),
        include_directories: [scenegraph_nodes_includes],
        dependencies: [stereokit]
)

scenegraph_nodes = declare_dependency(
	include_directories: scenegraph_nodes_includes,
	link_with: lib_scenegraph_nodes
)
