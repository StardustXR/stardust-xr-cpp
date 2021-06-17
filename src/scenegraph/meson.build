scenegraph_includes = include_directories('.')

subdir('nodes')

lib_scenegraph = static_library(
	'scenegraph',
	files(
		'scenegraph.cpp',
	),
	include_directories: [scenegraph_includes],
        dependencies: [scenegraph_nodes, stereokit]
)

scenegraph = declare_dependency(
	include_directories: scenegraph_includes,
	link_with: lib_scenegraph
)
