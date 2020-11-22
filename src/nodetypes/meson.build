nodetypes_includes = include_directories('.')

subdir('graphical')

lib_nodetypes = static_library(
	'nodetypes',
	files(
		'node.cpp',
	),
	include_directories: [nodetypes_includes],
	dependencies: [graphical_nodetypes]
)

nodetypes = declare_dependency(
	include_directories: nodetypes_includes,
	link_with: lib_nodetypes
)
