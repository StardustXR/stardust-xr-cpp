nodetypes_includes = include_directories('.')

subdir('fields')
subdir('graphical')
subdir('input')

lib_nodetypes = static_library(
	'nodetypes',
	files(
		'node.cpp',
		'spatialnode.cpp',
	),
	include_directories: [nodetypes_includes],
        dependencies: [fields_nodetypes, graphical_nodetypes, input_nodetypes, stereokit]
)

nodetypes = declare_dependency(
	include_directories: nodetypes_includes,
	link_with: lib_nodetypes
)
